#include "svBaseColor.h"

#ifndef __SV_BOY_SURFACE_H
#define __SV_BOY_SURFACE_H


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
#include "svBaseColor.h"

#include "./jcUtil/MGL.h"
#include "./jcUtil/MArray.h"
#include "./jcUtil/MVectorMatrix.h"

#define LEGEND_1 36
#define LEGEND_2 37
#define PI 3.1415926


namespace __svl_lib{

/* categorical color scale based on input data */
class svBoySurface:public svBaseColor
{
public:
        svBoySurface(int dataSize);
        ~svBoySurface(){}

	svVector4Array * line2rgb(svVector3Array *vector, int datasize);
	svVector4 * line2rgb(svVector3 *vector, int datasize);
        void convertcolor(char *indir, char *infile, char *outdir, char *outfile);
        void readcolor(char *dir, char *file);
        void readlegend(char *dir, char *file);


        void RenderLegend();
private:
        svFloat blegendx[2];
        svFloat blegendy[2];
};

}
#endif //__SV_BOY_SURFACE_H
