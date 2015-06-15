#include "svOutline.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <string.h>
using namespace std;

namespace __svl_lib
{

void svOutline::SetAxisWidgetPos(svVector3 l, svVector3 r)
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
	
        for(int i=0;i<10;i++)
        {
                AxisWidgetCol[i][0] = 0.6;
                AxisWidgetCol[i][1] = 0.6;
                AxisWidgetCol[i][2] = 0.6;
        }
}

void svOutline::SetContourWidgetPos(svVector3 l)
{
	AxisWidgetPos[6] = l;

        AxisWidgetPos[7][0] = l[0];
        AxisWidgetPos[7][1] = lbbox[1];
        AxisWidgetPos[7][2] = lbbox[2];

        AxisWidgetPos[8][0] = lbbox[0];
        AxisWidgetPos[8][1] = l[1];
        AxisWidgetPos[8][2] = lbbox[2];

        AxisWidgetPos[9][0] = lbbox[0];
        AxisWidgetPos[9][1] = lbbox[1];
        AxisWidgetPos[9][2] = l[2];

}

void svOutline::DrawAxis(svVector3 &l, svVector3 &r)
{
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glLineWidth(0.5);

	char str[50];

	//bounding box
	glColor3f(227./256.,26./256.,28./256.);//x
	glBegin(GL_LINES);
	glVertex3f(lbbox[0],lbbox[1],lbbox[2]);
	glVertex3f(rbbox[0],lbbox[1],lbbox[2]);
	glEnd();
	glColor3f(35./256.,139./256.,69./256.);//y
	glBegin(GL_LINES);
	glVertex3f(lbbox[0],lbbox[1],lbbox[2]);
	glVertex3f(lbbox[0],rbbox[1],lbbox[2]);
	glEnd();
	glColor3f(33./256.,113./256.,181./256.);//z
	glBegin(GL_LINES);
	glVertex3f(lbbox[0],lbbox[1],lbbox[2]);
	glVertex3f(lbbox[0],lbbox[1],rbbox[2]);
	glEnd();

       /* glColor4f(1,1,1,1);
        glRasterPos3f(AxisWidgetPos[0][0],AxisWidgetPos[0][1],AxisWidgetPos[0][2]);
        sprintf(str, "%f", AxisWidgetPos[0][0]);
        for(int j=0;j<strlen(str);j++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
        glRasterPos3f(AxisWidgetPos[3][0],AxisWidgetPos[3][1],AxisWidgetPos[3][2]);
        sprintf(str, "%f", AxisWidgetPos[3][0]);
        for(int j=0;j<strlen(str);j++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
        glRasterPos3f(AxisWidgetPos[7][0],AxisWidgetPos[7][1],AxisWidgetPos[7][2]);
        sprintf(str, "%f", AxisWidgetPos[7][0]);
        for(int j=0;j<strlen(str);j++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

        glRasterPos3f(AxisWidgetPos[1][0],AxisWidgetPos[1][1],AxisWidgetPos[1][2]);
        sprintf(str, "%f", AxisWidgetPos[1][1]);
        for(int j=0;j<strlen(str);j++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
        glRasterPos3f(AxisWidgetPos[4][0],AxisWidgetPos[4][1],AxisWidgetPos[4][2]);
        sprintf(str, "%f", AxisWidgetPos[4][1]);
        for(int j=0;j<strlen(str);j++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
        glRasterPos3f(AxisWidgetPos[8][0],AxisWidgetPos[8][1],AxisWidgetPos[8][2]);
        sprintf(str, "%f", AxisWidgetPos[8][1]);
        for(int j=0;j<strlen(str);j++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

        glRasterPos3f(AxisWidgetPos[2][0],AxisWidgetPos[2][1],AxisWidgetPos[2][2]);
        sprintf(str, "%f", AxisWidgetPos[2][2]);
        for(int j=0;j<strlen(str);j++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
        glRasterPos3f(AxisWidgetPos[5][0],AxisWidgetPos[5][1],AxisWidgetPos[5][2]);
        sprintf(str, "%f", AxisWidgetPos[5][2]);
        for(int j=0;j<strlen(str);j++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
        glRasterPos3f(AxisWidgetPos[9][0],AxisWidgetPos[9][1],AxisWidgetPos[9][2]);
        sprintf(str, "%f", AxisWidgetPos[9][2]);
        for(int j=0;j<strlen(str);j++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
*/

	
	glColor3f(227./256.,26./256.,28./256.);//x
	glBegin(GL_LINES);
	glVertex3f(lbbox[0],rbbox[1],lbbox[2]);
	glVertex3f(rbbox[0],rbbox[1],lbbox[2]);
	glEnd();

	glColor3f(35./256.,139./256.,69./256.); //y
        glBegin(GL_LINES);
        glVertex3f(rbbox[0],rbbox[1],lbbox[2]);
	glVertex3f(rbbox[0],lbbox[1],lbbox[2]);
	glEnd();

	glColor3f(35./256.,139./256.,69./256.); //y
        glBegin(GL_LINES);
        glVertex3f(lbbox[0],lbbox[1],rbbox[2]);
        glVertex3f(lbbox[0],rbbox[1],rbbox[2]);
	glEnd();

	glColor3f(227./256.,26./256.,28./256.);//x
        glBegin(GL_LINES);
        glVertex3f(lbbox[0],rbbox[1],rbbox[2]);
        glVertex3f(rbbox[0],rbbox[1],rbbox[2]);
	glEnd();

	glColor3f(35./256.,139./256.,69./256.); //y
        glBegin(GL_LINES);
        glVertex3f(rbbox[0],rbbox[1],rbbox[2]);
        glVertex3f(rbbox[0],lbbox[1],rbbox[2]);
        glEnd();

	glColor3f(227./256.,26./256.,28./256.);//x
        glBegin(GL_LINES);
        glVertex3f(lbbox[0],lbbox[1],rbbox[2]);
        glVertex3f(rbbox[0],lbbox[1],rbbox[2]);
        glEnd();

	glColor3f(33./256.,113./256.,181./256.);//z
        glBegin(GL_LINES);
        glVertex3f(lbbox[0],rbbox[1],lbbox[2]);
        glVertex3f(lbbox[0],rbbox[1],rbbox[2]);
        glEnd();

	glColor3f(33./256.,113./256.,181./256.);//z
        glBegin(GL_LINES);
        glVertex3f(rbbox[0],lbbox[1],lbbox[2]);
        glVertex3f(rbbox[0],lbbox[1],rbbox[2]);
        glEnd();

	glColor3f(33./256.,113./256.,181./256.);//z
        glBegin(GL_LINES);
        glVertex3f(rbbox[0],rbbox[1],lbbox[2]);
        glVertex3f(rbbox[0],rbbox[1],rbbox[2]);
        glEnd();
	//---

	//ROI bounding box
	/*svVector3 tmpl;
	svVector3 tmpr;
	tmpl[0]=AxisWidgetPos[0][0];tmpl[1]=AxisWidgetPos[1][1];tmpl[2]=AxisWidgetPos[2][2];
	tmpr[0]=AxisWidgetPos[3][0];tmpr[1]=AxisWidgetPos[4][1];tmpr[2]=AxisWidgetPos[5][2];

	glColor3f(1.,1.,1.);
	glBegin(GL_LINE_LOOP);
	glVertex3f(tmpl[0],tmpl[1],tmpl[2]);
	glVertex3f(tmpr[0],tmpl[1],tmpl[2]);
	glVertex3f(tmpr[0],tmpl[1],tmpr[2]);
	glVertex3f(tmpl[0],tmpl[1],tmpr[2]);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3f(tmpl[0],tmpr[1],tmpl[2]);
	glVertex3f(tmpr[0],tmpr[1],tmpl[2]);
	glVertex3f(tmpr[0],tmpr[1],tmpr[2]);
	glVertex3f(tmpl[0],tmpr[1],tmpr[2]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(tmpr[0],tmpl[1],tmpl[2]);
	glVertex3f(tmpr[0],tmpr[1],tmpl[2]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(tmpr[0],tmpl[1],tmpr[2]);
	glVertex3f(tmpr[0],tmpr[1],tmpr[2]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(tmpl[0],tmpl[1],tmpl[2]);
	glVertex3f(tmpl[0],tmpr[1],tmpl[2]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(tmpl[0],tmpl[1],tmpr[2]);
	glVertex3f(tmpl[0],tmpr[1],tmpr[2]);
	glEnd();*/

	
	//----------------------------



	l[0] = AxisWidgetPos[0][0];
	l[1] = AxisWidgetPos[1][1];
	l[2] = AxisWidgetPos[2][2];

	r[0] = AxisWidgetPos[3][0];
	r[1] = AxisWidgetPos[4][1];
	r[2] = AxisWidgetPos[5][2];

	//glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glLineWidth(1.);
}

