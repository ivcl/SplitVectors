  /**------------------------------------------------------------------
02/02/2015
---------------------------------------------------------------------
*/

#ifndef __QDOT_DISTANCE_FUNCTION_H
#define __QDOT_DISTANCE_FUNCTION_H

#include <iostream>
#include <fstream>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <vector>

#include "svType.h"
#include "svVectorMatrix.h"
#include "svArray.h"
#include "svUtil.h"
#include "CIEColor.h"

#include "./jcUtil/MGL.h"
#include "./jcUtil/MArray.h"
#include "./jcUtil/MVectorMatrix.h"

#ifndef PI
#define PI 3.1415926
#endif

using namespace std;

namespace __svl_lib {
class QDOTdistancefunction {
public:
  QDOTdistancefunction(){dataSize =0;datalayerpos=NULL;datalayerdis=NULL;dataPos=NULL;dataVel=NULL;
		dataNei_inspace_1=NULL;dataNei_inspace_2=NULL;dataNei_onelayer_1=NULL;dataNei_onelayer_2=NULL;
		datalayerdis2=NULL;datalayercol=NULL;}
  virtual ~QDOTdistancefunction(){cleanData();}

  void SetData(svVector3Array *pos,svVector3Array *vel,svInt num,int znum,
	svVector3 l, svVector3 r);

  void ReadNei(char *file);

  void SetNeighbours_Multiplelayer();
  void SetNeighbours_Onelayer(int n);
  void SetNeighbours_Ofspace(int n);
  void SetGlobalFunction(int x, int y);
  void SetPlaneClusterFunction(int *idx, svIntArray *n1, svIntArray *n2);
  void SetPlaneCosFunction(svIntArray *n1, svIntArray *n2);
  void SetPlaneCosClusterFunction(int *idx, svIntArray *n1, svIntArray *n2);//, int n);

  int GetXlength(){return x_num;}
  int GetYlength(){return y_num;}

  svVector3Array *GetLayerPos(){return datalayerpos;}
  svVector3Array GetLayerPos(int n){return datalayerpos[n];}
  svScalarArray *GetLayerDis(){return datalayerdis;}
  svScalarArray *GetLayerDis2(){return datalayerdis2;}
  svVector3Array *GetLayerCol(){return datalayercol;}

  svVector3Array GetPos(int n){return dataPos[n];}

  int SetNei(int *n1, int *n2){for(int i=0;i<dataSize;i++)
				{
					dataNei_planecomparsion1[i] = n1[i];
					dataNei_planecomparsion2[i] = n2[i];
				}
				}

  float GetXd(){return xd;}
  float GetYd(){return yd;}
    float GetXl(){return xl;}
  float GetYl(){return yl;}

  svVector3 GetCenter(){return center;}

protected:
  virtual void cleanData();

private:
  svVector3Array *dataPos;
  svVector3Array *dataVel;

  svVector3Array *datalayercol;
  svVector3Array *datalayerpos;
  svScalarArray *datalayerdis;
  svScalarArray *datalayerdis2;

  //svIntArray *datacorner;

  svIntArray *dataNei_inspace_1;
  svIntArray *dataNei_inspace_2;
  svIntArray *dataNei_onelayer_1;
  svIntArray *dataNei_onelayer_2;
  svIntArray *dataNei_multiplelayer_1;
  svIntArray *dataNei_multiplelayer_2;
  int *dataNei_planecomparsion1;
  int *dataNei_planecomparsion2;

  int dataSize;
  int zlayer;

   int y_num;
   int x_num;

	float xd;
	float yd;
	float xl;
	float yl;

   svVector3 lbbox;
   svVector3 rbbox;
   svVector3 center;
};
}
#endif // _QDOT_DISTANCE_FUNCTION_H

