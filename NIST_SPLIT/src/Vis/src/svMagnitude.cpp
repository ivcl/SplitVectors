#include "svMagnitude.h"
#include <string.h>
#include <fstream>
#include <algorithm>
#include <cmath>
//#include "parameters.h"
#ifdef MAC
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#ifndef PI
#define PI 3.1415926
#endif

#ifndef PARAMETERMAX
#define PARAMETERMAX 5.
#endif

#ifndef PARAMETERMIN
#define PARAMETERMIN 0.1
#endif

#define TIMES 10
float xscale=18.;
float yscale=10.;
float xratio = 9./10.;
float yratio = 7./8.;
float slice;
using namespace std;

namespace __svl_lib
{

svMagnitude::svMagnitude()
{
	level = 0;
	SELECTED = GL_FALSE;
	superquadricpos = NULL;
	LinearMap = false;
	super = false;
	
	encode_mode = 2;
}
void svMagnitude::SetQuadricMag(svScalar *density, int datasize, int w, int h)
{
	bool flag1 = true;
	if(level!=0)
	{
		cleanup();
		flag1 = false;
	}
	LEVEL1 = 6;
	LEVEL2 = 6;
	super = true;
	Linear = false;

	svScalar NumofWidget[100];
	svInt ex;
	svScalar tmp;
	svBool flag = false;

	svFloat alpha,beta;

	alpha = 0.1;
	beta = 5;
	level = 0;

	for(int i=0;i<datasize;i++)
	{
                tmp = density[i];
                ex = 0;
                for(int j=0;j<datasize;j++)
                {
                	if(tmp<10)break;
                	else                                
                        { ex ++;tmp = tmp/10;}
                }
		if(ex==0)
		{
			tmp = density[i];
			for(int j=0;j<datasize;j++)
			{
				if(tmp>=1)break;
				else
				{
					ex --;tmp = tmp * 10;
				}
			}
		}
		if(level == 0)
		{
			NumofWidget[level] = ex;
			level ++;
		}
		else
		{
			for(int j=0;j<level;j++)
			{
				if(NumofWidget[j]==ex){flag=false;break;}
				else{flag=true;}
			}
			if(flag==true)
			{
				NumofWidget[level]=ex;level++;
			}
		}
        }
	
	sort(NumofWidget, NumofWidget+level);

	NumofWidget[level] = NumofWidget[level-1] + 1; level++;

	int tmp1 = NumofWidget[0];
	int tmp2 = NumofWidget[level-1];	
	for(int i=0;i<tmp2-tmp1+1;i++)
	{
		NumofWidget[i] = tmp1 + i;
	}
	level = tmp2 - tmp1 + 1;

	if(level <=5)
	{
		rlevel = 2;
	}		
	else
	{
		if(level<=8)
			rlevel = 3;
		else
			rlevel = level/3;
	}
	llevel = level - rlevel;


	WidgetH = new svVector3[level];
	MagWidget = new svScalar[level];

	for(int i=0;i<level;i++) {MagWidget[i] = NumofWidget[i]-SCALE;}

	middle = MagWidget[llevel];

	for(int i=0;i<level;i++)
	{
		WidgetH[i][0] = w*xratio - (float)(level-1-i)*xscale;
		WidgetH[i][1] = h*yratio;
		//WidgetH[i][0] = w - (float)i*xscale;
		//WidgetH[i][1] = h*yratio;
		WidgetH[i][2] = 0;
	}
	

        CtrlPoints[0].x = WidgetH[0][0];
        CtrlPoints[0].y = WidgetH[0][1];
        CtrlPoints[0].z = 0;

        CtrlPoints[1].x = WidgetH[llevel][0];
        CtrlPoints[1].y = WidgetH[llevel][1];
        CtrlPoints[1].z = WidgetH[llevel][2];

        CtrlPoints[2].x = WidgetH[llevel][0];
        CtrlPoints[2].y = WidgetH[llevel][1];
        CtrlPoints[2].z = 0;

        CtrlPoints[3].x = WidgetH[level-1][0];
        CtrlPoints[3].y = WidgetH[level-1][1];
        CtrlPoints[3].z = 0;

        CtrlPoints[4].x = WidgetH[llevel][0];
        CtrlPoints[4].y = WidgetH[llevel][1];
        CtrlPoints[4].z = WidgetH[llevel][2];


	a1 =( CtrlPoints[1].y - CtrlPoints[0].y)/(CtrlPoints[1].x - CtrlPoints[0].x);
	b1 = CtrlPoints[1].y - a1 * CtrlPoints[1].x;
	a2 =( CtrlPoints[3].y - CtrlPoints[2].y)/(CtrlPoints[3].x - CtrlPoints[2].x);
	b2 = CtrlPoints[3].y - a2 * CtrlPoints[3].x;

	W = w;
	H = h;

	/*superquadricpos = new svVector3Array[level];
	
	for(int i=0;i<level;i++)
	{
		alpha = 0.1 + i*(5.-0.1)/(level-1);
		beta = 5 - i*(5-0.1)/(level-1);
		SetQuadric(alpha,beta,i);
	}*/
	//SetQuadric();

}
//don't forget to clean the superquadricpos before set the new llevel value
/*void svMagnitude::SetQuadric()
{
	superquadricpos = new svVector3Array[level-llevel];

	float x, y, z;	
	svVector3 tmp;
	float sinphi;

	int i;	
	float a,b;

	for(i=0;i<level-llevel;i++)
	{
		a = PARAMETERMIN + 	
			(MagWidget[llevel+i]-middle)/(MagWidget[level-1]-middle)
			* (PARAMETERMAX-PARAMETERMIN);
		b = PARAMETERMAX - 	
			(MagWidget[llevel+i]-middle)/(MagWidget[level-1]-middle)
			* (PARAMETERMAX-PARAMETERMIN);

		for(float ii=0; ii<=PI;)
		{
			if(cos(ii)<0)
			{
				x = (-1) *pow( (-1) * cos(ii), b); 
			}
			else if(cos(ii)>0)
			{
				x = pow(cos(ii),b);
			}
			else
			{
				x = 0;
			}
			if(sin(ii)<0)
			{
				sinphi = (-1) *pow( (-1) * sin(ii), b);
			}
			else if(sin(ii)>0)
			{
				sinphi = pow(sin(ii),b);
			}
			else
			{
				sinphi = 0;
			}
			for(float jj=0;jj<=PI*2;)
			{
				if(sin(jj)==0)y = 0;
				else	
				{
					if(sin(jj)>0)
					{
						y = (-1) * 1 * pow(sin(jj), a) * sinphi;
					}
					else
					{y = (-1) * (-1) * pow((-1)*sin(jj),a)*sinphi;}
				} 

				if(cos(jj)==0)z=0;
				else
				{
					if(cos(jj)>0) z = 1 * pow(cos(jj), a) * sinphi;
					else z = (-1) * pow((-1)*cos(jj),a) * sinphi;
				}
				tmp[0] = x*8;
				tmp[1] = y*8;
				tmp[2] = z*8;
				superquadricpos[i].add(tmp);
				jj = jj + (float)2*PI/LEVEL1;
			}
			ii = ii + (float)PI/LEVEL2;
		}
	}	
}
*/
void svMagnitude::SetLinearMag(svScalar *density, int datasize, int w, int h)
{
	bool flag1 = true;
	superquadricpos=NULL;
	if(level!=0)
	{
		cleanup();
		flag1 = false;
	}
	Linear = true;

	xratio = 1./12.;
	xscale = 90;

	svScalar max=-1;
	svScalar min=1000000000.;
	level = 0;

	for(int i=0;i<datasize;i++)
	{
		if(max<=density[i])
			max = density[i];
		if(min>=density[i])
			min = density[i];
	}

	level = 10.;
	slice = (max-min)/(level-1.);

	llevel = 6;

	WidgetH = new svVector3[level];
	MagWidget = new svScalar[level];

	for(int i=0;i<level;i++) {MagWidget[i] = (min+i*slice)/1000000000000;}
	middle = MagWidget[llevel];

	for(int i=0;i<level;i++)
	{
		WidgetH[i][0] = w*xratio - (float)(level-1-i)*xscale;
		WidgetH[i][1] = h*yratio;
		WidgetH[i][2] = 0;
	}

	CtrlPoints[0].x = WidgetH[0][0];
        CtrlPoints[0].y = h*yratio;
        CtrlPoints[0].z = 0;

        CtrlPoints[1].x = WidgetH[llevel][0];
        CtrlPoints[1].y = h*yratio + yscale;
        CtrlPoints[1].z = 0;

        CtrlPoints[2].x = WidgetH[llevel][0];
        CtrlPoints[2].y = h*yratio+ 2*yscale;
        CtrlPoints[2].z = 0;

        CtrlPoints[3].x = WidgetH[level-1][0];
        CtrlPoints[3].y = h * yratio + 6*yscale;
        CtrlPoints[3].z = 0;

        CtrlPoints[4].x = WidgetH[llevel][0];
        CtrlPoints[4].y = WidgetH[llevel][1];
        CtrlPoints[4].z = WidgetH[llevel][2];


	a1 =( CtrlPoints[1].y - CtrlPoints[0].y)/(CtrlPoints[1].x - CtrlPoints[0].x);
	b1 = CtrlPoints[1].y - a1 * CtrlPoints[1].x;
	a2 =( CtrlPoints[3].y - CtrlPoints[2].y)/(CtrlPoints[3].x - CtrlPoints[2].x);
	b2 = CtrlPoints[3].y - a2 * CtrlPoints[3].x;

	W = w;
	H = h;

}

void svMagnitude::SetMag(svScalar *density, int datasize, int w, int h)
{
	yscale = h * 0.012;

	bool flag1 = true;
	superquadricpos=NULL;
	if(level!=0)
	{
		cleanup();flag1 = false;
	}
	level = 0;
	Linear = false;

	svScalar *NumofWidget = new svScalar[100];
	svInt ex;
	svScalar tmp;
	svBool flag = false;
	
	double max = -9e+9;
	double min = 9e+9;

	for(int i=0;i<datasize;i++)
	{
		if(density[i]>max)
			max = density[i];
		if(density[i]<min)
			min = density[i];
		
                tmp = density[i];
                ex = 0;
                for(int j=0;j<datasize;j++)
                {
                	if(tmp<10)break;
                	else                                
                        { ex ++;tmp = tmp/10;}
                }
		if(ex==0&&density[i]!=0)
		{
			tmp = density[i];
			for(int j=0;j<datasize;j++)
			{
				if(tmp>=1)break;
				else
				{
					ex --;tmp = tmp * 10;
				}
			}
		}
		if(level == 0)
		{
			NumofWidget[level] = ex;
			level ++;
		}
		else
		{
			for(int j=0;j<level;j++)
			{
				if(NumofWidget[j]==ex){flag=false;break;}
				else{flag=true;}
			}
			if(flag==true)
			{
				NumofWidget[level]=ex;level++;
			}
		}
        }
	//cerr<<max<<" "<<min<<endl;
	//cerr<<level<<endl;
	
	sort(NumofWidget, NumofWidget+level);

	NumofWidget[level] = NumofWidget[level-1] + 1; level++;

	int tmp1 = NumofWidget[0];
	int tmp2 = NumofWidget[level-1];	
	//cerr<<tmp1<<" "<<tmp2<<endl;

	for(int i=0;i<tmp2-tmp1+1;i++)
	{
		NumofWidget[i] = tmp1 + i;
	}
	level = tmp2 - tmp1 + 1;

	if(level <=5)
	{
		rlevel = 2;
	}		
	else
	{
		if(level<=8)
			rlevel = 3;
		else
			rlevel = level/3;
	}
	llevel = level - rlevel;


	WidgetH = new svVector3[level];
	MagWidget = new svScalar[level];

	for(int i=0;i<level;i++) {MagWidget[i] = NumofWidget[i]-SCALE;}


	middle = MagWidget[llevel];

	for(int i=level-1;i>=0;i--)
	{
		//WidgetH[i][0] = w*xratio + (float)i*xscale;
		//WidgetH[i][1] = h*yratio;
		WidgetH[i][0] = w*xratio - (float)(level-1-i)*xscale;
		WidgetH[i][1] = h*yratio;
		WidgetH[i][2] = 0;
	}

	CtrlPoints[0].x = WidgetH[0][0];
        CtrlPoints[0].y = h*yratio;
        CtrlPoints[0].z = 0;

        CtrlPoints[1].x = WidgetH[llevel][0];
        CtrlPoints[1].y = h*yratio + yscale;
        CtrlPoints[1].z = 0;

        CtrlPoints[2].x = WidgetH[llevel][0];
        CtrlPoints[2].y = h*yratio+ 2*yscale;
        CtrlPoints[2].z = 0;

        CtrlPoints[3].x = WidgetH[level-1][0];
        CtrlPoints[3].y = h * yratio + 6*yscale;
        CtrlPoints[3].z = 0;

        CtrlPoints[4].x = WidgetH[llevel][0];
        CtrlPoints[4].y = WidgetH[llevel][1];
        CtrlPoints[4].z = WidgetH[llevel][2];


	a1 =( CtrlPoints[1].y - CtrlPoints[0].y)/(CtrlPoints[1].x - CtrlPoints[0].x);
	b1 = CtrlPoints[1].y - a1 * CtrlPoints[1].x;
	a2 =( CtrlPoints[3].y - CtrlPoints[2].y)/(CtrlPoints[3].x - CtrlPoints[2].x);
	b2 = CtrlPoints[3].y - a2 * CtrlPoints[3].x;

	W = w;
	H = h;
	
	//cerr<<level<<endl;

	delete [] NumofWidget;
}

void svMagnitude::RenderMagnitude(int  w, int h)
{

	glEnable( GL_LINE_SMOOTH );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth(0.2);


	if(w!=W||h!=H)
	{
		for(int i=0;i<level;i++)
		{
			//WidgetH[i][0] = w*xratio + (float)i*xscale;
			WidgetH[i][0] = w*xratio - (float)(level-1-i)*xscale;
			WidgetH[i][1] = h*yratio;
			WidgetH[i][2] = 0;
		}

		for(int i=0;i<5;i++)
		{
			CtrlPoints[i].x = CtrlPoints[i].x + (w - W)*xratio;//*xratio
			CtrlPoints[i].y = CtrlPoints[i].y + (h - H)*yratio;
		}

		W = w;
		H = h;
	}

	char str[50];
	glColor3f(1,1,1);
	

	int count = 0;

	for(int i=0;i<level;i++)
	{
		glRasterPos3f(WidgetH[i][0]-xscale/2.,WidgetH[i][1]-2*yscale,WidgetH[i][2]);
		
		//if(str[0] != '0')
		//	for(int j=0;j<50;j++)
		//	{
		//		if(str[j]!='.')
		//			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
		//		else
		//			break;
		//	}
		//else
		//{
		count = 0;
		if(encode_mode==2||encode_mode==1)
		{
			sprintf(str, "%0.0f", MagWidget[i]);
			for(int j=0;j<strlen(str);j++)
			{
					glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

			}	
		}
		else if(encode_mode==0)
		{
			if(i==0||i==level-1||i==level-rlevel)
			{
				sprintf(str, "%0.0f", MagWidget[i]);
			
				for(int j=0;j<strlen(str);j++)
				{
						glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
	
				}	
			}
		}	
	}

	char str2[50];
	
	if(encode_mode==2)
	{
		sprintf(str2, "Log10(Magnitude)");
	}
	else if(encode_mode==0)
	{
		sprintf(str2, "Linear(Magnitude)");
	}
	else if(encode_mode==1)
	{
		sprintf(str2, "SplitVectors(Magnitude)");
	}
	
	glRasterPos3f(WidgetH[0][0]-xscale/2.,WidgetH[0][1]+4*yscale,WidgetH[0][2]);
	for(int j=0;j<strlen(str2);j++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str2[j]);

	glPointSize(4);
	for(int i=0;i<5;i++)
	{
		if(SELECTED == GL_TRUE || NEAR == GL_TRUE)
		{
			if(i==ni)	
				glColor3f(0,1,0);
			else
				glColor3f(1,0,0);
		}
		else
			glColor3f(1,0,0);
		glBegin(GL_POINTS);
		glVertex3f(CtrlPoints[i].x, CtrlPoints[i].y, CtrlPoints[i].z);
		glEnd();
	}	
	
	glColor3f(1,1,1);
	glPointSize(1);
	

	float tmpdis = (CtrlPoints[0].y - WidgetH[0][1])/10.;
	for(int i=1;i<=10;i++)
        {
		glBegin(GL_POINTS);			
		glVertex3f(WidgetH[0][0],WidgetH[0][1]+tmpdis*i,WidgetH[0][2]);
		glEnd();
	}
	tmpdis = (CtrlPoints[3].y - WidgetH[level-1][1])/10.;
	for(int i=1;i<=10;i++)
        {
		glBegin(GL_POINTS);			
		glVertex3f(WidgetH[level-1][0],WidgetH[level-1][1]+tmpdis*i,WidgetH[level-1][2]);
		glEnd();
	}	
	glPointSize(1);

	char str3[3] = {'D','O', 'I'};
	glRasterPos3f((CtrlPoints[1].x+CtrlPoints[3].x)/2.-xscale/2., yscale*5+h*yratio, CtrlPoints[1].z);
	for(int j=0;j<3;j++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str3[j]);
	

        for(int i=0;i<3;i++)
        {
                glColor3f(1,1,1);
                glBegin(GL_LINES);
                glVertex3f(CtrlPoints[i].x, CtrlPoints[i].y, CtrlPoints[i].z);
		glVertex3f(CtrlPoints[i+1].x, CtrlPoints[i+1].y, CtrlPoints[i+1].z);
                glEnd();
        }

	glColor3f(1,1,1);
	for(int i=0;i<level;i++)
	{
		if(i<level-1)
                {
			glBegin(GL_LINES);
                	glVertex3f(WidgetH[i][0],WidgetH[i][1],WidgetH[i][2]);
                	glVertex3f(WidgetH[i+1][0],WidgetH[i+1][1],WidgetH[i+1][2]);
               		glEnd();
		}
		
		if(encode_mode==1||encode_mode==2
				||(encode_mode==0&&(i==0||i==level-1||i==level-rlevel)))
		{
			glPointSize(4);
			glBegin(GL_POINTS);
			glVertex3f(WidgetH[i][0],WidgetH[i][1],WidgetH[i][2]);
			glEnd();
			glPointSize(1);
		}
	}

	glLineWidth(1.);

}

void svMagnitude::RenderQuadric(int  w, int h)
{
	if(w!=W||h!=H)
	{
		for(int i=0;i<level;i++)
		{
			WidgetH[i][0] = w*xratio - (float)(level-1-i)*xscale;
			WidgetH[i][1] = h*yratio;
			WidgetH[i][2] = 0;
		}

			CtrlPoints[4].x = CtrlPoints[4].x + (w - W)*xratio;
			CtrlPoints[4].y = CtrlPoints[4].y + (h - H)*yratio;

		W = w;
		H = h;
	}

	char str[50];
	glColor3f(1,1,1);
	for(int i=0;i<level;i++)
	{
		if(i<level-1)
                {
			glBegin(GL_LINES);
                	glVertex3f(WidgetH[i][0],WidgetH[i][1],WidgetH[i][2]);
                	glVertex3f(WidgetH[i+1][0],WidgetH[i+1][1],WidgetH[i+1][2]);
               		glEnd();
		}
		
		glPointSize(4);
                glBegin(GL_POINTS);
                glVertex3f(WidgetH[i][0],WidgetH[i][1],WidgetH[i][2]);
                glEnd();
		glPointSize(1);
	}	


	for(int i=0;i<level;i++)
	{
		glRasterPos3f(WidgetH[i][0]-xscale/2.,WidgetH[i][1]-2*yscale,WidgetH[i][2]);
		sprintf(str, "%f", MagWidget[i]);
		if(str[0] != '0')
			for(int j=0;j<50;j++)
			{
				if(str[j]!='.')
					glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
				else
					break;
			}
		else
		{
			for(int j=1;j<7;j++)
			{
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
			}
		}
	}

	char str2[14] = {'l','o', 'g', '1','0','(','D','e','n','s','i','t','y',')'};
	glRasterPos3f(WidgetH[0][0]-xscale/2.,WidgetH[0][1]-4*yscale,WidgetH[0][2]);
	for(int j=0;j<14;j++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str2[j]);

	glPointSize(4);
	if(SELECTED == GL_TRUE || NEAR == GL_TRUE)
	{
			glColor3f(0,1,0);
	}
	else
		glColor3f(1,0,0);
	glBegin(GL_POINTS);
	glVertex3f(CtrlPoints[4].x, CtrlPoints[4].y, CtrlPoints[4].z);
	glEnd();

	glColor3f(1,1,1);
	glPointSize(1);
	

	float tmpdis = (CtrlPoints[0].y - WidgetH[0][1])/10.;
	for(int i=1;i<=10;i++)
       {
		glBegin(GL_POINTS);			
		glVertex3f(WidgetH[0][0],WidgetH[0][1]+tmpdis*i,WidgetH[0][2]);
		glEnd();
	}	
	glPointSize(1);


	char str3[3] = {'D','O', 'I'};
	glRasterPos3f((CtrlPoints[4].x+WidgetH[level-1][0])/2.-xscale/2., yscale*5+h*yratio, 0);
	for(int j=0;j<3;j++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str3[j]);

        glBegin(GL_LINES);
        glVertex3f(CtrlPoints[4].x, h*yratio+yscale*4, CtrlPoints[4].z);
	glVertex3f(CtrlPoints[4].x,h*yratio , CtrlPoints[4].z);
        glEnd();	
        glBegin(GL_LINES);
        glVertex3f(WidgetH[level-1][0],  h*yratio+yscale*4, WidgetH[level-1][2]);
	glVertex3f(WidgetH[level-1][0],h*yratio , WidgetH[level-1][2]);
        glEnd();
	
	glColor3f(1,1,1);
	int index1, index2, index3, index4;
	for(int i=llevel;i<level;i++)
	{
		for(int ii=0;ii<LEVEL1;ii++)
			for(int jj=0;jj<LEVEL2;jj++)
			{
				index1 = ii+ii*LEVEL2+jj;
				index2 = index1+LEVEL2+1;
				index3 = index2+1;
				index4 = index1+1;
				glBegin(GL_QUADS);
				glVertex3f(superquadricpos[i-llevel][index1][0]+WidgetH[i][0],
					superquadricpos[i-llevel][index1][1]+WidgetH[i][1]+10,0);
                                glVertex3f(superquadricpos[i-llevel][index2][0]+WidgetH[i][0],
					superquadricpos[i-llevel][index2][1]+WidgetH[i][1]+10,0);
                                glVertex3f(superquadricpos[i-llevel][index3][0]+WidgetH[i][0],
					superquadricpos[i-llevel][index3][1]+WidgetH[i][1]+10,0);
                                glVertex3f(superquadricpos[i-llevel][index4][0]+WidgetH[i][0],
					superquadricpos[i-llevel][index4][1]+WidgetH[i][1]+10,0);
                                glEnd();
			}
	}

	for(int i=0;i<llevel;i++)
	{
		glBegin(GL_LINES);
		glVertex3f(WidgetH[i][0]-4,WidgetH[i][1]+10,0);
		glVertex3f(WidgetH[i][0]+4,WidgetH[i][1]+10,0);
		glEnd();
	}

}

void svMagnitude::move_ctrlpoint(int x, int y, int w, int h) // need to be improved and the trackball thing.
{
    int whichAxis = -1;
    
	if(ni==4)whichAxis = 1;
	else whichAxis = 0;	


	    if(whichAxis == 1)
		{
	      CtrlPoints[ni].x = x;
		}
		else if(whichAxis == 0)
		{
	      CtrlPoints[ni].y = H-y;
		}

		SetLocation(w, h);

}

void svMagnitude::Find_Nearest(int x, int y)
{
    int i, j;
    GLdouble td, dd;

    dd=9e+9;

    ni = 0;
    
        for(i=4;i>=0;i--) {
            td = ((GLdouble)x-CtrlPoints[i].x) *  ((GLdouble)x-CtrlPoints[i].x) +
                 (H-(GLdouble)y-CtrlPoints[i].y) * (H-(GLdouble)y-CtrlPoints[i].y);
            if(td<dd) {
                dd=td;
                ni = i;
            }
        }
    if(dd<25.) SELECTED = GL_TRUE;
}

bool svMagnitude::Find_Near(int x, int y)
{
    int i, j;
    GLdouble td, dd;

    dd=9e+9;

    ni = 0;
    
        for(i=4;i>=0;i--) {
            td = ((GLdouble)x-CtrlPoints[i].x) *  ((GLdouble)x-CtrlPoints[i].x) +
                 (H-(GLdouble)y-CtrlPoints[i].y) * (H-(GLdouble)y-CtrlPoints[i].y);
            if(td<dd) {
                dd=td;
                ni = i;

            }
        }
    if(dd<25.) return true;
	else return false;
}

void svMagnitude::SetLocation(int w, int h)
{
	int i,j;


		if(ni==4)
		{
			if(CtrlPoints[ni].x>=CtrlPoints[3].x) CtrlPoints[ni].x=CtrlPoints[3].x;
			else  if(CtrlPoints[ni].x<=CtrlPoints[0].x) CtrlPoints[ni].x=CtrlPoints[0].x;
	 		//if(Linear == false)
				middle = MagWidget[0] + (CtrlPoints[ni].x - CtrlPoints[0].x)/(xscale);
			//else
			//	middle = MagWidget[0] + (slice/pow(10., 12.))*(CtrlPoints[ni].x - CtrlPoints[0].x)/(xscale);
			CtrlPoints[1].x	 = CtrlPoints[ni].x;
			CtrlPoints[2].x = CtrlPoints[ni].x;
		                a1 =( CtrlPoints[1].y - CtrlPoints[0].y)/(CtrlPoints[1].x - CtrlPoints[0].x);
		                b1  = CtrlPoints[1].y - a1 * CtrlPoints[1].x;
		                a2 =( CtrlPoints[3].y - CtrlPoints[2].y)/(CtrlPoints[3].x - CtrlPoints[2].x);
		                b2 = CtrlPoints[3].y - a2 * CtrlPoints[3].x;
		}
		else
		{
			if(ni==0)
			{
				if(CtrlPoints[ni].y>=CtrlPoints[ni+1].y)
					CtrlPoints[ni].y = CtrlPoints[ni+1].y;
				else
					if(CtrlPoints[ni].y<=h*yratio)
						CtrlPoints[ni].y = h*yratio;
				a1 =( CtrlPoints[1].y - CtrlPoints[0].y)/(CtrlPoints[1].x - CtrlPoints[0].x);
				b1  = CtrlPoints[1].y - a1 * CtrlPoints[1].x;
			}
			if(ni==1)
			{
		                if(CtrlPoints[ni].y<=CtrlPoints[ni-1].y)
		                        CtrlPoints[ni].y = CtrlPoints[ni-1].y+0.1;
		                else
		                        if(CtrlPoints[ni].y>=CtrlPoints[ni+1].y)
		                                CtrlPoints[ni].y = CtrlPoints[ni+1].y;
		                a1 =( CtrlPoints[1].y - CtrlPoints[0].y)/(CtrlPoints[1].x - CtrlPoints[0].x);
		                b1  = CtrlPoints[1].y - a1 * CtrlPoints[1].x;
			}
			if(ni==2)
			{
		                if(CtrlPoints[ni].y>=CtrlPoints[ni+1].y)
		                        CtrlPoints[ni].y = CtrlPoints[ni+1].y;
		                else
		                        if(CtrlPoints[ni].y<=CtrlPoints[ni-1].y)
		                                CtrlPoints[ni].y = CtrlPoints[ni-1].y;
	      		        a2 =( CtrlPoints[3].y - CtrlPoints[2].y)/(CtrlPoints[3].x - CtrlPoints[2].x);
				b2 = CtrlPoints[3].y - a2 * CtrlPoints[3].x;
			}
		        if(ni==3)
		        {
		                if(CtrlPoints[ni].y<=CtrlPoints[ni-1].y)
		                        CtrlPoints[ni].y = CtrlPoints[ni-1].y+0.1;
				else
					if(CtrlPoints[ni].y>=h)
						CtrlPoints[ni].y = h;
		                a2 =( CtrlPoints[3].y - CtrlPoints[2].y)/(CtrlPoints[3].x - CtrlPoints[2].x);
		                b2 = CtrlPoints[3].y - a2 * CtrlPoints[3].x;
		        }
		}
		if(a1==0.)b1 = CtrlPoints[1].y;
		if(a2==0.)b2 = CtrlPoints[3].y;	
}
/*
void svMagnitude::SetLinearLocation(int w, int h)
{
	int i,j;

		if(ni==4)
		{
			if(CtrlPoints[ni].x>=CtrlPoints[3].x) CtrlPoints[ni].x=CtrlPoints[3].x;
			else  if(CtrlPoints[ni].x<=CtrlPoints[0].x) CtrlPoints[ni].x=CtrlPoints[0].x;
			middle = MagWidget[0] + (CtrlPoints[ni].x - CtrlPoints[0].x)/(xscale);
			CtrlPoints[1].x	 = CtrlPoints[ni].x;
			CtrlPoints[2].x = CtrlPoints[ni].x;
		}
		else
		{
			if(ni==1)
			{
		                if(CtrlPoints[ni].y<=CtrlPoints[ni-1].y)
		                        CtrlPoints[ni].y = CtrlPoints[ni-1].y+0.1;
		                else
		                        if(CtrlPoints[ni].y>=CtrlPoints[ni+1].y)
		                                CtrlPoints[ni].y = CtrlPoints[ni+1].y;
				CtrlPoints[0].y = (CtrlPoints[ni].y - WidgetH[0][1])
						 * pow(10.,MagWidget[0])/pow(10.,middle)
						+WidgetH[0][1];


			}

		        if(ni==3)
		        {
		                if(CtrlPoints[ni].y<=CtrlPoints[ni-1].y)
		                        CtrlPoints[ni].y = CtrlPoints[ni-1].y+0.1;
				else
					if(CtrlPoints[ni].y>=h)
						CtrlPoints[ni].y = h;
				CtrlPoints[2].y = (CtrlPoints[ni].y - WidgetH[0][1])
						 * pow(10.,middle)/pow(10.,MagWidget[level-1])
						+WidgetH[0][1];
				if(CtrlPoints[1].y>=CtrlPoints[2].y)
					CtrlPoints[1].y=CtrlPoints[2].y;

		        }
		}
}
*/
svScalar *svMagnitude::GetMag(svScalar *den, int datasize, int w, int h)
{

                        a1 =( CtrlPoints[1].y - CtrlPoints[0].y)/(CtrlPoints[1].x - CtrlPoints[0].x);
                        b1  = CtrlPoints[1].y - a1 * CtrlPoints[1].x;
                        a2 =( CtrlPoints[3].y - CtrlPoints[2].y)/(CtrlPoints[3].x - CtrlPoints[2].x);
                        b2 = CtrlPoints[3].y - a2 * CtrlPoints[3].x;

	svFloat l;
	svScalar *size = new svScalar[datasize];
	svFloat lx;
	svScalar maxsize;

	for(int i=0;i<datasize;i++)
	{
		if(encode_mode==2)
		{
			l = log10(den[i]);
			lx = CtrlPoints[0].x + (l-SCALE-MagWidget[0]) * (xscale);
			//if(l>11)
				//cerr<<lx<<" ";
			if(l<middle+SCALE)
			size[i] = fabs(lx * a1 + b1 - h*yratio);
			else
			{	size[i] = fabs(lx * a2 + b2 - h*yratio);}
			size[i] = size[i]*magscale;// /yscale;
		}
		else if(encode_mode==0)
		{
			/*float tmppow = 1.;
			for(int iii=0;iii<middle;iii++)
				tmppow = tmppow * 10.;
			float tmppow2 = 1;
			for(int iii=0;iii<MagWidget[0];iii++)
				tmppow2 = tmppow2 * 10.;
			float tmppow3 = 1;
			for(int iii=0;iii<MagWidget[level-1];iii++)
                                tmppow3 = tmppow3 * 10.;
                        float tmppow4 = 1;
                        for(int iii=0;iii<SCALE;iii++)
                                tmppow4 = tmppow4 * 10.;

			float block_l1 = (tmppow - tmppow2)/(middle-MagWidget[0]);
			float block_l2 = (-tmppow + tmppow3)/(MagWidget[level-1]-middle);
			//cerr<<block_l<<endl;
			l = den[i]/tmppow4;//pow(10.,SCALE);//cerr<<l<<endl;!!!!!!!!!!!!!!
			//if(l>pow(10.,-4))
			//cerr<<lx<<" "; 
			//cerr<<middle<<" "<<MagWidget[0]<<endl;
			if(l<tmppow)
			{
				lx = CtrlPoints[0].x + (l-tmppow2)/block_l1 * (xscale);
				size[i] = fabs(lx * a1 + b1 - h*yratio);
			}
			else
			{	
				lx = CtrlPoints[2].x + (l-tmppow)/block_l2 * (xscale);
				size[i] = fabs(lx * a2 + b2 - h*yratio);
			}
			size[i] = size[i]/yscale;	*/
			float pow_middle = pow(10.,middle);
			float pow_m0 = pow(10.,MagWidget[0]);
			float block_l1 = (pow_middle - pow_m0 )/(middle-MagWidget[0]);
			float block_l2 = (-pow_middle + pow(10.,MagWidget[level-1]))/(MagWidget[level-1]-middle);
			//cerr<<block_l<<endl;
			l = den[i]/pow(10.,SCALE);//cerr<<l<<endl;!!!!!!!!!!!!!!
			//if(l>pow(10.,-4))
			//cerr<<lx<<" "; 
			//cerr<<middle<<" "<<MagWidget[0]<<endl;
			if(l<pow_middle)
			{
				lx = CtrlPoints[0].x + (l-pow_m0 )/block_l1 * (xscale);
				size[i] = abs(lx * a1 + b1 - h*yratio);
			}
			else
			{	
				lx = CtrlPoints[2].x + (l-pow_middle)/block_l2 * (xscale);
				size[i] = abs(lx * a2 + b2 - h*yratio);
			}
			size[i] = size[i]*magscale;// /yscale;	
		}
		else if(encode_mode==1)
		{
			l = (int)log10(den[i]);
			lx = CtrlPoints[0].x + (l-SCALE-MagWidget[0]) * (xscale);
			if(l<middle+SCALE)
			size[i] = fabs(lx * a1 + b1 - h*yratio);
			else
			{	size[i] = fabs(lx * a2 + b2 - h*yratio);}
			size[i] = size[i]*magscale;// /yscale;			
		}
	}

	return size;
}
svScalar *svMagnitude::GetSplitVectorMag(svScalar *den, int datasize, int w, int h)
{

                        a1 =( CtrlPoints[1].y - CtrlPoints[0].y)/(CtrlPoints[1].x - CtrlPoints[0].x);
                        b1  = CtrlPoints[1].y - a1 * CtrlPoints[1].x;
                        a2 =( CtrlPoints[3].y - CtrlPoints[2].y)/(CtrlPoints[3].x - CtrlPoints[2].x);
                        b2 = CtrlPoints[3].y - a2 * CtrlPoints[3].x;

	svFloat l;
	svScalar *size = new svScalar[datasize];
	svFloat lx;
	svScalar maxsize;

	for(int i=0;i<datasize;i++)
	{
		l = log10(den[i]);
		lx = CtrlPoints[0].x + (l-SCALE-MagWidget[0]) * (xscale);

		if(l<middle+SCALE)
			size[i] = fabs(lx * a1 + b1 - h*yratio);
		else
		{	size[i] = fabs(lx * a2 + b2 - h*yratio);}

		size[i] = size[i]/yscale;
	}
	return size;
}
svScalar *svMagnitude::GetQuadricMagA(svScalar *den, int datasize, int w, int h)
{
	svFloat l;
	svScalar *alpha = new svScalar[datasize];
	svFloat lx;
	svScalar maxsize;


	for(int i=0;i<datasize;i++)
	{
		l = log10(den[i]);
		//lx = l-SCALE-MagWidget[0];

		if(l<middle+SCALE)
			alpha[i] =-1;
		else
		{
			lx = l - SCALE - middle;	
			//alpha[i] = 0.1 + lx * (5.-0.1)/(level-1.);
			alpha[i] = PARAMETERMIN 
				+ (PARAMETERMAX-PARAMETERMIN)*lx/(MagWidget[level-1]-middle);
		}		

	}
	return alpha;
}
svScalar *svMagnitude::GetQuadricMagB(svScalar *den, int datasize, int w, int h)
{
	svFloat l;
	svScalar *beta = new svScalar[datasize];
	svFloat lx;
	svScalar maxsize;


	for(int i=0;i<datasize;i++)
	{
		l = log10(den[i]);
		//lx = l-SCALE-MagWidget[0];

		if(l<middle+SCALE)
			beta[i] =-1;
		else
		{	
			lx = l-SCALE-middle;
			beta[i] = PARAMETERMAX
				- (PARAMETERMAX-PARAMETERMIN)*lx/(MagWidget[level-1]-middle);			
			//beta[i] = 5 - lx * (5.-0.1)/(level-1.);
		}

	

	}
	return beta;
}/*
svScalar *svMagnitude::GetLinearMag(svScalar *den, int datasize,int w, int h)
{


	svFloat l;
	svScalar *size = new svScalar[datasize];
	svFloat lx;
	svScalar maxsize;
	svFloat m;

	m = pow(10,middle);
	lx = (CtrlPoints[3].y - WidgetH[0][1])
		* m/pow(10.,MagWidget[level-1]);

	for(int i=0;i<datasize;i++)
	{
		l = den[i]/pow(10.,12.);
		//lx = CtrlPoints[0].x + ((l-MagWidget[0])*pow(10.,12.)/slice) * (xscale);
		//cerr<<l<<" "<<m<<endl;
		if(l>=m)
		{
			size[i] = (CtrlPoints[3].y - WidgetH[0][1])
					* l/pow(10.,MagWidget[level-1]);
			//lx = l - m;
			//size[i] = CtrlPoints[2].y - WidgetH[0][1] 
			//		+ (CtrlPoints[3].y -CtrlPoints[2].y) 
			//		* lx /(pow(10.,MagWidget[level-1])-m);
		}
		else
		{
			if(CtrlPoints[1].y-CtrlPoints[2].y==1)
				size[i] = (CtrlPoints[1].y - WidgetH[0][1])
					* l/pow(10.,middle);
			else
				size[i] = (CtrlPoints[3].y - WidgetH[0][1])
					* l/pow(10.,MagWidget[level-1]);
			//lx = l - pow(10.,MagWidget[0]);
			//size[i] = CtrlPoints[0].y - WidgetH[0][1] 
			//		+ (CtrlPoints[1].y -CtrlPoints[0].y) 
			//		* lx /(m-pow(10.,MagWidget[0]));
			//cerr<<lx<<" "<<size[i]<<endl;
		}
		size[i] = size[i]/yscale;
	}


	return size;
}*/

void svMagnitude::cleanup()
{
	delete [] MagWidget;	
	delete [] WidgetH;

	if(superquadricpos!=NULL)
	{
		for(int i=0;i<level-llevel;i++)
			superquadricpos[i].free();
		delete [] superquadricpos;
		superquadricpos = NULL;
	}
	level = 0;	
}

void svMagnitude::cleanquadric()
{
	if(superquadricpos!=NULL)
	{
		for(int i=0;i<level-llevel;i++)
			superquadricpos[i].free();
		delete [] superquadricpos;
		superquadricpos = NULL;
	}
}

GLboolean svMagnitude::invert_matrix( const GLfloat *m, GLfloat *out )
{
/* NB. OpenGL Matrices are COLUMN major. */
#define SWAP_ROWS(a, b) { GLfloat *_tmp = a; (a)=(b); (b)=_tmp; }
#define MAT(m,r,c) (m)[(c)*4+(r)]

 GLfloat wtmp[4][8];
 GLfloat m0, m1, m2, m3, s;
 GLfloat *r0, *r1, *r2, *r3;

 r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];