void svOutline::DrawXYZ()
{
	//draw x,y,z
	glColor3f(227./256.,26./256.,28./256.);
	glBegin(GL_LINES);
	glVertex3f(rbbox[0]*1.5,0,0);
	glVertex3f(0,0,0);
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(rbbox[0]*1.5,0,0);
	glVertex3f(rbbox[0]*1.5-1,0.5,0);
	glVertex3f(rbbox[0]*1.5-1,0,0.5);
	glVertex3f(rbbox[0]*1.5-1,-0.5,0);
	glVertex3f(rbbox[0]*1.5-1,0,-0.5);
	glVertex3f(rbbox[0]*1.5-1,0.5,0);
	glEnd();
  	//glColor3f(1., 1, 1);
  	glRasterPos3f(rbbox[0]*1.5+2.,0,0);
  	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'x');

        glColor3f(35./256.,139./256.,69./256.); 
        glBegin(GL_LINES);
        glVertex3f(0,rbbox[1]*1.5,0);
        glVertex3f(0,0,0);
        glEnd();
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0,rbbox[1]*1.5,0);
        glVertex3f(-0.5,rbbox[1]*1.5-1,0);
        glVertex3f(0,rbbox[1]*1.5-1,0.5);
        glVertex3f(0.5,rbbox[1]*1.5-1,0);
        glVertex3f(0,rbbox[1]*1.5-1,-0.5);
        glVertex3f(-0.5,rbbox[1]*1.5-1,0);
	glEnd();
  	//glColor3f(1., 1, 1);
  	glRasterPos3f(0.,rbbox[1]*1.5+2.,0);
  	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'y');

        glColor3f(33./256.,113./256.,181./256.);
        glBegin(GL_LINES);
        glVertex3f(0,0,rbbox[2]*1.5);
        glVertex3f(0,0,0);
        glEnd();
	glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0,0,rbbox[2]*1.5);
        glVertex3f(0.5,0,rbbox[2]*1.5-1);
        glVertex3f(0,0.5,rbbox[2]*1.5-1);
        glVertex3f(-0.5,0,rbbox[2]*1.5-1);
        glVertex3f(0,-0.5,rbbox[2]*1.5-1);
        glVertex3f(0.5,0,rbbox[2]*1.5-1);
	glEnd();
  	//glColor3f(1., 1, 1);
  	glRasterPos3f(0.,0.,rbbox[2]*1.5+2.);
  	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'z');
}

