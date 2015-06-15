#include "svStickyPane.h"


#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <string.h>

#include <vector>
#include <algorithm>
#include <cmath>

#ifndef PI
#define PI 3.1415926
#endif

using namespace std;

namespace __svl_lib
{

void svStickyPane::SetData(svVector3 *pos, svVector3 *dir, int datasize)
{
	if(dataPos!=NULL)
	{
		cleanup();
	}

	dataPos = new svVector3Array[1];
	dataDir = new svVector3Array[1];
	dirIndex = new svIntArray[1];

	for(int i=0;i<datasize;i++)
	{
		dataPos[0].add(pos[i]);
		dataDir[0].add(normalize(dir[i]));
	}

/*
	arrowIndex = datasize/2;
	arrowPos = dataPos[0][arrowIndex];
	arrowDir = dataDir[0][arrowIndex];
*/

	for(int i=0;i<datasize;i++)
	{
		if(dataPos[0][i][0]>=ROIl[0]&&dataPos[0][i][0]<=ROIr[0]
			&&dataPos[0][i][1]>=ROIl[1]&&dataPos[0][i][1]<=ROIr[1]
			&&dataPos[0][i][2]>=ROIl[2]&&dataPos[0][i][2]<=ROIr[2])
		{arrowIndex=i;break;}
	}
	//arrowIndex = datasize/2;
	arrowPos = dataPos[0][arrowIndex];
	arrowDir = dataDir[0][arrowIndex];
	
}
void svStickyPane::Find_Widget(int x, int y)
{
    int i, j;

    GLdouble td, dd;
    GLdouble s;
    svVector3 p[2];

   // glGetIntegerv (GL_VIEWPORT, viewport);
   // glGetDoublev (GL_MODELVIEW_MATRIX, mvmatrix);
   // glGetDoublev (GL_PROJECTION_MATRIX, projmatrix);

    p[0][0]=0;p[0][1]=0;p[0][2]=0;
    p[1][0]=0;p[1][1]=0;p[1][2]=0;

	p[0][0]  = arrowPos[0] * tb_transform[0][0] +
		arrowPos[1] * tb_transform[1][0] +
		arrowPos[2] * tb_transform[2][0] +
		tb_transform[3][0] + p[0][0];
	p[0][1]  = arrowPos[0] * tb_transform[0][1] +
		arrowPos[1] * tb_transform[1][1] +
		arrowPos[2] * tb_transform[2][1] +
		tb_transform[3][1] + p[0][1];
	p[0][2]  = arrowPos[0] * tb_transform[0][2] +
		arrowPos[1] * tb_transform[1][2] +
		arrowPos[2] * tb_transform[2][2] +
		tb_transform[3][2] + p[0][2];	
	gluProject((GLdouble)p[0][0],
		   (GLdouble)p[0][1],
		   (GLdouble)p[0][2],
		   mvmatrix, projmatrix, viewport,
		   &(wx[0]), &(wy[0]), &(wz[0]));
	wy[0] = (GLdouble)width-wy[0];

	p[1][0]  = sizePos[0] * tb_transform[0][0] +
		sizePos[1] * tb_transform[1][0] +
		sizePos[2] * tb_transform[2][0] +
		tb_transform[3][0] + p[1][0];
	p[1][1]  = sizePos[0] * tb_transform[0][1] +
		sizePos[1] * tb_transform[1][1] +
		sizePos[2] * tb_transform[2][1] +
		tb_transform[3][1] + p[1][1];
	p[1][2]  = sizePos[0] * tb_transform[0][2] +
		sizePos[1] * tb_transform[1][2] +
		sizePos[2] * tb_transform[2][2] +
		tb_transform[3][2] + p[1][2];	
	gluProject((GLdouble)p[1][0],
		   (GLdouble)p[1][1],
		   (GLdouble)p[1][2],
		   mvmatrix, projmatrix, viewport,
		   &(wx[1]), &(wy[1]), &(wz[1]));

	wy[1] = (GLdouble)width-wy[1];	

	dd = ((GLdouble)x-wx[0]) *  ((GLdouble)x-wx[0]) +
                 ((GLdouble)y-wy[0]) * ((GLdouble)y-wy[0]);
	td = ((GLdouble)x-wx[1]) *  ((GLdouble)x-wx[1]) +
                 ((GLdouble)y-wy[1]) * ((GLdouble)y-wy[1]);
	s = (wx[1]-wx[0]) *  (wx[1]-wx[0]) +
                 (wy[1]-wy[0]) * (wy[1]-wy[0]);

	if(dd<td)
	{
		if(dd<s){SELECTED = GL_TRUE;ni=0;}
	}
	//else
	//{
		//if(td<36){SELECTED = GL_TRUE;ni=1;}
	//}

}

void svStickyPane::Set2DLocation()
{
    int i, j;

    GLdouble td, dd;
    svVector3 p[2];

   // glGetIntegerv (GL_VIEWPORT, viewport);
    //glGetDoublev (GL_MODELVIEW_MATRIX, mvmatrix);
  // glGetDoublev (GL_PROJECTION_MATRIX, projmatrix);

    p[0][0]=0;p[0][1]=0;p[0][2]=0;
    p[1][0]=0;p[1][1]=0;p[1][2]=0;

	p[0][0]  = arrowPos[0] * tb_transform[0][0] +
		arrowPos[1] * tb_transform[1][0] +
		arrowPos[2] * tb_transform[2][0] +
		tb_transform[3][0] + p[0][0];
	p[0][1]  = arrowPos[0] * tb_transform[0][1] +
		arrowPos[1] * tb_transform[1][1] +
		arrowPos[2] * tb_transform[2][1] +
		tb_transform[3][1] + p[0][1];
	p[0][2]  = arrowPos[0] * tb_transform[0][2] +
		arrowPos[1] * tb_transform[1][2] +
		arrowPos[2] * tb_transform[2][2] +
		tb_transform[3][2] + p[0][2];	
	gluProject((GLdouble)p[0][0],
		   (GLdouble)p[0][1],
		   (GLdouble)p[0][2],
		   mvmatrix, projmatrix, viewport,
		   &(wx[0]), &(wy[0]), &(wz[0]));
	wy[0] = (GLdouble)width-wy[0];
}

void svStickyPane::move_ctrlpoint(int x, int y, int w, int h)
{	
	int index;
    GLdouble xx, yy, zz;
    GLdouble wld_x, wld_y, wld_z;       /* world coordinate */
    GLdouble tx, ty, tz;

    GLfloat invert_tb[16], tm[16];
    int i, j, k;
    GLdouble td,dd;
    float dot;
    svVector3 tmp;
    svVector3 p;
    GLdouble tmpx, tmpy, tmpz;

	int tmpn;
	bool flagone;
	bool flagother;

    //glGetIntegerv (GL_VIEWPORT, viewport);
   // glGetDoublev (GL_MODELVIEW_MATRIX, mvmatrix);
   // glGetDoublev (GL_PROJECTION_MATRIX, projmatrix);

	if(ni == 0)
	{
		int sampleSize = dataPos[0].size();

		dd=9e+9;
		for(i=0;i<sampleSize;)
		{
			flagone = false;
			flagother = false;
			if(ONE)
			{
				tmpn = (int)dataPos[0][i][2];
				if(fabs(dataPos[0][i][2]-(float)tmpn)==0.
					|| fabs(dataPos[0][i][2]-(float)tmpn)==0.5)
				{
					flagone = true;
				}
			}
			else if(OTHER)
			{
				tmpn = (int)dataPos[0][i][2];
				if(fabs(dataPos[0][i][2]-(float)tmpn)==0.25
					|| fabs(dataPos[0][i][2]-(float)tmpn)==0.75)
				{
					flagother = true;
				}
			}

			if((ONE==true&&flagone==true)
				|| (OTHER==true&&flagother==true)
				||(ONE==false&&OTHER==false))
			{
			if(dataPos[0][i][0]>=ROIl[0]&&dataPos[0][i][0]<=ROIr[0]
				&&dataPos[0][i][1]>=ROIl[1]&&dataPos[0][i][1]<=ROIr[1]
				&&dataPos[0][i][2]>=ROIl[2]&&dataPos[0][i][2]<=ROIr[2])
   		 	{
				p[0]=0;p[1]=0;p[2]=0;

				p[0]  = dataPos[0][i][0] * tb_transform[0][0] +
					dataPos[0][i][1] * tb_transform[1][0] +
					dataPos[0][i][2] * tb_transform[2][0] +
					tb_transform[3][0] + p[0];
				p[1]  = dataPos[0][i][0] * tb_transform[0][1] +
					dataPos[0][i][1] * tb_transform[1][1] +
					dataPos[0][i][2] * tb_transform[2][1] +
					tb_transform[3][1] + p[1];
				p[2]  = dataPos[0][i][0] * tb_transform[0][2] +
					dataPos[0][i][1] * tb_transform[1][2] +
					dataPos[0][i][2] * tb_transform[2][2] +
					tb_transform[3][2] + p[2];	
				gluProject((GLdouble)p[0],
			   		(GLdouble)p[1],
			   		(GLdouble)p[2],
			  		 mvmatrix, projmatrix, viewport,
			  		 &(tmpx), &(tmpy), &(tmpz));
				tmpy = (GLdouble)width-tmpy;
				if(tmpx>=0.&&tmpx<=w&&tmpy>=0.&&tmpy<=h)
				{
					td = (x-tmpx)*(x-tmpx)+
						(y-tmpy)*(y-tmpy);
			
					if(td<dd)
					{	index = i;
						dd = td;
					}		
				}
			}
			}
			i = i + Frequency;
		}
		arrowPos = dataPos[0][index];
		arrowDir = normalize(dataDir[0][index]);
		arrowIndex = index;

		wx[0] = x;
		wy[0] = y;

	}
	else
	{
		/*dot =fabs( (sizePos[0]-arrowPos[0])*(wld_x-arrowPos[0])
			+(sizePos[1]-arrowPos[1])*(wld_x-arrowPos[1])
			+(sizePos[2]-arrowPos[2])*(wld_x-arrowPos[2]));
		//if(dot<=0)
		//{radius = 0; r = 0;}
		//else
		//{
			float tmp;
			tmp = dot/(sqrt(pow(sizePos[0]-arrowPos[0],2.)
				+pow(sizePos[1]-arrowPos[1],2.)
				+pow(sizePos[2]-arrowPos[2],2.)));
			if(tmp<2)
				radius = tmp;
		//}*/
	}
	tmp = svGetPerpendicularVector(arrowDir);
	tmp = tmp * (radius+1);
	sizePos = tmp + arrowPos;
	SetPane();
}

void svStickyPane::SetPane()
{
	svVector3 p;
	svVector3 n;
	svVector3 o,o2;
	svVector3 tmp;

	n = svGetPerpendicularVector(arrowDir);
	n = n * radius;
	p = arrowPos + n;

	n = svGetPerpendicularVector(arrowDir);
	n = n * (radius-r);
	o = n + arrowPos;

	tmp = arrowPos + arrowDir;
	n = svGetNorm(o, arrowPos, tmp);
	
	for(int i=0;i<SECTION2;i++)
	{
		panePos[i] = svGetRotatePoint(p, o, n, i * 360./SECTION2); 
		paneNorm[i] = panePos[i] - o;
	}	
	
	o2 = o;
	for(int i=1;i<SECTION1;i++)
	{
		o = svGetRotatePoint(o2, arrowPos, arrowDir, i * 360./SECTION1); 
		for(int j=0;j<SECTION2;j++)
		{
			panePos[j+i*SECTION2] = svGetRotatePoint(panePos[j], arrowPos, arrowDir, i * 360./SECTION1); 
			paneNorm[j+i*SECTION2] = panePos[j+i*SECTION2] - o;
			
		}
	}
}

void svStickyPane::SetConnection()
{
	if(dirIndex!=NULL)
	{
		dirIndex[0].free();
		delete [] dirIndex;
		dirIndex=NULL;
	}

	dirIndex = new svIntArray[1];

	int sample = dataPos[0].size();

	for(int i=0;i<sample;i++)
	{
		if(dataDir[0][i][0]==arrowDir[0]
			&&dataDir[0][i][1]==arrowDir[1]
			&&dataDir[0][i][2]==arrowDir[2])
		{
			dirIndex[0].add(i);
		}
	}
}

void svStickyPane::RenderStickyPane()
{
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	//glDisable(GL_DEPTH_TEST);
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	int index1,index2,index3,index4;

	glColor3f(1.,1.,0.);

	for(int i=0;i<SECTION1;i++)
	{
		if(i<SECTION1-1)
			for(int j=0;j<SECTION2;j++)
			{
				if(j!=SECTION2-1)
				{
					index1 = j+i*SECTION2;
					index2 = j+i*SECTION2+1;
					index3 = j+(i+1)*SECTION2+1;
					index4 = j+(i+1)*SECTION2;
				}
				else
				{
					index1 = j+i*SECTION2;
					index2 = i*SECTION2;
					index3 = (i+1)*SECTION2;
					index4 = j+(i+1)*SECTION2;
				}
				glBegin(GL_QUADS);
				glNormal3f(paneNorm[index1][0],paneNorm[index1][1],paneNorm[index1][2]);
				glVertex3f(panePos[index1][0],panePos[index1][1],panePos[index1][2]);

				glNormal3f(paneNorm[index2][0],paneNorm[index2][1],paneNorm[index2][2]);
				glVertex3f(panePos[index2][0],panePos[index2][1],panePos[index2][2]);

				glNormal3f(paneNorm[index3][0],paneNorm[index3][1],paneNorm[index3][2]);
				glVertex3f(panePos[index3][0],panePos[index3][1],panePos[index3][2]);

				glNormal3f(paneNorm[index4][0],paneNorm[index4][1],paneNorm[index4][2]);
				glVertex3f(panePos[index4][0],panePos[index4][1],panePos[index4][2]);
				glEnd();
			}
		else
		{
			for(int j=0;j<SECTION2;j++)
			{
				if(j!=SECTION2-1)
				{
					index1 = j+i*SECTION2;
					index2 = j+i*SECTION2+1;
					index3 = j+1;
					index4 = j;
				}
				else
				{
					index1 = j+i*SECTION2;
					index2 = i*SECTION2;
					index3 = 0;
					index4 = j;
				}	

				glBegin(GL_QUADS);
				glNormal3f(paneNorm[index1][0],paneNorm[index1][1],paneNorm[index1][2]);
				glVertex3f(panePos[index1][0],panePos[index1][1],panePos[index1][2]);

				glNormal3f(paneNorm[index2][0],paneNorm[index2][1],paneNorm[index2][2]);
				glVertex3f(panePos[index2][0],panePos[index2][1],panePos[index2][2]);

				glNormal3f(paneNorm[index3][0],paneNorm[index3][1],paneNorm[index3][2]);
				glVertex3f(panePos[index3][0],panePos[index3][1],panePos[index3][2]);

				glNormal3f(paneNorm[index4][0],paneNorm[index4][1],paneNorm[index4][2]);
				glVertex3f(panePos[index4][0],panePos[index4][1],panePos[index4][2]);
				glEnd();
			}
		}
	}
	
	svVector3 p[4];
	svVector3 n[5];
	svVector3 o;

	p[0] = 2*r*svGetPerpendicularVector(arrowDir)+arrowPos+arrowDir*1.5;
	for(int i=1;i<4;i++)
	{
		p[i] = svGetRotatePoint(p[0], arrowPos+arrowDir*1.5, arrowDir, i*90); 
	}
	for(int i=0;i<3;i++)
	{
		n[i] = svGetNorm(p[i], arrowPos+arrowDir*2,p[i+1]);
	}
	n[3] = svGetNorm(p[3], arrowPos+arrowDir*2,p[0]);
	n[0] = (n[0] + n[1])/2.;
	n[1] = (n[1] + n[2])/2.;
	n[2] = (n[2] + n[3])/2.;
	n[3] = (n[3] + n[0])/2.;
	n[4] = (n[0]+n[1] + n[2]+n[3])/4.;
	
	for(int i=0;i<4;i++)
	{
		glBegin(GL_TRIANGLES);
		glNormal3f(n[i][0],n[i][1],n[i][2]);
		glVertex3f(p[i][0],p[i][1],p[i][2]);
		if(i!=3)
		{
			glNormal3f(n[i+1][0],n[i+1][1],n[i+1][2]);
			glVertex3f(p[i+1][0],p[i+1][1],p[i+1][2]);
		}
		else
		{
			glNormal3f(n[0][0],n[0][1],n[0][2]);
			glVertex3f(p[0][0],p[0][1],p[0][2]);
		}
		glNormal3f(n[4][0],n[4][1],n[4][2]);
		glVertex3f(arrowPos[0]+arrowDir[0]*2,arrowPos[1]+arrowDir[1]*2,arrowPos[2]+arrowDir[2]*2);
		glEnd();
	}
/*
	o = svGetPerpendicularVector(arrowDir);
	p[0] = 2*r*arrowDir+sizePos-o*0.5;

	for(int i=1;i<4;i++)
	{
		p[i] = svGetRotatePoint(p[0], sizePos-o*0.5, o, i*90); 
	}
	for(int i=0;i<3;i++)
	{
		n[i] = svGetNorm(p[i], sizePos,p[i+1]);
	}
	n[3] = svGetNorm(p[3], sizePos,p[0]);
	n[0] = (n[0] + n[1])/2.;
	n[1] = (n[1] + n[2])/2.;
	n[2] = (n[2] + n[3])/2.;
	n[3] = (n[3] + n[0])/2.;
	n[4] = (n[0]+n[1] + n[2]+n[3])/4.;
	
	for(int i=0;i<4;i++)
	{
		glBegin(GL_TRIANGLES);
		glNormal3f(n[i][0],n[i][1],n[i][2]);
		glVertex3f(p[i][0],p[i][1],p[i][2]);
		if(i!=3)
		{
			glNormal3f(n[i+1][0],n[i+1][1],n[i+1][2]);
			glVertex3f(p[i+1][0],p[i+1][1],p[i+1][2]);
		}
		else
		{
			glNormal3f(n[0][0],n[0][1],n[0][2]);
			glVertex3f(p[0][0],p[0][1],p[0][2]);
		}
		glNormal3f(n[4][0],n[4][1],n[4][2]);
		glVertex3f(sizePos[0],sizePos[1],sizePos[2]);
		glEnd();
	}

	glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
*/
	glColor3f(1,1,0);
	glLineWidth(2.);
	glBegin(GL_LINES);
	glVertex3f(arrowPos[0],arrowPos[1],arrowPos[2]);
	glVertex3f(arrowPos[0]+arrowDir[0]*2,arrowPos[1]+arrowDir[1]*2,arrowPos[2]+arrowDir[2]*2);
	glEnd();
/*	glBegin(GL_LINES);
	glVertex3f(arrowPos[0],arrowPos[1],arrowPos[2]);
	glVertex3f(sizePos[0],sizePos[1],sizePos[2]);
	glEnd();
*/

	int sample = dirIndex[0].size();

	glColor3f(1,1,1);
	glBegin(GL_LINE_STRIP);
	for(int i=0;i<sample;i++)
	{
		glVertex3f(dataPos[0][dirIndex[0][i]][0],
			dataPos[0][dirIndex[0][i]][1],
			dataPos[0][dirIndex[0][i]][2]);
	}
	glEnd();
	glLineWidth(1);
}

void svStickyPane::RenderInformation(int w, int h)
{
	//glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	
	float alpha = 0.5;
	float barscale;

	float arrowtheta, theta[8];
	float s;
	int index;

	float ratio1 = 30.;
	float ratio = ratio1/2;

	char str[2]={'9','0'};

	barscale = h/(3*ratio)-4;

	glLineWidth(1.);
	glColor4f(0,0,0,alpha);
	glBegin(GL_LINES);
	glVertex2f(wx[0]- w/ratio1-w/ratio1,h-wy[0]-h/ratio1+h/(3.*ratio));
	glVertex2f(wx[0]+ w/ratio1-w/ratio1, h-wy[0]-h/ratio1+h/(3.*ratio));
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(wx[0]- w/ratio1-w/ratio1,h-wy[0]-h/ratio1+2.*h/(3.*ratio));
	glVertex2f(wx[0]+ w/ratio1-w/ratio1, h-wy[0]-h/ratio1+2.*h/(3.*ratio));
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(wx[0]- w/ratio1 +w/(3.*ratio)-w/ratio1,h-wy[0]+h/ratio1);
	glVertex2f(wx[0]- w/ratio1 +w/(3.*ratio)-w/ratio1, h-wy[0]-h/ratio1);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(wx[0]- w/ratio1 +2.*w/(3.*ratio)-w/ratio1,h-wy[0]+h/ratio1);
	glVertex2f(wx[0]- w/ratio1 +2.*w/(3.*ratio)-w/ratio1, h-wy[0]-h/ratio1);
	glEnd();

	svVector3 angle1,angle2;
	svVector3 o;
	svVector3 z;
	svVector3 c1,c2;
	int section = 30;

	z[0]=0;
	z[1]=0;
	z[2]=1;
	
	svVector3 p[8];
	//for pie chart
	/*p[0][0]=w/(ratio*(-3.));p[0][1]=0;
	p[1][0]=w/(ratio*3.);   p[1][1]=0;

	p[2][0]=w/(ratio*(-3.));p[2][1]=h/(ratio*3.);
	p[3][0]=0;	 	p[3][1]=h/(ratio*3.);
	p[4][0]=w/(ratio*3.);   p[4][1]=h/(ratio*3.);

	p[5][0]=w/(ratio*(-3.));p[5][1]=h/(ratio*(-3.));
	p[6][0]=0;	 	p[6][1]=h/(ratio*(-3.));
	p[7][0]=w/(ratio*3.);   p[7][1]=h/(ratio*(-3.));*/
	//for bar chart
	p[0][0]=w/(ratio*(-2.));p[0][1]=h/(ratio*(-6.));
	p[1][0]=w/(ratio*6.);   p[1][1]=h/(ratio*(-6.));

	p[2][0]=w/(ratio*(-2.));p[2][1]=h/(ratio*6.);
	p[3][0]=w/(ratio*(-6.));p[3][1]=h/(ratio*6.);
	p[4][0]=w/(ratio*6.);   p[4][1]=h/(ratio*6.);

	p[5][0]=w/(ratio*(-2.));p[5][1]=h/(ratio*(-2.));
	p[6][0]=w/(ratio*(-6.));p[6][1]=h/(ratio*(-2.));
	p[7][0]=w/(ratio*6.);   p[7][1]=h/(ratio*(-2.));

        /*glRasterPos2f(p[2][0]+wx[0]-w/ratio1,p[2][1]+h/(ratio*6.)+h-wy[0]);//
        for(int j=0;j<2;j++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, str[j]);
        glRasterPos2f(p[3][0]+wx[0]-w/ratio1,p[2][1]+h/(ratio*6.)+h-wy[0]);//
        for(int j=0;j<2;j++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, str[j]);
        glRasterPos2f(p[4][0]+wx[0]-w/ratio1,p[2][1]+h/(ratio*6.)+h-wy[0]);//
        for(int j=0;j<2;j++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, str[j]);

        glRasterPos2f(p[0][0]+wx[0]-w/ratio1,p[0][1]+h/(ratio*6.)+h-wy[0]);//
        for(int j=0;j<2;j++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, str[j]);
        glRasterPos2f(p[1][0]+wx[0]-w/ratio1,p[0][1]+h/(ratio*6.)+h-wy[0]);//
        for(int j=0;j<2;j++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, str[j]);

        glRasterPos2f(p[5][0]+wx[0]-w/ratio1,p[5][1]+h/(ratio*6.)+h-wy[0]);//
        for(int j=0;j<2;j++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, str[j]);
        glRasterPos2f(p[6][0]+wx[0]-w/ratio1,p[5][1]+h/(ratio*6.)+h-wy[0]);//
        for(int j=0;j<2;j++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, str[j]);
        glRasterPos2f(p[7][0]+wx[0]-w/ratio1,p[5][1]+h/(ratio*6.)+h-wy[0]);//
        for(int j=0;j<2;j++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, str[j]);	
*/
	glPointSize(2.);
	glBegin(GL_POINTS);
        glVertex2f(p[2][0]+wx[0]-w/ratio1,p[2][1]+h/(ratio*6.)+h-wy[0]);//
        glVertex2f(p[3][0]+wx[0]-w/ratio1,p[2][1]+h/(ratio*6.)+h-wy[0]);//
        glVertex2f(p[4][0]+wx[0]-w/ratio1,p[2][1]+h/(ratio*6.)+h-wy[0]);//

        glVertex2f(p[0][0]+wx[0]-w/ratio1,p[0][1]+h/(ratio*6.)+h-wy[0]);//
        glVertex2f(p[1][0]+wx[0]-w/ratio1,p[0][1]+h/(ratio*6.)+h-wy[0]);//

        glVertex2f(p[5][0]+wx[0]-w/ratio1,p[5][1]+h/(ratio*6.)+h-wy[0]);//
        glVertex2f(p[6][0]+wx[0]-w/ratio1,p[5][1]+h/(ratio*6.)+h-wy[0]);//
        glVertex2f(p[7][0]+wx[0]-w/ratio1,p[5][1]+h/(ratio*6.)+h-wy[0]);//
	glEnd();
	glPointSize(1.);

	for(int j=0;j<3;j++)
	{
		s = sqrt(arrowDir[0]*arrowDir[0]+arrowDir[1]*arrowDir[2]+arrowDir[2]*arrowDir[2]);
		if(j==0)
			arrowtheta = acos(arrowDir[0]/s);
		else if(j==1)
			arrowtheta = acos(arrowDir[1]/s);
		else if(j==2)
			arrowtheta = acos(arrowDir[2]/s);
		for(int i=0;i<8;i++)
		{
			if(selectIndex[i]>=0)
			{
			s = sqrt(dataDir[0][selectIndex[i]][0]*dataDir[0][selectIndex[i]][0]
				+dataDir[0][selectIndex[i]][1]*dataDir[0][selectIndex[i]][1]
				+dataDir[0][selectIndex[i]][2]*dataDir[0][selectIndex[i]][2]);
			if(j==0)
				theta[i] = acos(dataDir[0][selectIndex[i]][0]/s);
			else if(j==1)
				theta[i] = acos(dataDir[0][selectIndex[i]][1]/s);
			else if(j==2)
				theta[i] = acos(dataDir[0][selectIndex[i]][2]/s);
			theta[i] = theta[i] - arrowtheta;
			if(theta[i]<0)
				theta[i] = (-1)*theta[i];//2*PI + theta[i];	
			//theta[i] = theta[i]*180/PI;
			}
		}
		glLineWidth(5.);
		//bar chart
		for(int i=0;i<8;i++)
		{
			if(selectIndex[i]>=0)
			{			
				o[0] = p[i][0]+wx[0]-w/ratio1;
				o[1] = p[i][1]+h-wy[0];
				o[2] = 0;
	
				if(j==0)
				{
					glColor3f(227./256.,26./256.,28./256.); 
					glBegin(GL_LINES);
					glVertex3f(o[0]+w/(ratio*18.),o[1],o[2]);
					glVertex3f(o[0]+w/(ratio*18.),o[1]+barscale*theta[i]/PI,o[2]);
					glEnd();
				}
				else if(j==1)
				{
					glColor3f(35./256.,139./256.,69./256.); 
					glBegin(GL_LINES);
					glVertex3f(o[0]+w/(ratio*9.)+w/(ratio*18.),o[1],o[2]);
					glVertex3f(o[0]+w/(ratio*9.)+w/(ratio*18.),o[1]+barscale*theta[i]/PI,o[2]);
					glEnd();
				}	
				else if(j==2)
				{
					glColor3f(33./256.,113./256.,181./256.); 
					glBegin(GL_LINES);
					glVertex3f(o[0]+w/(ratio*9.)+w/(ratio*9.)+w/(ratio*18.),o[1],o[2]);
					glVertex3f(o[0]+w/(ratio*9.)+w/(ratio*9.)+w/(ratio*18.),
							o[1]+barscale*theta[i]/PI,o[2]);
					glEnd();
				}
			}
		}
		glLineWidth(1.);
		//pie chart
		/*for(int i=0;i<8;i++)
		{
			if(selectIndex[i]>=0)
			{			

			o[0] = p[i][0]+wx[0];
			o[1] = p[i][1]+h-wy[0];
			o[2] = 0;
			if(w<h)
				angle1[0] = o[0] + w/(ratio*6.);
			else
				angle1[0] = o[0] + h/(ratio*6.);
			angle1[1] = o[1];
			angle1[2] = 0;
			angle2 = svGetRotatePoint(angle1, o , z, theta[i]); 

			glColor3f(0,0,0);
			c1 = angle1;
			for(int jj=0;jj<section;jj++)
			{
				c2 = svGetRotatePoint(angle1, o , z, jj*360/(section-1));
				glBegin(GL_TRIANGLES);
				glVertex3f(c1[0],c1[1],c1[2]);
				glVertex3f(c2[0],c2[1],c2[2]);
				glVertex3f(o[0],o[1],o[2]);
				glEnd(); 
				c1 = c2;
			}

			if(j==0)glColor3f(1.,0.,0.);
			else if(j==1) glColor3f(0.,1.,0.);
			else if(j==2) glColor3f(1.,1.,1.);

			glBegin(GL_LINES);
			glVertex3f(angle2[0],angle2[1],angle2[2]);
			glVertex3f(o[0],o[1],o[2]);
			glEnd();

			}
		}*/
		//--------------------
	}

	glColor4f(1,1,1,alpha);
	glBegin(GL_QUADS);
	glVertex2f(wx[0]- w/ratio1-w/ratio1,h-wy[0]-h/ratio1);
	glVertex2f(wx[0]+ w/ratio1-w/ratio1, h-wy[0]-h/ratio1);
	glVertex2f(wx[0]+ w/ratio1-w/ratio1, h-wy[0]+h/ratio1);
	glVertex2f(wx[0]- w/ratio1-w/ratio1, h-wy[0]+h/ratio1);
	glEnd();
	glDisable(GL_BLEND);
	//glEnable(GL_DEPTH_TEST);
}


void svStickyPane::cleanup()
{
	if(dataPos!=NULL)
	{
		dataPos[0].free();
		delete [] dataPos;
		dataPos = NULL;
	}
	if(dataDir!=NULL)
	{
		dataDir[0].free();
		delete [] dataDir;
		dataDir = NULL;
	}
	if(dirIndex!=NULL)
	{
		dirIndex[0].free();
		delete [] dirIndex;
		dirIndex = NULL;
	}

}
GLboolean svStickyPane::invert_matrix( const GLfloat *m, GLfloat *out )
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