 r0[0] = MAT(m,0,0), r0[1] = MAT(m,0,1),
 r0[2] = MAT(m,0,2), r0[3] = MAT(m,0,3),
 r0[4] = 1.0, r0[5] = r0[6] = r0[7] = 0.0,

 r1[0] = MAT(m,1,0), r1[1] = MAT(m,1,1),
 r1[2] = MAT(m,1,2), r1[3] = MAT(m,1,3),
 r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0,

 r2[0] = MAT(m,2,0), r2[1] = MAT(m,2,1),
 r2[2] = MAT(m,2,2), r2[3] = MAT(m,2,3),
 r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0,

 r3[0] = MAT(m,3,0), r3[1] = MAT(m,3,1),
 r3[2] = MAT(m,3,2), r3[3] = MAT(m,3,3),
 r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;

 /* choose pivot - or die */
 if (fabs(r3[0])>fabs(r2[0])) SWAP_ROWS(r3, r2);
 if (fabs(r2[0])>fabs(r1[0])) SWAP_ROWS(r2, r1);
 if (fabs(r1[0])>fabs(r0[0])) SWAP_ROWS(r1, r0);
 if (0.0 == r0[0])  return GL_FALSE;
 /* eliminate first variable     */
 m1 = r1[0]/r0[0]; m2 = r2[0]/r0[0]; m3 = r3[0]/r0[0];
 s = r0[1]; r1[1] -= m1 * s; r2[1] -= m2 * s; r3[1] -= m3 * s;
 s = r0[2]; r1[2] -= m1 * s; r2[2] -= m2 * s; r3[2] -= m3 * s;
 s = r0[3]; r1[3] -= m1 * s; r2[3] -= m2 * s; r3[3] -= m3 * s;
 s = r0[4];
 if (s != 0.0) { r1[4] -= m1 * s; r2[4] -= m2 * s; r3[4] -= m3 * s; }
 s = r0[5];
 if (s != 0.0) { r1[5] -= m1 * s; r2[5] -= m2 * s; r3[5] -= m3 * s; }
 s = r0[6];
 if (s != 0.0) { r1[6] -= m1 * s; r2[6] -= m2 * s; r3[6] -= m3 * s; }
 s = r0[7];
 if (s != 0.0) { r1[7] -= m1 * s; r2[7] -= m2 * s; r3[7] -= m3 * s; }