void svOutline::DrawContour(float scalar)
{
	glEnable(GL_BLEND);

	char str[50];
//--the point to create contours---
	glColor4f(1,1,1,0.5);
	glBegin(GL_LINES);
	glVertex3f(AxisWidgetPos[6][0],AxisWidgetPos[6][1],AxisWidgetPos[6][2]);
	glVertex3f(lbbox[0],AxisWidgetPos[6][1],AxisWidgetPos[6][2]);
	glEnd();	

        glBegin(GL_LINES);
        glVertex3f(AxisWidgetPos[6][0],AxisWidgetPos[6][1],AxisWidgetPos[6][2]);
        glVertex3f(AxisWidgetPos[6][0],lbbox[1],AxisWidgetPos[6][2]);
        glEnd(); 

        glBegin(GL_LINES);
        glVertex3f(AxisWidgetPos[6][0],AxisWidgetPos[6][1],AxisWidgetPos[6][2]);
        glVertex3f(AxisWidgetPos[6][0],AxisWidgetPos[6][1],lbbox[2]);
        glEnd();

    GLdouble vmat[16], pmat[16], sx, sy, sz;
    GLint    viewport[4];

    glGetDoublev(GL_MODELVIEW_MATRIX, vmat);
    glGetDoublev(GL_PROJECTION_MATRIX, pmat);
    glGetIntegerv(GL_VIEWPORT, viewport);

    gluProject((double)AxisWidgetPos[6][0],(double)AxisWidgetPos[6][1],(double)AxisWidgetPos[6][2], vmat, pmat, viewport, &sx,&sy,&sz);
    //gluProject((double)locx[i], (double)locy[i], (double)0., vmat, pmat, viewport, &sx,&sy,&sz);
    sy = (float)viewport[3]-(float)sy;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,(GLfloat) viewport[2], (GLfloat) viewport[3],0.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_LIGHTING);

        glColor4f(1,1,1,1);
        //glRasterPos3f(AxisWidgetPos[6][0],AxisWidgetPos[6][1],AxisWidgetPos[6][2]);
	glRasterPos2f(sx,sy);
	float tmppow = 1;
	for(int iii=0;iii<scalar;iii++)	
		tmppow = tmppow * 10.;
        sprintf(str, "%g", ContourPointDen/tmppow);//pow(10.,scalar));
        for(int j=0;j<strlen(str);j++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);	

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(pmat);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(vmat);

	glPointSize(7);
        for(int i=6;i<10;i++)
        {
		if(i==ni&&(SELECTED==GL_TRUE||NEAR==GL_TRUE))                
			glColor3f(1,1,0);
		else
		{
			if(i==0||i==3)glColor3f(1.,0.,0.);
			else if(i==1||i==4)glColor3f(0.,1.,0.);
			else if(i==2||i==5)glColor3f(0.,0.,1.);
			else glColor3f(217./255.,240./255.,163./255.);

		}			

                glBegin(GL_POINTS);
                glVertex3f(AxisWidgetPos[i][0],AxisWidgetPos[i][1],AxisWidgetPos[i][2]);
		glEnd();
        }
	glPointSize(1);
}

