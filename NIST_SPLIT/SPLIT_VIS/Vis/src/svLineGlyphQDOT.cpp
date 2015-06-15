/*  
 * svLineGlyphQDOT
 */

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <fstream>
#include <string.h>

#include "svLineGlyphQDOT.h"
#include "svException.h"
#include "svUtil.h"
#include "readinputsvl.h"


using namespace std;

#define DEFAULT_SPHERE_FREQ  10
#define DEFAULT_SPHERE_SIZE  0.01
#define DEFAULT_SPHERE_SEGMENT 10 
#define BUFSIZE 512

namespace __svl_lib {

void svLineGlyphQDOT::init()
{
}

svLineGlyphQDOT::svLineGlyphQDOT(svVector3 _lbbox, svVector3 _rbbox, 
     svInt dataSize,
     svVector3 *posIn, svVector3 *velIn,  svScalar *denIn,
	 svVector4 *color4In)
   : svGlyph(_lbbox, _rbbox)
{
  init();

  // data attributes
  glyphPos = new svVector3Array[2]; // starting and ending positions
  glyphDir = new svVector3Array[1];
  glyphMag = new svScalar[dataSize];
  glyphScale = new svScalar[dataSize];

  // drawing attributes
  glyphColors = new svVector4Array[1];

  cerr << "... Setting lineGlyphQDOT ..." << endl;
  
 /* for(int j=0; j<dataSize; j++)
  {
    cerr << "J: " << j << ": ";
    cerr << posIn[j][0]<<", " << posIn[j][1]<<", "<<posIn[j][2]<<endl;
    cerr << velIn[j][0]<<", " << velIn[j][1]<<", "<<velIn[j][2]<<endl;
	cerr << denIn[j]<< endl;
  }*/
  

  for(int j=0; j<dataSize; j++)
  {
    // set position
    glyphPos[0].add(posIn[j]);

    // set vector dir
	svVector3 nvel = normalize(velIn[j]);
	glyphDir[0].add(nvel);

	// set glyph scale
	glyphScale[j]= 0.5; // default scale

    // set glyph ending point position
	svVector3 endPos = posIn[j]+glyphScale[j]*nvel;
	glyphPos[1].add(endPos);

    if(color4In!=NULL)  
	{
	  ENABLE_COLOR=true;
	  glyphColors[0].add(svVector4(color4In[j][0]/255., color4In[j][1]/255., color4In[j][2]/255., color4In[j][3]/255.));
	}
	else  
	{
	  glyphColors[0].add(svVector4(1,1,1,0)); // default white
	}
  }; // end for(j)

  cerr << "... Done init lineGlyphQDOT ... " << endl;

  green = 0;	ix = lbbox[0];iy=lbbox[1];iz=lbbox[2];offx=0;
  transflag = 0;
  ContourPointDen = 0;
}

svLineGlyphQDOT::svLineGlyphQDOT(svChar *dir, svChar *infname)
{
  char infileName[2056];
  sprintf(infileName, "%s/%s",dir, infname);

  svVector3 lb, rb; // bounding box

  // declaring a 2d array to save the ending points of
  // the line glyph
  glyphPos = new svVector3Array[2];
  glyphColors = new svVector4Array[1]; // regardless, assign glyph colors

  readQDOTLineGlyph(infileName,  glyphPos, glyphColors, &lb, &rb);
  SetBBox(lb, rb);

  int dataSize = glyphPos[0].size();

  glyphFrequency=1;

  ENABLE_COLOR=true;
}

svLineGlyphQDOT::~svLineGlyphQDOT()
{

}

void svLineGlyphQDOT::RenderStore()
{
}


// render data within (zmin, zmax) 
void svLineGlyphQDOT::RenderStoreDirect(svFloat zmin, svFloat zmax, 
                                        svBool enableDot)
{
  int samplingSites = glyphPos[0].size();
  svVector3 v0, v1;

  glBegin(GL_LINES);
  for(int i=0; i<samplingSites; i=i+glyphFrequency)
  {
    if(ENABLE_COLOR) 
	  glColor4fv(glyphColors[0][i].getValue());
	
	if(glyphPos[0][i][2]>=zmin && glyphPos[0][i][2]<=zmax)
	{
      glVertex3f(glyphPos[0][i][0], glyphPos[0][i][1], glyphPos[0][i][2]);
      glVertex3f(glyphPos[1][i][0], glyphPos[1][i][1], glyphPos[1][i][2]);
	}; // end if(zmin, zmax)
  }; // end for i
  glEnd();

  if(enableDot)
  {
    glPointSize(1.5);
    glBegin(GL_POINTS);
    //glColor3f(1,0,0);
    for(int i=0; i<samplingSites; i=i+glyphFrequency)
    {
      if(ENABLE_COLOR) 
	    glColor4fv(glyphColors[0][i].getValue());

	  if(glyphPos[0][i][2]>zmin && glyphPos[0][i][2]<zmax)
        glVertex3f(glyphPos[1][i][0], glyphPos[1][i][1], glyphPos[1][i][2]);
    }; // end for i
    glEnd();
    glPointSize(1.0);
  }; // end if(enableDot)
}
void svLineGlyphQDOT::RenderStoreDirect(svFloat zmin, svFloat zmax)
{
  int samplingSites = glyphPos[0].size();

    glPointSize(1.5);
    glBegin(GL_POINTS);
    glColor3f(1,1,1);
    for(int i=0; i<samplingSites; i=i+glyphFrequency)
    {
	  if(glyphPos[0][i][2]>zmin && glyphPos[0][i][2]<zmax)
        	glVertex3f(glyphPos[0][i][0], glyphPos[0][i][1], glyphPos[0][i][2]);
    }; // end for i
    glEnd();
    glPointSize(1.0);

}
// (JC: 1/8/14)this one works
void svLineGlyphQDOT::RenderStoreDirect(svBool enableDot)
{
  int samplingSites = glyphPos[0].size();
  svVector3 v0, v1;

  glBegin(GL_LINES);
  for(int i=0; i<samplingSites; i=i+glyphFrequency)
  {
    if(ENABLE_COLOR) 
	  glColor4fv(glyphColors[0][i].getValue());
    glVertex3f(glyphPos[0][i][0], glyphPos[0][i][1], glyphPos[0][i][2]);
    glVertex3f(glyphPos[1][i][0], glyphPos[1][i][1], glyphPos[1][i][2]);
  }; // end for i
  glEnd();

  if(enableDot)
  {
    glPointSize(1.5);
    glBegin(GL_POINTS);
    //glColor3f(1,0,0);
    for(int i=0; i<samplingSites; i=i+glyphFrequency)
    {
      if(ENABLE_COLOR) 
  	    glColor4fv(glyphColors[0][i].getValue());
      glVertex3f(glyphPos[1][i][0], glyphPos[1][i][1], glyphPos[1][i][2]);
    }; // end for i
    glEnd();
    glPointSize(1.0);
  }; // end if(enableDot)
}


void svLineGlyphQDOT::Draw3DAxis()
{
        GLfloat mat_diffuse[] = { 0.2, 0.2, 0.2, 1};
        GLfloat mat_specular[] = { 0.3, 0.3, 0.3, 1 };
        GLfloat mat_shininess[] = { 0.02 };
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);


        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	GLUquadricObj *quadratic;
	quadratic = gluNewQuadric();
	gluQuadricDrawStyle(quadratic, GLU_FILL);
	gluQuadricNormals(quadratic, GLU_SMOOTH);

