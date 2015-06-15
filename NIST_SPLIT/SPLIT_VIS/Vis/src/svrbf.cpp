
#include <math.h>
#include "svrbf.h"
#include "./LU/ludcmp.h"
#include "./LU/lubksb.h"
#include "./jcUtil/MVectorMatrix.h"
#include <iostream>
#include <fstream>

#ifndef max
#define max(a,b)  ((a)>(b)?(a):(b))
#endif
namespace __svl_lib {

svrbf::svrbf()
{
}

svrbf::~svrbf()
{
  if(ctrpos != NULL)
    delete [] ctrpos;
  if(ctrxval != NULL)
    delete [] ctrxval;
  if(ctryval != NULL)
    delete [] ctryval;
  if(ctrzval != NULL)
    delete [] ctrzval;
  if(wx != NULL)
    delete [] wx;
  if(wy != NULL)
    delete [] wy;
  if(wz != NULL)
    delete [] wz;

  if(dis != NULL)
    delete [] dis;
  if(EM != NULL)
    delete [] EM;
}

svVector3 svrbf::GetVector(const svParticle & p)
{
  return GetVector(p.position, p.time);
}

svVector3 svrbf::GetVector(const svVector3& pos, svScalar t)
{
   double vx=0.,vy=0,vz=0;
   gen_rbf(pos[0],pos[1],pos[2],&vx,&vy,&vz);
   //cerr << "----> v: (" << vx <<", " << vy<<", " << vz<<endl;
    
   //double s = sqrt(vx*vx+vy*vy+vz*vz);
   //cerr << "----> s: " << s <<endl;

   svVector3 rv(vx*10.,vy*10.,vz*10.);
   return rv;
}

void svrbf::CalculateRoughStrength(int num_seeds)
{
  svVectorField::CalculateRoughStrength(num_seeds);
/*
    int max_adv_time = 100;
    // random pick some points
    for(int i=0; i<num_seeds; i++)
    {
      svVector3 pos;
      pos[0] = float(rand())/float(RAND_MAX)*2.-1.f; 
      pos[1] = float(rand())/float(RAND_MAX)*2.-1.f; 
      pos[2] = float(rand())/float(RAND_MAX)*2.-1.f; 

      svParticle particle;

      particle.position = pos;
      particle.time = 0;

      svVector3 v = GetVector(particle);
      svScalar s = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
      if(s>svVectorField::max_strength) svVectorField::max_strength = s;
      if(s<svVectorField::min_strength) svVectorField::min_strength = s;
    }
    */
}

void svrbf::distanceMatrixOneDir(double p,int index, svFloat *ddis)
{
  for(int i=0; i<nvIn; i++) 
  {
     ddis[i]= p-ctrpos[i][index];
     //cerr << "distanceMatrixOneDir: " << ctrpos[i][index] << " " << ddis[i] << endl;
  }
}

void svrbf::distanceMatrix(double x, double y, double z/*, svFloat *dis*/)
{
  int i;
  //cerr << "distanceMatrix: at point --> " << endl;
    //cerr << x << " " << y << " " << z << "-> dis= " ;;
  for(i=0; i<nvIn; i++) 
  {
    dis[i] = (x-ctrpos[i][0])*(x-ctrpos[i][0])
            +(y-ctrpos[i][1])*(y-ctrpos[i][1])
            +(z-ctrpos[i][2])*(z-ctrpos[i][2]);
    svDouble sq = sqrt(dis[i]);
    dis[i]=sq;
   // cerr << dis[i] << "   ";
  };
  //cerr << endl;
}

void svrbf::jacobianMatrix(double x, double y, double z, svMatrix3 &jac)
{
  svFloat *EMDX = new svFloat[nvIn];
  svFloat *EMDY = new svFloat[nvIn];
  svFloat *EMDZ = new svFloat[nvIn];

  
  for(int i=0; i<nvIn; i++)
    EMDX[i]=0.f;
  for(int i=0; i<nvIn; i++)
    EMDY[i]=0.f;
  for(int i=0; i<nvIn; i++)
    EMDZ[i]=0.f;

  distanceMatrix(x,y,z);
  for(int i=0; i<nvIn; i++)
  {
   svDouble dx = x-ctrpos[i][0],
            dy = y-ctrpos[i][1],
	    dz = z-ctrpos[i][2];
   svDouble com = -2.* epIn * epIn * exp(-(epIn*dis[i])*(epIn*dis[i]));
   EMDX[i]=  dx * com;
   EMDY[i]=  dy * com;
   EMDZ[i]=  dz * com;

//cerr << "setEvaluationMatrix: dis (" << dx << ", " <<dy<<", "<< dz << ")" << endl;
   //cerr << com << endl;
   //cerr << EMDX[i] << " " << EMDY[i] << " " << EMDZ[i] << endl;
  }
  jac=svMatrix3(dot(EMDX, wx,nvIn),dot(EMDX,wy,nvIn), dot(EMDX,wz,nvIn),
                dot(EMDY, wx,nvIn),dot(EMDY,wy,nvIn), dot(EMDY,wz,nvIn),
                dot(EMDZ, wx,nvIn),dot(EMDZ,wy,nvIn), dot(EMDZ,wz,nvIn));
  //cerr << "===> Jacobian matrix: " << endl;
  //jac.print();

  delete [] EMDX;
  delete [] EMDY;
  delete [] EMDZ;
}

void svrbf::gen_rbf(double x, double y, double z,
                     double *vx, double *vy, double *vz)
{
  int i;
  distanceMatrix(x,y,z);
  rbfgaussian2();

  svFloat sumx=0., sumy=0., sumz=0.;
  for(i=0; i<nvIn; i++)
  {
    sumx+= EM[i]*wx[i];
    sumy+= EM[i]*wy[i];
    sumz+= EM[i]*wz[i];
  }
  *vx = (double)sumx; *vy = (double)sumy; *vz = (double)sumz;
}

// handle internal file format 
//  file format shall be something like
//   /map/gfx0/tools/linux/src/jchen/vtkCases/rbf/trial6/test6info17.log
void svrbf::New(char *infname)
{ New(infname, svVector3(-1,-1,-1),svVector3(1,1,1));}

void svrbf::New(char *infname, svVector3 lb, svVector3 rb)
{
  ifstream inf(infname);
  if(!inf)
  { 
    cerr << "svrbf: open file " << inf << " filed.. exit.. " << endl;
    exit(-1);
  };
  dimension = 3;
  max_varing_time = 0;
  flow_type = SV_STEADY;
  
  lbbox=lb; rbbox=rb;
  svFloat sx,sy,sz;

  sx = rbbox[0]-lbbox[0];
  sy = rbbox[1]-lbbox[1];
  sz = rbbox[2]-lbbox[2];
  longest_side = max(sx,max(sy,sz));

  char dummy[256], lineptr[256];

// skip the first line
  inf.getline(lineptr,  256);

// read epIn
  inf.getline(lineptr,  256);
  sscanf(lineptr, "%s %f", dummy, &epIn);

//cerr << epIn << endl;
// read nvIn
  inf.getline(lineptr,  256);
  sscanf(lineptr, "%s %d", dummy, &nvIn);
//cerr << nvIn << endl;

// skip one line
  inf.getline(lineptr,  256);

  ctrpos  = new svVector3[nvIn];
  ctrxval = new svFloat[nvIn];
  ctryval = new svFloat[nvIn];
  ctrzval = new svFloat[nvIn];
  wx = new svFloat[nvIn];
  wy = new svFloat[nvIn];
  wz = new svFloat[nvIn];
  dis= new svFloat[nvIn];
  EM =  new svFloat[nvIn];

  // get the random field
  for(int i=0; i<nvIn; i++) {
    inf.getline(lineptr,  256);
    sscanf(lineptr, "%f %f %f %f %f %f", 
            &ctrpos[i][0],&ctrpos[i][1],&ctrpos[i][2],
            &ctrxval[i],&ctryval[i],&ctrzval[i]);
  }; 

  // skip another line
  inf.getline(lineptr,  256);
  for(int i=0; i<nvIn; i++) {
    inf.getline(lineptr,  256);
    sscanf(lineptr, "%f %f %f", &wx[i],&wy[i],&wz[i]);
    //cerr << wx[i] << " " << wy[i] << " " << wz[i] << endl;
  }

  inf.close();
}


