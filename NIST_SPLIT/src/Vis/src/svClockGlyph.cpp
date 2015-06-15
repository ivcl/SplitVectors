/*  
 * svClockGlyph
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

#include "svClockGlyph.h"
#include "svException.h"
#include "svUtil.h"


#ifndef PI
#define PI 3.1415926
#endif

using namespace std;


namespace __svl_lib {

svClockGlyph::svClockGlyph( svVector3 l, svVector3 r,svInt dataSize,
     svVector3 *posIn, svVector3 *velIn,  svScalar *denIn,
	 svVector4 *color4In)
{
  float tmpden;
  maxMag = -9e+9;

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
  glyphColors = new svVector4Array[1];

  cerr << "... Setting ClockGlyph ..." << endl;

  for(int j=0; j<dataSize; j++)
  {
    	// set position
   	glyphPos[0].add(posIn[j]);

   	// set vector dir
	svVector3 nvel = normalize(velIn[j]);
	glyphDir[0].add(nvel);

	//set the angles
	thetax[j] = acos(nvel[0])*(-1);
	thetay[j] = acos(nvel[1])*(-1);
	thetaz[j] = acos(nvel[2])*(-1);

	//set magnitude
	glyphMag[j]=(log10(denIn[j]));
	if(glyphMag[j]>=maxMag)maxMag=glyphMag[j];

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
	for(int i=0;i<exp[j];i++)
	{
		tmppow = tmppow * 10.;
	}
	coe[j]=denIn[j]/tmppow;//pow(10.,exp[j]);
	exp[j] = exp[j] - 12;
	//set colors	
    	if(color4In!=NULL)  
	{
	  glyphColors[0].add(svVector4(color4In[j][0]/255., color4In[j][1]/255., color4In[j][2]/255., color4In[j][3]/255.));
	}
	else  
	{
	  glyphColors[0].add(svVector4(1,1,1,0)); // default white
	}

	
  }; // end for(j)

  cerr << "... Done init ClockGlyph ... " << endl;
}
void svClockGlyph::SetData(int dataSize,svVector3 *posIn, svVector3 *velIn,  svScalar *denIn,
	 svVector4 *color4In)
{
  cleanup();

  float tmpden;
  maxMag = -9e+9;

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
  glyphColors = new svVector4Array[1];

  //cerr << "... Setting ClockGlyph ..." << endl;

  for(int j=0; j<dataSize; j++)
  {
    	// set position
   	glyphPos[0].add(posIn[j]);

   	// set vector dir
	svVector3 nvel = normalize(velIn[j]);
	glyphDir[0].add(nvel);

	//set the angles
	thetax[j] = acos(nvel[0])*(-1);
	thetay[j] = acos(nvel[1])*(-1);
	thetaz[j] = acos(nvel[2])*(-1);

	//set magnitude
	glyphMag[j]=(log10(denIn[j]));
	if(glyphMag[j]>=maxMag)maxMag=glyphMag[j];

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
	for(int i=0;i<exp[j];i++)
	{
		tmppow = tmppow * 10.;
	}
	coe[j]=denIn[j]/tmppow;//pow(10.,exp[j]);
	exp[j] = exp[j] ;
	//set colors	
    	if(color4In!=NULL)  
	{
	  glyphColors[0].add(svVector4(color4In[j][0]/255., color4In[j][1]/255., color4In[j][2]/255., color4In[j][3]/255.));
	}
	else  
	{
	  glyphColors[0].add(svVector4(1,1,1,0)); // default white
	}

	
  }; // end for(j)

 // cerr << "... Done init ClockGlyph ... " << endl;
}

void svClockGlyph::GenerateDisplay(svVector3 l,svVector3 r)
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
	float radius=0.15;
	float deltar=0.01;
	int round;
	float barlength=radius;
	float base;
	z1[0]=0.;z1[1]=0.;z1[2]=1.;
	z2[0]=0.;z2[1]=0.;z2[2]=-1.;

	glLineWidth(2.5);
	glPointSize(3);
	for(i=0;i<size;)
	{	
		if(glyphPos[0][i][0]>=l[0]&&glyphPos[0][i][0]<=r[0]
			&&glyphPos[0][i][1]>=l[1]&&glyphPos[0][i][1]<=r[1]
			&&glyphPos[0][i][2]>=l[2]&&glyphPos[0][i][2]<=r[2])
		{
			glColor3f(glyphColors[0][i][0],glyphColors[0][i][1],glyphColors[0][i][2]);
			glBegin(GL_LINE_STRIP);
			startp[1]=0;
			startp[0]=-radius;
			startp[2]=glyphPos[0][i][2];
			angle = 2*PI*exp[i]/12.*(-1);//clockwise:positive
			section = fabs(exp[i]);
			round = section/13;
			for(k=0;k<=round;k++)
			{
				for(j=0;j<=section;j++)
				{		
					subangle = angle * j/ section;	
					p[0]=(startp[0]-deltar*k)*cos(subangle)-startp[1]*sin(subangle);
					p[1]=(startp[0]-deltar*k)*sin(subangle)+startp[1]*cos(subangle);
					p[2]=startp[2];
					glVertex3f(p[0]+glyphPos[0][i][0],p[1]+glyphPos[0][i][1],
						p[2]);
					//glVertex2f(p[0]+glyphPos[0][i][0],p[1]+glyphPos[0][i][1]);
				}
				section=section-12;
			}
			glEnd();
			
			glColor3f(1.,1.,1.);
			base = (glyphMag[i]/maxMag) * radius;
			glBegin(GL_LINE_LOOP);
			glVertex3f(startp[0]+glyphPos[0][i][0],startp[1]+glyphPos[0][i][1],startp[2]);
			glVertex3f(glyphPos[0][i][0],glyphPos[0][i][1]-base/2.,glyphPos[0][i][2]);
			glVertex3f(glyphPos[0][i][0],glyphPos[0][i][1]+base/2.,glyphPos[0][i][2]);
			glEnd();

			/*glBegin(GL_POINTS);
			glVertex3f(p[0]+glyphPos[0][i][0],p[1]+glyphPos[0][i][1],
				p[2]);
			glEnd();*/
		
			glColor3f(253./256.,212./256.,158./256.); //x
			p[0]=startp[0]*cos(thetax[i])-startp[1]*sin(thetax[i]);
			p[1]=startp[0]*sin(thetax[i])+startp[1]*cos(thetax[i]);
			p[2]=startp[2];
			glBegin(GL_LINES);
			glVertex3f(glyphPos[0][i][0],glyphPos[0][i][1],glyphPos[0][i][2]);	
			glVertex3f(p[0]+glyphPos[0][i][0],p[1]+glyphPos[0][i][1],p[2]);
			glEnd();

			glColor3f(252./256.,141./256.,89./256.); //y	
			p[0]=startp[0]*cos(thetay[i])-startp[1]*sin(thetay[i]);
			p[1]=startp[0]*sin(thetay[i])+startp[1]*cos(thetay[i]);
			p[2]=startp[2];
			glBegin(GL_LINES);
			glVertex3f(glyphPos[0][i][0],glyphPos[0][i][1],glyphPos[0][i][2]);	
			glVertex3f(p[0]+glyphPos[0][i][0],p[1]+glyphPos[0][i][1],p[2]);
			glEnd();

			glColor3f(215./256.,48./256.,31./256.); //z	
			p[0]=startp[0]*cos(thetaz[i])-startp[1]*sin(thetaz[i]);
			p[1]=startp[0]*sin(thetaz[i])+startp[1]*cos(thetaz[i]);
			p[2]=startp[2];
			glBegin(GL_LINES);
			glVertex3f(glyphPos[0][i][0],glyphPos[0][i][1],glyphPos[0][i][2]);	
			glVertex3f(p[0]+glyphPos[0][i][0],p[1]+glyphPos[0][i][1],p[2]);
			glEnd();

			glColor3f(127./255.,0.,0.);
			//bar=startp[0]-coe[i]/10.*barlength;
			bar = coe[i]/10. * radius;
			glBegin(GL_LINES);
			//glVertex3f(startp[0]+glyphPos[0][i][0],startp[1]+glyphPos[0][i][1],startp[2]);
			glVertex3f(glyphPos[0][i][0],glyphPos[0][i][1],glyphPos[0][i][2]);
			glVertex3f(glyphPos[0][i][0],glyphPos[0][i][1]+bar,glyphPos[0][i][2]);
			glEnd();	
		}
		i = i + glyphFrequency;
	}
	glLineWidth(1);
	glPointSize(1);

	glEndList();
}
void svClockGlyph::GenerateDisplay(float *x, float *y, int *index, int num,float R)
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
	float radius=R/2.;
	float deltar=0.01;
	int round;
	float barlength=radius;
	float base;
	z1[0]=0.;z1[1]=0.;z1[2]=1.;
	z2[0]=0.;z2[1]=0.;z2[2]=-1.;

	glLineWidth(2.5);
	glPointSize(3);
	for(int jj=0;jj<num;jj++)
	{	
		i = index[jj];
		glColor3f(glyphColors[0][i][0],glyphColors[0][i][1],glyphColors[0][i][2]);
		glBegin(GL_LINE_STRIP);
		startp[0]=0;
		startp[1]=radius;
		startp[2]=glyphPos[0][i][2];
		angle = 2*PI*exp[i]/12.*(-1);//clockwise:positive
		section = fabs(exp[i]);
		round = section/13;
		for(k=0;k<=round;k++)
		{
			for(j=0;j<=section;j++)
			{		
				subangle = angle * j/ section;	
				p[0]=(startp[0]-deltar*k)*cos(subangle)-startp[1]*sin(subangle);
				p[1]=(startp[0]-deltar*k)*sin(subangle)+startp[1]*cos(subangle);
				p[2]=startp[2];
				glVertex2f(p[0]+x[jj],p[1]+y[jj]);
			}
			section=section-12;
		}
		glEnd();
			
		/*glColor3f(1.,1.,1.);
		base = (glyphMag[i]/maxMag) * radius;
		glBegin(GL_LINE_LOOP);
		glVertex2f(startp[0]+x[jj],startp[1]+y[jj]);
		glVertex2f(x[jj],y[jj]-base/2.);
		glVertex2f(x[jj],y[jj]+base/2.);
		glEnd();*/
		
		glColor3f(253./256.,212./256.,158./256.); //x
		p[0]=startp[0]*cos(thetax[i])-startp[1]*sin(thetax[i]);
		p[1]=startp[0]*sin(thetax[i])+startp[1]*cos(thetax[i]);
		p[2]=startp[2];
		glBegin(GL_LINES);
		glVertex2f(x[jj],y[jj]);	
		glVertex2f(p[0]+x[jj],p[1]+y[jj]);
		glEnd();

		glColor3f(252./256.,141./256.,89./256.); //y	
		p[0]=startp[0]*cos(thetay[i])-startp[1]*sin(thetay[i]);
		p[1]=startp[0]*sin(thetay[i])+startp[1]*cos(thetay[i]);
		p[2]=startp[2];
		glBegin(GL_LINES);
		glVertex2f(x[jj],y[jj]);	
		glVertex2f(p[0]+x[jj],p[1]+y[jj]);
		glEnd();

		glColor3f(215./256.,48./256.,31./256.); //z	
		p[0]=startp[0]*cos(thetaz[i])-startp[1]*sin(thetaz[i]);
		p[1]=startp[0]*sin(thetaz[i])+startp[1]*cos(thetaz[i]);
		p[2]=startp[2];
		glBegin(GL_LINES);
		glVertex2f(x[jj],y[jj]);	
		glVertex2f(p[0]+x[jj],p[1]+y[jj]);
		glEnd();

		glColor3f(127./255.,0.,0.);
		//bar=startp[0]-coe[i]/10.*barlength;
		bar = coe[i]/10. * radius;
		glBegin(GL_LINES);
		//glVertex3f(startp[0]+glyphPos[0][i][0],startp[1]+glyphPos[0][i][1],startp[2]);
		glVertex2f(x[jj],y[jj]);
		glVertex2f(x[jj]-bar,y[jj]);
		glEnd();	

		i = i + glyphFrequency;
	}
	glLineWidth(1);
	glPointSize(1);

	glEndList();
}

