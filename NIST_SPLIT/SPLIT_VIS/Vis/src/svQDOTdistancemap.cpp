#include <vector>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <string.h>
#include "svQDOTdistancemap.h"
#include "./EMclustering/colorsEM.h"

using namespace std;

#ifndef PI
#define PI 3.1415926
#endif

namespace __svl_lib {

svQDOTdistancemap::svQDOTdistancemap()
{
	selected_layer = 0;
	selected_p = 0;

	SpiralColor = new svVector3[100];
	
	CIEColor2 *c = new CIEColor2();

	for(int i=0;i<100;i++)
	{
		vector<double> RGB = c->getHueColor(0.5, (double)i * 2.7);
		SpiralColor[i][0] = RGB[0];
		SpiralColor[i][1] = RGB[1];
		SpiralColor[i][2] = RGB[2];
	}

	delete [] c;

	dataColor = NULL;
	Selected= false;
}

void svQDOTdistancemap::SetColors(svVector3Array *pos,double * hue,svScalarArray *vel2, float z, char *file, int zlayer, int n)
{
	if(dataColor!=NULL)
	{
		dataColor[0].free();
		delete [] dataColor;
		dataColor = NULL;
	}

	dataColor = new svVector3Array[1];

	bool flag = false;
	ifstream infile(file);
	if(infile.is_open())
	{ flag = true;}
	if(flag){cerr<<file<<endl;
		for(int i=0;i<zlayer;i++)
		{//cerr<<hue[count]<<endl;
			for(int j=0;j<pos[i].size();j++)
			{
				if(abs((vel2[i][j])<=PI&&vel2[i][j]>=0)||z == pos[i][j][2])
				{
					svVector3 r;
					infile>>r[0]>>r[1]>>r[2];
					dataColor[0].add(r);
				}
				//count ++;
			}
		}
		infile.close();		
	}
	else{
		ofstream outfile(file);
		Color_Num = n + 1;

		int count = 0;
		CIEColor2 *c = new CIEColor2();

		for(int i=0;i<zlayer;i++)
		{
			for(int j=0;j<pos[i].size();j++)
			{
				if(abs(vel2[i][j])<=PI&&vel2[i][j]>=0&&pos[i][0][2]!=z)
				{
	
					vector<double> RGB = c->SinglehueSequential(0.5,0.6,0.25,0.75,
					 hue[count]);//vel2[i][j]/PI
					svVector3 r;
					//r[0] =RGB[0];r[1]=RGB[1];r[2]=RGB[2];
					double r2,g2,b2;
					ColorBlindSafe(RGB[0],RGB[1],RGB[2],r2,g2,b2);
					r[0] = r2;r[1] = g2;r[2] = b2;
					dataColor[0].add(r);
					outfile<<r[0]<<" "<<r[1]<<" "<<r[2]<<endl;
					
				}
				else if(z == pos[i][j][2])
				{
					svVector3 r;
					r[0] =0;r[1]=0;r[2]=0;
					dataColor[0].add(r);
					outfile<<r[0]<<" "<<r[1]<<" "<<r[2]<<endl;
				}

				count ++;
			}
		}
		outfile.close();
		delete [] c;

	}

}

void svQDOTdistancemap::SetColors(int n)
{
	delete [] SpiralColor;

	Color_Num = n + 1;

	SpiralColor = new svVector3[Color_Num];
	
	CIEColor2 *c = new CIEColor2();

	for(int i=0;i<Color_Num;i++)
	{
		
		vector<double> RGB = c->getHueColor(0.5, (double)i * (double)(270./Color_Num));
		SpiralColor[i][0] = RGB[0];
		SpiralColor[i][1] = RGB[1];
		SpiralColor[i][2] = RGB[2];
	}

	delete [] c;
}

void svQDOTdistancemap::SetColors(int n, int ln, vector<double> hue, int *idx, svVector3Array *pos, svScalarArray *vel, int zlayer, float z, double *vr, double *vg, double *vb)
{
	num_h = n;
	num_l = ln;
	select_x = 9e+9;
	select_y = 9e+9;
	select_index = -1;
	select_col[0]= -1;
	select_col[1] = -1;
	select_col[2] = -1;

	if(dataColor!=NULL)
	{
		dataColor[0].free();
		delete [] dataColor;
		dataColor = NULL;
	}

	dataColor = new svVector3Array[1];

	
	//delete [] SpiralColor;
	SpiralColor = new svVector3[n*ln];
	//delete [] SpiralColor_order;
	SpiralColor_order = new svVector3[n*ln];

	double	h1 = 0;
	double	h2 = 300;

	double t =0;
	double degree = 1./(ln-1.);
	CIEColor2 *cc = new CIEColor2();
	for(int i=0;i<n;i++)
	{	
		t = 0;
		for(int j=0;j<ln;j++)
		{
			vector<double> RGB = cc->SinglehueSequential(t, 1, 0.25, 0.5, hue[i]);
			t = t + degree;
			svVector3 r;
			double r2,g2,b2;
			ColorBlindSafe(RGB[0],RGB[1],RGB[2],r2,g2,b2);
			r[0] = r2;r[1] = g2;r[2] = b2;
			SpiralColor[ln*i+j] = r; 
		}
	}	

	int count = 0;
	double min_degree = PI/(double)ln;
	for(int i=0;i<zlayer;i++)
	{
		for(int j=0;j<pos[i].size();j++)
		{
			if(fabs(vel[i][j])<4&&(vel[i][j]>0||fabs(vel[i][j])<0.0001)&&pos[i][0][2]!=z)
			{
				int index = vel[i][j]/min_degree;
				if(index == ln) index = index - 1;
				index = idx[count] * ln + index;
				dataColor[0].add(SpiralColor[index]);
			}
			else if(pos[i][0][2]==z)
			{
				svVector3 r;
				r[0] = vr[count];
				r[1] = vg[count];
				r[2] = vb[count];
				dataColor[0].add(r);

			}		
			count ++;
		}

		if(pos[i][0][2] == z)
		{
			lb = pos[i][0];
			rb = pos[i][pos[i].size()-1];
		}
		
	}

	sort(hue.begin(), hue.end());
	t = 0;
	for(int i=0;i<n;i++)
	{	
		t = 0;
		for(int j=0;j<ln;j++)
		{
			vector<double> RGB = cc->SinglehueSequential(t, 1, 0.25, 0.5, hue[i]);
			t = t + degree;
			svVector3 r;
			double r2,g2,b2;
			ColorBlindSafe(RGB[0],RGB[1],RGB[2],r2,g2,b2);
			r[0] = r2;r[1] = g2;r[2] = b2;
			SpiralColor_order[ln*i+j] = r; 
		}
	}
	
	delete [] cc;
}


void svQDOTdistancemap::Render2dLegend()
{
	float x = 0;
	float y = 0;
	float size_x = 10./(float)num_h;
	float size_y = 10./(float)num_l;
	char s[50];

	for(int i=0;i<num_h;i++)
	{
		for(int j=0;j<num_l;j++)
		{
			glColor3f(SpiralColor_order[i*num_l + j][0],SpiralColor_order[i*num_l + j][1],SpiralColor_order[i*num_l + j][2]);
			glBegin(GL_QUADS);
			glVertex2f(x,y);
			glVertex2f(x+size_x,y);
			glVertex2f(x+size_x,y+size_y);
			glVertex2f(x,y+size_y);		
			glEnd();

			if(fabs(SpiralColor_order[i*num_l + j][0] - select_col[0])<=0.0001
			&& fabs(SpiralColor_order[i*num_l + j][1] - select_col[1])<=0.0001
			&& fabs(SpiralColor_order[i*num_l + j][2] - select_col[2])<=0.0001)
			{
				glColor3f(0.,0.,0.);
				glBegin(GL_LINE_LOOP);
				glVertex2f(x,y);
				glVertex2f(x+size_x,y);
				glVertex2f(x+size_x,y+size_y);
				glVertex2f(x,y+size_y);		
				glEnd();
			}

			y = y + size_y;


		}
		y = 0;
		x = x + size_x;
	}

	glColor3f(1.,1.,1.);
	x = -1;
	y = 0;
	sprintf(s,"0");
	glRasterPos2f(x,y);
	for(int ii=0;ii<strlen(s);ii++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, s[ii]);

	y = num_l * size_y;
	sprintf(s,"180");
	glRasterPos2f(x,y);
	for(int ii=0;ii<strlen(s);ii++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, s[ii]);

	y = num_l * size_y;
	sprintf(s,"180");
	glRasterPos2f(x,y);
	for(int ii=0;ii<strlen(s);ii++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, s[ii]);

	y = num_l* size_y + 1.;
	sprintf(s,"Angle");
	glRasterPos2f(x,y);
	for(int ii=0;ii<strlen(s);ii++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, s[ii]);

	x = num_h * size_x + 1.;
	y = 0;
	sprintf(s,"Cluster");
	glRasterPos2f(x,y);
	for(int ii=0;ii<strlen(s);ii++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, s[ii]);

}

void svQDOTdistancemap::Render(svVector3Array *pos,svScalarArray *vel2, bool *in,int zlayer,  float xd, float yd, float z)
{
	glEnable (GL_BLEND); 
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	int count = 0;
	int count2 = 0;	
	char str[100];
	glEnable(GL_BLEND);
	double alpha = 1.;
	for(int i=0;i<zlayer;i++)
	{//cerr<<i<<" ";


		for(int j=0;j<pos[i].size();j++)
		{
			if(in[count2])
				alpha = 1;
			else
				alpha = 0.5;
			
			if((fabs(vel2[i][j])<4&&(vel2[i][j]>0||fabs(vel2[i][j])<0.0001)&&z!=pos[i][0][2])
			|| z == pos[i][0][2])
			{
				
				glColor4f(dataColor[0][count][0],dataColor[0][count][1],dataColor[0][count][2],alpha);
			
				glBegin(GL_QUADS);
				glVertex2f(pos[i][j][0]-xd/2.,pos[i][j][1]-yd/2.);
				glVertex2f(pos[i][j][0]+xd/2.,pos[i][j][1]-yd/2.);
				glVertex2f(pos[i][j][0]+xd/2.,pos[i][j][1]+yd/2.);
				glVertex2f(pos[i][j][0]-xd/2.,pos[i][j][1]+yd/2.);
				glEnd();

				if(select_x == i&&select_y==j)
				{
				glColor3f(0.,0.,0.);
				glBegin(GL_LINE_LOOP);
				glVertex2f(pos[i][j][0]-xd/2.,pos[i][j][1]-yd/2.);
				glVertex2f(pos[i][j][0]+xd/2.,pos[i][j][1]-yd/2.);
				glVertex2f(pos[i][j][0]+xd/2.,pos[i][j][1]+yd/2.);
				glVertex2f(pos[i][j][0]-xd/2.,pos[i][j][1]+yd/2.);
				glEnd();
				}

				count ++; 
			}
			else
			{
				//if(pos[i][j][2] == 7.)
					//cerr<<pos[i][j][0]<<" "<<pos[i][j][1]<<" "<<vel2[i][j]<<endl;
			}
			count2++;
		}


		if(pos[i][0][2] == z)
		{
			glColor3f(1.,1.,0.);
			glBegin(GL_LINE_LOOP);
			glVertex2f(pos[i][0][0]-xd/2.,pos[i][0][1]-yd/2.);
			glVertex2f(pos[i][pos[i].size()-1][0]+xd/2.,pos[i][0][1]-yd/2.);
			glVertex2f(pos[i][pos[i].size()-1][0]+xd/2.,pos[i][pos[i].size()-1][1]+yd/2.);
			glVertex2f(pos[i][0][0]-xd/2.,pos[i][pos[i].size()-1][1]+yd/2.);
			glEnd();			
		}

		//if(i%4==0){

		glColor3f(1.,1.,1.);
		sprintf(str,"%.2f",pos[i][0][2]);
		glRasterPos2f(pos[i][0][0],pos[i][0][1]);
        	for(int j=0;j<strlen(str);j++)
            		 glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
		//}
		
	}
}

void svQDOTdistancemap::Render(svVector3Array *pos,svScalarArray *vel,bool *in,int zlayer,  float xd, float yd)
{
glEnable (GL_BLEND); glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	double alpha;
	char str[100];
	int count = 0;
	for(int i=0;i<zlayer;i++)
	{

		if(in[count])
			alpha = 1;
		else
			alpha = 0.1;
		for(int j=0;j<pos[i].size();j++)
		{
			/*float d = PI/18.;
			float c = vel[i][j]/d;
			float cc = (float)c / 18.;*/
			//int c = 50 + (int)vel[i][j];
			float cc = vel[i][j]/PI;

			if(vel[i][j]>=0)
			{
				//cerr<<cc<<" ";
				glColor4f(cc,cc,cc,alpha);
			
				glBegin(GL_QUADS);
				glVertex2f(pos[i][j][0]-xd/2.,pos[i][j][1]-yd/2.);
				glVertex2f(pos[i][j][0]+xd/2.,pos[i][j][1]-yd/2.);
				glVertex2f(pos[i][j][0]+xd/2.,pos[i][j][1]+yd/2.);
				glVertex2f(pos[i][j][0]-xd/2.,pos[i][j][1]+yd/2.);
				glEnd();
			}
		}
		count ++;
		if(i%5==0){

		glColor3f(1.,1.,1.);
		sprintf(str,"%.2f",pos[i][0][2]);
		glRasterPos2f(pos[i][0][0],pos[i][0][1]);
        	for(int j=0;j<strlen(str);j++)
            		 glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);}
	}
}

