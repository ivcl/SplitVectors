
#ifndef __SV_YELLOW_BLUE_H
#define __SV_YELLOW_BLUE_H


#include <iostream>
#include <fstream>
#include <cmath>
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
using namespace std;

using namespace __svl_lib;

/* categorical color scale based on input data */
class svYellowBlue : public svBaseColor
{
  public: 
    svYellowBlue() {colors=NULL; init(); colorNumOfCategories=0;}
    svYellowBlue(svScalar *data, int dataSize);
    svYellowBlue(svScalar *data, int dataSize, int min, int max);
    svYellowBlue(svScalar *data, int dataSize, int num);
    virtual ~svYellowBlue();

    //svVector4* GetColors() {return colors;}
	void SetColorsFromData(svScalar *data, int dataSize);
void SetColorsFromData(svScalar *data, int dataSize, int num);
	void BuildLegendList(svScalar *data, int dataSize);
	void RenderLegend(int num);
	void RenderLegend();
     void SetColors(svScalar *data, int dataSize, int num);
	 void SetColors(svScalar *data, int dataSize);

	int GetMin(){return minexp;}
	int GetMax(){return maxexp;}
	void SetScalar(int s){DEN_SCALAR = s;}
  protected:
	void init();
	int  getNumOfIntegerDigits(float num);

    void cleanUp();

	svVector4Array  *brewerColors;
	int              bcolors; // =6; --> 4, 5, 6, 7, 8, 9

  private:
    //svVector4 *colors;
	int        colorNumOfCategories;
	GLuint theList;

	int minexp;
	int maxexp;

	vector<int> legendx;
	vector<int> legendy;
	int legendmax,legendmin,legendnum;
	int DEN_SCALAR;
};

#endif //__SV_YELLOW_BLUE_H