void svOutline::Draw3dAxis(svVector3 &l, svVector3 &r)
{
        GLfloat mat_diffuse[] = { 0.2, 0.2, 0.2, 1};
        GLfloat mat_specular[] = { 0.3, 0.3, 0.3, 1 };
        GLfloat mat_shininess[] = { 0.02 };
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

	glDisable(GL_BLEND);
        //glEnable(GL_DEPTH_TEST);
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
        gluCylinder(quadratic,0.3f,0.3f,rbbox[0]-lbbox[0],10, 10);
	glPopMatrix();

        char str[50];

        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);

        glColor4f(1,1,1,1);
        glRasterPos3f(AxisWidgetPos[0][0],AxisWidgetPos[0][1],AxisWidgetPos[0][2]);
        sprintf(str, "%f", AxisWidgetPos[0][0]);
        for(int j=0;j<strlen(str);j++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[j]);
        glRasterPos3f(AxisWidgetPos[3][0],AxisWidgetPos[3][1],AxisWidgetPos[3][2]);
        sprintf(str, "%f", AxisWidgetPos[3][0]);
        for(int j=0;j<strlen(str);j++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[j]);

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glColor3f(0,1,0);

        glPushMatrix();
        glTranslatef(lbbox[0],lbbox[1],lbbox[2]);
        glRotatef(-90,1,0,0);
        gluCylinder(quadratic,0.3f,0.3f,rbbox[1]-lbbox[1],10,10);
        glPopMatrix();

        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);

        glColor4f(1,1,1,1);
        glRasterPos3f(AxisWidgetPos[1][0],AxisWidgetPos[1][1],AxisWidgetPos[1][2]);
        sprintf(str, "%f", AxisWidgetPos[1][1]);
        for(int j=0;j<strlen(str);j++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[j]);
        glRasterPos3f(AxisWidgetPos[4][0],AxisWidgetPos[4][1],AxisWidgetPos[4][2]);
        sprintf(str, "%f", AxisWidgetPos[4][1]);
        for(int j=0;j<strlen(str);j++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[j]);


        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

        glColor3f(0,0,1);
        glPushMatrix();
        glTranslatef(lbbox[0],lbbox[1],lbbox[2]);
        gluCylinder(quadratic,0.3f,0.3f,rbbox[2]-lbbox[2],10,10);
        glPopMatrix();

        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);

        glColor4f(1,1,1,1);
        glRasterPos3f(AxisWidgetPos[2][0],AxisWidgetPos[2][1],AxisWidgetPos[2][2]);
        sprintf(str, "%f", AxisWidgetPos[2][2]);
        for(int j=0;j<strlen(str);j++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[j]);
        glRasterPos3f(AxisWidgetPos[5][0],AxisWidgetPos[5][1],AxisWidgetPos[5][2]);
        sprintf(str, "%f", AxisWidgetPos[5][2]);
        for(int j=0;j<strlen(str);j++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[j]);

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

        for(int i=0;i<6;i++)
        {
		if(i!=ni&&SELECTED==GL_TRUE)
                	glColor3f(AxisWidgetCol[i][0],AxisWidgetCol[i][1],AxisWidgetCol[i][2]);
		else
			glColor3f(1,1,0);
                glPushMatrix();
                glTranslatef(AxisWidgetPos[i][0],AxisWidgetPos[i][1],AxisWidgetPos[i][2]);
		if(i==0||i==3)
                glRotatef(90,0,1,0);
		else if(i==1 || i==4)glRotatef(-90,1,0,0);
                glutSolidTorus (0.3f, 0.7f, 30, 30);
                glPopMatrix();
        }

        glPushMatrix();
        glTranslatef(AxisWidgetPos[6][0],AxisWidgetPos[6][1],AxisWidgetPos[6][2]);
	glColor3f(1,1,0);
        glutSolidSphere(0.5,30,30);

        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);

        glColor4f(1,1,1,1);
        glRasterPos3f(0,0,1);
        sprintf(str, "%f", ContourPointDen);
        for(int j=0;j<strlen(str);j++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[j]);
	glPopMatrix();

       
	glColor3f(1,1,1);
	glBegin(GL_LINES);
	glVertex3f(AxisWidgetPos[6][0],AxisWidgetPos[6][1],AxisWidgetPos[6][2]);
	glVertex3f(lbbox[0],AxisWidgetPos[6][1],AxisWidgetPos[6][2]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(AxisWidgetPos[8][0],AxisWidgetPos[8][1],AxisWidgetPos[8][2]);
        glVertex3f(lbbox[0],AxisWidgetPos[6][1],AxisWidgetPos[6][2]);
        glEnd();	

        glBegin(GL_LINES);
        glVertex3f(AxisWidgetPos[6][0],AxisWidgetPos[6][1],AxisWidgetPos[6][2]);
        glVertex3f(AxisWidgetPos[6][0],lbbox[1],AxisWidgetPos[6][2]);
        glEnd(); 
	glBegin(GL_LINES);
       	glVertex3f(AxisWidgetPos[7][0],AxisWidgetPos[7][1],AxisWidgetPos[7][2]);
        glVertex3f(AxisWidgetPos[6][0],lbbox[1],AxisWidgetPos[6][2]);
        glEnd();

        glBegin(GL_LINES);
        glVertex3f(AxisWidgetPos[6][0],AxisWidgetPos[6][1],AxisWidgetPos[6][2]);
        glVertex3f(AxisWidgetPos[6][0],AxisWidgetPos[6][1],lbbox[2]);
        glEnd();
        glBegin(GL_LINES);
        glVertex3f(AxisWidgetPos[9][0],AxisWidgetPos[9][1],AxisWidgetPos[9][2]);
        glVertex3f(AxisWidgetPos[6][0],AxisWidgetPos[6][1],AxisWidgetPos[6][2]);
        glEnd(); 

	glBegin(GL_LINES);
	glVertex3f(lbbox[0],rbbox[1],lbbox[2]);
	glVertex3f(rbbox[0],rbbox[1],lbbox[2]);
	glEnd();

        glBegin(GL_LINES);
        glVertex3f(rbbox[0],rbbox[1],lbbox[2]);
	glVertex3f(rbbox[0],lbbox[1],lbbox[2]);
	glEnd();

        glBegin(GL_LINE_LOOP);
        glVertex3f(lbbox[0],lbbox[1],rbbox[2]);
        glVertex3f(lbbox[0],rbbox[1],rbbox[2]);
        glVertex3f(rbbox[0],rbbox[1],rbbox[2]);
        glVertex3f(rbbox[0],lbbox[1],rbbox[2]);
        glEnd();


        glBegin(GL_LINES);
        glVertex3f(lbbox[0],rbbox[1],lbbox[2]);
        glVertex3f(lbbox[0],rbbox[1],rbbox[2]);
        glEnd();

        glBegin(GL_LINES);
        glVertex3f(rbbox[0],lbbox[1],lbbox[2]);
        glVertex3f(rbbox[0],lbbox[1],rbbox[2]);
        glEnd();

        glBegin(GL_LINES);
        glVertex3f(rbbox[0],rbbox[1],lbbox[2]);
        glVertex3f(rbbox[0],rbbox[1],rbbox[2]);
        glEnd();

	glColor3f(1,1,0);
	glPointSize(30);
	glBegin(GL_POINTS);
	glVertex3f(AxisWidgetPos[7][0],AxisWidgetPos[7][1],AxisWidgetPos[7][2]);
	glEnd();
        glBegin(GL_POINTS);
        glVertex3f(AxisWidgetPos[8][0],AxisWidgetPos[8][1],AxisWidgetPos[8][2]);
        glEnd();
        glBegin(GL_POINTS);
        glVertex3f(AxisWidgetPos[9][0],AxisWidgetPos[9][1],AxisWidgetPos[9][2]);
        glEnd();
	glPointSize(1);

	l[0] = AxisWidgetPos[0][0];
	l[1] = AxisWidgetPos[1][1];
	l[2] = AxisWidgetPos[2][2];

	r[0] = AxisWidgetPos[3][0];
	r[1] = AxisWidgetPos[4][1];
	r[2] = AxisWidgetPos[5][2];

	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
}

