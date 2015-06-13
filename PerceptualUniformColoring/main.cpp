#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <assert.h>
#include <string>
#include <ctime>


#include "./src/svUtil.h"
#include "./src/ivview.h"
#include "./src/jcUtil/MGL.h"
#include "./src/ivTrackball.h"
#include "./src/Readdata.h"

#ifndef M_PI 
#define M_PI 3.1415926
#endif

int image_width = 1000;
int image_height = 1000;
int mode = 0;
int order_type = 0;

using namespace __svl_lib;
using namespace std;

GLfloat m[16];
GLfloat m2[16];
Trackball trackball;
Readdata *readfa;


int type = 2;

void InitLight()
{
  //setting of lighting
  GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8, .4};
  GLfloat mat_specular[] = { 1, 1, 1, 1 };
  GLfloat mat_shininess[] = { 20.0 };
 // GLfloat light_position[] = { 0, 0, 60, 0.0 };
  GLfloat light_position[] = { 0.,0.,100, 0.0 };
  GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };
  glClearColor(0.0, 0.0, 0.0, 0.0);

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);

}

void reshape(int width, int height)
{
	glViewport (0., 0., width, height);
  	glMatrixMode(GL_PROJECTION);
  	glLoadIdentity();

        gluPerspective(45,
                   (GLfloat)width/(GLfloat)height,
                   0.1,
                   1000);
	glMatrixMode(GL_MODELVIEW);
  	glLoadIdentity();
        gluLookAt(0,0,200,
              0,0,0,
              0, 1, 0);


	image_height = height;
	image_width = width;

	readfa->SetImageWidth(width-150., height/2.);

}


void display(void)
{

        glClearColor(1.,1.,1.,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	InitLight();

	trackball.getMatrix().getValue(m);//cerr<<m[0]<<endl;

	glEnable(GL_DEPTH_TEST);
        glPushMatrix();
	glMultMatrixf(m);	
	
	if(mode ==0)
		readfa->Render(1);

	glPopMatrix();

    GLdouble vmat[16], pmat[16], sx, sy, sz;
    GLint    viewport[4];

    glGetDoublev(GL_MODELVIEW_MATRIX, vmat);
    glGetDoublev(GL_PROJECTION_MATRIX, pmat);
    glGetIntegerv(GL_VIEWPORT, viewport);


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,(GLfloat) viewport[2], 0.0, (GLfloat) viewport[3]);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_LIGHTING);

	if(type==1)
		readfa->RenderSingleLegend();
	if(type == 2)
		readfa->RenderMultipleLegend();
	if(type == 3)
		readfa->RenderDivergingLegend();

	__svl_lib::Matrix4f panintwod = trackball.getPanMatrix();	
	double twotranx = panintwod[0][3]/2.;
	double twotrany = panintwod[1][3]/2.;

	__svl_lib::Matrix4f zoomintwod = trackball.getZoomMatrix();
	double twodzoom = zoomintwod[2][3]/10.;
	
        glPushMatrix();
	glTranslatef(twotranx, twotrany, 1);
	glScalef(twodzoom, twodzoom,1);

	if(mode==1)
		readfa->Generate_Bar(type);
	if(mode==2)
	{		
		//cerr<<"a"<<endl;
		readfa->Render_OrderBar(type, order_type);
	}

	/*glBegin(GL_TRIANGLES);
	glColor3f(1.,1.,1.);
	glVertex2f(0.,0.);
	glVertex2f(100.,0.);
	glVertex2f(0,100);
	glColor3f(0.,0.,0.);
	glVertex2f(200.,0.);
	glVertex2f(300.,0.);
	glVertex2f(200,100);
	glEnd();


	glBegin(GL_LINES);
	glColor3f(1.,1.,1.);
	glLineWidth(2.);
	glVertex2f(200.,200.);
	glVertex2f(100.,0.);

	glColor3f(0.,0.,0.);
	glLineWidth(3.);
	glVertex2f(200.,200.);
	glVertex2f(100.,0.);



	glEnd();*/
	glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(pmat);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(vmat);

        glutSwapBuffers();

}

int mouse_button;
int mouse_state;

