/*  
 * svIconGlyph
 */

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <fstream>
#include <string.h>
#include <cmath>

#include "svIconGlyph.h"
#include "svException.h"
#include "svUtil.h"


#ifndef PI
#define PI 3.1415926
#endif

using namespace std;


namespace __svl_lib {

svIconGlyph::svIconGlyph( svVector3 l, svVector3 r,int dataSize,
     svVector3 *posIn, svVector3 *velIn,  svScalar *denIn,
	 svVector4 *color4In1,svVector4 *color4In2)
{
  float tmpden;

  // data attributes
  glyphPos = new svVector3Array[1]; // starting and ending positions
  glyphDir = new svVector3Array[1];
  glyphMag = new svScalar[dataSize];
  glyphScale = new svScalar[dataSize];
  thetax = new float[dataSize];
  thetay = new float[dataSize];
  thetaz = new float[dataSize];
  exp= new float[dataSize];
  coe = new float[dataSize];
  glyphFrequency = 1;
  lb = l;
  rb = r;

  // drawing attributes
  glyphColors1 = new svVector4Array[1];
  glyphColors2 = new svVector4Array[1];

  cerr << "... Setting IconGlyph ..." << endl;

  for(int j=0; j<dataSize; j++)
  {
    	// set position
   	glyphPos[0].add(posIn[j]);

   	// set vector dir
	svVector3 nvel = normalize(velIn[j]);
	glyphDir[0].add(nvel);

	//set the angles
	thetax[j] = acos(nvel[0]);
	thetay[j] = acos(nvel[1]);
	thetaz[j] = acos(nvel[2]);

	//set magnitude
	glyphMag[j]=(denIn[j]);

	//set exp and coeffecient
	tmpden = fabs(denIn[j]);
	exp[j]=0;
	if(tmpden==0)
	{
		exp[j]=0;
		coe[j]=0;
	}
	else if(tmpden>=1)
	{
		while(tmpden>=10.)
		{
			tmpden=tmpden/10.; 
			exp[j]=exp[j]+1;
		}
	}
	else
	{
		while(tmpden<1)
		{
			tmpden=tmpden*10.;
			exp[j]=exp[j]-1;
		}
	}
	double tmppow = 1.;
	for(int iii=0;iii<exp[j];iii++)
		tmppow = tmppow * 10.;
	coe[j]=denIn[j]/tmppow;//pow(10.,exp[j]);
	exp[j] = exp[j];
	//set colors	
    	if(color4In1!=NULL)  
	{
	  glyphColors1[0].add(svVector4(color4In1[j][0]/255., color4In1[j][1]/255., color4In1[j][2]/255., color4In1[j][3]/255.));
	}
	else  
	{
	  glyphColors1[0].add(svVector4(1,1,1,0)); // default white
	}
    	if(color4In2!=NULL)  
	{
	  glyphColors2[0].add(svVector4(color4In2[j][0], color4In2[j][1], color4In2[j][2], color4In2[j][3]));
	}
	else  
	{
	  glyphColors2[0].add(svVector4(1,1,1,0)); // default white
	}
	
  }; // end for(j)

  cerr << "... Done init IconGlyph ... " << endl;
}
void svIconGlyph::SetData(int dataSize,
     svVector3 *posIn, svVector3 *velIn,  svScalar *denIn,
	 svVector4 *color4In1,svVector4 *color4In2)
{
  cleanup();

  float tmpden;

  // data attributes
  glyphPos = new svVector3Array[1]; // starting and ending positions
  glyphDir = new svVector3Array[1];
  glyphMag = new svScalar[dataSize];
  glyphScale = new svScalar[dataSize];
  thetax = new float[dataSize];
  thetay = new float[dataSize];
  thetaz = new float[dataSize];
  exp= new float[dataSize];
  coe = new float[dataSize];
  glyphFrequency = 1;

  // drawing attributes
  glyphColors1 = new svVector4Array[1];
  glyphColors2 = new svVector4Array[1];

  //cerr << "... Setting IconGlyph ..." << endl;

//  maxexp = -9e+9;

  for(int j=0; j<dataSize; j++)
  {
    	// set position
   	glyphPos[0].add(posIn[j]);

   	// set vector dir
	svVector3 nvel = normalize(velIn[j]);
	glyphDir[0].add(nvel);

	//set the angles
	thetax[j] = acos(nvel[0]);
	thetay[j] = acos(nvel[1]);
	thetaz[j] = acos(nvel[2]);

	//set magnitude
	glyphMag[j]=(denIn[j]);

	//set exp and coeffecient
	tmpden = fabs(denIn[j]);
	exp[j]=0;
	if(tmpden==0)
	{
		exp[j]=0;
		coe[j]=0;
	}
	else if(tmpden>=1)
	{
		while(tmpden>=10.)
		{
			tmpden=tmpden/10.; 
			exp[j]=exp[j]+1;
		}
	}
	else
	{
		while(tmpden<1)
		{
			tmpden=tmpden*10.;
			exp[j]=exp[j]-1;
		}
	}
	double tmppow = 1.;
	for(int iii=0;iii<exp[j];iii++)
		tmppow = tmppow * 10.;
	coe[j]=denIn[j]/tmppow;//pow(10.,exp[j]);
	exp[j] = exp[j];
	//if(exp[j]>maxexp)
	//	maxexp = exp[j];

	//set colors	
    	if(color4In1!=NULL)  
	{
	  glyphColors1[0].add(svVector4(color4In1[j][0]/255., color4In1[j][1]/255., color4In1[j][2]/255., color4In1[j][3]/255.));
	}
	else  
	{
	  glyphColors1[0].add(svVector4(1,1,1,0)); // default white
	}
    	if(color4In2!=NULL)  
	{
	  glyphColors2[0].add(svVector4(color4In2[j][0], color4In2[j][1], color4In2[j][2], color4In2[j][3]));
	}
	else  
	{
	  glyphColors2[0].add(svVector4(1,1,1,0)); // default white
	}
	
  }; // end for(j)

 // cerr << "... Done init IconGlyph ... " << endl;
}

void svIconGlyph::GenerateDisplay(svVector3 l,svVector3 r)
{
	glEnable( GL_LINE_SMOOTH );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        theList = glGenLists(1);
	if(theList==0)
	{
		glDeleteLists(theList, 1);
		theList = glGenLists(1);
	}
        glNewList(theList, GL_COMPILE);

	int i,j;
	int size = glyphPos[0].size();
	float radius=0.15;
	float barlength;

	for(i=0;i<size;)
	{	
		if(glyphPos[0][i][0]>=l[0]&&glyphPos[0][i][0]<=r[0]
			&&glyphPos[0][i][1]>=l[1]&&glyphPos[0][i][1]<=r[1]
			&&glyphPos[0][i][2]>=l[2]&&glyphPos[0][i][2]<=r[2])
		{
			barlength = 2*radius*coe[i]/10.;

			glColor3f(glyphColors1[0][i][0],glyphColors1[0][i][1],glyphColors1[0][i][2]);
			glBegin(GL_QUADS);
			glVertex3f(glyphPos[0][i][0]-radius/2.,glyphPos[0][i][1]-radius,glyphPos[0][i][2]);
			glVertex3f(glyphPos[0][i][0]-radius/2.,glyphPos[0][i][1]-radius+barlength,glyphPos[0][i][2]);
			glVertex3f(glyphPos[0][i][0]-radius,glyphPos[0][i][1]-radius+barlength,glyphPos[0][i][2]);
			glVertex3f(glyphPos[0][i][0]-radius,glyphPos[0][i][1]-radius,glyphPos[0][i][2]);
			glEnd();
			glBegin(GL_QUADS);
			glVertex3f(glyphPos[0][i][0],glyphPos[0][i][1]-radius+barlength,glyphPos[0][i][2]);
			glVertex3f(glyphPos[0][i][0],glyphPos[0][i][1]+radius,glyphPos[0][i][2]);
			glVertex3f(glyphPos[0][i][0]-radius,glyphPos[0][i][1]+radius,glyphPos[0][i][2]);
			glVertex3f(glyphPos[0][i][0]-radius,glyphPos[0][i][1]-radius+barlength,glyphPos[0][i][2]);
			glEnd();

/*
			glBegin(GL_QUADS);
			glVertex3f(glyphPos[0][i][0]-radius,glyphPos[0][i][1]-radius,glyphPos[0][i][2]);
			glVertex3f(glyphPos[0][i][0],glyphPos[0][i][1]-radius,glyphPos[0][i][2]);
			glVertex3f(glyphPos[0][i][0],glyphPos[0][i][1]+radius,glyphPos[0][i][2]);
			glVertex3f(glyphPos[0][i][0]-radius,glyphPos[0][i][1]+radius,glyphPos[0][i][2]);
			glEnd();*/

			glColor3f(glyphColors2[0][i][0],glyphColors2[0][i][1],glyphColors2[0][i][2]);
			glBegin(GL_QUADS);
			glVertex3f(glyphPos[0][i][0],glyphPos[0][i][1]-radius,glyphPos[0][i][2]);
			glVertex3f(glyphPos[0][i][0]+radius,glyphPos[0][i][1]-radius,glyphPos[0][i][2]);
			glVertex3f(glyphPos[0][i][0]+radius,glyphPos[0][i][1]+radius,glyphPos[0][i][2]);
			glVertex3f(glyphPos[0][i][0],glyphPos[0][i][1]+radius,glyphPos[0][i][2]);
			glEnd();

			glColor3f(0.,0.,0.);
			glBegin(GL_QUADS);
			glVertex3f(glyphPos[0][i][0],glyphPos[0][i][1]-radius,glyphPos[0][i][2]);
			glVertex3f(glyphPos[0][i][0],glyphPos[0][i][1]-radius+barlength,glyphPos[0][i][2]);
			glVertex3f(glyphPos[0][i][0]-radius/2.,glyphPos[0][i][1]-radius+barlength,glyphPos[0][i][2]);
			glVertex3f(glyphPos[0][i][0]-radius/2.,glyphPos[0][i][1]-radius,glyphPos[0][i][2]);
			glEnd();
		}
		i = i + glyphFrequency;
	}

	glEndList();
}
void svIconGlyph::GenerateDisplay(float *x, float *y, int *index, int num)
{
	glEnable( GL_LINE_SMOOTH );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        theList = glGenLists(1);
	if(theList==0)
	{
		glDeleteLists(theList, 1);
		theList = glGenLists(1);
	}
        glNewList(theList, GL_COMPILE);

	int i,j;
	int size = glyphPos[0].size();
	float radius=0.15;
	float barlength;

	for(int jj=0;jj<num;jj++)
	{	
		i = index[jj];

		barlength = 2*radius*coe[i]/10.;

		glColor3f(glyphColors1[0][i][0],glyphColors1[0][i][1],glyphColors1[0][i][2]);
		glBegin(GL_QUADS);
		glVertex2f(x[jj]-radius/2.,y[jj]-radius);
		glVertex2f(x[jj]-radius/2.,y[jj]-radius+barlength);
		glVertex2f(x[jj]-radius,y[jj]-radius+barlength);
		glVertex2f(x[jj]-radius,y[jj]-radius);
		glEnd();
		glBegin(GL_QUADS);
		glVertex2f(x[jj],y[jj]-radius+barlength);
		glVertex2f(x[jj],y[jj]+radius);
		glVertex2f(x[jj]-radius,y[jj]+radius);
		glVertex2f(x[jj]-radius,y[jj]-radius+barlength);
		glEnd();

		glColor3f(glyphColors2[0][i][0],glyphColors2[0][i][1],glyphColors2[0][i][2]);
		glBegin(GL_QUADS);
		glVertex2f(x[jj],y[jj]-radius);
		glVertex2f(x[jj]+radius,y[jj]-radius);
		glVertex2f(x[jj]+radius,y[jj]+radius);
		glVertex2f(x[jj],y[jj]+radius);
		glEnd();

		glColor3f(0.,0.,0.);
		glBegin(GL_QUADS);
		glVertex2f(x[jj],y[jj]-radius);
		glVertex2f(x[jj],y[jj]-radius+barlength);
		glVertex2f(x[jj]-radius/2.,y[jj]-radius+barlength);
		glVertex2f(x[jj]-radius/2.,y[jj]-radius);
		glEnd();

		i = i + glyphFrequency;
	}

	glEndList();
}

//the power is from -20 to 20
void svIconGlyph::GenerateDisplay(float *x, float *y, int num,float R)
{
	glEnable( GL_LINE_SMOOTH );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        theList = glGenLists(1);
	if(theList==0)
	{
		glDeleteLists(theList, 1);
		theList = glGenLists(1);
	}
        glNewList(theList, GL_COMPILE);

	int i,j,k;
	int section;
	int size = glyphPos[0].size();
	svVector3 z1,z2;
	svVector3 draw,startp;
	svVector3 p;
	float bar;
	float angle,subangle;
	float radius;
	if(R<1)
		radius=R/4.;
	else 	
		radius=0.25;
	float deltar=0.01;
	int round;
	float barlength=radius;
	float barexplength = radius;
	float base;
	z1[0]=0.;z1[1]=0.;z1[2]=1.;
	z2[0]=0.;z2[1]=0.;z2[2]=-1.;

	float max,min;
	max = -9e+9;min = 9e+9;
	for(int jj=0;jj<num;jj++)
	{	
		if(x[jj]>max)
			max = x[jj];
		if(y[jj]>max)
			max = y[jj];
		if(x[jj]<min)
			min = x[jj];
		if(y[jj]<min)
			min = y[jj];				
	}

	float scale;

	if(windowswidth>windowsheight)
		scale = windowsheight/(max - min);
	else
		scale = windowswidth/(max - min);

	//glPushMatrix();

	//glScalef(scale,scale,scale);
	glLineWidth(2.5);
	glPointSize(3);

	for(int jj=0;jj<num;)
	{	
		i = jj;
		if(exp[i]!=0||coe[i]!=0)
		{		
		barlength = 2*radius*coe[i]/10.;
		if(exp[i]>=0)
			barexplength = 2*radius*exp[i]/20.;
		else
		{
			barexplength = 2*radius*fabs(exp[i])/20.;
		}
		glColor3f(glyphColors1[0][i][0],glyphColors1[0][i][1],glyphColors1[0][i][2]);

		/*glBegin(GL_QUADS);
		glVertex2f(x[jj]-radius/2.,y[jj]-radius);
		glVertex2f(x[jj]-radius/2.,y[jj]-radius+barlength);
		glVertex2f(x[jj]-radius,y[jj]-radius+barlength);
		glVertex2f(x[jj]-radius,y[jj]-radius);
		glEnd();
		glBegin(GL_QUADS);
		glVertex2f(x[jj],y[jj]-radius+barlength);
		glVertex2f(x[jj],y[jj]+radius);
		glVertex2f(x[jj]-radius,y[jj]+radius);
		glVertex2f(x[jj]-radius,y[jj]-radius+barlength);
		glEnd();*/
		glBegin(GL_QUADS);
		glVertex3f(x[jj]-radius,y[jj]-radius,0.);
		glVertex3f(x[jj]+radius,y[jj]-radius,0.);
		glVertex3f(x[jj]+radius,y[jj]-radius+barexplength,0.);
		glVertex3f(x[jj]-radius,y[jj]-radius+barexplength,0.);
		glEnd();

		/*glColor3f(glyphColors2[0][i][0],glyphColors2[0][i][1],glyphColors2[0][i][2]);
		glBegin(GL_QUADS);
		glVertex2f(x[jj],y[jj]-radius);
		glVertex2f(x[jj]+radius,y[jj]-radius);
		glVertex2f(x[jj]+radius,y[jj]+radius);
		glVertex2f(x[jj],y[jj]+radius);
		glEnd();*/

		if(coe[i]>=0)
			glColor3f(1.,1.,1.);
		else
			glColor3f(0.,0.,0.);
		glBegin(GL_QUADS);
		glVertex3f(x[jj]-radius/4.,y[jj]-radius,0.);
		glVertex3f(x[jj]-radius/4.,y[jj]-radius+barlength,0.);
		glVertex3f(x[jj]+radius/4.,y[jj]-radius+barlength,0.);
		glVertex3f(x[jj]+radius/4.,y[jj]-radius,0.);
		glEnd();	

		}
		jj = jj + glyphFrequency;
	}

	glLineWidth(1);
	glPointSize(1);
	//glPopMatrix();

	glEndList();
}
void svIconGlyph::RenderIcon()
{
	glCallList(theList);


}
void svIconGlyph::RenderIcon(float *x, float *y, int num,float R)
{
	glEnable( GL_LINE_SMOOTH );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int i,j,k;
	int section;
	int size = glyphPos[0].size();
	svVector3 z1,z2;
	svVector3 draw,startp;
	svVector3 p;
	float bar;
	float angle,subangle;
	float radius;
	if(R<1)
		radius=R/4.;
	else 	
		radius=0.25;
	float deltar=0.01;
	int round;
	float barlength=radius;
	float barexplength = radius;
	float base;
	z1[0]=0.;z1[1]=0.;z1[2]=1.;
	z2[0]=0.;z2[1]=0.;z2[2]=-1.;

	float max,min;
	max = -9e+9;min = 9e+9;
	for(int jj=0;jj<num;jj++)
	{	
		if(x[jj]>max)
			max = x[jj];
		if(y[jj]>max)
			max = y[jj];
		if(x[jj]<min)
			min = x[jj];
		if(y[jj]<min)
			min = y[jj];				
	}

	float scale;

	if(windowswidth>windowsheight)
		scale = windowsheight/(max - min);
	else
		scale = windowswidth/(max - min);

	//glPushMatrix();

	//glScalef(scale,scale,scale);
	glLineWidth(2.5);
	glPointSize(3);

	for(int jj=0;jj<num;)
	{	
		i = jj;
		if(exp[i]!=0||coe[i]!=0)
		{		
		barlength = 2*radius*coe[i]/10.;
		if(exp[i]>=0)
			barexplength = 2*radius*exp[i]/maxexp;
		else
		{
			barexplength = 2*radius*fabs(exp[i])/maxexp;
		}

		if(coe[i]>=0)
			glColor3f(1.,1.,1.);
		else
			glColor3f(0.,0.,0.);
		glBegin(GL_QUADS);
		glVertex3f(x[jj]-radius/4.,y[jj]-radius,0.);
		glVertex3f(x[jj]-radius/4.,y[jj]-radius+barlength,0.);
		glVertex3f(x[jj]+radius/4.,y[jj]-radius+barlength,0.);
		glVertex3f(x[jj]+radius/4.,y[jj]-radius,0.);
		glEnd();

		glColor3f(glyphColors1[0][i][0],glyphColors1[0][i][1],glyphColors1[0][i][2]);

		/*glBegin(GL_QUADS);
		glVertex2f(x[jj]-radius/2.,y[jj]-radius);
		glVertex2f(x[jj]-radius/2.,y[jj]-radius+barlength);
		glVertex2f(x[jj]-radius,y[jj]-radius+barlength);
		glVertex2f(x[jj]-radius,y[jj]-radius);
		glEnd();
		glBegin(GL_QUADS);
		glVertex2f(x[jj],y[jj]-radius+barlength);
		glVertex2f(x[jj],y[jj]+radius);
		glVertex2f(x[jj]-radius,y[jj]+radius);
		glVertex2f(x[jj]-radius,y[jj]-radius+barlength);
		glEnd();*/
		glBegin(GL_QUADS);
		glVertex3f(x[jj]-radius,y[jj]-radius,0.);
		glVertex3f(x[jj]+radius,y[jj]-radius,0.);
		glVertex3f(x[jj]+radius,y[jj]-radius+barexplength,0.);
		glVertex3f(x[jj]-radius,y[jj]-radius+barexplength,0.);
		glEnd();

		/*glColor3f(glyphColors2[0][i][0],glyphColors2[0][i][1],glyphColors2[0][i][2]);
		glBegin(GL_QUADS);
		glVertex2f(x[jj],y[jj]-radius);
		glVertex2f(x[jj]+radius,y[jj]-radius);
		glVertex2f(x[jj]+radius,y[jj]+radius);
		glVertex2f(x[jj],y[jj]+radius);
		glEnd();*/
	

		}
		jj = jj + glyphFrequency;
	}

	glLineWidth(1);
	glPointSize(1);
	//glPopMatrix();

}
void svIconGlyph::RenderWindow()
{
	glColor3f(1.,1.,0.);
	glBegin(GL_LINE_LOOP);
	for(int i=0;i<4;i++)
		glVertex3f(choosewindow[i][0],choosewindow[i][1],0.);
	glEnd();
}

void svIconGlyph::RenderOrientation(float *x, float *y, int num,float R)
{
	glEnable(GL_BLEND);
	int i,j,k;
	int section;
	int size = glyphPos[0].size();
	svVector3 z1,z2;
	svVector3 draw,startp;
	svVector3 p;
	float bar;
	float angle,subangle;
	float radius;
	if(R<1)
		radius=R/4.;
	else 	
		radius=0.25;
	float deltar=0.01;
	int round;
	float barlength=radius;
	float base;
	z1[0]=0.;z1[1]=0.;z1[2]=1.;
	z2[0]=0.;z2[1]=0.;z2[2]=-1.;

	startp[0] = radius;
	startp[1] = 0;
	startp[2] = 0;

	glLineWidth(2);
	for(int jj=0;jj<num;)
	{	
		i = jj;
		if(exp[i]!=0||coe[i]!=0)
		{		

			glColor3f(1.,1.,1.); //x
			p[0]=startp[0]*cos(thetax[i])-startp[1]*sin(thetax[i]);
			p[1]=startp[0]*sin(thetax[i])+startp[1]*cos(thetax[i]);
			p[2]=startp[2];
			glBegin(GL_LINES);
			glVertex3f(x[jj],y[jj],0);	
			glVertex3f(p[0]+x[jj],p[1]+y[jj],0);
			glEnd();

			glColor3f(0.5,0.5,0.5); //y	
			p[0]=startp[0]*cos(thetay[i])-startp[1]*sin(thetay[i]);
			p[1]=startp[0]*sin(thetay[i])+startp[1]*cos(thetay[i]);
			p[2]=startp[2];
			glBegin(GL_LINES);
			glVertex3f(x[jj],y[jj],0);	
			glVertex3f(p[0]+x[jj],p[1]+y[jj],0);
			glEnd();

			glColor3f(0.,0.,0.); //z	
			p[0]=startp[0]*cos(thetaz[i])-startp[1]*sin(thetaz[i]);
			p[1]=startp[0]*sin(thetaz[i])+startp[1]*cos(thetaz[i]);
			p[2]=startp[2];
			glBegin(GL_LINES);
			glVertex3f(x[jj],y[jj],0);	
			glVertex3f(p[0]+x[jj],p[1]+y[jj],0);
			glEnd();

			glColor3f(glyphColors2[0][i][0],glyphColors2[0][i][1],glyphColors2[0][i][2]);
			glBegin(GL_QUADS);
			glVertex3f(x[jj]-radius,y[jj]-radius,0.);
			glVertex3f(x[jj]+radius,y[jj]-radius,0.);
			glVertex3f(x[jj]+radius,y[jj]+radius,0.);
			glVertex3f(x[jj]-radius,y[jj]+radius,0.);
			glEnd();	

		}
		jj = jj + glyphFrequency;
	}
	glLineWidth(1);
}

void svIconGlyph::cleanup()
{
	glyphPos[0].free();
	delete [] glyphPos;
	glyphPos=NULL;

	glyphDir[0].free();
	delete [] glyphDir;
	glyphDir=NULL;

	glyphColors1[0].free();
	delete [] glyphColors1;
	glyphColors1=NULL;

	glyphColors2[0].free();
	delete [] glyphColors2;
	glyphColors2=NULL;

	delete [] glyphMag;
	delete [] glyphScale;

	delete [] thetax;
	delete [] thetay;
	delete [] thetaz;
	delete [] coe;
	delete [] exp;
}

}