void svOutline::move_ctrlpoint(int x, int y)
{
#define THRESHOLD 0.00001
    int whichAxis = -1;

    GLdouble xx, yy, zz;
    GLdouble wld_x, wld_y, wld_z;       /* world coordinate */
    GLdouble tx, ty, tz;

    GLfloat invert_tb[16], tm[16];
    int i, j, k;

   // glGetIntegerv (GL_VIEWPORT, viewport);
   // glGetDoublev (GL_MODELVIEW_MATRIX, mvmatrix);
   // glGetDoublev (GL_PROJECTION_MATRIX, projmatrix);

    xx = (GLdouble)x;
    yy = (GLdouble)width-(GLdouble)y;
    zz = wz[ni];

	if(ni==0||ni==3||ni==7)whichAxis = 0;
        if(ni==1||ni==4||ni==8)whichAxis = 1;
        if(ni==2||ni==5||ni==9)whichAxis = 2;

    gluUnProject(xx, yy, zz,
                mvmatrix, projmatrix, viewport,
                &tx, &ty, &tz);
    k=0;
    for(i=0; i<4; i++)
        for(j=0; j<4; j++)
            tm[k++]=tb_transform[i][j];

    if(!invert_matrix(tm, invert_tb)) {
        printf("Erron in inverting transform matrix\n");
        exit(0);
    }

    wld_x = tx * invert_tb[0] +
            ty * invert_tb[4] +
            tz * invert_tb[8] +
            invert_tb[12];

    wld_y = tx * invert_tb[1] +
            ty * invert_tb[5] +
            tz * invert_tb[9] +
            invert_tb[13];

    wld_z = tx * invert_tb[2] +
            ty * invert_tb[6] +
            tz * invert_tb[10] +
            invert_tb[14];
    if(whichAxis == 0)
        {
		if(ni==0) 
		{
			if(wld_x>=AxisWidgetPos[3][0])wld_x=AxisWidgetPos[3][0];
			else if(wld_x<lbbox[0]) wld_x = lbbox[0];
		}
		else 
		{
			if(ni==3) 
			{
				if(wld_x<=AxisWidgetPos[0][0])wld_x=AxisWidgetPos[0][0]+0.1;
				else if(wld_x>rbbox[0]) wld_x = rbbox[0];
			}
			else
			{
				if(wld_x<=lbbox[0]) wld_x=lbbox[0];
				else if(wld_x>rbbox[0]) wld_x = rbbox[0];
			}
		}
  	   	 AxisWidgetPos[ni][0] = wld_x;
     		 AxisWidgetPos[ni][1] = lbbox[1];
     		 AxisWidgetPos[ni][2] = lbbox[2];
        }
        else if(whichAxis == 1)
        {
                if(ni==1) 
                {       
                        if(wld_y>=AxisWidgetPos[4][1])wld_y=AxisWidgetPos[4][1];
                        else if(wld_y<lbbox[1]) wld_y = lbbox[1];
                }
                else 
                {
                        if(ni==4) 
                        {
                                if(wld_y<=AxisWidgetPos[1][1])wld_y=AxisWidgetPos[1][1]+0.1;
                                else if(wld_y>rbbox[1]) wld_y = rbbox[1];
                        }
                        else
                        {
                                if(wld_y<=lbbox[1]) wld_y=lbbox[1];
                                else if(wld_y>rbbox[1]) wld_y = rbbox[1];
                        }
                }

    		  AxisWidgetPos[ni][0] = lbbox[0];
     	 	  AxisWidgetPos[ni][1] = wld_y;
      		  AxisWidgetPos[ni][2] = lbbox[2];
        }
        else if(whichAxis == 2)
        {
                if(ni==2)
                {
                        if(wld_z>=AxisWidgetPos[5][2])wld_z=AxisWidgetPos[5][2];
                        else if(wld_z<lbbox[2]) wld_z = lbbox[2];
                }
                else
                {
                        if(ni==5)
                        {
                                if(wld_z<=AxisWidgetPos[2][2])wld_z=AxisWidgetPos[2][2]+0.1;
                                else if(wld_z>rbbox[2]) wld_z = rbbox[2];
                        }
                        else
                        {
                                if(wld_z<=lbbox[2]) wld_z=lbbox[2];
                                else if(wld_z>rbbox[2]) wld_z = rbbox[2];
                        }
                }
    		 AxisWidgetPos[ni][0] = lbbox[0];
     		 AxisWidgetPos[ni][1] = lbbox[1];
     		 AxisWidgetPos[ni][2] = wld_z;
        }
	if(ni>=7)
	{
		 AxisWidgetPos[6][0] = AxisWidgetPos[7][0];
                 AxisWidgetPos[6][1] = AxisWidgetPos[8][1];
                 AxisWidgetPos[6][2] = AxisWidgetPos[9][2];
	}
	else if(ni==6)
	{
		if(wld_z<=lbbox[2]) wld_z=lbbox[2];
                else if(wld_z>=rbbox[2]) wld_z = rbbox[2];
		if(wld_y<=lbbox[1]) wld_y=lbbox[1];
                else if(wld_y>=rbbox[1]) wld_y = rbbox[1];
		if(wld_x<=lbbox[0]) wld_x=lbbox[0];
                else if(wld_x>=rbbox[0]) wld_x = rbbox[0];
		  AxisWidgetPos[6][0] = wld_x;
		  AxisWidgetPos[6][1] = wld_y;
		  AxisWidgetPos[6][2] = wld_z;
		  AxisWidgetPos[7][0] = AxisWidgetPos[6][0];
                  AxisWidgetPos[8][1] = AxisWidgetPos[6][1];
                  AxisWidgetPos[9][2] = AxisWidgetPos[6][2];
	}
#undef THRESHOLD

}

