#include "svContour.h"
#include "svUtil.h"
//#include "parameters.h"

#include <fstream>
#include <algorithm>
#include <cmath>
#include <vector>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


using namespace std;

#define isoScale 1

#ifndef PI
#define PI 3.1415926
#endif

#ifndef ZLAYER
#define ZLAYER 109
#endif

namespace __svl_lib{

void svContour::NewContours()
{
        cleanup();
        isoPointPos = new svVector3Array[2];
        isoPointDir = new svVector3Array[1];
        isoPointDen = new svScalarArray[1];

		isoPointExp = new svScalarArray[1];
		isoPointCoe = new svScalarArray[1];

	isoLayerNum = new svIntArray[1];
	isoRatio = new svScalarArray[1];
	isoSample = new svIntArray[1];
	isoZnum = new svIntArray[zlayer];
	
	Frequency = 1;
	maxden = -9e+9;

	zLayer =0 ;
}

int svContour::getNumOfIntegerDigits(float num)
{
	  int multiplier = 0;

	  float numCopy = num;

	  if(numCopy<1&&numCopy!=0) 
	  {
	    while (1)
		{
	      if(numCopy>=1)
		  {
		    break;
		  }; // end if(num)
		  multiplier--;
		  numCopy *= 10;
		}; // end while(1)
	  }
	  else if(numCopy==0)
		multiplier=0;
	  else
	  {
	    while(1)
		{
		  if(numCopy>=0 && numCopy <10)
		  {
		    break;
		  };
		  multiplier++;
		  numCopy /= 10.;
		}; // end while (1)
	  }; // end if(numCopy)

	  return multiplier;	
}

#ifdef _VTK
void svContour::Get2DContour(char *infile,svScalar density)
{
	//cerr<<infile<<endl;
	//cerr<<density<<" ";
        vtkUnstructuredGridReader *pl3d = vtkUnstructuredGridReader::New();
        pl3d->SetFileName(infile);
        pl3d->SetScalarsName("density");
        pl3d->SetVectorsName("velocity");
        pl3d->Update();
        pl3d->DebugOn();

        vtkDelaunay2D *del2d = vtkDelaunay2D::New();
        del2d->SetInputConnection(pl3d->GetOutputPort());
        del2d->SetTolerance(0.001);
        del2d->SetAlpha(0);

        vtkContourFilter *iso = vtkContourFilter::New();
        iso->SetInputConnection(del2d->GetOutputPort());
        iso->SetValue(0,density);

        vtkDataSetSurfaceFilter *filter = vtkDataSetSurfaceFilter::New();
        filter->SetInput(iso->GetOutput());
        filter->Update();
        vtkPolyData *pd = filter->GetOutput();

        double *p = new  double[3];
        double *pos = new double[3];
        double *den = new double[1];

	svScalar tmpden;

	svVector3 tmp;
	svVector4 col;
	col[0]=0;col[1]=0;col[2]=0;col[3]=1;

	int count = 0;

	vector<double> p3[3];
	vector<double> v3[3];
	vector<double> d3;

	/*svVector3 l;
	svVector3 r;
	l[0]=9e+9;l[1]=9e+9;l[2]=9e+9;
	r[0]=-9e+9;r[1]=-9e+9;r[2]=-9e+9;	*/

        for(int i=0;i<pd->GetNumberOfPoints();i++)
        {
		if(i==0)zLayer++;

                p = pd->GetPoint(i);
				tmp[0] = p[0];tmp[1]=p[1];tmp[2]=p[2];
			p3[0].push_back(tmp[0]);
			p3[1].push_back(tmp[1]);
			p3[2].push_back(tmp[2]);				
				isoPointPos[0].add(tmp);
		/*if(tmp[0]>r[0])r[0]=tmp[0];
		if(tmp[1]>r[1])r[1]=tmp[1];
		if(tmp[2]>r[2])r[2]=tmp[2];
		if(tmp[0]<l[0])l[0]=tmp[0];
		if(tmp[1]<l[1])l[1]=tmp[1];
		if(tmp[2]<l[2])l[2]=tmp[2];*/

                pd->GetPointData()->GetArray(0)->GetTuple(i,pos);
                tmp[0]=pos[0];tmp[1]=pos[1];tmp[2]=pos[2];
                tmp.normalize();
                isoPointDir[0].add(tmp);
			v3[0].push_back(tmp[0]);
			v3[1].push_back(tmp[1]);
			v3[2].push_back(tmp[2]);	
		tmp[0]=p[0]+tmp[0];
		tmp[1]=p[1]+tmp[1];
		tmp[2]=p[2]+tmp[2];

		isoPointPos[1].add(tmp);

                pd->GetPointData()->GetArray(1)->GetTuple(i,den);
                tmpden = den[0];
                isoPointDen[0].add(tmpden);
		d3.push_back(tmpden);

		svScalar exp = (svScalar)getNumOfIntegerDigits(tmpden);
		double tmppow = 1;
		for(int iii=0;iii<exp;iii++)
			tmppow = tmppow * 10.;		
		svScalar coe = tmpden/tmppow;//pow(10.,exp);
		
		isoPointExp[0].add(exp);
		isoPointCoe[0].add(coe);

                if(tmpden>maxden)maxden = tmpden;


                pointnum++;
                count ++;
        }

	/*svVector3 c;
	c = l +r;
	c = c/2;
cerr<<c[0]<<" "<<c[1]<<" "<<c[2]<<endl;*/

		sortP[0].push_back(p3[0]);
		sortP[1].push_back(p3[1]);
		sortP[2].push_back(p3[2]);
		sortV[0].push_back(v3[0]);
		sortV[1].push_back(v3[1]);
		sortV[2].push_back(v3[2]);
		sortD.push_back(d3);

		p3[0].clear();
		p3[1].clear();
		p3[2].clear();
		v3[0].clear();
		v3[1].clear();
		v3[2].clear();
		d3.clear();

        isoLayerNum[0].add(pd->GetNumberOfPoints());//count
        int tmpn;
        for(int ii=0;ii<isoSampleNum;ii++)
        {
        	tmpn = pointnum- pd->GetNumberOfPoints()+ ii*isoSampleNum;
        	if(tmpn<pointnum)
        		isoSample[0].add(tmpn);
        }
	

	delete [] p;
	delete [] pos;
	delete [] den;
}

#endif

void svContour::GetLineContour(char *infile,svScalar density)
{
//cerr<<infile<<endl;
	ifstream file;

	file.open(infile);

	int n;
	svVector3 *p, *v, tmp, tmpv;
	svScalar *d;
	double r;
	int count = 0;

	vector<double> p3[3];
	vector<double> v3[3];
	vector<double> d3;

	file>>n;
	
	p = new svVector3[n];
	v = new svVector3[n];
	d = new svScalar[n];

	int count2 = 0;

	for(int i=0;i<n;i++)
	{
		if(i==0)zLayer++;
		file>>p[i][0]>>p[i][1]>>p[i][2]>>v[i][0]>>v[i][1]>>v[i][2]>>d[i];
		if(i!=0)
		{
			//cerr<<d[i]<<" "<<d[i-1]<<" "<<density<<endl;

			if( (density>d[i-1] && density<d[i])
			|| (density>d[i] && density<d[i-1]) )
			{
				count2++;

				r = (density - d[i-1])/(d[i] - d[i-1]);
				tmp[0] = r * p[i-1][0] + (1-r) * p[i][0];
				tmp[1] = r * p[i-1][1] + (1-r) * p[i][1];
				tmp[2] = r * p[i-1][2] + (1-r) * p[i][2];
				tmpv[0] = r * v[i-1][0] + (1-r) * v[i][0];
				tmpv[1] = r * v[i-1][1] + (1-r) * v[i][1];
				tmpv[2] = r * v[i-1][2] + (1-r) * v[i][2];


				if(count2==1)
				{
					p3[0].push_back(tmp[0]);
					p3[1].push_back(tmp[1]);
					p3[2].push_back(tmp[2]);
					v3[0].push_back(tmp[0]);
					v3[1].push_back(tmp[1]);
					v3[2].push_back(tmp[2]);

					isoPointPos[0].add(tmp);
				
					tmpv = normalize(tmpv);
					isoPointDir[0].add(tmpv);

					tmp[0] = tmp[0] + tmpv[0];
					tmp[1] = tmp[1] + tmpv[1];
					tmp[2] = tmp[2] + tmpv[2];
					isoPointPos[1].add(tmp);

					d3.push_back(density);
					isoPointDen[0].add(density);

					svScalar exp = (svScalar)getNumOfIntegerDigits(density);
					double tmppow = 1;
					for(int iii=0;iii<exp;iii++)
						tmppow = tmppow * 10.;		
					svScalar coe = density/tmppow;//pow(10.,exp);
		
					isoPointExp[0].add(exp);
					isoPointCoe[0].add(coe);

		     			if(density>maxden)maxden = density;

		        		pointnum++;
					count ++;
				}
			}
		}
		if(density == d[i])
		{
			count2++;


			tmp = p[i];
			tmpv = v[i];
			if(count2==1)
			{
				p3[0].push_back(tmp[0]);
				p3[1].push_back(tmp[1]);
				p3[2].push_back(tmp[2]);
				v3[0].push_back(tmp[0]);
				v3[1].push_back(tmp[1]);
				v3[2].push_back(tmp[2]);

				isoPointPos[0].add(tmp);
				
				tmpv = normalize(tmpv);
				isoPointDir[0].add(tmpv);

				tmp[0] = tmp[0] + tmpv[0];
				tmp[1] = tmp[1] + tmpv[1];
				tmp[2] = tmp[2] + tmpv[2];
				isoPointPos[1].add(tmp);

				d3.push_back(density);
				isoPointDen[0].add(density);

				svScalar exp = (svScalar)getNumOfIntegerDigits(density);
				double tmppow = 1;
				for(int iii=0;iii<exp;iii++)
					tmppow = tmppow * 10.;		
				svScalar coe = density/tmppow;//pow(10.,exp);
		
				isoPointExp[0].add(exp);
				isoPointCoe[0].add(coe);

		     		if(density>maxden)maxden = density;

		        	pointnum++;
				count ++;
			}
		}
		if(i==n-1 && count2!=1)
			{
				p3[0].push_back(tmp[0]);
				p3[1].push_back(tmp[1]);
				p3[2].push_back(tmp[2]);
				v3[0].push_back(tmp[0]);
				v3[1].push_back(tmp[1]);
				v3[2].push_back(tmp[2]);

				isoPointPos[0].add(tmp);
				
				tmpv = normalize(tmpv);
				isoPointDir[0].add(tmpv);

				tmp[0] = tmp[0] + tmpv[0];
				tmp[1] = tmp[1] + tmpv[1];
				tmp[2] = tmp[2] + tmpv[2];
				isoPointPos[1].add(tmp);

				d3.push_back(density);
				isoPointDen[0].add(density);

				svScalar exp = (svScalar)getNumOfIntegerDigits(density);
				double tmppow = 1;
				for(int iii=0;iii<exp;iii++)
					tmppow = tmppow * 10.;		
				svScalar coe = density/tmppow;//pow(10.,exp);
		
				isoPointExp[0].add(exp);
				isoPointCoe[0].add(coe);

		     		if(density>maxden)maxden = density;

		        	pointnum++;
				count ++;
			}
	}

	sortP[0].push_back(p3[0]);
	sortP[1].push_back(p3[1]);
	sortP[2].push_back(p3[2]);
	sortV[0].push_back(v3[0]);
	sortV[1].push_back(v3[1]);
	sortV[2].push_back(v3[2]);
	sortD.push_back(d3);

	p3[0].clear();
	p3[1].clear();
	p3[2].clear();
	v3[0].clear();
	v3[1].clear();
	v3[2].clear();
	d3.clear();

        isoLayerNum[0].add(count);//count
        int tmpn;
        for(int ii=0;ii<1;ii++)
        {
        	tmpn = pointnum- count+ ii*isoSampleNum;
        	if(tmpn<pointnum)
        		isoSample[0].add(tmpn);
        }
	

	file.close();
	
	delete []p;
	delete []v;
	delete []d;
}


void svContour::Read2DColorFile(char *Dir, char *file1, char *file2)
{


       /* isoPointPos = new svVector3Array[2];
        isoPointDir = new svVector3Array[1];
        isoPointDen = new svScalarArray[1];

		isoPointExp = new svScalarArray[1];
		isoPointCoe = new svScalarArray[1];

        isoLayerNum = new svIntArray[1];
        isoRatio = new svScalarArray[1];
        isoSample = new svIntArray[1];*/

        Frequency = 1;

        pointnum = 0;

        svVector3 pos1, pos2;
        svVector3 dir;
        svVector4 col;
        col[0]=1;col[1]=1;col[2]=1;col[3]=1;
        svFloat den;
        double num, temp;
        char filename[50];
        float sum;
	vector<double> p3[3];
	vector<double> v3[3];
	vector<double> d3;

        ifstream infile;
	zLayer = zlayer;
        for(int j=0;j<zlayer;j++)
        {
               // sprintf(filename, "%s/%d_%s_safe", Dir, j, file1);//data/z_10z/%d_2D_z_10z_1
			sprintf(filename, "%s/%d%s", Dir, j, file1);//data/z_10z/%d_2D_z_10z_1
			infile.open(filename);//cerr<<filename<<endl;
			if(infile.fail())
			{
				cerr<<"not found contour files"<<endl;exit(-1);
			}
                infile>>num;
            for(int i=0; i<num; i++)
            {
                        infile>>temp;
                        infile>>pos1[0]>>pos1[1]>>pos1[2]>>col[0]>>col[1]>>col[2];
                        infile>>pos2[0]>>pos2[1]>>pos2[2]>>col[0]>>col[1]>>col[2];
                        dir[0] = pos2[0] - pos1[0];
                        dir[1] = pos2[1] - pos1[1];
                        dir[2] = pos2[2] - pos1[2];
                        sum = sqrt(dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2]);
                        dir[0] = (float)dir[0]/sum;
                        dir[1] = (float)dir[1]/sum;
                        dir[2] = (float)dir[2]/sum;
                        v3[0].push_back(dir[0]);
			v3[1].push_back(dir[1]);
			v3[2].push_back(dir[2]);
                        isoPointDir[0].add(dir);
			p3[0].push_back(pos1[0]);
			p3[1].push_back(pos1[1]);
			p3[2].push_back(pos1[2]);
                        isoPointPos[0].add(pos1);
			pos1[0]=pos1[0]+dir[0];
                        pos1[1]=pos1[1]+dir[1];
                        pos1[2]=pos1[2]+dir[2];
			isoPointPos[1].add(pos1);

            }
			for(int ii=0;ii<10;ii++)
			{
				isoSample[0].add(pointnum+ii);
			}
            infile.close();
            pointnum = pointnum + num;
            isoLayerNum[0].add(num);

		isoZnum[j].add(isoLayerNum[0].size()-1);
//cerr<<p3[0].size()<<endl;
		sortP[0].push_back(p3[0]);
		sortP[1].push_back(p3[1]);
		sortP[2].push_back(p3[2]);
		sortV[0].push_back(v3[0]);
		sortV[1].push_back(v3[1]);
		sortV[2].push_back(v3[2]);

		p3[0].clear();
		p3[1].clear();
		p3[2].clear();
		v3[0].clear();
		v3[1].clear();
		v3[2].clear();
        }
        for(int j = 0; j<zlayer; j++)
        {

                sprintf(filename, "%s/%d%s", Dir, j, file2);
                infile.open(filename);
                if(infile.fail())
				{
					cerr<<"not found contour files"<<endl;
					exit(-1);
				}
                infile>>num;
                for(int i=0; i<num; i++)
                {
                        infile>>pos1[0]>>pos1[1]>>pos1[2]>>dir[0]>>dir[1]>>dir[2]>>den;
                        den = den * 1000;
			d3.push_back(den);
                        isoPointDen[0].add(den);

						svScalar exp = (svScalar)getNumOfIntegerDigits(den);
			double tmppow = 1;
			for(int iii=0;iii<exp;iii++)
				tmppow = tmppow * 10.;			
			svScalar coe = den/tmppow;//pow(10.,exp);
		
						isoPointExp[0].add(exp);
						isoPointCoe[0].add(coe);
                        
                        if(den>maxden)
                        	maxden = den;
                }
                infile.close();
//cerr<<d3.size()<<endl;

		sortD.push_back(d3);
		d3.clear();
        }

