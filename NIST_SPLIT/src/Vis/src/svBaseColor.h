#ifndef __SV_BASE_COLOR_H
#define __SV_BASE_COLOR_H

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

#define YELLOW_BLUE_COLOR 10
#define BOY_SURFACE_COLOR 11
#define PARAVIEW_X_COLOR 12
#define PARAVIEW_Y_COLOR 13
#define PARAVIEW_Z_COLOR 14
#define PARAVIEW_MAG_COLOR 15
using namespace std;
namespace __svl_lib{

/* the base class of color */
class svBaseColor {
public:
        svVector4* GetColors(){return colors;}
	svVector4Array *GetColorsArray(){if(colorsarray!=NULL)colorsarray[0].free();delete [] colorsarray;colorsarray=NULL;colorsarray = new svVector4Array[1];
			for(int i=0;i<colornum;i++)colorsarray[0].add(colors[i]);
			return colorsarray;}
        svVector4  GetColors(int num){return colors[num];}

        void SetMode(svScalar n){ColorMode = n;}

        svBaseColor(){colors=NULL;ColorMode = BOY_SURFACE_COLOR;legendPos=NULL;legendCol=NULL;colorsarray=NULL;}
        ~svBaseColor(){clean();}

        void SetColor(svVector4Array *c, int datasize);
        void SetColor(svVector4 *c, int datasize);
	
	svVector3 RGB2LAB(svVector4 rgb);
	svVector4 LAB2RGB(svVector3 lab);
	
	void ColorBlindSafe();
	svVector4Array * ColorBlindSafe(svVector4Array * c, int datasize);

	svVector4 *Color2Gray(int datasize){svVector4 *cc=new svVector4[datasize]; for(int i=0;i<datasize;i++){cc[i][0]=1;cc[i][1]=1;cc[i][2]=1;cc[i][3]=1;}return cc;}
protected:
        void clean();
        svVector4 *colors;
	svVector4Array *colorsarray;
        int        colornum;
        svScalar ColorMode;

        svVector3 *legendPos;
        svVector4 *legendCol;
};
}
#endif //__SV_BASE_COLOR_H