	glColor3f(1,0,0);
	glPushMatrix();
	glTranslatef(lbbox[0],lbbox[1],lbbox[2]);
	glRotatef(90,0,1,0);
	gluCylinder(quadratic,0.3f,0.3f,rbbox[0]-lbbox[0],32,32);
	glPopMatrix();

	char str[50];

	glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);

	glColor4f(1,1,1,1);
	glRasterPos3f(AxisWidgetPos[0][0],AxisWidgetPos[0][1],AxisWidgetPos[0][2]);
	sprintf(str, "%f", AxisWidgetPos[0][0]);
	//cerr<<ContourPointDen<<endl;
	for(int j=0;j<strlen(str);j++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[j]);
	glRasterPos3f(AxisWidgetPos[3][0],AxisWidgetPos[3][1],AxisWidgetPos[3][2]);
	sprintf(str, "%f", AxisWidgetPos[3][0]);
	//cerr<<ContourPointDen<<endl;
	for(int j=0;j<strlen(str);j++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[j]);

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

	glColor3f(0,1,0);
	glPushMatrix();
	glTranslatef(lbbox[0],lbbox[1],lbbox[2]);
	glRotatef(-90,1,0,0);
	gluCylinder(quadratic,0.3f,0.3f,rbbox[1]-lbbox[1],32,32);
	glPopMatrix();

        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);

	glColor4f(1,1,1,1);
	glRasterPos3f(AxisWidgetPos[1][0],AxisWidgetPos[1][1],AxisWidgetPos[1][2]);
	sprintf(str, "%f", AxisWidgetPos[1][1]);
	//cerr<<ContourPointDen<<endl;
	for(int j=0;j<strlen(str);j++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[j]);
	glRasterPos3f(AxisWidgetPos[4][0],AxisWidgetPos[4][1],AxisWidgetPos[4][2]);
	sprintf(str, "%f", AxisWidgetPos[4][1]);
	//cerr<<ContourPointDen<<endl;
	for(int j=0;j<strlen(str);j++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[j]);


        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

	glColor3f(0,0,1);
	glPushMatrix();
	glTranslatef(lbbox[0],lbbox[1],lbbox[2]);
	gluCylinder(quadratic,0.3f,0.3f,rbbox[2]-lbbox[2],32,32);
	glPopMatrix();

        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);

	glColor4f(1,1,1,1);
	glRasterPos3f(AxisWidgetPos[2][0],AxisWidgetPos[2][1],AxisWidgetPos[2][2]);
	sprintf(str, "%f", AxisWidgetPos[2][2]);
	//cerr<<ContourPointDen<<endl;
	for(int j=0;j<strlen(str);j++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[j]);
	glRasterPos3f(AxisWidgetPos[5][0],AxisWidgetPos[5][1],AxisWidgetPos[5][2]);
	sprintf(str, "%f", AxisWidgetPos[5][2]);
	//cerr<<ContourPointDen<<endl;
	for(int j=0;j<strlen(str);j++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[j]);

        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
}