void svQDOTdistancemap::RenderPlaneComparision(svVector3Array *pos,svScalarArray *vel,bool *in, int zlayer, float xd, float yd)
{
	//CIEColor *c = new CIEColor();
glEnable (GL_BLEND); glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	double alpha;
	char str[100];
	int count = 0;

	for(int i=0;i<zlayer;i++)
	{

		for(int j=0;j<pos[i].size();j++)
		{

			if(in[count])
				alpha = 1;
			else
				alpha = 0.1;
			//cerr<<alpha<<endl;

			if(fabs(vel[i][j])<=100.)
			{
				int cc = (int)(Color_Num/2) + vel[i][j];
				//cerr<<cc<<" "<<vel[i][j]<<" ";

				if(vel[i][j]==0)
					glColor4f(12./255.,44./255.,132./355.,alpha);
				else
					glColor4f(255./255.,237./255.,160./355.,alpha);	
		//glColor4f(SpiralColor[cc][0],SpiralColor[cc][1],SpiralColor[cc][2],alpha);
			
				glBegin(GL_QUADS);
				glVertex2f(pos[i][j][0]-xd/2.,pos[i][j][1]-yd/2.);
				glVertex2f(pos[i][j][0]+xd/2.,pos[i][j][1]-yd/2.);
				glVertex2f(pos[i][j][0]+xd/2.,pos[i][j][1]+yd/2.);
				glVertex2f(pos[i][j][0]-xd/2.,pos[i][j][1]+yd/2.);
				glEnd();
			}
			count ++;
		}
		if(i%5==0){

		glColor3f(1.,1.,1.);
		sprintf(str,"%.2f",pos[i][0][2]);
		glRasterPos2f(pos[i][0][0],pos[i][0][1]);
        	for(int j=0;j<strlen(str);j++)
            		 glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);}
	}

	//delete [] c;
}