	    isoRatio[0].add(0.4);//!!!!!!!!!!!!!!!!!!!!hard code

	//cerr<<isoSample[0].size()<<endl;

}

void svContour::SavePointtoFile(char *file)
{
        ofstream ofile;
        ofile.open(file);

        ofile<<pointnum<<endl;

        for(int i=0;i<pointnum;i++)
        {
                ofile<<isoPointPos[0][i][0]<<" "<<isoPointPos[0][i][1]<<" "<<isoPointPos[0][i][2]<<" "<<
                       isoPointDir[0][i][0]<<" "<<isoPointDir[0][i][1]<<" "<<isoPointDir[0][i][2]<<" "<<
                       isoPointDen[0][i]<<" "<<endl;
        }
        
        ofile.close();
}
void svContour::SavetoFile(char *file)
{
        ofstream ofile;
        ofile.open(file);

        ofile<<pointnum<<endl;

        for(int i=0;i<pointnum;i++)
        {
                ofile<<isoPointPos[0][i][0]<<" "<<isoPointPos[0][i][1]<<" "<<isoPointPos[0][i][2]<<" "<<
                		1<<" "<<2<<" "<<
                       isoPointDir[0][i][0]<<" "<<isoPointDir[0][i][1]<<" "<<isoPointDir[0][i][2]<<" "<<			   
                       isoPointDen[0][i]<<" "<<endl;
        }
        
        ofile.close();
}
void svContour::SavetosvlFile(char *file)
{
        ofstream ofile;
        ofile.open(file);

        ofile<<pointnum<<endl;

        for(int i=0;i<pointnum;i++)
        {
        	ofile<<2<<endl;
                ofile<<isoPointPos[0][i][0]<<" "<<isoPointPos[0][i][1]<<" "<<isoPointPos[0][i][2]<<" "<<1<<" "<<1<<" "<<1<<endl;
                ofile<<isoPointPos[0][i][0]+isoPointDir[0][i][0]<<" "
                		<<isoPointPos[0][i][1]+isoPointDir[0][i][1]<<" "
						<<isoPointPos[0][i][2]+isoPointDir[0][i][2]<<" "
						<<1<<" "<<1<<" "<<1<<endl;
        }
        
        ofile.close();
}