void svClockGlyph::GenerateDisplay(float *x, float *y, int num,float R)//pow is from -15 to 15
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
	float rr ;
	if(R<=1)rr =R/4.;
	else rr=0.25;
	float radius=R/4.;
	float deltar=0.01;
	int round;
	float barlength=radius;
	float base;
	z1[0]=0.;z1[1]=0.;z1[2]=1.;
	z2[0]=0.;z2[1]=0.;z2[2]=-1.;


	glPointSize(3);
	for(int jj=0;jj<num;jj++)
	{	
		glLineWidth(2.5);
		i = jj;
		if(exp[i]!=0||coe[i]!=0)
		{
		if(exp[i]>=0)
			radius = exp[i]/15. * rr;
		else
			radius = fabs(exp[i])/15. * rr;
		glColor3f(glyphColors[0][i][0],glyphColors[0][i][1],glyphColors[0][i][2]);
		glBegin(GL_LINE_STRIP);
		startp[0]=0;
		startp[1]=radius;
		startp[2]=glyphPos[0][i][2];
		angle = 2*PI*exp[i]/12.*(-1);//clockwise:positive
		section = fabs(exp[i]);
		round = section/13;
		for(k=0;k<=round;k++)
		{
			for(j=0;j<=section;j++)
			{		
				subangle = angle * j/ section;	
				p[0]=startp[0]*cos(subangle)-(startp[1]-deltar*k)*sin(subangle);
				p[1]=startp[0]*sin(subangle)+(startp[1]-deltar*k)*cos(subangle);
				p[2]=startp[2];
				glVertex2f(p[0]+x[jj],p[1]+y[jj]);
			}
			section=section-12;
		}
		glEnd();
		glLineWidth(1);
	
		/*glColor3f(1.,1.,1.);
		base = (glyphMag[i]/maxMag) * radius;
		glBegin(GL_LINE_LOOP);
		glVertex2f(startp[0]+x[jj],startp[1]+y[jj]);
		glVertex2f(x[jj],y[jj]-base/2.);
		glVertex2f(x[jj],y[jj]+base/2.);
		glEnd();*/
		
		glColor3f(253./256.,212./256.,158./256.); //x
		p[0]=startp[0]*cos(thetax[i])-startp[1]*sin(thetax[i]);
		p[1]=startp[0]*sin(thetax[i])+startp[1]*cos(thetax[i]);
		p[2]=startp[2];
		glBegin(GL_LINES);
		glVertex2f(x[jj],y[jj]);	
		glVertex2f(p[0]+x[jj],p[1]+y[jj]);
		glEnd();

		glColor3f(252./256.,141./256.,89./256.); //y	
		p[0]=startp[0]*cos(thetay[i])-startp[1]*sin(thetay[i]);
		p[1]=startp[0]*sin(thetay[i])+startp[1]*cos(thetay[i]);
		p[2]=startp[2];
		glBegin(GL_LINES);
		glVertex2f(x[jj],y[jj]);	
		glVertex2f(p[0]+x[jj],p[1]+y[jj]);
		glEnd();

		glColor3f(215./256.,48./256.,31./256.); //z	
		p[0]=startp[0]*cos(thetaz[i])-startp[1]*sin(thetaz[i]);
		p[1]=startp[0]*sin(thetaz[i])+startp[1]*cos(thetaz[i]);
		p[2]=startp[2];
		glBegin(GL_LINES);
		glVertex2f(x[jj],y[jj]);	
		glVertex2f(p[0]+x[jj],p[1]+y[jj]);
		glEnd();

		glColor3f(127./255.,0.,0.);
		//bar=startp[0]-coe[i]/10.*barlength;
		bar = coe[i]/10. * radius;
		glBegin(GL_LINES);
		//glVertex3f(startp[0]+glyphPos[0][i][0],startp[1]+glyphPos[0][i][1],startp[2]);
		glVertex2f(x[jj],y[jj]);
		glVertex2f(x[jj],y[jj]+bar);
		glEnd();	
		}
		i = i + glyphFrequency;
	}

	glPointSize(1);

	glEndList();
}