void svQDOTdistancemap::RenderLegend()
{
	float x = 0;
	float y = 0;
	float size = 0.1;
	char s[50];

	sprintf(s,"Angle");
		glRasterPos2f(x-2,y-2);
		for(int ii=0;ii<strlen(s);ii++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, s[ii]);

	for(int i=0;i<=180;i++)
	{
		//glColor3f((float)i/18.,(float)i/18.,(float)i/18.);
		glColor3f((float)i/180.,(float)i/180.,(float)i/180.);
		glBegin(GL_QUADS);
		glVertex2f(x,y);
		glVertex2f(x,y-1);
		glVertex2f(x+size,y-1);
		glVertex2f(x+size,y);		
		glEnd();

		glColor3f(1.,1.,1.);

		if(i%90==0){
		sprintf(s,"%d",(180-i)/90-1);

		glRasterPos2f(x,y-2);
		for(int ii=0;ii<strlen(s);ii++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, s[ii]);

		glColor3f(1.,1.,0.);
		glBegin(GL_LINES);
		glVertex2f(x,y);
		glVertex2f(x,y-1);
		glEnd();			
		}

		x = x + size;		
	}
}

void svQDOTdistancemap::RenderColorLegend()
{
	CIEColor2 *c = new CIEColor2();
	float x = 0;
	float y = 0;
	float size = 0.1*(180/Color_Num);
	char s[50];

	for(int i=0;i<2;i++)//Color_Num;i++)
	{
		double H = i;
		vector<double> RGB = c->getHueColor(0.5,H);
		glColor3f(SpiralColor[i][0],SpiralColor[i][1],SpiralColor[i][2]);

		glBegin(GL_QUADS);
		glVertex2f(x,y);
		glVertex2f(x,y-1);
		glVertex2f(x+size,y-1);
		glVertex2f(x+size,y);		
		glEnd();

		glColor3f(1.,1.,1.);
		if(i%(Color_Num/2)==0||i==Color_Num-1)
		{sprintf(s,"%d",i);//-Color_Num/2);

		glRasterPos2f(x+size/3.,y-2);
		for(int ii=0;ii<strlen(s);ii++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, s[ii]);
		}

		x = x + size;		
	}
	delete [] c;
}