void svLineGlyphQDOT::DrawAxisWidgets(Matrix4f m, GLenum mode)
{
	GLfloat ma[16];
	m.getValue(ma);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

   	glEnable(GL_DEPTH_TEST);

	GLUquadricObj *quadratic;
	quadratic = gluNewQuadric();
	gluQuadricDrawStyle(quadratic, GLU_FILL);
	gluQuadricNormals(quadratic, GLU_SMOOTH);

        if (mode == GL_SELECT)
        	 glLoadName (1);
	//cerr<<green<<endl;
	//glColor3f(1,green,0);
	glColor3f(AxisWidgetCol[0][0],AxisWidgetCol[0][1],AxisWidgetCol[0][2]);
	glPushMatrix();
        glMultMatrixf(ma);
        if (mode == GL_SELECT)
            glPushName (0);
	glPushMatrix();
	//glTranslatef(l[0]+offx,lbbox[1],lbbox[2]);
	glTranslatef(AxisWidgetPos[0][0],AxisWidgetPos[0][1],AxisWidgetPos[0][2]);
	glRotatef(90,0,1,0);
 	glutSolidTorus (0.3f, 0.7f, 30, 30);
	glPopMatrix();
         if (mode == GL_SELECT)
            glPopName ();

        if (mode == GL_SELECT)
            glPushName (3);
	glColor3f(AxisWidgetCol[3][0],AxisWidgetCol[3][1],AxisWidgetCol[3][2]);
	glPushMatrix();
	//glTranslatef(r[0],lbbox[1],lbbox[2]);
	glTranslatef(AxisWidgetPos[3][0],AxisWidgetPos[3][1],AxisWidgetPos[3][2]);
	glRotatef(90,0,1,0);
 	glutSolidTorus (0.3f, 0.7f, 30, 30);
	glPopMatrix();
         if (mode == GL_SELECT)
            glPopName ();

        if (mode == GL_SELECT)
            glPushName (1);
	glColor3f(AxisWidgetCol[1][0],AxisWidgetCol[1][1],AxisWidgetCol[1][2]);
	glPushMatrix();
	//glTranslatef(lbbox[0],l[1],lbbox[2]);
	glTranslatef(AxisWidgetPos[1][0],AxisWidgetPos[1][1],AxisWidgetPos[1][2]);
	glRotatef(-90,1,0,0);
 	glutSolidTorus (0.3f, 0.7f, 30, 30);
	glPopMatrix();
         if (mode == GL_SELECT)
            glPopName ();

        if (mode == GL_SELECT)
            glPushName (4);
	glColor3f(AxisWidgetCol[4][0],AxisWidgetCol[4][1],AxisWidgetCol[4][2]);
	glPushMatrix();
	//glTranslatef(lbbox[0],r[1],lbbox[2]);
	glTranslatef(AxisWidgetPos[4][0],AxisWidgetPos[4][1],AxisWidgetPos[4][2]);
	glRotatef(-90,1,0,0);
 	glutSolidTorus (0.3f, 0.7f, 30, 30);
	glPopMatrix();
         if (mode == GL_SELECT)
            glPopName ();

        if (mode == GL_SELECT)
            glPushName (2);
	glColor3f(AxisWidgetCol[2][0],AxisWidgetCol[2][1],AxisWidgetCol[2][2]);
	glPushMatrix();
	//glTranslatef(lbbox[0],lbbox[1],l[2]);
	glTranslatef(AxisWidgetPos[2][0],AxisWidgetPos[2][1],AxisWidgetPos[2][2]);
 	glutSolidTorus (0.3f, 0.7f, 30, 30);
	glPopMatrix();
         if (mode == GL_SELECT)
            glPopName ();

        if (mode == GL_SELECT)
            glPushName (5);
	glColor3f(AxisWidgetCol[5][0],AxisWidgetCol[5][1],AxisWidgetCol[5][2]);
	glPushMatrix();
	//glTranslatef(lbbox[0],lbbox[1],r[2]);
	glTranslatef(AxisWidgetPos[5][0],AxisWidgetPos[5][1],AxisWidgetPos[5][2]);
 	glutSolidTorus (0.3f, 0.7f, 30, 30);
	glPopMatrix();
         if (mode == GL_SELECT)
            glPopName ();
	
	glPopMatrix();
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
}

