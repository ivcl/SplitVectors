  /**------------------------------------------------------------------
Convert it from Keqin's code
Just remove QT part
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

#include "./jcUtil/MGL.h"
#include "./jcUtil/MArray.h"
#include "./jcUtil/MVectorMatrix.h"

#ifndef PI
#define PI 3.1415926
#endif

using namespace std;

namespace __svl_lib {

class svSpiralColor{
public:
	svVector3 dividingLCHuvforRGB(double L, double C1, double C2, double H);
	svVector3 CIEluvtoRGB(double Y, double u, double v);
	svVector3 SinglehueSequential(double t, double s, double b, double c, double h);
	void closestLCHuvtoRGB(double L, double C, double H, double &r, double &g, double &b);
	void SetSpiralColor();
private:
	svVector4Array *spiralcolors;
	int zlayer;
	int dataSize;
};

}