void svQDOTdistancemap::RenderPlane(int xnum, int ynum, int zlayer,float xl, float yl,
		int mouse_x, int mouse_y, float scale,float zoom, float xpan, float ypan,float r ,int w, int h)
{
	glColor3f(1.,1.,1.);

	int count =  ynum * ynum;
	int sub = zlayer - count + 1;
	float x,y;
	float size_x = 1;
	float size_y = size_x * yl/xl;
	x = 0;
	y = 0;
	char s[50];

	//mouse_x = mouse_x / (xl*scale*r) + size_x*xnum/2.;
	//mouse_y = mouse_y / (yl*scale*r) - size_y*ynum/2.;

	glPushMatrix();

	glScalef(xl*scale*r,yl*scale*r,1.);
	glTranslatef(-size_x*xnum/2.,size_y*ynum/2.,0.);
	for(int i=0;i<xnum+1;i++)
	{
		glBegin(GL_LINES);
		glVertex2f(x,y);
		glVertex2f(x,y-size_y*(ynum-1));
		glEnd();
		
	

		x = x + size_x;
	}

	x=0;y=0;
	for(int i=0;i<ynum;i++)
	{
		glBegin(GL_LINES);
		glVertex2f(x,y);
		glVertex2f(x + size_x * xnum,y);
		glEnd();
		y = y - size_y;
	}
	if(sub!=0)
	{
		for(int i=0;i<sub+1;i++)
		{
			glBegin(GL_LINES);
			glVertex2f(x,y);
			glVertex2f(x,y+size_y);
			glEnd();
			x = x + size_x;		
		}
		glBegin(GL_LINES);
		glVertex2f(x-size_x,y);
		glVertex2f(x-size_x*(sub+1),y);
		glEnd();

	}

	glPopMatrix();


	size_x = xl*scale*r;
	size_y = yl*scale*r *  yl/xl;
	x = -size_x*xnum/2.;
	y = size_y*ynum/2.;
	bool flag = false;
	for(int i=0;i<zlayer;i++)
	{
		//cerr<<x + size_x * (i%xnum)<<" "<<x + size_x * (i%xnum+1)
		//	<<" "<<y-size_y*(i/xnum)<<" "<<y-size_y*(i/xnum+1)<<endl;
		if(mouse_x>x + size_x * (i%xnum)
		&& mouse_x<x + size_x * (i%xnum+1)
		&& mouse_y<y-size_y*(i/xnum)
		&& mouse_y>y-size_y*(i/xnum+1))
		{
			flag = true;
			selected_layer = i;
		}
		if(flag)
			glColor3f(1.,1.,0.);
		else
			glColor3f(1.,1.,1.);
		sprintf(s,"%d", i+1);
		glRasterPos2f(x + size_x * (i%xnum),y-size_y*(i/xnum+1)+size_y/10.);
		for(int ii=0;ii<strlen(s);ii++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, s[ii]);
		flag = false;
	}

	x = 0;
	y = 0;
	glColor3f(1.,1.,0.);
	glPushMatrix();
	glTranslatef(-xpan*r/zoom,-ypan*r/zoom,0.);
	glScalef(1./zoom,1./zoom,1.);
	glBegin(GL_LINE_LOOP);
	glVertex2f(x-w/2.,y-h/2.);
	glVertex2f(x+w/2.,y-h/2.);
	glVertex2f(x+w/2.,y+h/2.);
	glVertex2f(x-w/2.,y+h/2.);
	glEnd();
	glPopMatrix();
}