void mouse(int button, int state, int x, int y)
{
	mouse_button = button;mouse_state = state;
	long s=0x00000000;


		  s |= (button == GLUT_LEFT_BUTTON)   ? ((state == GLUT_DOWN) ? Trackball::LBUTTON_DOWN : Trackball::LBUTTON_UP) : 0;
       		  s |= (button == GLUT_MIDDLE_BUTTON) ? ((state == GLUT_DOWN) ? Trackball::MBUTTON_DOWN : Trackball::MBUTTON_UP) : 0;
		  s |= (button == GLUT_RIGHT_BUTTON)  ? ((state == GLUT_DOWN) ? Trackball::RBUTTON_DOWN : Trackball::RBUTTON_UP) : 0;

		int key_state = glutGetModifiers();
		s |= (key_state & GLUT_ACTIVE_CTRL)  ? Trackball::CTRL_DOWN  : 0;
		s |= (key_state & GLUT_ACTIVE_ALT)   ? Trackball::ALT_DOWN   : 0;
		s |= (key_state & GLUT_ACTIVE_SHIFT) ? Trackball::SHIFT_DOWN : 0;

		 if (s & Trackball::BUTTON_DOWN){//cerr<<x<<endl;
			  trackball.mouseDown(s, x, y);
		}

		if (s & Trackball::BUTTON_UP){
			 trackball.mouseUp(s, x, y);
		 }

	if(button==GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		 if (s & Trackball::BUTTON_DOWN){
			  trackball.mouseDown(s, x, y);
		}
	}
 	else if(button==GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		if (s & Trackball::BUTTON_UP){
			 trackball.mouseUp(s, x, y);
		 }
	}

        glutPostRedisplay();
}


void motion(int x, int y)
{
        trackball.mouseMotion(x, y);
        glutPostRedisplay();

}




void key(unsigned char key, int x, int y)
{
  switch (key) {
   
	case '1':
		type = 1;
		readfa->Generate(type);
		//readfa->Generate_Bar(type);
		//readfa->Generate_OrderBar(type);
		break;
	case '2':
		type = 2;
		readfa->Generate(type);
		//readfa->Generate_Bar(type);
		//readfa->Generate_OrderBar(type);
		break;
	case '3':
		type = 3;
		readfa->Generate(type);
		//readfa->Generate_Bar(type);
		//readfa->Generate_OrderBar(type);
		break;
	case 'a':
		if(mode==0)
			mode=1;
		else if(mode==1)
			mode=2;
		else if(mode==2)
			mode=0;
		//cerr<<mode<<endl;
		break;
	case 'b':
		if(order_type==0)
			order_type=1;
		else if(order_type==1)
			order_type=2;
		else if(order_type==2)
			order_type=0;
		//cerr<<mode<<endl;
		break;

    case '\033':
    case 'q':
    case 'Q':
      exit(0);
      break;
  }

	glutPostRedisplay();
}


void init(char *fileIn, char *fileOut)
{

 readfa = new Readdata();
 readfa->ProcessData(fileIn, fileOut);
 readfa->New(fileOut);
 readfa->SetColorDegree(100);cerr<<"a"<<endl;
 readfa->Removedata(-1);cerr<<"a"<<endl;
 readfa->ReOrder();cerr<<"a"<<endl;
 readfa->NewMultipleColors();cerr<<"a"<<endl;
 readfa->NewDivergingColors();cerr<<"a"<<endl;
 readfa->NewSingleColors();cerr<<"a"<<endl;

  Vector3f eye;
  eye[0] = 0; eye[1] = 0; eye[2] = 200;
  Vector3f center;
  center[0] = 0; center[1] = 0; center[2] = 1;

  trackball.setEye(eye);
  trackball.setFocus(center);
  trackball.setWindowSize(image_width,image_height);
  trackball.getMatrix().getValue(m);
}



int main(int argc, char** argv)
{
    //srand(12345);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInit(&argc, argv);

    glutInitWindowSize(1000,1000);
    glutInitWindowPosition(0, 0);

    glutCreateWindow("FA");
    glutReshapeFunc(reshape);

    init(argv[1],argv[2]);
    readfa->Generate(type);
   // readfa->Generate_Bar(type);
    //readfa->Generate_OrderBar(type);
    glutDisplayFunc(display);

    glutKeyboardFunc(key);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    glutMainLoop();

	
    return 0;
}