void svOutline::Find_Nearest(int x, int y)
{
    int i, j;

    GLdouble td, dd;
    svVector3 p[10];

   // glGetIntegerv (GL_VIEWPORT, viewport);
   // glGetDoublev (GL_MODELVIEW_MATRIX, mvmatrix);
   // glGetDoublev (GL_PROJECTION_MATRIX, projmatrix);

	for(i=0;i<10;i++)
	{
		p[i][0]=0;p[i][1]=0;p[i][2]=0;	
	}

	for(i=0;i<10;i++)
	{
		p[i][0]  = AxisWidgetPos[i][0] * tb_transform[0][0] +
			    AxisWidgetPos[i][1] * tb_transform[1][0] +
			    AxisWidgetPos[i][2] * tb_transform[2][0] +
			    tb_transform[3][0] + p[i][0];
                p[i][1]  = AxisWidgetPos[i][0] * tb_transform[0][1] +
                            AxisWidgetPos[i][1] * tb_transform[1][1] +
                            AxisWidgetPos[i][2] * tb_transform[2][1] +
                            tb_transform[3][1] + p[i][1];
                p[i][2]  = AxisWidgetPos[i][0] * tb_transform[0][2] +
                            AxisWidgetPos[i][1] * tb_transform[1][2] +
                            AxisWidgetPos[i][2] * tb_transform[2][2] +
                            tb_transform[3][2] + p[i][2];
	}
	
	for(i=0;i<10;i++)
	{
		gluProject((GLdouble)p[i][0],
                        (GLdouble)p[i][1],
                        (GLdouble)p[i][2],
                        mvmatrix, projmatrix, viewport,
                        &(wx[i]), &(wy[i]), &(wz[i]));
		wy[i] = (GLdouble)width-wy[i];
		//printf("%lf, %lf, %lf\n", wx[i], wy[i], wz[i]);
	}
   // printf("\n");
  //  printf("x=%d, y=%d\n", x, y);
    dd=9e+9;

        ni = 0;
	for(i=6;i<10;i++)
	{
		td = ((GLdouble)x-wx[i]) *  ((GLdouble)x-wx[i]) +
                 ((GLdouble)y-wy[i]) * ((GLdouble)y-wy[i]);
		if(td<dd)
		{
			dd = td;
			ni = i;
		}
	}
	if(dd<25.){SELECTED = GL_TRUE;if(ni>=6)SELECTED_CONTOUR=GL_TRUE;}
}