void svQDOTdistancemap::RenderOnePlane(svVector3Array pos, int w, int h, float xl, float yl,
		int mouse_x, int mouse_y, bool global)
{
	float size_x;
	float size_y;
	float x = 0;
	float y = 0;
	float r;
	char s[50];

	if(h/yl<w/xl)
	{
		r = (float)h/yl;
		r = r -1;
	}
	else
	{
		r = (float)w/xl;
		r = r -1;
	}
	
	size_x = xl * r;
	size_y = yl * r;
	x = -size_x/2.;
	y = size_y/2.;

	glColor3f(1.,1.,1.);
	glBegin(GL_LINE_LOOP);
	glVertex2f(x,y);
	glVertex2f(x,y-size_y);
	glVertex2f(x+size_x,y-size_y);
	glVertex2f(x+size_x,y);
	glEnd();

	svVector3 p = selected_point;
	double td = 9e+9;
	double dd;
	if(global)
	{
		//cerr<<mouse_x<<" "<<mouse_y<<" "<<x<<" "<<y<<endl; 
		if(mouse_x>=x&&mouse_x<=x+size_x
			&&mouse_y>=y-size_y&&mouse_y<=y)
		{for(int i=0;i<pos.size();i++)
		{
			dd = (pos[i][0]*r-mouse_x)*(pos[i][0]*r-mouse_x)
				+ (pos[i][1]*r-mouse_y)*(pos[i][1]*r-mouse_y);
			if(dd<td)
			{
				td = dd;
				p = pos[i];
				selected_point = p;
				selected_p = i;
			}
		}}

		glPointSize(5.);
		glColor3f(1.,1.,0.);
		glBegin(GL_POINTS);
		glVertex2f(p[0]*r,p[1]*r);
		glEnd();

		sprintf(s,"(%0.2f, %0.2f, %0.2f)", p[0], p[1], p[2]);
		glRasterPos2f(p[0]*r-5., p[1]*r + 5.);
		for(int ii=0;ii<strlen(s);ii++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, s[ii]);
	}
}