//to find the 8 nearest points
void svContour::Find2Nei(int selecti, int &index1, int &index2, int e, int s)
{
	int i;
	svVector3 *layer = new svVector3[e-s];
	vector<float> x1,x2,xtmp1,xtmp2;
	vector<int> xi1,xi2;
	vector<float> y1,y2;
	float x,y;
	double dd,td;

	float	alpha = acos(fabs(isoPointPos[0][selecti][0])/
			sqrt(isoPointPos[0][selecti][0]*isoPointPos[0][selecti][0]+
			isoPointPos[0][selecti][1]*isoPointPos[0][selecti][1]
			));
	if(isoPointPos[0][selecti][0]<0&&isoPointPos[0][selecti][1]>0)
		alpha = PI - alpha;
	else if(isoPointPos[0][selecti][0]<0&&isoPointPos[0][selecti][1]<0)
		alpha = PI + alpha;
	else if(isoPointPos[0][selecti][0]>=0&&isoPointPos[0][selecti][1]<0)
		alpha = 2.*PI - alpha;

	for(i=0;i<e-s;i++)
	{
		if(i+s!=selecti)
		{
			layer[i][0] = cos(alpha) * isoPointPos[0][i+s][0]
					+ sin(alpha) * isoPointPos[0][i+s][1];
			layer[i][1] = (-1)*sin(alpha) * isoPointPos[0][i+s][0]
					+ cos(alpha) * isoPointPos[0][i+s][1];

			if(layer[i][1]>=0){x1.push_back(layer[i][0]);y1.push_back(layer[i][1]);
						xtmp1.push_back(layer[i][0]);xi1.push_back(i);}
			else	{x2.push_back(layer[i][0]);y2.push_back(layer[i][1]);
				xtmp2.push_back(layer[i][0]);xi2.push_back(i);}
		}
	}
	x = cos(alpha) * isoPointPos[0][selecti][0]
		+ sin(alpha) * isoPointPos[0][selecti][1];
	y = (-1)*sin(alpha) * isoPointPos[0][selecti][0]
		+ cos(alpha) * isoPointPos[0][selecti][1];

	//sort(x1.begin(),x1.end());
	//sort(x2.begin(),x2.end());
	dd = 9e+9;
	for(i=0;i<x1.size();i++)
	{
		td = (x1[i]-x)*(x1[i]-x)+(y1[i]-y)*(y1[i]-y);
		if(td<dd&&(xi1[i]+s)%Frequency==0)
		{
			dd = td;
			index1 = xi1[i]+s;
		}
	}
	dd = 9e+9;
	for(i=0;i<x2.size();i++)
	{
		td = (x2[i]-x)*(x2[i]-x)+(y2[i]-y)*(y2[i]-y);
		if(td<dd&&(xi2[i]+s)%Frequency==0)
		{
			dd = td;
			index2 = xi2[i]+s;
		}
	}

	svVector3 u,v;
	u = isoPointPos[0][selecti] - isoPointPos[0][index1];
	v = isoPointPos[0][index2] - isoPointPos[0][selecti];

	float tmpz = u[0]*v[1] - u[1]*v[0];
	int tmpindex;

	if(tmpz<0)
	{
		tmpindex = index1;
		index1 = index2;
		index2 = tmpindex;
	}
}