 /* choose pivot - or die */
 if (fabs(r3[1])>fabs(r2[1])) SWAP_ROWS(r3, r2);
 if (fabs(r2[1])>fabs(r1[1])) SWAP_ROWS(r2, r1);
 if (0.0 == r1[1])  return GL_FALSE;

 /* eliminate second variable */
 m2 = r2[1]/r1[1]; m3 = r3[1]/r1[1];
 r2[2] -= m2 * r1[2]; r3[2] -= m3 * r1[2];
 r2[3] -= m2 * r1[3]; r3[3] -= m3 * r1[3];
 s = r1[4]; if (0.0 != s) { r2[4] -= m2 * s; r3[4] -= m3 * s; }
 s = r1[5]; if (0.0 != s) { r2[5] -= m2 * s; r3[5] -= m3 * s; }
 s = r1[6]; if (0.0 != s) { r2[6] -= m2 * s; r3[6] -= m3 * s; }
 s = r1[7]; if (0.0 != s) { r2[7] -= m2 * s; r3[7] -= m3 * s; }

 /* choose pivot - or die */
 if (fabs(r3[2])>fabs(r2[2])) SWAP_ROWS(r3, r2);
 if (0.0 == r2[2])  return GL_FALSE;
 /* eliminate third variable */
 m3 = r3[2]/r2[2];
 r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
 r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6],
 r3[7] -= m3 * r2[7];
 /* last check */
 if (0.0 == r3[3]) return GL_FALSE;

 s = 1.0/r3[3];              /* now back substitute row 3 */
 r3[4] *= s; r3[5] *= s; r3[6] *= s; r3[7] *= s;

 m2 = r2[3];                 /* now back substitute row 2 */
 s  = 1.0/r2[2];
 r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
 r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
 m1 = r1[3];
 r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1,
 r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;
 m0 = r0[3];
 r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0,
 r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;

 m1 = r1[2];                 /* now back substitute row 1 */
 s  = 1.0/r1[1];
 r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1),
 r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);
 m0 = r0[2];
 r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0,
 r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;

 m0 = r0[1];                 /* now back substitute row 0 */
 s  = 1.0/r0[0];
 r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0),
 r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);

 MAT(out,0,0) = r0[4]; MAT(out,0,1) = r0[5],
 MAT(out,0,2) = r0[6]; MAT(out,0,3) = r0[7],
 MAT(out,1,0) = r1[4]; MAT(out,1,1) = r1[5],
 MAT(out,1,2) = r1[6]; MAT(out,1,3) = r1[7],
 MAT(out,2,0) = r2[4]; MAT(out,2,1) = r2[5],
 MAT(out,2,2) = r2[6]; MAT(out,2,3) = r2[7],
 MAT(out,3,0) = r3[4]; MAT(out,3,1) = r3[5],
 MAT(out,3,2) = r3[6]; MAT(out,3,3) = r3[7];

 return GL_TRUE;
#undef MAT
#undef SWAP_ROWS
}


}
