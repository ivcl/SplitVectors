/**------------------------------------------------------------------
 Author: Jian Chen (jichen@umbc.edu) 
 Description: NIST QDOT data parser
 Data: Nov 2013
 Version 1.0:
---------------------------------------------------------------------
*/

#ifndef __QDOT_DATA_NORM_H
#define __QDOT_DATA_NORM_H

#include <iostream>
#include <fstream>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <vector>
#include <algorithm>

#include "svType.h"
#include "svVectorMatrix.h"
#include "svArray.h"

#include "./jcUtil/MGL.h"
#include "./jcUtil/MArray.h"
#include "./jcUtil/MVectorMatrix.h"

using namespace __svl_lib;
using namespace std;

class QDOTdataNorm {
public:
  // read data
  QDOTdataNorm(svChar *dir, svChar *inf);
  virtual ~QDOTdataNorm();

  // TODO: return velocity at a certain position 
  svVector3  CalcVelocityAt(svVector3 pos)  const
  {
    svVector3  vel(0,0,0);   // TODO: now only assign 0s
	return vel;
  }

  // ---------- GETS ----------- 
  // size
  svInt GetDatasize() const {return dataSize;}
  svInt GetZvalue() const {return zValue.size(); }

  svVector3Array * GetPosition() const
  { return dataPos; }
  svVector3Array * GetVelocity() const
  { return dataVel; }
  svScalarArray * GetDensity() const
  { return dataDen; }

  svScalarArray GetDenMin() const
  {return denMin;}
  svScalarArray GetDenMax() const
  {return denMax;}

  svVector3  GetPosition(int index) const
  { return dataPos[0][index]; }
  svVector3 GetVelocity(int index) const
  { return dataVel[0][index]; }
  svScalar GetDensity(int index) const
  { return dataDen[0][index]; }

  svInt  GetTotalROIPoints() 
  { return totalROIPoints; }
  svIntArray  GetROINumOfPoints() 
  { return roiNumOfPoints; }
  svVector3  GetROIPosition(int zIndex, int index) const
  { return roiPos[zIndex][index]; }
  svVector3  GetROIVelocity(int zIndex, int index) const
  { return roiVel[zIndex][index]; }
  svScalar   GetROIDensity(int zIndex, int index) const
  { return roiDen[zIndex][index]; }
  
  svVector3  GetPosition(int zIndex, int index) const
  { return seedPos[zIndex][index]; }
  svVector3  GetVelocity(int zIndex, int index) const
  { return seedVel[zIndex][index]; }
  svScalar   GetDensity(int zIndex, int index) const
  { return seedDen[zIndex][index]; }
  svIntArray  *GetPointsNum() 
    { return seedNum; }
  
  // bounding box
  void GetBoundingBox(svVector3 *lb, svVector3 *rb) const
  {
    (*lb)[0]=lbbox[0]; (*lb)[1]=lbbox[1]; (*lb)[2]=lbbox[2]; 
	(*rb)[0]=rbbox[0]; (*rb)[1]=rbbox[1]; (*rb)[2]=rbbox[2];
  }


  // ----------- SETS -------------
  void SetSeedNumber(int seed_num);
  void SetIsoRegion(float contour_value);// 04/06/14 by Henan

  // ----------- PRINTS AND SAVE ------------
  virtual void Print();

protected:
  virtual void cleanData();
  virtual void cleanSeedData();
  virtual void cleanROIData();

  virtual void readData();
  // read the original data and assign values to 
  //     dataPos
  //     dataVel
  //     dataDen
  //     dataSize
  virtual void storeByZ();


private:
  svChar *inDir;     // input dir name
  svChar *inf;       // input file name

  // Org data
  // ---> sampling sites
  svVector3Array *dataPos; // position
  svVector3Array *dataVel; // velocity 
  svScalarArray  *dataDen; // charge density 
  svInt          dataSize; // number of sampling sites
  svVector3      lbbox, rbbox; // left and right bounding box size

  // ---> charge density profile
  //svScalarArray  zValue;           // all unique z values
  vector<float>  zValue;         
  svScalarArray  denMin, denMax;   // density range at each z plane

  // ------------- Seeding sites (in the orginal file) -----------------
  //
  svVector3Array   *seedPos; // position
  svVector3Array   *seedVel; // velocity 
  svScalarArray    *seedDen; // charge density 
  svIntArray 	   *seedNum;
  //int              seedDataSize; 

  // ------------- Sampling sites (regions of interest) -----------------
  // 
  svInt            totalROIPoints;
  svIntArray       roiNumOfPoints;  // # of points 
  svVector3Array   *roiPos;
  svVector3Array   *roiVel;
  svScalarArray    *roiDen;
};

#endif // _QDOT_DATA_NORM_H

