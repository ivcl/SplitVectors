
#ifndef __SV_CUT_H
#define __SV_CUT_H


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

namespace __svl_lib{

class svCut
{
public:
	svCut(svVector3Array *pos, svVector4Array *col);
	~svCut(){cleanup();}

	void SetCut(svVector3Array *pos, svVector4Array *col);

	void SetCutRegion(svVector3 l, svVector3 r, int lable);

	void RenderCutting(float z,bool *inbox);
	void SetColors(svVector4 *col);

	void SetNBlockView(GLfloat *m, int num, svVector3 l, svVector3 r);

	int Frequency;

	bool ONE;
	bool OTHER;
protected:
	void cleanup();
private:
	svVector3Array *wholepos;
	svVector4Array *wholecol;
	svVector3Array *projectionpos;
	svVector4Array *projectioncol;
	svInt *projectionlable;

	svInt projectionnum;

	GLfloat tb_transform[4][4];

	svVector3 lbbox;
	svVector3 rbbox;
	float planexy;
	float planexz;
	float planeyz;
	svVector3 rb;
};

}
#endif //__SV_CUT_H
