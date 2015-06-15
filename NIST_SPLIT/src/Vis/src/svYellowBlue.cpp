
/* 
 * svYellowBlue.cpp
 * 
 */

#include "svYellowBlue.h"
#include <iostream>
#include <fstream>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "./jcUtil/MGL.h"

using namespace std;

svYellowBlue::svYellowBlue(svScalar *data, int dataSize)
{
  float minData=data[0], maxData=data[0];
  init();

	for(int di=1; di<dataSize; di++)
  	{
	     if(data[di] < minData)
		   minData = data[di];
		 if(data[di] > maxData)
		   maxData = data[di];
	  }; // end for(di)

	  cerr << "...  minData = " << minData;
	  cerr << "...  maxData = " << maxData;

	  // choose the # of categories
	  int minDigit = getNumOfIntegerDigits(minData);
	  int maxDigit = getNumOfIntegerDigits(maxData);

	  minexp = minDigit;
	  maxexp = maxDigit;
  

  colorNumOfCategories = maxDigit-minDigit+1;
  cerr << "...  minDigit = " << minDigit;
  cerr << "...  maxDigit = " << maxDigit;
  cerr << "...  colorNumOfCategories = " << colorNumOfCategories << endl;

  // set the color values
  colors = new svVector4[dataSize];

  if(colorNumOfCategories>=4&&colorNumOfCategories<=12)
  {
	colorNumOfCategories = colorNumOfCategories-4;
  }
  else if(colorNumOfCategories<4)
  {
	colorNumOfCategories = 0;
  }
  else if(colorNumOfCategories>12)
  {
	colorNumOfCategories = 8;
  }

  int numofinteger;
  for(int di=0; di<dataSize; di++)
  {
	numofinteger = maxDigit - getNumOfIntegerDigits(data[di]);
	if(numofinteger>colorNumOfCategories+3)
		colors[di] = brewerColors[colorNumOfCategories][colorNumOfCategories+4-1];
	else
		colors[di] = brewerColors[colorNumOfCategories][numofinteger];
    //colors[di] = brewerColors[colorNumOfCategories-4][getNumOfIntegerDigits(data[di])-minDigit];

  }; // end for(di)
}
svYellowBlue::svYellowBlue(svScalar *data, int dataSize, int min, int max)
{

  init();

	  // choose the # of categories
	  int minDigit = min;
	  int maxDigit = max;

	  minexp = minDigit;
	  maxexp = maxDigit;
  
  colorNumOfCategories = maxDigit-minDigit+1;
  cerr << "...  minDigit = " << minDigit;
  cerr << "...  maxDigit = " << maxDigit;
  cerr << "...  colorNumOfCategories = " << colorNumOfCategories << endl;

  // set the color values
  colors = new svVector4[dataSize];

  if(colorNumOfCategories>=4&&colorNumOfCategories<=12)
  {
	colorNumOfCategories = colorNumOfCategories-4;
  }
  else if(colorNumOfCategories<4)
  {
	colorNumOfCategories = 0;
  }
  else if(colorNumOfCategories>12)
  {
	colorNumOfCategories = 8;
  }

  int numofinteger;
  for(int di=0; di<dataSize; di++)
  {
	numofinteger = maxDigit - getNumOfIntegerDigits(data[di]);//cerr<<numofinteger<<endl;
	if(numofinteger>colorNumOfCategories+3)
		colors[di] = brewerColors[colorNumOfCategories][colorNumOfCategories+4-1];
	else
		colors[di] = brewerColors[colorNumOfCategories][numofinteger];
    //colors[di] = brewerColors[colorNumOfCategories-4][getNumOfIntegerDigits(data[di])-minDigit];

  }; // end for(di)
}
svYellowBlue::svYellowBlue(svScalar *data, int dataSize, int num)
{
  init();
  colors = new svVector4[dataSize];
  for(int di=0; di<dataSize; di++)
  {
	if(num==9)
	{
		if(data[di]<1e+1)		colors[di] = brewerColors[5][7]/255.;
		if(data[di]>=1e+1&&data[di]<1e+2)	colors[di] = brewerColors[5][6]/255.;
		if(data[di]>=1e+2&&data[di]<1e+3)	colors[di] = brewerColors[5][5]/255.;
		if(data[di]>=1e+3&&data[di]<1e+4)	colors[di] = brewerColors[5][4]/255.;
		if(data[di]>=1e+4&&data[di]<1e+5)	colors[di] = brewerColors[5][3]/255.;
		if(data[di]>=1e+5&&data[di]<1e+6)	colors[di] = brewerColors[5][2]/255.;
		if(data[di]>=1e+6&&data[di]<1e+7)	colors[di] = brewerColors[5][1]/255.;
		if(data[di]>=1e+7&&data[di]<1e+8)	colors[di] = brewerColors[5][0]/255.;
		if(data[di]>=1e+8)	colors[di] = brewerColors[5][8];
	}
  }
}
void svYellowBlue::SetColors(svScalar *data, int dataSize, int num)
{
  delete [] colors;
  colors = new svVector4[dataSize];
  for(int di=0; di<dataSize; di++)
  {
	if(num==9)
	{
		if(data[di]<1e+1)		colors[di] = brewerColors[5][0];
		if(data[di]>=1e+1&&data[di]<1e+2)	colors[di] = brewerColors[5][1];
		if(data[di]>=1e+2&&data[di]<1e+3)	colors[di] = brewerColors[5][2];
		if(data[di]>=1e+3&&data[di]<1e+4)	colors[di] = brewerColors[5][3];
		if(data[di]>=1e+4&&data[di]<1e+5)	colors[di] = brewerColors[5][4];
		if(data[di]>=1e+5&&data[di]<1e+6)	colors[di] = brewerColors[5][5];
		if(data[di]>=1e+6&&data[di]<1e+7)	colors[di] = brewerColors[5][6];
		if(data[di]>=1e+7&&data[di]<1e+8)	colors[di] = brewerColors[5][7];
		if(data[di]>=1e+8)	colors[di] = brewerColors[5][8];
	}
  }
}
void svYellowBlue::SetColors(svScalar *data, int dataSize)
{
  delete [] colors;

 /* float minData=data[0], maxData=data[0];

  // get the data rangenumofinteger
  for(int di=1; di<dataSize; di++)
  {
     if(data[di] < minData)
	   minData = data[di];
	 if(data[di] > maxData)
	   maxData = data[di];
  }; // end for(di)
*/
  // choose the # of categories
  int minDigit = minexp;
  int maxDigit = maxexp;
/*
  colorNumOfCategories = maxDigit-minDigit+1;

  // set the color values
  colors = new svVector4[dataSize];

  if(colorNumOfCategories>=4&&colorNumOfCategories<=12)
  {
	colorNumOfCategories = colorNumOfCategories-4;
  }
  else if(colorNumOfCategories<4)
  {
	colorNumOfCategories = 0;
  }
  else if(colorNumOfCategories>12)
  {
	colorNumOfCategories = 8;
  }
*/colors = new svVector4[dataSize];
  int numofinteger;
  for(int di=0; di<dataSize; di++)
  {
	numofinteger = maxDigit - getNumOfIntegerDigits(data[di]);
	if(numofinteger>colorNumOfCategories+3)
		colors[di] = brewerColors[colorNumOfCategories][colorNumOfCategories+4-1];
	else
		colors[di] = brewerColors[colorNumOfCategories][numofinteger];
    //colors[di] = brewerColors[colorNumOfCategories-4][getNumOfIntegerDigits(data[di])-minDigit];

  }; // end for(di)
}
svYellowBlue::~svYellowBlue()
{
  cleanUp();
}
void svYellowBlue::init()
{
	cerr<<"init color"<<endl;
  bcolors = 9;
  //12 colors
  brewerColors = new svVector4Array[bcolors];

  brewerColors[8].add(svVector4(165,0,38, 255));
  brewerColors[8].add(svVector4(215,48,39, 255));
  brewerColors[8].add(svVector4(244,109,67, 255));
  brewerColors[8].add(svVector4(253,174,97, 255));
  brewerColors[8].add(svVector4(254,224,144, 255));
  brewerColors[8].add(svVector4(255,255,191, 255));
  brewerColors[8].add(svVector4(224,243,248, 255));
  brewerColors[8].add(svVector4(171,217,233, 255));
  brewerColors[8].add(svVector4(116,173,209, 255));
  brewerColors[8].add(svVector4(69,117,180, 255));
  brewerColors[8].add(svVector4(49,54,149, 255));
  brewerColors[8].add(svVector4(8,29,88, 255));

 //11 colors
  brewerColors[7].add(svVector4(165,0,38, 255));
  brewerColors[7].add(svVector4(215,48,39, 255));
  brewerColors[7].add(svVector4(244,109,67, 255));
  brewerColors[7].add(svVector4(253,174,97, 255));
  brewerColors[7].add(svVector4(254,224,144, 255));
  brewerColors[7].add(svVector4(255,255,191, 255));
  brewerColors[7].add(svVector4(224,243,248, 255));
  brewerColors[7].add(svVector4(171,217,233, 255));
  brewerColors[7].add(svVector4(116,173,209, 255));
  brewerColors[7].add(svVector4(69,117,180, 255));
  brewerColors[7].add(svVector4(49,54,149, 255));

  //10 colors
  brewerColors[6].add(svVector4(165,0,38, 255));
  brewerColors[6].add(svVector4(215,48,39, 255));
  brewerColors[6].add(svVector4(244,109,67, 255));
  brewerColors[6].add(svVector4(253,174,97, 255));
  brewerColors[6].add(svVector4(254,224,144, 255));
  brewerColors[6].add(svVector4(224,243,248, 255));
  brewerColors[6].add(svVector4(171,217,233, 255));
  brewerColors[6].add(svVector4(116,173,209, 255));
  brewerColors[6].add(svVector4(69,117,180, 255));
  brewerColors[6].add(svVector4(49,54,149, 255));

  //9 colors
  brewerColors[5].add(svVector4(215,48,39, 255));
  brewerColors[5].add(svVector4(244,109,67, 255));
  brewerColors[5].add(svVector4(253,174,97, 255));
  brewerColors[5].add(svVector4(254,224,144, 255));
  brewerColors[5].add(svVector4(255,255,191, 255));
  brewerColors[5].add(svVector4(224,243,248, 255));
  brewerColors[5].add(svVector4(171,217,233, 255));
  brewerColors[5].add(svVector4(116,173,209, 255));
  brewerColors[5].add(svVector4(69,117,180, 255));

  // 8 colors
  brewerColors[4].add(svVector4(215,48,39, 255));
  brewerColors[4].add(svVector4(244,109,67, 255));
  brewerColors[4].add(svVector4(253,174,97, 255));
  brewerColors[4].add(svVector4(254,224,144, 255));
  brewerColors[4].add(svVector4(224,243,248, 255));
  brewerColors[4].add(svVector4(171,217,233, 255));
  brewerColors[4].add(svVector4(116,173,209, 255));
  brewerColors[4].add(svVector4(69,117,180, 255));

  // 7 colors
  brewerColors[3].add(svVector4(215,48,39, 255));
  brewerColors[3].add(svVector4(244,109,67, 255));
  brewerColors[3].add(svVector4(253,174,97, 255));
  brewerColors[3].add(svVector4(254,224,144, 255));
  brewerColors[3].add(svVector4(224,243,248, 255));
  brewerColors[3].add(svVector4(171,217,233, 255));
  brewerColors[3].add(svVector4(116,173,209, 255));
  brewerColors[3].add(svVector4(69,117,180, 255));

  // 6 colors
  brewerColors[2].add(svVector4(215,48,39, 255));
  brewerColors[2].add(svVector4(252,141,89, 255));
  brewerColors[2].add(svVector4(254,224,144, 255));
  brewerColors[2].add(svVector4(224,243,248, 255));
  brewerColors[2].add(svVector4(145,191,219, 255));
  brewerColors[2].add(svVector4(69,117,180, 255));

  // 5 colors
  brewerColors[1].add(svVector4(215,25,28, 255));
  brewerColors[1].add(svVector4(253,174,97, 255));
  brewerColors[1].add(svVector4(255,255,191, 255));
  brewerColors[1].add(svVector4(171,217,233, 255));
  brewerColors[1].add(svVector4(44,123,182, 255));

  // 4 colors
  brewerColors[0].add(svVector4(215,25,28, 255));
  brewerColors[0].add(svVector4(253,174,97, 255));
  brewerColors[0].add(svVector4(171,217,233, 255));
  brewerColors[0].add(svVector4(44,123,182, 255));

}
/*
void svYellowBlue::init()
{
  bcolors = 9;
  //12 colors
  brewerColors = new svVector4Array[bcolors];
  brewerColors[8].add(svVector4(255, 255, 255, 255));
  brewerColors[8].add(svVector4(255, 255, 217, 255));
  brewerColors[8].add(svVector4(237, 248, 177, 255));
  brewerColors[8].add(svVector4(199, 233, 180, 255));
  brewerColors[8].add(svVector4(127, 205, 187, 255));
  brewerColors[8].add(svVector4(65, 182, 196, 255));
  brewerColors[8].add(svVector4(29, 145, 192, 255));
  brewerColors[8].add(svVector4(34, 94, 168, 255));
  brewerColors[8].add(svVector4(37, 52, 148, 255));
  brewerColors[8].add(svVector4(8, 29, 88, 255));
  brewerColors[8].add(svVector4(6, 21, 63, 255));
  brewerColors[8].add(svVector4(3, 10, 30, 255));
  //11 colors

  brewerColors[7].add(svVector4(255, 255, 255, 255));
  brewerColors[7].add(svVector4(255, 255, 217, 255));
  brewerColors[7].add(svVector4(237, 248, 177, 255));
  brewerColors[7].add(svVector4(199, 233, 180, 255));
  brewerColors[7].add(svVector4(127, 205, 187, 255));
  brewerColors[7].add(svVector4(65, 182, 196, 255));
  brewerColors[7].add(svVector4(29, 145, 192, 255));
  brewerColors[7].add(svVector4(34, 94, 168, 255));
  brewerColors[7].add(svVector4(37, 52, 148, 255));
  brewerColors[7].add(svVector4(8, 29, 88, 255));
  brewerColors[7].add(svVector4(6, 21, 63, 255));
  //10 colors

  brewerColors[6].add(svVector4(255, 255, 255, 255));
  brewerColors[6].add(svVector4(255, 255, 217, 255));
  brewerColors[6].add(svVector4(237, 248, 177, 255));
  brewerColors[6].add(svVector4(199, 233, 180, 255));
  brewerColors[6].add(svVector4(127, 205, 187, 255));
  brewerColors[6].add(svVector4(65, 182, 196, 255));
  brewerColors[6].add(svVector4(29, 145, 192, 255));
  brewerColors[6].add(svVector4(34, 94, 168, 255));
  brewerColors[6].add(svVector4(37, 52, 148, 255));
  brewerColors[6].add(svVector4(8, 29, 88, 255));
  //9 colors

  brewerColors[5].add(svVector4(255, 255, 217, 255));
  brewerColors[5].add(svVector4(237, 248, 177, 255));
  brewerColors[5].add(svVector4(199, 233, 180, 255));
  brewerColors[5].add(svVector4(127, 205, 187, 255));
  brewerColors[5].add(svVector4(65, 182, 196, 255));
  brewerColors[5].add(svVector4(29, 145, 192, 255));
  brewerColors[5].add(svVector4(34, 94, 168, 255));
  brewerColors[5].add(svVector4(37, 52, 148, 255));
  brewerColors[5].add(svVector4(8, 29, 88, 255));
  // 8 colors
  brewerColors[4].add(svVector4(255, 255, 217, 255));
  brewerColors[4].add(svVector4(237, 248, 177, 255));
  brewerColors[4].add(svVector4(199, 233, 180, 255));
  brewerColors[4].add(svVector4(127, 205, 187, 255));
  brewerColors[4].add(svVector4(65, 182, 196, 255));
  brewerColors[4].add(svVector4(29, 145, 192, 255));
  brewerColors[4].add(svVector4(34, 94, 168, 255));
  brewerColors[4].add(svVector4(12, 44, 132, 255));
  // 7 colors
  brewerColors[3].add(svVector4(255, 255, 204, 255));
  brewerColors[3].add(svVector4(199, 233, 180, 255));
  brewerColors[3].add(svVector4(127, 205, 187, 255));
  brewerColors[3].add(svVector4(65, 182, 196, 255));
  brewerColors[3].add(svVector4(29, 145, 192, 255));
  brewerColors[3].add(svVector4(34, 94, 168, 255));
  brewerColors[3].add(svVector4(12, 44, 132, 255));

  // 6 colors
  brewerColors[2].add(svVector4(255, 255, 204, 255));
  brewerColors[2].add(svVector4(199, 233, 180, 255));
  brewerColors[2].add(svVector4(127, 205, 187, 255));
  brewerColors[2].add(svVector4(65, 182, 196, 255));
  brewerColors[2].add(svVector4(44, 127, 184, 255));
  brewerColors[2].add(svVector4(37, 52, 148, 255));

  // 5 colors
  brewerColors[1].add(svVector4(255, 255, 204, 255));
  brewerColors[1].add(svVector4(161, 218, 180, 255));
  brewerColors[1].add(svVector4(65, 182, 196, 255));
  brewerColors[1].add(svVector4(44, 127, 184, 255));
  brewerColors[1].add(svVector4(37, 52, 148, 255));

  // 4 colors
  brewerColors[0].add(svVector4(255, 255, 204, 255));
  brewerColors[0].add(svVector4(161, 218, 180, 255));
  brewerColors[0].add(svVector4(65, 182, 196, 255));
  brewerColors[0].add(svVector4(34, 94, 168, 255));
}*/

