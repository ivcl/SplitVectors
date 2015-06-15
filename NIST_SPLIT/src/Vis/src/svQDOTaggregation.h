/**------------------------------------------------------------------
09/02/2014
---------------------------------------------------------------------
*/

#ifndef __QDOT_AGGREGATION_H
#define __QDOT_AGGREGATION_H

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

#include "./jcUtil/MGL.h"
#include "./jcUtil/MArray.h"
#include "./jcUtil/MVectorMatrix.h"

using namespace std;

namespace __svl_lib {

class svQDOTaggregation{

public:
    svQDOTaggregation(svVector3Array *pos, svScalarArray *dir, svScalarArray *den,
			svScalarArray *exp, svScalarArray *coe, svVector3Array *col,
                      int num, int layer, int xn, int yn, float xl,float yl);
    void SetData(svVector3Array *pos, svScalarArray *dir,svScalarArray *den,
			svScalarArray *exp, svScalarArray *coe,svVector3Array *col,
                 int num, int layer, int xn, int yn, float xl,float yl);
    ~svQDOTaggregation(){cleanup();}

    void SetAggregation(float pixel);
    void SetSampleAggregation(float pixel);

    svVector3Array *GetPosition(){return dataAggPos;}
    svScalarArray *GetOrientation(){return dataAggOri;}
    svScalarArray *GetDensity(){return dataAggDen;}
    svScalarArray *GetExp(){return dataAggExp;}
    svScalarArray *GetCoe(){return dataAggCoe;}

    void cleanupaggregation();

private:
    svVector3Array *dataPos;
    svScalarArray *dataDir;
    svScalarArray *dataDen;
    svScalarArray *dataExp;
    svScalarArray *dataCoe;
    svVector3Array *dataCol;

    svVector3Array *dataAggPos;
    svScalarArray *dataAggOri;
    svScalarArray *dataAggExp;
    svScalarArray *dataAggCoe;
    svScalarArray *dataAggDen;
    svVector3Array *dataAggCol;

    svScalarArray *dataMax;  
    svScalarArray *dataMaxExp;
    svScalarArray *dataMaxCoe;
    svScalarArray *dataMaxDen;
    svVector3Array *dataMaxPos;
    svVector3Array *dataMaxPos2;

    svVector3Array *dataXY;
    int dataSize;
    int zlayer;

protected:
    void cleanup();
};
}
#endif // _QDOT_DISTANCE_FUNCTION_H