   //svScalar sx, svScalar sy, svScalar sz, 
void  svrbf::New(int numvec, 
   svVector3 _lbbox, svVector3 _rbbox, svVector3 * pos, 
   svFloat * posxvalue, svFloat * posyvalue, svFloat * poszvalue,
   svScalar e, svScalar max_time)
{
  int i;
  float sx, sy, sz;
  nvIn = numvec; epIn = e;
  lbbox = _lbbox; rbbox = _rbbox;

  dimension = 3;
  max_varing_time = max_time;
  flow_type = max_time>0. ? SV_UNSTEADY : SV_STEADY;
  sx = _rbbox[0]-_lbbox[0];
  sy = _rbbox[1]-_lbbox[1];
  sz = _rbbox[2]-_lbbox[2];
  longest_side = max(sx,max(sy,sz));

  cerr << "svrbf New ................. " << endl;
  ctrpos  = new svVector3[nvIn];
  ctrxval = new svFloat[nvIn];
  ctryval = new svFloat[nvIn];
  ctrzval = new svFloat[nvIn];
  wx = new svFloat[nvIn];
  wy = new svFloat[nvIn];
  wz = new svFloat[nvIn];
  dis= new svFloat[nvIn];
  EM =  new svFloat[nvIn];

  for(i=0; i<nvIn; i++) {
    ctrpos[i] = pos[i];
    ctrxval[i] = posxvalue[i];
    ctryval[i] = posyvalue[i];
    ctrzval[i] = poszvalue[i];
  }
  max_varing_time = max_time;
  flow_type = max_time > 0. ? SV_UNSTEADY : SV_STEADY;

  distanceMatrix0();
}

void svrbf::distanceMatrix0()
{
  svDouble **mdis;
  svInt    i,j;

  mdis = new svDouble *[nvIn];
  for(int i=0; i<nvIn; i++)
    mdis[i]=new svDouble[nvIn];
  for(int i=0; i<nvIn; i++)
    for(int j=0; j<nvIn; j++)
	  mdis[i][j]=0.f;
   

  for(i=0; i<nvIn;i++) {
    for(j=0; j<nvIn;j++) {
      if(i==j)
        mdis[i][j]=0;
      else if(i<j) {
        mdis[i][j]= ((ctrpos[i][0]-ctrpos[j][0])*(ctrpos[i][0]-ctrpos[j][0])
                    +(ctrpos[i][1]-ctrpos[j][1])*(ctrpos[i][1]-ctrpos[j][1])
                    +(ctrpos[i][2]-ctrpos[j][2])*(ctrpos[i][2]-ctrpos[j][2]));
	double sq = sqrt(mdis[i][j]);
	mdis[i][j]=sq;
	mdis[j][i]=sq;
      } // i<j
    }; // j
  }; // i

  cerr << "====> mdis " << endl;
  for(i=0; i<nvIn; i++) {
    for(j=0;j<nvIn; j++)
	cerr << mdis[i][j] << "  " ;
    cerr << endl;
  }
  cerr << "====> end mdis " << endl;

  // get weight
  svDouble *b, **a; 
  svInt    *indx, dummyvec = nvIn+1;
  a = new svDouble *[dummyvec];
  for(int i=0; i<dummyvec; i++)
    a [i]=new svDouble[dummyvec];

  for(int i=0; i<dummyvec; i++)
    for(int j=0; j<dummyvec; j++)
	  a[i][j]=0.f;

  b = new svDouble[dummyvec];
  indx = new svInt[dummyvec];
  for(int i=0; i<dummyvec; i++)
  {
    b[i]=0.f;
	indx[i]=0;
  }

  for(i=1;i<=nvIn; i++)
    for(j=1;j<=nvIn; j++)
  {
    a[i][j]= exp(-(epIn*mdis[i-1][j-1])*(epIn*mdis[i-1][j-1]));
  };

// debug
/*
  cerr << "====> a " << endl;
  for(i=1; i<=nvIn; i++) {
    for(j=1;j<=nvIn; j++)
	cerr << a[i][j] << "  " ;
    cerr << endl;
  }
  cerr << "====>end a " << endl;
  */


// get wx
  for(i=1;i<=nvIn; i++)
    b[i]=ctrxval[i-1];
  ludcmp(a,nvIn,indx,b);
  lubksb(a,nvIn,indx,b);
  //cerr << "wx: ";
  for(i=0; i<nvIn; i++) {
    wx[i]=(svFloat)b[i+1];
    //cerr << wx[i] << "  " ;
  };
  //cerr << endl;

// get wy
  for(i=1;i<=nvIn; i++)
    for(j=1;j<=nvIn; j++)
    a[i][j]= exp(-(epIn*mdis[i-1][j-1])*(epIn*mdis[i-1][j-1]));
  for(i=1;i<=nvIn; i++)
    b[i]=ctryval[i-1];
  ludcmp(a,nvIn,indx,b);
  lubksb(a,nvIn,indx,b);
  //cerr <<  "wy: ";
  for(i=0; i<nvIn; i++) {
    wy[i]=(svFloat)b[i+1];
    //cerr << wy[i] <<"  " ;
  };
  //cerr << endl;

// get wz
  for(i=1;i<=nvIn; i++)
    for(j=1;j<=nvIn; j++)
    a[i][j]= exp(-(epIn*mdis[i-1][j-1])*(epIn*mdis[i-1][j-1]));
  for(i=1;i<=nvIn; i++)
    b[i]=ctrzval[i-1];
  ludcmp(a,nvIn,indx,b);
  lubksb(a,nvIn,indx,b);
  //cerr <<  "wz: ";
  for(i=0; i<nvIn; i++) {
    wz[i]=(svFloat)b[i+1];
    //cerr << wz[i] << "  ";
  }
  //cerr << endl;

  for(i=0; i<nvIn; i++)
    delete mdis[i];
  delete [] mdis;
  for(i=0; i<dummyvec; i++)
    delete a[i];
  delete [] b;
  delete [] indx;
}

svFloat svrbf::rbfgaussian(svFloat mdis)
{
    return ( exp(-(epIn*mdis)*(epIn*mdis)) );
}

void svrbf::rbfgaussian2()
{
  //cerr << "svrbf:rbfgaussian2: EM: " << endl;
  for (int i=0; i< nvIn; i++) {
    EM[i]= exp(-((dis[i] *dis[i])  * ((epIn) * (epIn))));
    //cerr << EM[i] << "  "<<endl; 
    //rbfgaussian(dis[i]);
  }
  //cerr << endl;
}

svFloat svrbf::rbfgaussianDer(svFloat ddis, svFloat dx)
{
    return ( -2.* dx * epIn * epIn * exp(-(epIn*ddis)*(epIn*ddis)) );
}

void svrbf::rbfgaussianDer2(svFloat *dis, svFloat *dx, svFloat *EMDer)
{
  for (int i=0; i< nvIn; i++)
  {
    EMDer[i]= -2.* dx[i] * epIn * epIn * exp(-(epIn*dis[i])*(epIn*dis[i])) ;
  };
}


}