void svYellowBlue::SetColorsFromData(svScalar *data, int dataSize)
{
   // use categorical 
   //data 
}

void svYellowBlue::SetColorsFromData(svScalar *data, int dataSize, int num)
{	
  //delete [] colors;
  //colors = new svVector4[dataSize];cerr<<"1"<<endl;
  for(int di=0; di<dataSize; di++)
  {
	if(num==9)
	{
		if(data[di]<1e+1)		colors[di] = brewerColors[5][0];
		if(data[di]>=1e+1&&data[di]<1e+2)	colors[di] = brewerColors[5][1];
		if(data[di]>=1e+2&&data[di]<1e+3)	colors[di] = brewerColors[5][2];
		if(data[di]>=1e+3&&data[di]<1e+4)	colors[di] = brewerColors[5][3];
		if(data[di]>=1e+4&&data[di]<1e+5)	colors[di] = brewerColors[5][4];
		if(data[di]>=1e+5&&data[di]<1e+6)	colors[di] = brewerColors[5][5];
		if(data[di]>=1e+6&&data[di]<1e+7)	colors[di] = brewerColors[5][6];
		if(data[di]>=1e+7&&data[di]<1e+8)	colors[di] = brewerColors[5][7];
		if(data[di]>=1e+8)	colors[di] = brewerColors[5][8];
	}
  }
}
int svYellowBlue::getNumOfIntegerDigits(float num)
{
	  int multiplier = 0;

	  svScalar numCopy = num;

	  if(numCopy<1&&numCopy!=0) 
	  {
	    while ((fabs(numCopy-1.)>0.000001)&&(numCopy-1.)<0.000001)
		{

		  multiplier--;//cerr<<num<<" "<<multiplier<<endl;
		  numCopy *= 10;
		}; // end while(1)
	  }
	  else if(numCopy==0)
		multiplier=0;
	  else if(numCopy>=1)
	  {
	    while(1)
		{
		  if(numCopy>=0 && numCopy <10)
		  {
		    break;
		  };
		  multiplier++;
		  numCopy /= 10.;
		}; // end while (1)
	  }; // end if(numCopy)

	  return multiplier;	
}