void svClockGlyph::RenderClock()
{
	glCallList(theList);
}

void svClockGlyph::RenderClock(float *x, float *y, int num,float R)
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
	float rr ;
	if(R<=1)rr =R/4.;
	else rr=0.25;
	float radius=R/4.;
	float deltar=0.01;
	int round;
	float barlength=radius;
	float base;
	z1[0]=0.;z1[1]=0.;z1[2]=1.;
	z2[0]=0.;z2[1]=0.;z2[2]=-1.;


	glPointSize(3);
	for(int jj=0;jj<num;jj++)
	{	
		glLineWidth(2.5);
		i = jj;
		if(exp[i]!=0||coe[i]!=0)
		{
		if(exp[i]>=0)
			radius = exp[i]/maxexp * rr;
		else
			radius = fabs(exp[i])/maxexp * rr;
		glColor3f(glyphColors[0][i][0],glyphColors[0][i][1],glyphColors[0][i][2]);
		glBegin(GL_LINE_STRIP);
		startp[0]=0;
		startp[1]=radius;
		startp[2]=glyphPos[0][i][2];
		angle = 2*PI*exp[i]/maxexp*(-1);//clockwise:positive
		section = fabs(exp[i]);
		round = section/(maxexp+1);
		for(k=0;k<=round;k++)
		{
			for(j=0;j<=section;j++)
			{		
				subangle = angle * j/ section;	
				p[0]=startp[0]*cos(subangle)-(startp[1]-deltar*k)*sin(subangle);
				p[1]=startp[0]*sin(subangle)+(startp[1]-deltar*k)*cos(subangle);
				p[2]=startp[2];
				glVertex2f(p[0]+x[jj],p[1]+y[jj]);
			}
			section=section-12;
		}
		glEnd();
		glLineWidth(1);
	
		/*glColor3f(1.,1.,1.);
		base = (glyphMag[i]/maxMag) * radius;
		glBegin(GL_LINE_LOOP);
		glVertex2f(startp[0]+x[jj],startp[1]+y[jj]);
		glVertex2f(x[jj],y[jj]-base/2.);
		glVertex2f(x[jj],y[jj]+base/2.);
		glEnd();*/
		
		glColor3f(227./256.,26./256.,28./256.); //x
		p[0]=startp[0]*cos(thetax[i])-startp[1]*sin(thetax[i]);
		p[1]=startp[0]*sin(thetax[i])+startp[1]*cos(thetax[i]);
		p[2]=startp[2];
		glBegin(GL_LINES);
		glVertex2f(x[jj],y[jj]);	
		glVertex2f(p[0]+x[jj],p[1]+y[jj]);
		glEnd();

		glColor3f(35./256.,139./256.,69./256.); //y	
		p[0]=startp[0]*cos(thetay[i])-startp[1]*sin(thetay[i]);
		p[1]=startp[0]*sin(thetay[i])+startp[1]*cos(thetay[i]);
		p[2]=startp[2];
		glBegin(GL_LINES);
		glVertex2f(x[jj],y[jj]);	
		glVertex2f(p[0]+x[jj],p[1]+y[jj]);
		glEnd();

		glColor3f(33./256.,113./256.,181./256.); //z	
		p[0]=startp[0]*cos(thetaz[i])-startp[1]*sin(thetaz[i]);
		p[1]=startp[0]*sin(thetaz[i])+startp[1]*cos(thetaz[i]);
		p[2]=startp[2];
		glBegin(GL_LINES);
		glVertex2f(x[jj],y[jj]);	
		glVertex2f(p[0]+x[jj],p[1]+y[jj]);
		glEnd();

		glColor3f(127./255.,0.,0.);
		//bar=startp[0]-coe[i]/10.*barlength;
		bar = coe[i]/10. * radius;
		glBegin(GL_LINES);
		//glVertex3f(startp[0]+glyphPos[0][i][0],startp[1]+glyphPos[0][i][1],startp[2]);
		glVertex2f(x[jj],y[jj]);
		glVertex2f(x[jj],y[jj]+bar);
		glEnd();	
		}
		i = i + glyphFrequency;
	}

	glPointSize(1);
}

void svClockGlyph::cleanup()
{
	glyphPos[0].free();
	delete [] glyphPos;
	glyphPos=NULL;

	glyphDir[0].free();
	delete [] glyphDir;
	glyphDir=NULL;

	glyphColors[0].free();
	delete [] glyphColors;
	glyphColors=NULL;

	delete [] glyphMag;
	delete [] glyphScale;

	delete [] thetax;
	delete [] thetay;
	delete [] thetaz;
	delete [] coe;
	delete [] exp;
}

}