void svQDOTdistancemap::SelectPoint(float x, float y,svVector3Array *pos,svScalarArray *vel2,int zlayer, float xd, float yd, float z)
{

	int count = 0;
	int count2 = 0;

	Selected = false;
	//cerr<<x<<" "<<y<<endl;
	for(int i=0;i<zlayer;i++)
	{
		for(int j=0;j<pos[i].size();j++)
		{
			if((fabs(vel2[i][j])<4&&vel2[i][j]>=0&&z!=pos[i][0][2])
			|| z == pos[i][0][2])
			{
				if(x>=pos[i][j][0]-xd/2.
				&& x<=pos[i][j][0]+xd/2.
				&& y>=pos[i][j][1]-yd/2.
				&& y<=pos[i][j][1]+yd/2.)
				{
					Selected = true;
					select_x = i;
					select_y = j;
					select_index = count2;
					select_col = dataColor[0][count];
					break;
				}
				count ++;
			}
			count2++;
		}
		if(Selected)
		{
			cerr<<select_index<<endl;
			cerr<<select_col[0]<<" "<<select_col[1]<<" "<<select_col[2]<<endl;
			break;
		}		

	}

}

void svQDOTdistancemap::SelectBox(int x, int y)
{
	if(x >= lb[0] && x <=rb[0]
		&& y>=lb[1] && y<=rb[1])
	{
		Selected = true;
	}
	else
		Selected = false;

	mouse_x = x;
	mouse_y = y;
}

void svQDOTdistancemap::MoveBox_bymouse(int x, int y)
{
	float deltax = x - mouse_x;
	float deltay = y - mouse_y;

		

	mouse_x = x;
	mouse_y = y;
}

void svQDOTdistancemap::RenderText(int n)
{
	char s[50];
	sprintf(s,"Please input #neighbors: %d", n);
	glRasterPos2f(0,0);
	for(int ii=0;ii<strlen(s);ii++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, s[ii]);
}

}