void svYellowBlue::BuildLegendList(svScalar *data, int dataSize)
{
  theList = glGenLists(1);
  if(theList==0)
  {
	glDeleteLists(theList, 1);
	theList = glGenLists(1);
  }
  glNewList(theList, GL_COMPILE);

  float minData=data[0], maxData=data[0];

  // get the data range
  for(int di=1; di<dataSize; di++)
  {
     if(data[di] < minData)
	   minData = data[di];
	 if(data[di] > maxData)
	   maxData = data[di];
  }; // end for(di)

  // choose the # of categories
  int minDigit = getNumOfIntegerDigits(minData);
  int maxDigit = getNumOfIntegerDigits(maxData);

  colorNumOfCategories = maxDigit-minDigit+1;

  if(colorNumOfCategories>=4&&colorNumOfCategories<=9)
  {
	colorNumOfCategories = colorNumOfCategories-4;
  }
  else if(colorNumOfCategories<4)
  {
	colorNumOfCategories = 0;
  }
  else if(colorNumOfCategories>9)
  {
	colorNumOfCategories = 5;
  }

  char str[10];
  float x = 0.;
  float y = 0.;
  float z = 0.;
  float size = 1.;	
 
  int j = 0;
  int num = maxDigit-minDigit+1 - (colorNumOfCategories+4);
  for(int i=maxDigit-minDigit;i>=0;i--)
  {
	glColor4f(brewerColors[colorNumOfCategories][j][0]/255.,
		brewerColors[colorNumOfCategories][j][1]/255.,
		brewerColors[colorNumOfCategories][j][2]/255.,
		brewerColors[colorNumOfCategories][j][3]/255.);
	if(i>num) j ++;
	glBegin(GL_QUADS);
	glVertex3f(x,y,z);
	glVertex3f(x,y-size,z);
	glVertex3f(x-size,y-size,z);
	glVertex3f(x-size,y,z);
	glEnd();
	glColor4f(1,1,1,1);
	glRasterPos3f(x,y-0.2,z);
	if(minexp+i-DEN_SCALAR<0)
		sprintf(str,"1e%-d",(i-DEN_SCALAR+minexp));
	else
		sprintf(str,"1e%d",(minDigit+i-12));
	for(int ii=0;ii<strlen(str);ii++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[ii]);
	y = y - size;

	legendx.push_back(x);
	legendy.push_back(y);
  }
  legendmin = minDigit;
  legendmax = maxDigit;
  legendnum = colorNumOfCategories;
	glEndList();
}
void svYellowBlue::RenderLegend()
{
	//glCallList(theList);
  char str[10];
  float x = 0.;
  float y = 0.;
  float z = 0.;
  float size = 1.;	
 
  int j = 0;
  int num = maxexp-minexp+1;
	if(num>12)
	{
		legendnum = 8;
	}
	else if(num<4)
	{
		legendnum = 0;
	}
	else
	{
		legendnum = maxexp - minexp - 4;
	}
 /* for(int i=0;i<num;i++)
  {
	glColor4f(brewerColors[legendnum][j][0]/255.,
		brewerColors[legendnum][j][1]/255.,
		brewerColors[legendnum][j][2]/255.,
		brewerColors[legendnum][j][3]/255.);
	if(i<legendnum+3) j ++;
	glBegin(GL_QUADS);
	glVertex2f(x,y);
	glVertex2f(x,y-size);
	glVertex2f(x+size,y-size);
	glVertex2f(x+size,y);
	glEnd();
	glColor4f(1,1,1,1);
	glRasterPos2f(x+size*1.1,y-0.2);
	if((maxexp-DEN_SCALAR)<0)
		sprintf(str,"1e%-d",(maxexp-i-DEN_SCALAR));
	else
		sprintf(str,"1e%d",maxexp-i-DEN_SCALAR);
	for(int ii=0;ii<strlen(str);ii++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[ii]);
	y = y + size;
  }*/
 
  j = legendnum + 3;
  for(int i=0;i<num;i++)
  {
	glColor4f(brewerColors[legendnum][j][0]/255.,
		brewerColors[legendnum][j][1]/255.,
		brewerColors[legendnum][j][2]/255.,
		brewerColors[legendnum][j][3]/255.);
	if(i>num-legendnum-4) j --;
	glBegin(GL_QUADS);
	glVertex2f(x,y);
	glVertex2f(x+size,y);
	glVertex2f(x+size,y+size);
	glVertex2f(x,y+size);
	glEnd();
	glColor4f(1,1,1,1);
	glRasterPos2f(x+size*1.1,y);
	if((maxexp-DEN_SCALAR)<0)
		sprintf(str,"1e%-d",(i-DEN_SCALAR));
	else
		sprintf(str,"1e%d",i-DEN_SCALAR);
	for(int ii=0;ii<strlen(str);ii++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[ii]);
	y = y + size;
  }
}
void svYellowBlue::RenderLegend(int num)
{
	glColor4f(1,1,1,1);
	glRasterPos3f(0,-6,0);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'M');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'a');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'g');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'n');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'i');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 't');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'u');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'd');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'e');
	float x = -1;
	float y = -4;
	float z = 0;

	char name[100] = {'1','0','^','-','4', '1','0','^','-','5', '1','0','^','-','6', '1','0','^','-','7', 
				'1','0','^','-','8','1','0','^','-','9',
				'1','0','^','-','1','0',
				'1','0','^','-','1','1',
				'1','0','^','-','1','2'};
	for(int i=1;i<=num;i++)
	{
		glColor4f(brewerColors[num-4][num-i][0]/255,brewerColors[num-4][num-i][1]/255,brewerColors[num-4][num-i][2]/255,brewerColors[num-4][num-i][3]/255);
		//glColor4f(1,0,0,1);
		glBegin(GL_QUADS);
		glVertex3f(x,y,z);
		glVertex3f(x+1,y,z);
		glVertex3f(x+1,y+1,z);
		glVertex3f(x,y+1,z);
		glEnd();
		glColor4f(1,1,1,1);
		glRasterPos3f(x+2,y+0.5,z);
		if(i<=6)
		{
			for(int j =0;j<5;j++)glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, name[(i-1)*5+j]);
		}
		else
		{
			for(int j =0;j<6;j++)glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, name[30+(i-1-6)*6+j]);
		}
		y = y + 1;
	}
}

void svYellowBlue::cleanUp()
{
  if(brewerColors != NULL)
  {
    for(int i=0; i<bcolors; i++)
      brewerColors[i].free();
  }
  delete [] brewerColors;
}