void svLineGlyphQDOT::DrawContourWidgets(Matrix4f m,GLenum mode)
{
	GLfloat ma[16];
	m.getValue(ma);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

   	glEnable(GL_DEPTH_TEST);

	GLUquadricObj *quadratic;
	quadratic = gluNewQuadric();
	gluQuadricDrawStyle(quadratic, GLU_FILL);
	gluQuadricNormals(quadratic, GLU_SMOOTH);

	glColor3f(ContourWidgetCol[0][0],ContourWidgetCol[0][1],ContourWidgetCol[0][2]);
	glPushMatrix();
        glMultMatrixf(ma);

        if (mode == GL_SELECT)
            glPushName (6);
	glPushMatrix();
	glTranslatef(ContourWidgetPos[0][0],ContourWidgetPos[0][1],ContourWidgetPos[0][2]);
	glRotatef(90,0,1,0);
 	glutSolidTorus (0.3f, 0.7f, 30, 30);
	glPopMatrix();
        if (mode == GL_SELECT)
            glPopName ();

        if (mode == GL_SELECT)
            glPushName (7);
	glColor3f(ContourWidgetCol[1][0],ContourWidgetCol[1][1],ContourWidgetCol[1][2]);
	glPushMatrix();
	glTranslatef(ContourWidgetPos[1][0],ContourWidgetPos[1][1],ContourWidgetPos[1][2]);
	glRotatef(-90,1,0,0);
 	glutSolidTorus (0.3f, 0.7f, 30, 30);
	glPopMatrix();
         if (mode == GL_SELECT)
            glPopName ();

        if (mode == GL_SELECT)
            glPushName (8);
	glColor3f(ContourWidgetCol[2][0],ContourWidgetCol[2][1],ContourWidgetCol[2][2]);
	glPushMatrix();
	glTranslatef(ContourWidgetPos[2][0],ContourWidgetPos[2][1],ContourWidgetPos[2][2]);
 	glutSolidTorus (0.3f, 0.7f, 30, 30);
	glPopMatrix();
         if (mode == GL_SELECT)
            glPopName ();

        if (mode == GL_SELECT)
            glPushName (9);
	glColor3f(ContourPointCol[0],ContourPointCol[1],ContourPointCol[2]);
	glPushMatrix();
	glTranslatef(ContourWidgetPos[0][0],ContourWidgetPos[1][1],ContourWidgetPos[2][2]);
 	//glutSolidTorus (0.3f, 0.7f, 30, 30);
	glutSolidSphere(0.5,30,30);
	
	glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);

	glColor4f(1,1,1,1);
	glRasterPos3f(0,0,1);
	char str[50];
	sprintf(str, "%f", ContourPointDen);
	//cerr<<ContourPointDen<<endl;
	for(int j=0;j<strlen(str);j++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[j]);
	glPopMatrix();
         if (mode == GL_SELECT)
            glPopName ();
	
	glColor4f(1,1,1,1);
	glPushMatrix();
	glBegin(GL_LINES);
	glVertex3f(ContourWidgetPos[0][0],ContourWidgetPos[1][1],ContourWidgetPos[2][2]);
	glVertex3f(lbbox[0]-3,ContourWidgetPos[1][1],ContourWidgetPos[2][2]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(ContourWidgetPos[0][0],ContourWidgetPos[1][1],ContourWidgetPos[2][2]);
	glVertex3f(ContourWidgetPos[0][0],lbbox[1]-3,ContourWidgetPos[2][2]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(ContourWidgetPos[0][0],ContourWidgetPos[1][1],ContourWidgetPos[2][2]);
	glVertex3f(ContourWidgetPos[0][0],ContourWidgetPos[1][1],lbbox[2]-3);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(ContourWidgetPos[0][0],ContourWidgetPos[1][1],ContourWidgetPos[2][2]);
	glVertex3f(ContourWidgetPos[0][0],ContourWidgetPos[0][1],ContourWidgetPos[0][2]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(ContourWidgetPos[0][0],ContourWidgetPos[1][1],ContourWidgetPos[2][2]);
	glVertex3f(ContourWidgetPos[1][0],ContourWidgetPos[1][1],ContourWidgetPos[1][2]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(ContourWidgetPos[0][0],ContourWidgetPos[1][1],ContourWidgetPos[2][2]);
	glVertex3f(ContourWidgetPos[2][0],ContourWidgetPos[2][1],ContourWidgetPos[2][2]);
	glEnd();
	glPopMatrix();
	
	glPopMatrix();
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);	
}

void svLineGlyphQDOT::ProcessHit (GLint hits, GLuint buffer[])
{
   unsigned int i, j;
   GLuint ii, jj, names, *ptr;
   int gflag = 0;

   //printf ("hits = %d\n", hits);
   ptr = (GLuint *) buffer;
   for (i = 0; i < 1; i++) { /*  for each hit  */
      names = *ptr; //cerr<<"name : "<<names<<endl;
      ptr++;
      ptr++;
      ptr++;
      for (j = 0; j < names; j++) { /*  for each name */
         if (j == 0)  /*  set row and column  */
            ii = *ptr;
         else if (j == 1)
            jj = *ptr;
         ptr++;
      }
   }
   if(hits>0 )
   {	AxisWidget = jj; //cerr<<"widget: "<<AxisWidget<<endl;
	green=1;
	//if( jj <=5)	
		hitflag = 1;
   }
   else green =0;
}

void svLineGlyphQDOT::SetAxisWidgetPos(svVector3 l, svVector3 r)
{
	AxisWidgetPos[0][0] = l[0];
	AxisWidgetPos[0][1] = lbbox[1];
	AxisWidgetPos[0][2] = lbbox[2];

	AxisWidgetPos[1][0] = lbbox[0];
	AxisWidgetPos[1][1] = l[1];
	AxisWidgetPos[1][2] = lbbox[2];

	AxisWidgetPos[2][0] = lbbox[0];
	AxisWidgetPos[2][1] = lbbox[1];
	AxisWidgetPos[2][2] = l[2];

	AxisWidgetPos[3][0] = r[0];
	AxisWidgetPos[3][1] = lbbox[1];
	AxisWidgetPos[3][2] = lbbox[2];

	AxisWidgetPos[4][0] = lbbox[0];
	AxisWidgetPos[4][1] = r[1];
	AxisWidgetPos[4][2] = lbbox[2];

	AxisWidgetPos[5][0] = lbbox[0];
	AxisWidgetPos[5][1] = lbbox[1];
	AxisWidgetPos[5][2] = r[2];

	for(int i=0;i<6;i++)
 	{
		AxisWidgetCol[i][0] = 0.6;
		AxisWidgetCol[i][1] = 0.6;
		AxisWidgetCol[i][2] = 0.6;
	}
}

void svLineGlyphQDOT::SetContourWidgetPos(svVector3 l)
{
	ContourWidgetPos[0][0] = l[0];
	ContourWidgetPos[0][1] = rbbox[1];
	ContourWidgetPos[0][2] = rbbox[2];

	ContourWidgetPos[1][0] = rbbox[0];
	ContourWidgetPos[1][1] = l[1];
	ContourWidgetPos[1][2] = rbbox[2];

	ContourWidgetPos[2][0] = rbbox[0];
	ContourWidgetPos[2][1] = rbbox[1];
	ContourWidgetPos[2][2] = l[2];	

	for(int i=0;i<3;i++)
 	{
		ContourWidgetCol[i][0] = 0.6;
		ContourWidgetCol[i][1] = 0.6;
		ContourWidgetCol[i][2] = 0.6;
	}

	ContourPointPos[0] = l[0];
	ContourPointPos[1] = l[1];
	ContourPointPos[2] = l[2];

	ContourPointCol[0] = 1;
	ContourPointCol[1] = 1;
	ContourPointCol[2] = 0;
		
	contour = 0;
}

void svLineGlyphQDOT::SetAxisWidgetCol()
{
	for(int i=0;i<6;i++)
 	{
		AxisWidgetCol[i][0] = 0.6;
		AxisWidgetCol[i][1] = 0.6;
		AxisWidgetCol[i][2] = 0.6;
	}
	for(int i=0;i<3;i++)
 	{
		ContourWidgetCol[i][0] = 0.6;
		ContourWidgetCol[i][1] = 0.6;
		ContourWidgetCol[i][2] = 0.6;
	}
}

int svLineGlyphQDOT::PickWidgets(Matrix4f m, svVector3 l, svVector3 r, int x, int y,int w, int h)
{

   GLuint selectBuf[BUFSIZE];

   GLint viewport[4];
   hitflag = 0;

   //if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) {hitflag = 0;green=0;glutPostRedisplay();return;}

  // if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
   //   return;

   glGetIntegerv (GL_VIEWPORT, viewport);

   glSelectBuffer (BUFSIZE, selectBuf);
   (void) glRenderMode (GL_SELECT);

   glInitNames();
   glPushName(0);

   glMatrixMode (GL_PROJECTION);
   glPushMatrix ();

   glLoadIdentity ();

   gluPickMatrix ((GLdouble) x, (GLdouble) (viewport[3] - y), 
                  5, 5, viewport);
   gluPerspective(45,(float)w/h, 0.001,1000);
   glMatrixMode(GL_MODELVIEW);
  
   DrawAxisWidgets(m,GL_SELECT);
   DrawContourWidgets(m,GL_SELECT);

   glMatrixMode(GL_MODELVIEW);
   glFlush();
   glMatrixMode (GL_PROJECTION);
   glPopMatrix ();
   glMatrixMode(GL_MODELVIEW);


   hits = glRenderMode (GL_RENDER);
   ProcessHit (hits, selectBuf);
  
   if(hits!=0) 
   {			
	  cerr<<AxisWidget<<endl;
	  transflag = 1;
	   if(AxisWidget<=5)
	   {
		
		AxisWidgetCol[AxisWidget][0] = 1;
	   	AxisWidgetCol[AxisWidget][1] = 1;
	   	AxisWidgetCol[AxisWidget][2] = 0;
	   }
	   else
	   {
		ContourWidgetCol[AxisWidget-6][0] = 1;
	   	ContourWidgetCol[AxisWidget-6][1] = 1;
	   	ContourWidgetCol[AxisWidget-6][2] = 0;
           }
   }

   ix = x;
   iy = y;

   return transflag;
   //glutPostRedisplay();	
}

void svLineGlyphQDOT::MoveWidgets(Matrix4f m, int x, int y, svVector3 &l, svVector3 &r)
{
	   GLfloat ma[16];
	   m.getValue(ma);

	   GLdouble wx,wy,wz,ox,oy,oz;
	   float wwz;
	   GLint viewport[4];
	   GLdouble modelview[16];
	   GLdouble projection[16];

	   wx = (float)x;
	   glPushMatrix();
	   //glTranslatef(offx,offy,offz);
           glMultMatrixf(ma);
	   glGetIntegerv(GL_VIEWPORT,viewport);//calculate the viewport matrix   
	   glGetDoublev(GL_MODELVIEW_MATRIX,modelview);// calculate the modelview matrix
	   glGetDoublev(GL_PROJECTION_MATRIX,projection);//calculate the projection matrix
	   glPopMatrix();

	   wy=(float)viewport[3]-(float)y;

	   glReadPixels((int)wx,(int)wy,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&wwz);
	   //if(wwz == 1 || wwz < 0.1) wwz = wwz2;
	   gluUnProject((GLdouble)wx,(GLdouble)wy,(GLdouble)wwz,modelview,projection,viewport,&ox,&oy,&oz); 

	if(hitflag)
	{

	   //cerr<<"---------------------------------"<<endl;
	   //cerr<<ix<<" "<<iy<<" "<<iz<<endl;
	   //cerr<<ox<<" "<<oy<<" "<<oz<<endl;
	   //cerr<<AxisWidget<<endl;
	   
	   float tmp;
	   if(AxisWidget == 0 )
	   {
		contour = 0;
		tmp = ox - ix + AxisWidgetPos[AxisWidget][0];
		if(tmp <= AxisWidgetPos[AxisWidget+3][0] && tmp >= lbbox[0])
			AxisWidgetPos[AxisWidget][0] = tmp;			
           }
	   if(AxisWidget == 1 )
	   {
		contour = 0;
		tmp = oy - iy + AxisWidgetPos[AxisWidget][1];
		if(tmp <= AxisWidgetPos[AxisWidget+3][1] && tmp >= lbbox[1])
			AxisWidgetPos[AxisWidget][1] = tmp;			
           }
	   if(AxisWidget == 2 )
	   {
		contour = 0;
		tmp = oz - iz + AxisWidgetPos[AxisWidget][2];
		if(tmp <= AxisWidgetPos[AxisWidget+3][2] && tmp >= lbbox[2])
			AxisWidgetPos[AxisWidget][2] = tmp;			
           }
	   if(AxisWidget == 3 )
	   {
		contour = 0;
		tmp = ox - ix + AxisWidgetPos[AxisWidget][0];
		if(tmp >= AxisWidgetPos[AxisWidget-3][0] && tmp <= rbbox[0])
			AxisWidgetPos[AxisWidget][0] = tmp;			
           }
	   if(AxisWidget == 4 )
	   {
		contour = 0;
		tmp = oy - iy + AxisWidgetPos[AxisWidget][1];
		if(tmp >= AxisWidgetPos[AxisWidget-3][1] && tmp <= rbbox[1])
			AxisWidgetPos[AxisWidget][1] = tmp;			
           }
	   if(AxisWidget == 5 )
	   {
		contour = 0;
		tmp = oz - iz + AxisWidgetPos[AxisWidget][2];
		if(tmp >= AxisWidgetPos[AxisWidget-3][2] && tmp <= rbbox[2])
			AxisWidgetPos[AxisWidget][2] = tmp;
		//cerr<<tmp<<endl;			
           }	 

	   if(AxisWidget == 6 )
	   {
		contour = 1;
		tmp = ox - ix + ContourWidgetPos[AxisWidget-6][0];
		if(tmp >= lbbox[0] && tmp <= rbbox[0])
			ContourWidgetPos[AxisWidget-6][0] = tmp;
		cerr<<tmp<<endl;			
           }
	   if(AxisWidget == 7 )
	   {
		contour = 1;
		tmp = oy - iy + ContourWidgetPos[AxisWidget-6][1];
		if(tmp >= lbbox[1] && tmp <= rbbox[1])
			ContourWidgetPos[AxisWidget-6][1] = tmp;			
           }
	   if(AxisWidget == 8 )
	   {
		contour = 1;
		tmp = oz - iz + ContourWidgetPos[AxisWidget-6][2];
		if(tmp >= lbbox[2] && tmp <= rbbox[2])
			ContourWidgetPos[AxisWidget-6][2] = tmp;			
           }		
	  

	l[0]=AxisWidgetPos[0][0];//cerr<<l[0]<<" "<<l[1]<<" "<<l[2]<<endl;
	l[1]=AxisWidgetPos[1][1];
	l[2]=AxisWidgetPos[2][2];
//cerr<<l[0]<<" "<<l[1]<<" "<<l[2]<<endl;
	r[0]=AxisWidgetPos[3][0];
	r[1]=AxisWidgetPos[4][1];
	r[2]=AxisWidgetPos[5][2];   
//cerr<<r[0]<<" "<<r[1]<<" "<<r[2]<<endl;
	}
	else
	{
		for(int i=0;i<6;i++)
	 	{
			AxisWidgetCol[i][0] = 0.6;
			AxisWidgetCol[i][1] = 0.6;
			AxisWidgetCol[i][2] = 0.6;
		}
	}

	   wwz2 = wwz;
	   ix = ox;
	   iy = oy;
	   iz = oz;	
}

/// TODO (JC. 1/8/14): there might be a bug in it. it does not work.
void svLineGlyphQDOT::BuildDisplayListFromStore()
{
  STILL_UPDATE = true;
  cleanDisplayList(SV_IMMEDIATE);

  display_list = glGenLists(1);
  glNewList(display_list, GL_COMPILE);

  int samplingSites = glyphPos[0].size();
  svVector3 v0, v1;

  glPointSize(2.0);

  glBegin(GL_POINTS);
  glColor3f(1,0,0);
  for(int i=0; i<samplingSites; i++)
  {
    //if(ENABLE_COLOR) glColor4fv(glyphColors[0][i].getValue());
	svVector3 v0=glyphPos[0][i].getValue();
	v1[0]=glyphPos[0][i][0]+glyphMag[i]*glyphDir[0][i][0];
	v1[1]=glyphPos[0][i][1]+glyphMag[i]*glyphDir[0][i][1];
	v1[2]=glyphPos[0][i][2]+glyphMag[i]*glyphDir[0][i][2];
//cerr << "render dot: "<<i<<": "<< v0[0] << ", " << v0[1]<<", " << v0[2]<<endl;
//cerr << "render dot: "<<i<<": "<< v1[0] << ", " << v1[1]<<", " << v1[2]<<endl;
  }; // end for i
  //glVertex3fv(&v0[0]);
  glVertex3f(v0[0], v0[1], v0[2]);
  // just draw the starting points
  glEnd();

  glEndList();
  glPointSize(1.0);
}

// TODO: not implemented yet
void svLineGlyphQDOT::RebuildDisplayList()
{
	
}

void svLineGlyphQDOT::Print()
{
  for(int i=0; i<seed_num; i++)
  {
    int s = glyphPos[i].size();
    cerr << s << endl;
    for(int j=0; j<s; j++)
    {
      cerr <<"(" << i<<", " << j<<"): "<< glyphPos[i][j][0] << "  "
           << glyphPos[i][j][1] << "  "
	       << glyphPos[i][j][2] << endl;
    };
  }
}

void svLineGlyphQDOT::SetGlyphScale(svFloat sc)
{
/* TODO
  int gs = glyphPos[0].size();

  // update the scale
  if(ENABLE_GLYPH_SCALE==false) // uniformed scale
  {
     for(int gi=0; gi<gs; gi++)
     {
       glyphScale[gi]=sc;
	   glyphPos[1][gi]= glyphPos[0][gi]+glyphScale[gi]*glyphDir[0][gi];
     }; // end for(gi)
  }
  else
  {
     // map the largest magnitude to the length of sc
	 // (1) use the global scale
     svFloat scalar = sc/glyphMagRange[1];
	 cerr << "svLineGlyphQDOT: glyph scalar  = " << scalar << endl;
	 for(int gi=0; gi<gs; gi++)
	 {
       glyphScale[gi]=glyphMag[gi]* scalar;
	   //cerr << "current glyphScales: " << glyphScale[gi]<<endl;
	   glyphPos[1][gi]= glyphPos[0][gi]+ glyphScale[gi]*glyphDir[0][gi];
	 }; // end for(gi)
  }; // end if(ENABLE_GLYPH_SCALE)
  */
}

//
void svLineGlyphQDOT::SetGlyphScale(svFloat sc, svFloat zmin, svFloat zmax)
{
  int gs = glyphPos[0].size();

  // update the scale
  if(ENABLE_GLYPH_SCALE==false) // uniformed scale
  {
     for(int gi=0; gi<gs; gi++)
     {
       glyphScale[gi]=sc;
	   glyphPos[1][gi]= glyphPos[0][gi]+glyphScale[gi]*glyphDir[0][gi];
     }; // end for(gi)
  }
  else
  {
     svFloat localMaxMag=-1000000, localMinMag=1000000;
	 for(int gi=0; gi<gs; gi++)
	 {
	   if(glyphPos[0][gi][2] >= zmin && glyphPos[0][gi][2] <= zmax)
	   {
	     if(glyphMag[gi]<localMinMag) localMinMag=glyphMag[gi];
	     if(glyphMag[gi]>localMaxMag) localMaxMag=glyphMag[gi];
	   }; // end if(glyphPos)
	 }; // end for(gi)
	 cerr << "local min/max mag: "<<localMinMag<<", "<<localMaxMag<<endl;

     // map the largest magnitude to the length of sc
	 // (1) use the global scale
     svFloat scalar = sc/localMaxMag;
	 cerr << "svLineGlyphQDOT: glyph scalar  = " << scalar << endl;
	 for(int gi=0; gi<gs; gi++)
	 {
	    //cerr << "current glyphScales: " << glyphScale[gi]<<endl;
	    glyphPos[1][gi]= glyphPos[0][gi]+ 
		            glyphMag[gi]*scalar*glyphDir[0][gi];
	 }; // end for(gi)
  }; // end if(ENABLE_GLYPH_SCALE)
}


void svLineGlyphQDOT::SaveToFile(char *dir, char *fname, int freq)
{
  char outFileName[2056];
  sprintf(outFileName, "%s/%s.txt", dir, fname);

  ofstream outf(outFileName);
  if(outf==NULL) 
  {
    cerr << "SaveToFile failed: " << outFileName << endl;
	exit(-1);
  }
  for(int i=0; i<seed_num; i++)
  {
    int s = glyphPos[i].size();
	svInt  c = s/freq;
    outf << s/freq << endl;
	int counter=0;
    for(int j=0; j<s; j=j+freq)
    {
	  outf << 2 << endl;
      outf << glyphPos[0][j][0] << "  "
           << glyphPos[0][j][1] << "  "
	       << glyphPos[0][j][2] << endl;

	  // Dec 25 2013: JC: for coloring purpose, shift the glyphPos
	  //   a bit if the two points are too close. In this way, the
	  //   user can still see the orientation of the glyph.
	  if( ( 
	     (glyphPos[1][j][0]-glyphPos[0][j][0])*
	     (glyphPos[1][j][0]-glyphPos[0][j][0])+
	     (glyphPos[1][j][1]-glyphPos[0][j][1])*
	     (glyphPos[1][j][1]-glyphPos[0][j][1])+
	     (glyphPos[1][j][2]-glyphPos[0][j][2])*
	     (glyphPos[1][j][2]-glyphPos[0][j][2])) < 0.0000001)
	  {
	    //cerr << "reposition.." << endl;
	    glyphPos[1][j]=glyphPos[0][j]+glyphDir[0][j]*0.001;
	  }

      outf << glyphPos[1][j][0] << "  "
           << glyphPos[1][j][1] << "  "
	       << glyphPos[1][j][2] << endl;

	  counter++;
	  if(counter==c) break;
    };
  };
  outf.close();
}

}
