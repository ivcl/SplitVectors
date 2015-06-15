/**------------------------------------------------------------------
 ** Author: Jian Chen (jichen@umbc.edu)
 * Nov 2013
 *
 * QDOTdataNorm.cpp
---------------------------------------------------------------------
*/

#include "QDOTdataNorm.h"

//#define VEL_SCALAR 1000000000000000  // 10^12 scale up the velocity field
#define VEL_SCALAR 1  // scale up the velocity field
#define DEN_SCALAR 1000000000000000  // 10^12 scale up the velocity field

#define LARGE_DUMMY 10000
#define SMALL_DUMMY -10000

#define FLOATING_POINT_ERROR 0.00001

using namespace std; 

// read data
QDOTdataNorm::QDOTdataNorm(svChar *dir, svChar *filename)
{
  totalROIPoints = 0;

  inDir = strdup(dir);
  inf = strdup(filename);

  lbbox=svVector3(LARGE_DUMMY, LARGE_DUMMY, LARGE_DUMMY);
  rbbox=svVector3(SMALL_DUMMY, SMALL_DUMMY, SMALL_DUMMY);

  dataPos = NULL;
  dataVel = NULL;
  dataDen = NULL;

  seedPos = NULL;
  seedVel = NULL;
  seedDen = NULL;
  seedNum = NULL;

  roiPos = NULL;
  roiVel = NULL;
  roiDen = NULL;

  readData();
  storeByZ();
}

void QDOTdataNorm::Print()
{
  int ds = dataPos[0].size(); // data size in that layer
  cerr << "------Print() -----------------------------"<<endl;
  cerr << "Print: dataPos size = " << ds << endl;
  for(int j=0; j<ds; j++)
    cerr << j << ": " 
	     << dataPos[0][j][0]<<" "<<dataPos[0][j][1]<<" "<<dataPos[0][j][2]<<", "
	 	 << dataVel[0][j][0]<<" "<<dataVel[0][j][1]<<" "<<dataVel[0][j][2]<<endl;
  cerr << "------end of Print()------------------------"<<endl;
}

QDOTdataNorm::~QDOTdataNorm()
{
  cleanData();
  cleanSeedData();
  cleanROIData();
}

bool decendingOrder2(float a, float b) { return a>b; }

void QDOTdataNorm::SetSeedNumber(int seed_num)
{
  cerr << "... SetSeedNumber to " << seed_num << endl;
  for(int si=0; si<zValue.size(); si++) 
    roiNumOfPoints.add(seed_num);

  // on the safe side, clean the data first so they can be reseeded.
  cleanROIData();

  roiPos = new svVector3Array[zValue.size()];
  roiVel = new svVector3Array[zValue.size()];
  roiDen = new svScalarArray[zValue.size()];

  // find out the cut off density value
  for(int dsi=0; dsi<zValue.size(); dsi++)
  {
     int dsis, dsi_size = seedPos[dsi].size();
	 vector<float>  den;

	 for(dsis=0; dsis<dsi_size; dsis++)
	 {
        den.push_back(seedDen[dsi][dsis]);
	 };
	 sort(den.begin(), den.end(), decendingOrder2);
	 //float min_den = min_element(begin(den), end(den));
	 //float max_den = max_element(den.begin(), .end(den));

	 // << "(min, max) = "<< min_den << ", " << max_den 
	 cerr << "... Index = " << dsi 
	      << ", dsi_size = " << dsi_size
		  << ", den range: (" << denMin[dsi] << ", "<< denMax[dsi]
		  << "), density cutoff is " << den[seed_num] 
		  << endl;
	 den.clear();

	 // save to ROI
	 int sum=0;
	 for(dsis=0; dsis<dsi_size; dsis++)
	 {
	     if(seedDen[dsi][dsis] > den[seed_num]) // keep
		 {
		   roiPos[dsi].add(seedPos[dsi][dsis]);
		   roiVel[dsi].add(seedVel[dsi][dsis]);
		   roiDen[dsi].add(seedDen[dsi][dsis]);
		   sum++;
		 }
	 };
	 roiNumOfPoints[dsi]=sum;
	 totalROIPoints += sum;
	 cerr << "... added " << sum << " points... " << endl;
  }; // end for(dsi)
}