bool svOutline::Find_Near(int x, int y)
{
    int i, j;

    GLdouble td, dd;
    svVector3 p[10];

   // glGetIntegerv (GL_VIEWPORT, viewport);
   // glGetDoublev (GL_MODELVIEW_MATRIX, mvmatrix);
   // glGetDoublev (GL_PROJECTION_MATRIX, projmatrix);

	for(i=0;i<10;i++)
	{
		p[i][0]=0;p[i][1]=0;p[i][2]=0;	
	}

	for(i=0;i<10;i++)
	{
		p[i][0]  = AxisWidgetPos[i][0] * tb_transform[0][0] +
			    AxisWidgetPos[i][1] * tb_transform[1][0] +
			    AxisWidgetPos[i][2] * tb_transform[2][0] +
			    tb_transform[3][0] + p[i][0];
                p[i][1]  = AxisWidgetPos[i][0] * tb_transform[0][1] +
                            AxisWidgetPos[i][1] * tb_transform[1][1] +
                            AxisWidgetPos[i][2] * tb_transform[2][1] +
                            tb_transform[3][1] + p[i][1];
                p[i][2]  = AxisWidgetPos[i][0] * tb_transform[0][2] +
                            AxisWidgetPos[i][1] * tb_transform[1][2] +
                            AxisWidgetPos[i][2] * tb_transform[2][2] +
                            tb_transform[3][2] + p[i][2];
	}
	
	for(i=0;i<10;i++)
	{
		gluProject((GLdouble)p[i][0],
                        (GLdouble)p[i][1],
                        (GLdouble)p[i][2],
                        mvmatrix, projmatrix, viewport,
                        &(wx[i]), &(wy[i]), &(wz[i]));
		wy[i] = (GLdouble)width-wy[i];
		//printf("%lf, %lf, %lf\n", wx[i], wy[i], wz[i]);
	}
   // printf("\n");
  //  printf("x=%d, y=%d\n", x, y);
    dd=9e+9;

        ni = 0;
	for(i=6;i<10;i++)
	{
		td = ((GLdouble)x-wx[i]) *  ((GLdouble)x-wx[i]) +
                 ((GLdouble)y-wy[i]) * ((GLdouble)y-wy[i]);
		if(td<dd)
		{
			dd = td;
			ni = i;
		}
	}
	if(dd<100.){return true;}
	else return false;
}

GLboolean svOutline::invert_matrix( const GLfloat *m, GLfloat *out )
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
