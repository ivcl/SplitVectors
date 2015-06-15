/**------------------------------------------------------------------

 * readdata.cpp
---------------------------------------------------------------------
*/

#include "readdata.h"

//#define VEL_SCALAR 1000000000000000  // 10^12 scale up the velocity field
#define VEL_SCALAR 1  // scale up the velocity field
#define DEN_SCALAR 1000000000000000  // 10^12 scale up the velocity field

#define LARGE_DUMMY 10000
#define SMALL_DUMMY -10000

#define FLOATING_POINT_ERROR 0.00001

using namespace std; 

// read data
readdata::readdata(svChar *dir, svChar *filename)
{

  inDir = strdup(dir);
  inf = strdup(filename);

  lbbox=svVector3(LARGE_DUMMY, LARGE_DUMMY, LARGE_DUMMY);
  rbbox=svVector3(SMALL_DUMMY, SMALL_DUMMY, SMALL_DUMMY);

  dataPos = NULL;
  dataVel = NULL;
  dataDen = NULL;

  readData();
}

readdata::~readdata()
{
  cleanData();
}

void readdata::readData()
{
  long double vx, vy, vz;
  long double px, py, pz;
  long double dummy;

  svVector3 pos;
  svVector3 vel;
  svScalar  den;

  svChar filename[2056];
  sprintf(filename, "%s/%s", inDir, inf);

  ifstream infile(filename);
  if(infile.fail())
  {
    cerr << inf << "not found.\n";
	cerr << "Load QDOT data failed. prog exit\n" <<endl;
	exit(-1);
  }
  else
  {
	cerr << "Loading data: " << filename << endl;
  }

  // all data are saved in a one log array
  dataPos = new svVector3Array[1];
  dataVel = new svVector3Array[1];
  dataDen = new svScalarArray[1];
  infile>>dataSize;
  while(infile>>pos[0] >> pos[1] >> pos[2] >> vel[0] >> vel[1] >> vel[2] >> den)
  {
    //vel*=VEL_SCALAR;
    vel = normalize(vel);
	dataPos[0].add(pos);
	dataVel[0].add(vel);
    //den*=DEN_SCALAR;
	dataDen[0].add(den);

	// set the bounding box
	if(pos[0]<lbbox[0]) lbbox[0]=pos[0];
	if(pos[0]>rbbox[0]) rbbox[0]=pos[0];
	if(pos[1]<lbbox[1]) lbbox[1]=pos[1];
	if(pos[1]>rbbox[1]) rbbox[1]=pos[1];
	if(pos[2]<lbbox[2]) lbbox[2]=pos[2];
	if(pos[2]>rbbox[2]) rbbox[2]=pos[2];

    // set the charge density profile
    int zi;
    for(zi=0; zi<zValue.size(); zi++)
    {
      if(pos[2]==zValue[zi])
	  {
        if(denMin[zi] > den) denMin[zi]= den;
        if(denMax[zi] < den) denMax[zi]= den;
	    break;
	  };
    }; // end for(zi)

    if(zi==zValue.size()) // not found, add
    {
      zValue.push_back(pos[2]);
  	  denMin.add(den);
	  denMax.add(den);
	  
	  cerr << "--> zValue: insert " << pos[2]<< endl;
    }; // end if(zi)
  }; // end while

  
  //for(int i=0;i<zValue.size();i++)cerr<<denMax[i]<<" ";
  
  infile.close();

  dataSize = dataPos[0].size();
  cerr <<"... Done loading sampling sites of "<<dataPos[0].size()<<" points......"<<endl;
  cerr <<"    Bounding box size: ("
       <<lbbox[0] <<", "<<lbbox[1]<<", "<<lbbox[2]<<"), (" 
       <<rbbox[0] <<", "<<rbbox[1]<<", "<<rbbox[2]<<")" 
       <<endl;
  cerr <<"    Number of zValue: " << zValue.size() << endl;
  /*
  for(int zi=0; zi<zValue.size(); zi++)
  {
    cerr<<"Den bound: " << zi << ": "<<zValue[zi]
	    <<": ("<<denMin[zi]<<", "<<denMax[zi]<<")" << endl;
  }; // end for(zi)
  */
}


/*--------------------------------------------------------------------------------------
 * clean up routines 
 *--------------------------------------------------------------------------------------*/
void readdata::cleanData()
{
  cerr << "... Cleaning up dataPos and dataVel. "<< endl;
  if(dataPos!=NULL)
  {
	dataPos[0].free();
	dataVel[0].free();
	dataDen[0].free();

	delete [] dataPos;
	delete [] dataVel;
	delete [] dataDen;

    cerr << "... Cleaned ... " << endl;
  };
}