void svContour::FindNeighbors(int selecti, int *index, int &num)
{
	num = 8;
	int sum = 0;
	int sample = isoLayerNum[0].size();
	int i;
	int count;
	int e,s;
	int e1, s1, e2, s2;
	double dd, td;
	float start = 0;
	//int ii;
	int z,z1,z2;


	for(i=0;i<8;i++)
		index[i] = 0;

	//find the range of three layers
	sum = -1;
	for(i=0;i<sample;i++)
	{
		//cerr<<i<<" "<<isoLayerNum[0][i]<<endl;
		sum = sum + isoLayerNum[0][i];
		if(selecti<=sum)
			break;
		start = start + isoLayerNum[0][i];
	}

	e = sum+1;
	s = start;

	for(int ii=0;ii<zlayer;ii++)
	{
		for(int jj=0;jj<isoZnum[ii].size();jj++)
		{
			if(isoZnum[ii][jj]==i)
			{z=ii;break;}
		}
	}
	//cerr<<z<<" "<<e<<" "<<s<<endl;
	if(ONE==false&&OTHER==false)
	{
		if(z+1>=zlayer)	
		{
			index[2] = -1;
			index[3] = -1;	
			index[4] = -1;
		}
		else
		{
			if(isoZnum[z+1].size()!=0)
				z2 = z+1;
			else
			{
				index[2] = -1;
				index[3] = -1;	
				index[4] = -1;
			}
		}
		if(z-1<0)
		{
			index[5] = -1;
			index[6] = -1;
			index[7] = -1;
		}
		else
		{
			if(isoZnum[z-1].size()!=0)
				z1 = z-1;
			else
			{
				index[5] = -1;
				index[6] = -1;
				index[7] = -1;
			}
		}
	}
	else
	{
		if(z+2>=zlayer)	
		{
			index[2] = -1;
			index[3] = -1;	
			index[4] = -1;
		}
		else
		{
			if(isoZnum[z+2].size()!=0)
				z2 = z+2;
			else
			{
				index[2] = -1;
				index[3] = -1;	
				index[4] = -1;
			}
		}
		if(z-2<0)
		{
			index[5] = -1;
			index[6] = -1;
			index[7] = -1;
		}
		else
		{
			if(isoZnum[z-2].size()!=0)
				z1 = z-2;
			else
			{
				index[5] = -1;
				index[6] = -1;
				index[7] = -1;
			}
		}
	}

	Find2Nei(selecti, index[0], index[1],e,s);
	//cerr<<z<<" "<<e<<" "<<s<<endl;
	//for the upper layer
	if(index[2]!=-1)
	{
		//cerr<<"x "<<z2<<endl;
		dd = 9e+9;sum = 0;
		//cerr<<z2<<" "<<isoZnum[z2].size()<<" "<<isoZnum[z2][0]<<" "<<isoZnum[z2][isoZnum[z2].size()-1]<<" "<<isoLayerNum[0].size()<<endl;
		for(int ii=0;ii<isoZnum[z2].size();ii++)
		{
			sum = 0;
			for(int jj=0;jj<=isoZnum[z2][ii];jj++)
			{	sum = sum + isoLayerNum[0][jj];}
			e2 = sum;
			s2 = sum - isoLayerNum[0][isoZnum[z2][ii]];
			//cerr<<"2 "<<e2<<" "<<s2<<endl;
			if(s2%Frequency!=0)
				s2 = s2 + Frequency-s2%Frequency;
			for(i=s2;i<e2;)
			{
				td = (isoPointPos[0][i][0]-isoPointPos[0][selecti][0])*(isoPointPos[0][i][0]-isoPointPos[0][selecti][0]);
					+ (isoPointPos[0][i][1]-isoPointPos[0][selecti][1])*(isoPointPos[0][i][1]-isoPointPos[0][selecti][1]);
				if(td<dd)
				{
					dd = td;
					index[3] = i;
				}
				i = i + Frequency;
			}
			//cerr<<ii<<endl;
		}	
		Find2Nei(index[3], index[2], index[4],e2,s2);
	}
	if(index[5]!=-1)
	{
		//cerr<<"y "<<z1<<endl;
		dd = 9e+9;sum = 0;		
		//cerr<<z1<<" "<<isoZnum[z1].size()<<" "<<isoZnum[z1][0]<<" "<<isoZnum[z1][isoZnum[z1].size()-1]<<endl;
		for(int ii=0;ii<isoZnum[z1].size();ii++)
		{
			sum = 0;
			for(int jj=0;jj<=isoZnum[z1][ii];jj++)
				sum = sum + isoLayerNum[0][jj];
			e1 = sum;
			s1 = sum - isoLayerNum[0][isoZnum[z1][ii]];			//cerr<<"1 "<<e1<<" "<<s1<<endl;	
			if(s1%Frequency!=0)
				s1 = s1 + Frequency-s1%Frequency;
			for(i=s1;i<e1;)
			{
				td =(isoPointPos[0][i][0]-isoPointPos[0][selecti][0])*(isoPointPos[0][i][0]-isoPointPos[0][selecti][0])
					+ (isoPointPos[0][i][1]-isoPointPos[0][selecti][1])*(isoPointPos[0][i][1]-isoPointPos[0][selecti][1]);
				if(td<dd)
				{
					dd = td;
					index[6] = i;
				}
				i = i + Frequency;
			}
			//cerr<<"1 "<<e1<<" "<<s1<<endl;
		}
		Find2Nei(index[6], index[5], index[7],e1,s1);	
	}
}