void QDOTdataNorm::SetIsoRegion(float contour_value)
{
  cerr << "... SetContourValue to " << contour_value << endl;
  for(int si=0; si<zValue.size(); si++) 
    roiNumOfPoints.add(0);

  // on the safe side, clean the data first so they can be reseeded.
  cleanROIData();

  roiPos = new svVector3Array[zValue.size()];
  roiVel = new svVector3Array[zValue.size()];
  roiDen = new svScalarArray[zValue.size()];

  for(int dsi=0; dsi<zValue.size(); dsi++)
  {
     int dsis, dsi_size = seedPos[dsi].size();


	 // save to ROI
	 int sum=0;
	 for(dsis=0; dsis<dsi_size; dsis++)
	 {
	     if(seedDen[dsi][dsis] >= contour_value) // keep
		 {
		   roiPos[dsi].add(seedPos[dsi][dsis]);
		   roiVel[dsi].add(seedVel[dsi][dsis]);
		   roiDen[dsi].add(seedDen[dsi][dsis]);
		   sum++;
		 }
	 };
	 roiNumOfPoints[dsi]=sum;
	 totalROIPoints += sum;
	 //cerr << "... added " << sum << " points... " << endl;
  }; // end for(dsi)	
}

void QDOTdataNorm::storeByZ()
{
  // on the safe side, clean first;
  cleanSeedData();
  cerr << "\tDone cleanSeedData()" << endl;

  // allocate memory
  seedPos = new svVector3Array[zValue.size()];
  seedVel = new svVector3Array[zValue.size()];
  seedDen = new svScalarArray[zValue.size()];
  seedNum = new svIntArray[zValue.size()];

  // split the data by Z
  for(int dj=0; dj<dataSize; dj++)
  {
    int  zIndex;

    // find the zIndex
	vector<float>::iterator it;
	it=find(zValue.begin(), zValue.end(), dataPos[0][dj][2]);
	zIndex = distance(zValue.begin(), it);

	seedPos[zIndex].add(dataPos[0][dj]);
	seedVel[zIndex].add(dataVel[0][dj]);
	seedDen[zIndex].add(dataDen[0][dj]);
	

		seedNum[zIndex].add(1);

	
	//cerr << "dataX, ZPos: " << dataPos[0][dj][0]<<", "<<dataPos[0][dj][2] 
	     //<< ", zIndex = " << zIndex
		 //<< endl;
  }; // end for(dj)

  cerr << "... Done storeByZ.. data saved in seed positions... " << endl;
}


void QDOTdataNorm::readData()
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
  while(infile>>pos[0] >> pos[1] >> pos[2] >> dummy >> dummy >> vel[0] >> vel[1] >> vel[2] >> den)
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
void QDOTdataNorm::cleanData()
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

void QDOTdataNorm::cleanSeedData()
{
  cerr <<"... Cleaning up seed data ... " << endl;
  if(seedPos!=NULL)
  {
	seedPos[0].free();
	seedVel[0].free();
	seedDen[0].free();
	for(int i=0;i<zValue.size();i++)
	seedNum[i].free();
	
    delete [] seedPos;
    delete [] seedVel;
    delete [] seedDen;
    delete [] seedNum;

    cerr << "... Cleaned ... " << endl;
  }
}

void QDOTdataNorm::cleanROIData()
{
  cerr <<"... Cleaning up ROI data ... " << endl;
  if(roiPos!=NULL)
  {
	roiPos[0].free();
	roiVel[0].free();
	roiDen[0].free();

    delete [] roiPos;
    delete [] roiVel;
    delete [] roiDen;

	cerr << "... Cleaned ... " << endl;
  }
}
