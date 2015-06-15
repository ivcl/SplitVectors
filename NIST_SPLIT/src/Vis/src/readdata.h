/**------------------------------------------------------------------
 Author: Jian Chen (jichen@umbc.edu) 
 Description: NIST QDOT data parser
 Data: Nov 2013
 Version 1.0:
---------------------------------------------------------------------
*/

#ifndef __READ_DATA_H
#define __READ_DATA_H

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

class readdata {
public:
  // read data
  readdata(svChar *dir, svChar *inf);
  virtual ~readdata();

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

  // bounding box
  void GetBoundingBox(svVector3 *lb, svVector3 *rb) const
  {
    (*lb)[0]=lbbox[0]; (*lb)[1]=lbbox[1]; (*lb)[2]=lbbox[2]; 
	(*rb)[0]=rbbox[0]; (*rb)[1]=rbbox[1]; (*rb)[2]=rbbox[2];
  }

protected:
  virtual void cleanData();

  virtual void readData();
  // read the original data and assign values to 
  //     dataPos
  //     dataVel
  //     dataDen
  //     dataSize


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


};

#endif // _READ_DATA_H

