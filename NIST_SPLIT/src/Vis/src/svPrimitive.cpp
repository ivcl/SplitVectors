/**
 *  This file implements svPrimitive
 */

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "svPrimitive.h"
#include "svException.h"

#include <string.h>

namespace __svl_lib {
//Constructor
svPrimitive::svPrimitive(svInt  _seed_num)
{
  field = NULL;
  seed = NULL;
  seed_num = _seed_num;
  //display_mode = SV_DISPLAYLIST;
  display_mode = SV_STORE;

  svColor4 color(0.1, 0.1, 0.9, 0.5);
  render_property.color = color;
  render_property.line_width = 1.0;

  STILL_UPDATE = false;
  ENABLE_SEGMENT_FREQ = false;
  ENABLE_LINEWIDTH = false;

  //_colors = NULL;
  ENABLE_COLOR = false;

  lbbox=svVector3(-1,-1,-1); 
  rbbox=svVector3(1,1,1);
}

svPrimitive::svPrimitive(svVectorField* f, svParticle* sd, svInt sd_num, svUint mode)
{
  field    = f;
  seed     = sd;
  seed_num = sd_num;

  if (field!=NULL) {
    if(field->GetTimeStepNum() == 1)  //static field
      field->SetFreezeTime(0.0);
    else if(field->GetTimeStepNum() > 1) //unstatic field
      field->SetFreezeTime(seed[0].GetTime());
  }

  max_advection_time = DEFAULT_MAX_ADVECTION_TIME;
  segment_freq = DEFAULT_SEGMENT_FREQ;
  direction = SV_FORWARD;

  display_mode = mode;
  display_list = 0;	
  
  svColor4 color(0.1, 0.1, 0.9, 0.5);
  render_property.color = color;
  render_property.line_width = 1.0;

  STILL_UPDATE = false;
  ENABLE_SEGMENT_FREQ = false;
  ENABLE_LINEWIDTH = false;

  //_colors = NULL;
  ENABLE_COLOR = false;

  lbbox=svVector3(-1,-1,-1); 
  rbbox=svVector3(1,1,1);
}

svPrimitive::~svPrimitive()
{
  if (glIsList(display_list)){
    glDeleteLists(display_list,1);
	display_list = 0;
  }
  if(seed!=NULL) 
    delete [] seed;
}

void svPrimitive::DrawAxis()
{
  glLineWidth(3);
  glColor3f(1., 0, 0);
  glBegin(GL_LINES);
    glVertex3f(lbbox[0],lbbox[1],lbbox[2]);
    glVertex3f(rbbox[0],lbbox[1],lbbox[2]);
  glEnd();

  glColor3f(0., 1, 0);
  glBegin(GL_LINES);
    glVertex3f(lbbox[0],lbbox[1],lbbox[2]);
    glVertex3f(lbbox[0],rbbox[1],lbbox[2]);
  glEnd();

  glColor3f(0., 0, 1);
  glBegin(GL_LINES);
    glVertex3f(lbbox[0],lbbox[1],lbbox[2]);
    glVertex3f(lbbox[0],lbbox[1],rbbox[2]);
  glEnd();
  glLineWidth(1);
}

void svPrimitive::DrawAxis_scale() //02/01/14 by Henan
{
  int i,j;
  double x,y,z;
  char value;
  int times;
  int scale;
  char str[4],temp[3];
  int len;

  glColor3f(1,0,0);
  glPointSize(5);
  glBegin(GL_POINTS);
  glVertex3f(rbbox[0],rbbox[1],rbbox[2]);
  glEnd();
  glColor3f(1,1,1);

  times = rbbox[0] - lbbox[0];
  x=rbbox[0];y=rbbox[1];z=rbbox[2];
  glColor3f(1,1,1);
  glPointSize(3);
  glBegin(GL_POINTS);
  for(i=0;i<times;i++)
  {
	if(i%5==0)
	{	glColor3f(8./255.,48./255.,107/255.);
		glPointSize(4.);
	}
	else
	{
		glColor3f(1.,1.,1.);
		glPointSize(3.);
	}
	glVertex3f(x,y,z);
	x = x - 1;	
  }
  glEnd();

  times = rbbox[1] - lbbox[1];
  x=rbbox[0];y=rbbox[1];z=rbbox[2];
  glColor3f(1,1,1);
  glPointSize(3);
  glBegin(GL_POINTS);
  for(i=0;i<times;i++)
  {
	if(i%5==0)
	{	glColor3f(8./255.,48./255.,107/255.);
		glPointSize(4.);
	}
	else
	{
		glColor3f(1.,1.,1.);
		glPointSize(3.);
	}
	glVertex3f(x,y,z);
	y = y - 1;
  }
  glEnd();
 
  times = rbbox[2] - lbbox[2];
  x=rbbox[0];y=rbbox[1];z=rbbox[2];
  glColor3f(1,1,1);
  glPointSize(3);
  glBegin(GL_POINTS);
  for(i=0;i<times;i++)
  {
	if(i%5==0)
	{	glColor3f(8./255.,48./255.,107/255.);
		glPointSize(4.);
	}
	else
	{
		glColor3f(1.,1.,1.);
		glPointSize(3.);
	}
	glVertex3f(x,y,z);
	z = z - 1;
  }
  glEnd();

  scale = rbbox[0];
  x=rbbox[0];y=rbbox[1];z=rbbox[2]+0.5;
  glColor3f(1,1,1);
  times = fabs(rbbox[0] - lbbox[0])/5;
  for(i=0;i<times;i++)
  {
	glRasterPos3f(x,y,z);
	sprintf(str,"%d",scale);
	len = strlen(str);
	for(j=0;j<len;j++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
	x=x-5;
	scale=scale-5;
  }

  scale = rbbox[1];
  x=rbbox[0];y=rbbox[1];z=rbbox[2]+0.5;
  glColor3f(1,1,1);
  times = fabs(rbbox[1] - lbbox[1])/5;
  for(i=0;i<times;i++)
  {
	glRasterPos3f(x,y,z);
	sprintf(str,"%d",scale);
	len = strlen(str);
	for(j=0;j<len;j++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
	y=y-5;
	scale=scale-5;
  }

  scale = rbbox[2];
  x=rbbox[0]+0.5;y=rbbox[1]+0.5;z=rbbox[2];
  glColor3f(1,1,1);
  times = fabs(rbbox[2] - lbbox[2])/5;
  for(i=0;i<times;i++)
  {
	glRasterPos3f(x,y,z);
	sprintf(str,"%d",scale);
	len = strlen(str);
	for(j=0;j<len;j++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
	z=z-5;
	scale=scale-5;
  }

/*
  glColor3f(1., 1, 1);
  glRasterPos3f(lbbox[0]-1,rbbox[1],rbbox[2]+1);
  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'x');
  glColor3f(1., 1, 1);
  glRasterPos3f(rbbox[0],lbbox[1]-1,rbbox[2]+1);
  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'y');
  glColor3f(1., 1, 1);
  glRasterPos3f(rbbox[0],rbbox[1]+1,lbbox[2]-1);
  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'z');*/
}
void svPrimitive::DrawAxis_scale(int width)
{ 
	 GLint viewport[4];
  GLdouble mvmatrix[16];
  GLdouble projmatrix[16];

    int i, j;

    svVector3 p[4];
  GLdouble wx[4];
  GLdouble wy[4];
  GLdouble wz[4];

    glGetIntegerv (GL_VIEWPORT, viewport);
    glGetDoublev (GL_MODELVIEW_MATRIX, mvmatrix);
    glGetDoublev (GL_PROJECTION_MATRIX, projmatrix);

	for(i=0;i<4;i++)
	{
		p[i][0]=0;p[i][1]=0;p[i][2]=0;	
	}

	svVector3 widgetp[4];
	widgetp[0] = rbbox;
	widgetp[1] = rbbox;widgetp[1][0]=lbbox[0];
	widgetp[2] = rbbox;widgetp[2][1]=lbbox[1];
	widgetp[3] = rbbox;widgetp[3][2]=lbbox[2];

	for(i=0;i<4;i++)
	{
		/*p[i][0]  = widgetp[i][0] * tb_transform[0][0] +
			    widgetp[i][1] * tb_transform[1][0] +
			    widgetp[i][2] * tb_transform[2][0] +
			    tb_transform[3][0] + p[i][0];
                p[i][1]  = widgetp[i][0] * tb_transform[0][1] +
                            widgetp[i][1] * tb_transform[1][1] +
                            widgetp[i][2] * tb_transform[2][1] +
                            tb_transform[3][1] + p[i][1];
                p[i][2]  = widgetp[i][0] * tb_transform[0][2] +
                            widgetp[i][1] * tb_transform[1][2] +
                            widgetp[i][2] * tb_transform[2][2] +
                            tb_transform[3][2] + p[i][2];*/
		p[i] = widgetp[i];
	}
	
	for(i=0;i<4;i++)
	{
		gluProject((GLdouble)p[i][0],
                        (GLdouble)p[i][1],
                        (GLdouble)p[i][2],
                        mvmatrix, projmatrix, viewport,
                        &(wx[i]), &(wy[i]), &(wz[i]));
		wy[i] = (GLdouble)width-wy[i];
	}

        float dd[3];

	for(i=1;i<4;i++)
	{
		dd[i-1] = sqrt((wy[i]-wy[0])*(wy[i]-wy[0])+
				(wx[i]-wx[0])*(wx[i]-wx[0]));
	}

  double x,y,z;
  char value;
  int times[2];
  int scale;
  char str[4],temp[3];
  int len;
  float l;
  int l2[2];

  times[0] = rbbox[0] - lbbox[0];
  x=rbbox[0];y=rbbox[1];z=rbbox[2];
  glColor3f(1,1,1);
  glPointSize(2.);
  glBegin(GL_POINTS);
  for(i=0;i<times[0];i++)
  {

	glVertex3f(x,y,z);
	x = x - 1;	
  }
  glEnd();

  times[0] = rbbox[1] - lbbox[1];
  x=rbbox[0];y=rbbox[1];z=rbbox[2];
  glColor3f(1,1,1);
  glPointSize(2.);
  glBegin(GL_POINTS);
  for(i=0;i<times[0];i++)
  {

	glVertex3f(x,y,z);
	y = y - 1;
  }
  glEnd();
 
  times[0] = rbbox[2] - lbbox[2];
  x=rbbox[0];y=rbbox[1];z=rbbox[2];
  glColor3f(1,1,1);
  glPointSize(2.);
  glBegin(GL_POINTS);
  for(i=0;i<times[0];i++)
  {

	glVertex3f(x,y,z);
	z = z - 1;
  }
  glEnd();

  glPointSize(5);
  l = dd[0]/50.;
  //l2 = (rbbox[0]-lbbox[0])/l;
  l2[0] = (rbbox[0] - lbbox[0])/l;
  l2[1] = (rbbox[0] - lbbox[0])/l;
  if(l2[0]==0) l2[0] = 1;
  if(l2[1]==0) l2[1] = 1;
  //scale = rbbox[0];
  //x=rbbox[0];y=rbbox[1];z=rbbox[2]+0.5;
  glColor3f(1,1,1);
  //times = fabs(rbbox[0] - lbbox[0])/l2;
  times[0] = fabs(0 - lbbox[0])/l2[0];
  times[1] = fabs(rbbox[0] - 0)/l2[1];
  /*for(i=0;i<times;i++)
  {
	glColor3f(1.,1.,1.);
	glRasterPos3f(x,y,z);
	sprintf(str,"%.0f",(float)scale);
	len = strlen(str);
	for(j=0;j<len;j++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
	x=x-l2;
	scale=scale-l2;
	glColor3f(217./255.,240./255.,163./255.);
	glBegin(GL_POINTS);
	glVertex3f(x,y,z-0.5);
	glEnd();
  }*/
  x=0;y=rbbox[1];z=rbbox[2]+0.5;
  scale = 0;
  for(i=0;i<=times[0];i++)
  {
	if(x>=lbbox[0])
	{
	glColor3f(1.,1.,1.);
	glRasterPos3f(x,y,z);
	sprintf(str,"%.0f",(float)scale);
	len = strlen(str);
	for(j=0;j<len;j++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

	glColor3f(217./255.,240./255.,163./255.);
	glBegin(GL_POINTS);
	glVertex3f(x,y,z-0.5);
	glEnd();
	x=x-l2[0];
	scale=scale-l2[0];
	}
  }
  x=0;y=rbbox[1];z=rbbox[2]+0.5;
  scale = 0;
  for(i=0;i<=times[1];i++)
  {
	if(x<=rbbox[0])
	{
	glColor3f(1.,1.,1.);
	glRasterPos3f(x,y,z);
	sprintf(str,"%.0f",(float)scale);
	len = strlen(str);
	for(j=0;j<len;j++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

	glColor3f(217./255.,240./255.,163./255.);
	glBegin(GL_POINTS);
	glVertex3f(x,y,z-0.5);
	glEnd();
	x=x+l2[1];
	scale=scale+l2[1];
	}
  }

  l = dd[1]/50.;
  //l2 = (rbbox[1]-lbbox[1])/l;
  l2[0] = (rbbox[1] - lbbox[1])/l;
  l2[1] = (rbbox[1] - lbbox[1])/l;
  //if(l2==0) l2 = 1;
  if(l2[0]==0)l2[0]=1;
  if(l2[1]==0)l2[1]=1;
  //scale = rbbox[1];
 // x=rbbox[0];y=rbbox[1];z=rbbox[2]+0.5;
  glColor3f(1,1,1);
  //times = fabs(rbbox[1] - lbbox[1])/l2;
  times[0] = fabs(0 - lbbox[1])/l2[0];
  times[1] = fabs(rbbox[1] - 0)/l2[1];
  /*for(i=0;i<times;i++)
  {
	glColor3f(1.,1.,1.);
	glRasterPos3f(x,y,z);
	sprintf(str,"%.0f",(float)scale);
	len = strlen(str);
	for(j=0;j<len;j++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
	y=y-l2;
	scale=scale-l2;
	glColor3f(217./255.,240./255.,163./255.);
	glBegin(GL_POINTS);
	glVertex3f(x,y,z-0.5);
	glEnd();
  }*/
  x=rbbox[0];y=0;z=rbbox[2]+0.5;
  scale = 0;
  for(i=0;i<=times[0];i++)
  {
	if(y>=lbbox[1])
	{
	glColor3f(1.,1.,1.);
	glRasterPos3f(x,y,z);
	sprintf(str,"%.0f",(float)scale);
	len = strlen(str);
	for(j=0;j<len;j++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

	glColor3f(217./255.,240./255.,163./255.);
	glBegin(GL_POINTS);
	glVertex3f(x,y,z-0.5);
	glEnd();
	y=y-l2[0];
	scale=scale-l2[0];
	}
  }
  x=rbbox[0];y=0;z=rbbox[2]+0.5;
  scale = 0;
  for(i=0;i<=times[1];i++)
  {
	if(y<=rbbox[1])
	{
	glColor3f(1.,1.,1.);
	glRasterPos3f(x,y,z);
	sprintf(str,"%.0f",(float)scale);
	len = strlen(str);
	for(j=0;j<len;j++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

	glColor3f(217./255.,240./255.,163./255.);
	glBegin(GL_POINTS);
	glVertex3f(x,y,z-0.5);
	glEnd();
	y=y+l2[1];
	scale=scale+l2[1];
	}
  }

  l = dd[2]/50.;
  //l2 = (rbbox[2]-lbbox[2])/l;
  l2[0] = (rbbox[2] - lbbox[2])/l;
  l2[1] = (rbbox[2] - lbbox[2])/l;
 // if(l2==0) l2 = 1;
  if(l2[0]==0)l2[0]=1;
  if(l2[1]==0)l2[1]=1;
  //scale = rbbox[2];
  //x=rbbox[0]+0.5;y=rbbox[1]+0.5;z=rbbox[2];
  glColor3f(1,1,1);
 // times = fabs(rbbox[2] - lbbox[2])/l2;
  times[0] = fabs(0 - lbbox[2])/l2[0];
  times[1] = fabs(rbbox[2] - 0)/l2[1];
 /* for(i=0;i<times;i++)
  {
	glColor3f(1.,1.,1.);
	glRasterPos3f(x,y,z);
	sprintf(str,"%.0f",(float)scale);
	len = strlen(str);
	for(j=0;j<len;j++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
	z=z-l2;
	scale=scale-l2;
	glColor3f(217./255.,240./255.,163./255.);
	glBegin(GL_POINTS);
	glVertex3f(x-0.5,y-0.5,z);
	glEnd();
  }*/
  x=rbbox[0]+0.5;y=rbbox[1]+0.5;z=0;
  scale = 0;
  for(i=0;i<=times[0];i++)
  {
	if(z>=lbbox[2])
	{

	glColor3f(1.,1.,1.);
	glRasterPos3f(x,y,z);
	sprintf(str,"%.0f",(float)scale);
	len = strlen(str);
	for(j=0;j<len;j++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

	glColor3f(217./255.,240./255.,163./255.);
	glBegin(GL_POINTS);
	glVertex3f(x-0.5,y-0.5,z);
	glEnd();

	z=z-l2[0];
	scale=scale-l2[0];
	}
  }
  x=rbbox[0]+0.5;y=rbbox[1]+0.5;z=0;
  scale = 0;
  for(i=0;i<=times[1];i++)
  {
	if(z<=rbbox[2])
	{
	glColor3f(1.,1.,1.);
	glRasterPos3f(x,y,z);
	sprintf(str,"%.0f",(float)scale);
	len = strlen(str);
	for(j=0;j<len;j++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

	glColor3f(217./255.,240./255.,163./255.);
	glBegin(GL_POINTS);
	glVertex3f(x-0.5,y-0.5,z);
	glEnd();

	z=z+l2[1];
	scale=scale+l2[1];
	}
  }



 /* glColor3f(1., 1, 1);
  glRasterPos3f(lbbox[0]-1,rbbox[1],rbbox[2]+1);
  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'x');
  glColor3f(1., 1, 1);
  glRasterPos3f(rbbox[0],lbbox[1]-1,rbbox[2]+1);
  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'y');
  glColor3f(1., 1, 1);
  glRasterPos3f(rbbox[0],rbbox[1]+1,lbbox[2]-1);
  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'z');*/

  glPointSize(1);

}
GLboolean svPrimitive::invert_matrix( const GLfloat *m, GLfloat *out )
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
void svPrimitive::DrawOutline( svBool set_color_flag )
{
  if (set_color_flag)
    glColor3f(1., 1., 1.);
  glLineWidth(1.0);
  if(field!=NULL)
       field->GetBoundingBox(&lbbox,&rbbox);

  //cerr << "svPrimitive::DrawOutline: lbbox" 
       //<< lbbox[0]<<", "<<lbbox[1]<<", " << lbbox[2]<<endl;
  //cerr << "svPrimitive::DrawOutline: rbbox" 
       //<< rbbox[0]<<", "<<rbbox[1]<<", " << rbbox[2]<<endl;

  glBegin(GL_LINE_LOOP);
  glVertex3f(lbbox[0],lbbox[1],lbbox[2]);
  glVertex3f(rbbox[0],lbbox[1],lbbox[2]);
  glVertex3f(rbbox[0],rbbox[1],lbbox[2]);
  glVertex3f(lbbox[0],rbbox[1],lbbox[2]);
  glEnd();

  glBegin(GL_LINE_LOOP);
  glVertex3f(lbbox[0],lbbox[1],rbbox[2]);
  glVertex3f(rbbox[0],lbbox[1],rbbox[2]);
  glVertex3f(rbbox[0],rbbox[1],rbbox[2]);
  glVertex3f(lbbox[0],rbbox[1],rbbox[2]);
  glEnd();

  glBegin(GL_LINES);
  glVertex3f(lbbox[0],lbbox[1],lbbox[2]);
  glVertex3f(lbbox[0],lbbox[1],rbbox[2]);
  glVertex3f(rbbox[0],lbbox[1],lbbox[2]);
  glVertex3f(rbbox[0],lbbox[1],rbbox[2]);
  glVertex3f(rbbox[0],rbbox[1],lbbox[2]);
  glVertex3f(rbbox[0],rbbox[1],rbbox[2]);
  glVertex3f(lbbox[0],rbbox[1],lbbox[2]);
  glVertex3f(lbbox[0],rbbox[1],rbbox[2]);
  glEnd();
}

void svPrimitive::cleanDisplayList(svUint newMode)
{
  if(glIsList(display_list))
  {
      glDeleteLists(display_list, 1);
      display_mode = newMode;
      display_list = 0;
  };
}

svInt svPrimitive::gotoNextPosition(svParticle &p, svInt &dir,
              svFloat max_time, svBool adv) 
{
    if(field->pointOutOfBound(p.position) == true)
      return SV_EXP_OUT_OF_RANGE_SPACE;
    if(adv) p.Advect(dir, max_time);
    else  p.Step(dir, max_time);
    return SV_STEP_OK;
}

svInt svPrimitive::gotoNextPosition(svParticle &p, svInt &dir) 
{
    if(field->pointOutOfBound(p.position) == true)
      return SV_EXP_OUT_OF_RANGE_SPACE;
    p.Step(dir);
    return SV_STEP_OK;
}
}