svScalar svContour::GetAngle(svVector3 p)
{
	float sum = p[0] * p[0] + p[1] * p[1];
	sum = sqrt(sum);
	svVector3 p2;
	svScalar a;
	p2[0] = p[0]/sum;
	p2[1] = p[1]/sum;
	if(p2[1]>=0)
	{
	a = acos(p2[0]);
	}
	else
	{
	a = M_PI*2. - acos(p2[0]);
	}
	return a;
}
bool * svContour::GetInsidePos_Mag(svScalar minmag, svScalar maxmag)
{
	bool *in = new bool[pointnum];
	int t=0;
	for(int i=0;i<pointnum;i++)
	{
		if(isoPointDen[0][i]>=minmag*pow(10.,(double)DenScalar)&&isoPointDen[0][i]<=maxmag*pow(10.,(double)DenScalar))
		{
			in[t]=true;
		}
		else
		{
			in[t] = false;
		}
		t++;
	}
	return in;
}

bool * svContour::GetInsidePos_Vel(vector<double> minangle, vector<double> maxangle, svScalar zangle1, svScalar zangle2)//zangle1,zangle2 [0,180]
{
	bool *in = new bool[pointnum];

	for(int i=0;i<pointnum;i++)
		in[i] = false;

	zangle1 = zangle1/180. * M_PI;
	zangle2 = zangle2/180. * M_PI;

	for(int j=0;j<minangle.size();j++)
	{
		if(minangle[j]>maxangle[j])
		{
			svScalar z;
			int t=0;
			for(int i=0;i<pointnum;i++)
			{
				svScalar angle = GetAngle(isoPointDir[0][i]);
				z = acos(isoPointDir[0][i][2]);
				if(((angle>=minangle[j]&&angle<=M_PI*2.)||(angle>=0.&&angle<=maxangle[j]))
					&&z>=zangle1&&z<=zangle2)
				{
					in[t]=true;
				}
				t++;
			}	
		}
		else
		{
			svScalar z;
			int t=0;
			for(int i=0;i<pointnum;i++)
			{
				svScalar angle = GetAngle(isoPointDir[0][i]);
				z = acos(isoPointDir[0][i][2]);
				if(angle>=minangle[j]&&angle<=maxangle[j]&&z>=zangle1&&z<=zangle2)
				{
					in[t]=true;
				}

				t++;
			}	
		}
	}
	return in;	
}
void svContour::PrintData(char *file)
{
	ofstream ofile(file);
	for(int i=0;i<pointnum;i++)
	{
		ofile<<isoPointPos[0][i][0]<<" "<<isoPointPos[0][i][1]<<" "<<isoPointPos[0][i][2]<<" "
			<<1<<" "<<2<<" "
			<<isoPointDir[0][i][0]<<" "<<isoPointDir[0][i][1]<<" "<<isoPointDir[0][i][2]<<" "
			<<isoPointDen[0][i]/pow(10.,15.)<<endl;
	}
	ofile.close();

	char str[50];
	sprintf(str,"%s.txt",file);
	ofile.open(str);
	for(int i=0;i<zLayer;i++)
	{
		for(int j=0;j<sortP[0][i].size();j++)
			ofile<<sortP[0][i][j]<<" "<<sortP[1][i][j]<<" "<<sortP[2][i][j]<<" "
				<<1<<" "<<2<<" "
				<<sortV[0][i][j]<<" "<<sortV[1][i][j]<<" "<<sortV[2][i][j]<<" "
				<<sortD[i][j]/pow(10.,15.)<<endl;
	}
	ofile.close();
}
void svContour::cleanup()
{

        if(isoPointPos!=NULL)
        {
                for(int i=0;i<2;i++)
                {
                        isoPointPos[i].free();
                }
                delete [] isoPointPos;
                isoPointPos = NULL;
        }


        if(isoPointDen!=NULL)
        {
                for(int i=0;i<1;i++)
                {
                        isoPointDen[i].free();
						isoPointCoe[i].free();
						isoPointExp[i].free();
                }
                delete [] isoPointDen;
				delete [] isoPointCoe;
				delete [] isoPointExp;
                isoPointDen = NULL;
				isoPointExp = NULL;
				isoPointCoe = NULL;
        }
        if(isoPointDir!=NULL)
        {
                for(int i=0;i<1;i++)
                {
                        isoPointDir[i].free();
                }
                delete [] isoPointDir;
                isoPointDir = NULL;
        }


	if(isoLayerNum!=NULL)
	{
		isoLayerNum[0].free();
		delete [] isoLayerNum;
		isoLayerNum = NULL;
	}
	if(isoRatio!=NULL)
	{
		isoRatio[0].free();
		delete [] isoRatio;
		isoRatio = NULL;
	}
	if(isoSample!=NULL)
	{
		isoSample[0].free();
		delete [] isoSample;
		isoSample = NULL;
	}
	if(isoZnum!=NULL)
	{
                for(int i=0;i<zlayer;i++)
                {
                        isoZnum[i].free();
                }
                delete [] isoZnum;
                isoZnum = NULL;
	}

	if(sortP[0].size()!=0){
	for(int i=0;i<zlayer;i++)
	{
		sortP[0][i].clear();
		sortP[1][i].clear();
		sortP[2][i].clear();
		sortV[0][i].clear();
		sortV[1][i].clear();
		sortV[2][i].clear();
		sortD[i].clear();
	}
	sortP[0].clear();
	sortP[1].clear();
	sortP[2].clear();
	sortV[0].clear();
	sortV[1].clear();
	sortV[2].clear();
	sortD.clear();
	}
        datanum = 0;
	pointnum = 0;
	


}

}
