/* 
 * Jian Chen (jichen@umbc.edu)
 *
 * Description: 
 *   v0: Nov 27 2013: file parser to read QDOT input and write to VTK format
 *   Usage: 
 *   TODO: ./main <input data directory> <input file name> <# of layers> <# of quatile (default 2> <output file directory> <zmin> <zmax>
 *                 or
 *   ./main <input data directory> <input file name> <# of layers> <# of quatile (default 2> <output file directory>
 *
 *   v1 (todo): add rendering
 *
 */

#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

#include <iostream>
#include <fstream>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>
#include <ctime>

#include "QDOTdataNorm.h"
#include "QDOTglyph.h"

#include "svType.h"
#include "svVectorMatrix.h"
#include "./jcUtil/MArray.h"
#include "./jcUtil/MGL.h"
#include "./jcUtil/MVectorMatrix.h"
#include "ivTrackball.h"
#include "svYellowBlue.h"
#include "dataSet.h"

using namespace __svl_lib;
using namespace std;

QDOTglyph *myDraw;
dataSet *myData;

Trackball trackball;
GLfloat m[16];
GLfloat m_zoom[16];
int image_width=1000;
int image_height=1000;

int mouse_x,mouse_y;

int num_page = 0;
int num_task = 0;
int num_mode = 0;
int squareorder[4][4]= {1,2,3,4,
			2,1,4,3,
			3,4,1,2,
			4,3,2,1};
int num_data[4] = {2,3,4,5};

bool end_task = false;
bool ifdone = false;
bool ifpause = false;
bool ifnext = false;
bool ifshowanswer = false;
bool end_singletask=false;

    GLdouble vmat[16], pmat[16];
    GLint    viewport[4];

int Direct = 1;
int Distribute = 2;
int Text = 3;
int Log = 4;

time_t nowtime;
char *namelog;

char *dataName;
char *dataDir;
char *colorName;
char *colorDir;
char *answername;
char *correctname;
float correctanswer;

vector<char> task14answer;
float useranswer;
float confidentlevel;
//float correctanswer;

ofstream answerfile;

time_t begin_time;
time_t end_time;
double wholetime = 0;
svVector3 l,r;

float movex,movey;
float mousex,mousey;
int mousebutton;

#ifndef M_PI
#define M_PI 3.1415926
#endif

void InitLight()
{
  //setting of lighting
  GLfloat mat_diffuse[] = { 1.0, 0.0, 0.0, 0.2 };
  GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 0.5 };
  GLfloat mat_shininess[] = { 10.0 };
  GLfloat light_position[] = { 100.0, 100.0, 100.0, 0.0 };
  GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_SMOOTH);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
}

void 
reshape(int width, int height)
{
	glViewport (0., 0., width, height);
  	glMatrixMode(GL_PROJECTION);
  	glLoadIdentity();
        gluPerspective(45, 1.0*width/height,0.1, 10000);
	glMatrixMode(GL_MODELVIEW);
  	glLoadIdentity();
        gluLookAt(0,0,15,
              0,0,0,
              0, 1, 0);

	image_width = width;
	image_height = height;
	myDraw->image_width = width;
	myDraw->image_height = height;

	InitLight();
}

double GetTime()
{
timeval tv;
gettimeofday (&tv, NULL);
return double (tv.tv_sec) + 0.000001 * tv.tv_usec;
}

void 
Draw_Page()
{
    float x, y;
    char str[40];
    char *tmps;
    x = viewport[2]*4.5/5.-50.;	
    y = viewport[3]*9./10.;
    if(movex>=x&&movex<=x+150.
	&&movey>=y-50.&&movey<=y)
	glColor3f(1.,1.,0.);
    else
   	 glColor3f(1.,1.,1.);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x,y);
    glVertex2f(x+150.,y);
    glVertex2f(x+150.,y-50.);
    glVertex2f(x,y-50.);
    glEnd();	
    tmps = strdup("next");
	glRasterPos2f(x+50., y-20.);
	sprintf(str,"%s",tmps);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);	
}

void 
Draw_Done()
{
    float x, y;
    char str[40];
    char *tmps;
    x = viewport[2]*4.5/5.-50.;	
    y = viewport[3]*9./10.;
    if(movex>=x&&movex<=x+150.
	&&movey>=y-50.&&movey<=y)
	glColor3f(1.,1.,0.);
    else
   	 glColor3f(1.,1.,1.);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x,y);
    glVertex2f(x+150.,y);
    glVertex2f(x+150.,y-50.);
    glVertex2f(x,y-50.);
    glEnd();	
    tmps = strdup("done");
	glRasterPos2f(x+50., y-20.);
	sprintf(str,"%s",tmps);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);	
}

void 
Draw_Label()
{
	    glGetDoublev(GL_MODELVIEW_MATRIX, vmat);
	    glGetDoublev(GL_PROJECTION_MATRIX, pmat);
	    glGetIntegerv(GL_VIEWPORT, viewport);

	    glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();
	    gluOrtho2D(0.0,(GLfloat) viewport[2], (GLfloat) viewport[3],0);
	    glMatrixMode(GL_MODELVIEW);
	    glLoadIdentity();
	glColor3f(1.,1.,1.);
	char str[40];
	char *input = strdup("Please input your answer.");
	sprintf(str,"%s",input);
	glRasterPos2f(image_width/3.,image_height/2.-image_height/18.);
	for(int i=0;i<strlen(str);i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
	}	

	glColor3f(0.,0.,0);
	glRasterPos2f(image_width/3.,image_height/2.);
	//cerr<<image_width/3.<<" "<<image_height/2.<<endl;
	for(int i=0;i<task14answer.size();i++)
	{
		//cerr<<task14answer[i]<<" ";
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, task14answer[i]);
	}	
	glColor3f(1.,1.,1.);
	glBegin(GL_QUADS);
	glVertex2f(image_width/3.,image_height/2.-image_height/40.);
	glVertex2f(image_width/3.,image_height/2.+image_height/40.);
	glVertex2f(image_width/3.+image_width/10.,image_height/2.+image_height/40.);
	glVertex2f(image_width/3.+image_width/10.,image_height/2.-image_height/40.);
	glEnd();

	    glMatrixMode(GL_PROJECTION);
	    glLoadMatrixd(pmat);
	    glMatrixMode(GL_MODELVIEW);
	    glLoadMatrixd(vmat);	
}

void 
Draw_End()
{
	    glGetDoublev(GL_MODELVIEW_MATRIX, vmat);
	    glGetDoublev(GL_PROJECTION_MATRIX, pmat);
	    glGetIntegerv(GL_VIEWPORT, viewport);

	    glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();
	    gluOrtho2D(0.0,(GLfloat) viewport[2], (GLfloat) viewport[3],0);
	    glMatrixMode(GL_MODELVIEW);
	    glLoadIdentity();
	
	glColor3f(1.,1.,1.);
    float x, y;
    char str[40];
    char *tmps = strdup("Thanks.");
    x = viewport[2]/2.;	
    y = viewport[3]/3.;
    glRasterPos2f(x, y);
    sprintf(str,"%s",tmps);
    for(int i=0;i<strlen(str);i++)
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);	
	    glMatrixMode(GL_PROJECTION);
	    glLoadMatrixd(pmat);
	    glMatrixMode(GL_MODELVIEW);
	    glLoadMatrixd(vmat);

}

void 
Draw_Time()
{
    float x, y;
    char str[40];
    char *tmps = strdup("pause");
    x = viewport[2]*4.5/5.-50.;	
    y = viewport[3]/10.;

    if(movex>=x&&movex<=x+150.
	&&movey>=y-50.&&movey<=y)
	glColor3f(1.,1.,0.);
    else
   	 glColor3f(1.,1.,1.);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x,y);
    glVertex2f(x+150.,y);
    glVertex2f(x+150.,y-50.);
    glVertex2f(x,y-50.);
    glEnd();	
    if(!ifpause)
    	tmps = strdup("pause");
    else
    	tmps = strdup("start");
	glRasterPos2f(x+50., y-20.);
	sprintf(str,"%s",tmps);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);	
}

void Draw_OTHER()
{	glColor3f(1.,1.,1.);
	char str[40];
	char *s;
	if(num_mode == 0)
	{
		s = strdup("Linear");
		sprintf(str,"%s",s);
	}
	else if(num_mode == 1)
	{
		s = strdup("Split");
		sprintf(str,"%s",s);
	}
	if(num_mode == 2)
	{
		s = strdup("Text");
		sprintf(str,"%s",s);
	}
	if(num_mode == 3)
	{
		s = strdup("Logarithm");
		sprintf(str,"%s",s);
	}
  	float  x = viewport[2]*4.5/5.-50.;	
    	float y = viewport[3]*9.5/10.;
	glRasterPos2f(x,y);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);	
}

void
Draw_Previous()
{
    float x, y;
    char str[40];
    char *tmps;
    x = viewport[2]*4.5/5.-50.;	
    y = viewport[3]*8.5/10.;
    if(movex>=x&&movex<=x+150.
	&&movey>=y-50.&&movey<=y)
	glColor3f(1.,1.,0.);
    else
   	 glColor3f(1.,1.,1.);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x,y);
    glVertex2f(x+150.,y);
    glVertex2f(x+150.,y-50.);
    glVertex2f(x,y-50.);
    glEnd();	
    tmps = strdup("previous");
	glRasterPos2f(x+50., y-20.);
	sprintf(str,"%s",tmps);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);	
}

void 
Draw_Correct()
{
    float x, y;
    char str[40];
    char *tmps;
    x = viewport[2]*4.5/5.-50.;	
    y = viewport[3]*7.5/10.;
    if(movex>=x&&movex<=x+150.
	&&movey>=y-50.&&movey<=y)
	glColor3f(1.,1.,0.);
    else
   	 glColor3f(1.,1.,1.);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x,y);
    glVertex2f(x+150.,y);
    glVertex2f(x+150.,y-50.);
    glVertex2f(x,y-50.);
    glEnd();	
    tmps = strdup("answer");
	glRasterPos2f(x+50., y-20.);
	sprintf(str,"%s",tmps);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);	
}

void
Draw_Answer()
{
	glColor3f(1.,1.,1.);
   float x = viewport[2]*4./5.-50.;	
   float y = viewport[3]*6.5/10.;
	char str[40];
	sprintf(str,"The correct answer is %.2e",correctanswer);
	glRasterPos2f(x,y);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
	if(num_task==2||num_task==1)	
	{
		sprintf(str,"Your answer is %.2e",myDraw->GetAnswer());
	glRasterPos2f(x,viewport[3]*6./10.);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
	}
}
void Draw_Level()
{
	    glGetDoublev(GL_MODELVIEW_MATRIX, vmat);
	    glGetDoublev(GL_PROJECTION_MATRIX, pmat);
	    glGetIntegerv(GL_VIEWPORT, viewport);

	    glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();
	    gluOrtho2D(0.0,(GLfloat) viewport[2], (GLfloat) viewport[3],0);
	    glMatrixMode(GL_MODELVIEW);
	    glLoadIdentity();
	glColor3f(1.,1.,1.);
	char str[100];
	char *s = strdup("Please select you confidence level for your answer. 1 is the lease confident and 7 is the most.");
	sprintf(str,"%s",s);
	//cerr<<str<<endl;

  	float x = viewport[2]/3.;	
    	float y = viewport[3]*2./3.;
	glRasterPos2f(x,y);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);	

	y = y + image_height/20.;
	for(int i=0;i<7;i++)
	{

		x = x + image_width/40.;
		if(mousex>=x-image_width/80.&&mousex<=x+image_width/80.
		&&mousey>=y-image_height/20.&&mousey<=y+image_height/20.)
		{
			glColor3f(1.,1.,0.);
			confidentlevel=i+1;//cerr<<confidentlevel<<endl;
		}
		else
			glColor3f(1.,1.,1.);
		sprintf(str,"%d",i+1);
		glRasterPos2f(x,y);
		for(int j=0;j<strlen(str);j++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[j]);
	}
	    glMatrixMode(GL_PROJECTION);
	    glLoadMatrixd(pmat);
	    glMatrixMode(GL_MODELVIEW);
	    glLoadMatrixd(vmat);
}

void
Draw_Task()
{	    glGetDoublev(GL_MODELVIEW_MATRIX, vmat);
	    glGetDoublev(GL_PROJECTION_MATRIX, pmat);
	    glGetIntegerv(GL_VIEWPORT, viewport);

	    glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();
	    gluOrtho2D(0.0,(GLfloat) viewport[2], (GLfloat) viewport[3],0);
	    glMatrixMode(GL_MODELVIEW);
	    glLoadIdentity();

	glColor3f(1.,1.,1.);
	char str[100];
	sprintf(str, "Task %d", num_task+1);
  	float x = viewport[2]/2.;	
    	float y = viewport[3]/2.;
	glRasterPos2f(x,y);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);	
    x = viewport[2]*4.5/5.-50.;	
    y = viewport[3]*9./10.;
    if(movex>=x&&movex<=x+150.
	&&movey>=y-50.&&movey<=y)
	glColor3f(1.,1.,0.);
    else
   	 glColor3f(1.,1.,1.);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x,y);
    glVertex2f(x+150.,y);
    glVertex2f(x+150.,y-50.);
    glVertex2f(x,y-50.);
    glEnd();	
    char *tmps = strdup("next");
	glRasterPos2f(x+50., y-20.);
	sprintf(str,"%s",tmps);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);	
	    glMatrixMode(GL_PROJECTION);
	    glLoadMatrixd(pmat);
	    glMatrixMode(GL_MODELVIEW);
	    glLoadMatrixd(vmat);	
}

void 
display(void)
{
        glClearColor(0.5,0.5,0.5,1.0);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	trackball.getMatrix().getValue(m);

	if(!end_task&&!end_singletask)
	{
	    glGetDoublev(GL_MODELVIEW_MATRIX, vmat);
	    glGetDoublev(GL_PROJECTION_MATRIX, pmat);
	    glGetIntegerv(GL_VIEWPORT, viewport);

	    glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();
	    gluOrtho2D(0.0,(GLfloat) viewport[2], (GLfloat) viewport[3],0);
	    glMatrixMode(GL_MODELVIEW);
	    glLoadIdentity();
		if(num_task==0)
		{
			myDraw->RenderTask1(num_data[squareorder[num_mode][num_page]-1],viewport[2],viewport[3]);
			if(!ifnext){Draw_Done();}
		}
		else if(num_task==1)
		{
			myDraw->RenderTask2_Sub1(num_page,viewport[2],viewport[3]);
		}
		else if(num_task==2)
		{
			myDraw->RenderTask2_Sub2(num_data[squareorder[num_mode][num_page]-1],viewport[2],viewport[3]);
		}
		else if(num_task==3)
		{
			myDraw->RenderTask3(num_data[squareorder[num_mode][num_page]-1],viewport[2],viewport[3]);
			if(!ifnext){Draw_Done();}
		}
		
		if(num_task==1||num_task==2)
		{
			Draw_Page();if(ifshowanswer)Draw_Answer();Draw_Correct();
		}
		else
		{
			if(ifnext){Draw_Page();if(ifshowanswer)Draw_Answer();Draw_Correct();}
		}
		Draw_Time();
		Draw_OTHER();
		myDraw->RenderQuestionN(viewport[2],viewport[3]);
	     glMatrixMode(GL_PROJECTION);
	    glLoadMatrixd(pmat);
	    glMatrixMode(GL_MODELVIEW);
	    glLoadMatrixd(vmat);	

		if(!ifdone&&!ifpause)
		{
			glPushMatrix();
			glMultMatrixf(m);

			myDraw->RenderBounding();
			myDraw->Render();

			glPopMatrix();
			myDraw->RenderMarker(m);
			if(num_mode+1==Text)
				myDraw->RenderText(m);
			trackball.getZoomMatrix().getValue(m_zoom);
			glPushMatrix();
			glTranslatef(l[0]*2,0.,0.);
			//glMultMatrixf(m_zoom);
			//glTranslatef(0.,0.,m_zoom[14]);
			
			glScalef(15./(15.-m_zoom[14]),1.,1.);
			glTranslatef(-l[0]*2,0.,0.);
			if(num_mode+1==Distribute)
				myDraw->RenderDistributeLegend();
			else if(num_mode+1==Direct)
				myDraw->RenderDirectLegend();
			else if(num_mode+1==Log)
				myDraw->RenderLogLegend();
			glPopMatrix();

		}
		else
		{	
			if(!ifpause)
			{Draw_Label();Draw_Level();}
		}
	}
	else if(end_task)
		Draw_End();
	else if(end_singletask)
		Draw_Task();

        glFlush();
        glutSwapBuffers();
}

void
init(int argc, char **argv)
{
  correctname = argv[7];
  answername = argv[6];
   myData=new dataSet(argv[1], argv[2]);
   myDraw = new QDOTglyph();
  dataName = argv[2];
  dataDir = argv[1];
  colorName = argv[5];
  colorDir = argv[4];
  myData->readColorFile(argv[4], argv[5]);	
  myDraw = new QDOTglyph();
  myDraw->SetData(myData->GetPos(),myData->GetDir(),myData->GetDen(),NULL,
		myData->GetNum(),myData->GetLeftBounding(),myData->GetRightBounding());
  l = myData->GetLeftBounding();
  r = myData->GetRightBounding();
  myDraw->SetMode(num_mode+1);
  myDraw->SetEndPos();
  myDraw->SetArrows();
  myDraw->SetLogName(argv[3]);

  svVector3 eye;
  svVector3 center;
  center[0]=0.;center[1]=0.;center[2]=0.;
  eye[0]=0.;eye[1]=0.;eye[2]=15;
  trackball.setEye(eye);
  trackball.setFocus(center);
  trackball.SetLogName(argv[3]);
  namelog = argv[3];

  myDraw->SetEye(eye);

  trackball.setWindowSize(image_width,image_height);
  trackball.getMatrix().getValue(m);  
  trackball.getZoomMatrix().getValue(m_zoom); 
  confidentlevel = -1; 

  wholetime = GetTime();
}

void 
mouse(int button, int state, int x, int y)
{
	long s=0x00000000;

    float nextx = viewport[2]*4.5/5.-50.;	
    float nexty = viewport[3]*9./10.;
    char name[20];
    char str[20];
    mousebutton = button;

		s |= (button == GLUT_LEFT_BUTTON)   ? ((state == GLUT_DOWN) ? Trackball::LBUTTON_DOWN : Trackball::LBUTTON_UP) : 0;
		  s |= (button == GLUT_RIGHT_BUTTON)  ? ((state == GLUT_DOWN) ? Trackball::RBUTTON_DOWN : Trackball::RBUTTON_UP) : 0;

		int key_state = glutGetModifiers();
		s |= (key_state & GLUT_ACTIVE_CTRL)  ? Trackball::CTRL_DOWN  : 0;
		s |= (key_state & GLUT_ACTIVE_ALT)   ? Trackball::ALT_DOWN   : 0;
		s |= (key_state & GLUT_ACTIVE_SHIFT) ? Trackball::SHIFT_DOWN : 0;

	if((button == GLUT_LEFT_BUTTON) && (state==GLUT_DOWN))
	{
		mousex = (float)x;mousey = (float)y;
		if(x>=nextx&&x<=nextx+150.&&y>=viewport[3]/10.-50&&y<=viewport[3]/10.)
		{
			if(ifpause==false)
			{
				ifpause=true;
				time(&end_time);
				wholetime = difftime(end_time,begin_time) + wholetime;
				//cerr<<GetTime()-wholetime<<endl;
			}
			else
			{
				ifpause=false;
				time(&begin_time);
			}
		}

		else if(x>=nextx&&x<=nextx+150.&&y<=viewport[3]*7.5/10.&&y>=viewport[3]*7.5/10.-50.)
		{
			ifshowanswer = true;
			correctanswer = myDraw->GetCorrectAnswer(num_data[squareorder[num_mode][num_page]-1],num_task+1);		
		}


		else if(x>=nextx&&x<=nextx+150.&&y<=nexty&&y>=nexty-50.)
		{
			if(end_singletask==true)end_singletask=false;

			else if(end_singletask==false)
			{
			if(num_task==1||num_task==2)
			{
				ifnext = true;
				ifdone = false;
			}
			else
			{
				if(ifdone == true&&task14answer.size()!=0&&confidentlevel!=-1)ifdone = false;
				else ifdone = true;
				if(ifnext == true&&task14answer.size()!=0&&confidentlevel!=-1)ifnext = false;
				else ifnext = true;
			}
			//ifdone = false;
			//ifnext = false;
			if(ifdone == false)
			{
				ifshowanswer = false;
				myDraw->GetCorrect(num_data[squareorder[num_mode][num_page]-1],num_task+1,correctname);
				num_page++;
				myDraw->questionnum++;//cerr<<num_mode<<endl;

				if(num_task==0||num_task==3)
				{
					for(int i=0;i<task14answer.size();i++)
						str[i] = task14answer[i];
					useranswer = atof(str);//cerr<<useranswer<<endl;
					answerfile.open(answername,std::ios_base::app);
					answerfile<<useranswer<<endl;
					answerfile.close();
					task14answer.clear();
				}
				else 
				{
					answerfile.open(answername,std::ios_base::app);
					answerfile<<myDraw->GetAnswer()<<endl;
					answerfile.close();		
				}

				if(num_page==1)
				{
					num_page = 0;
					num_mode ++;

					if(num_mode==4)
					{
						num_mode=0;
						num_task++;
						end_singletask=true;
					}
					myDraw->SetMode(num_mode+1);

				}

			
			if(num_task==4)
			{
				end_task = true;
				time(&end_time);
				wholetime = difftime(end_time,begin_time) + wholetime;
			}
			else
			{if(num_task==3&&num_page==0){ifnext=false;ifdone=false;}
				//cerr<<num_task<<" "<<num_page<<endl;
				if(num_task<4&&num_page<4)
					sprintf(name,"data%d_%d.txt",num_task+1,squareorder[num_mode][num_page]);
				myData->SetData(dataDir, name);
				//myData->readColorFile(colorDir,colorName);
 				 myDraw->SetData(myData->GetPos(),myData->GetDir(),myData->GetDen(),NULL,
						myData->GetNum(),myData->GetLeftBounding(),myData->GetRightBounding());
				myDraw->SetEndPos();
				myDraw->SetArrows();
				myDraw->ResetArrow();
				if(num_task==3)myDraw->SetTask3(num_data[squareorder[num_mode][num_page]-1]);
				svVector3 center;
  				center[0]=0.;center[1]=0.;center[2]=0.;
  				trackball.setFocus(center);
				trackball.reset();
				confidentlevel = -1;
				//myDraw->SetMode(num_mode);
			}
			//cerr<<num_page<<endl;
			time(&nowtime);
			char str[100];
			std::ofstream log(namelog, std::ios_base::app | std::ios_base::out);
			sprintf (str, "%s Click Next. Start Task %d Mode %d Question %d", ctime(&nowtime),num_task,num_mode,num_page);
			log<<str<<endl;
			log.close();}
			}
		}

		else
		{
				  if (s & Trackball::BUTTON_DOWN){
				    trackball.mouseDown(s, x, y);
				  }

				  if (s & Trackball::BUTTON_UP){
				    trackball.mouseUp(s, x, y);
			  	}
		}

	}
	else
	{
		if((button == GLUT_MIDDLE_BUTTON) && (state==GLUT_DOWN))
		if(num_task==1||num_task==2)
		{
			//cerr<<x<<" "<<y<<endl;
			//cerr<<num_task<<endl;
			myDraw->FindArrow(x, y, m);	
		
			if(!myDraw->select_task2)
			{
				  if (s & Trackball::BUTTON_DOWN){
				    trackball.mouseDown(s, x, y);
				  }

				  if (s & Trackball::BUTTON_UP){
				    trackball.mouseUp(s, x, y);
			  	}
			}
		}

		if((button == GLUT_RIGHT_BUTTON) && (state==GLUT_DOWN))
		{
			  if (s & Trackball::BUTTON_DOWN){
			    trackball.mouseDown(s, x, y);
			  }

			  if (s & Trackball::BUTTON_UP){
			    trackball.mouseUp(s, x, y);
		  	}
		}
	}
	glutPostRedisplay();
}

void motion(int x, int y)
{
	if(mousebutton!=GLUT_MIDDLE_BUTTON)
		trackball.mouseMotion(x, y);
	glutPostRedisplay();
}

void
movement(int x, int y)
{
    movex = (float)x;	
	movey = (float)y;	
	glutPostRedisplay();
}

void key(unsigned char key, int x, int y)
{
  switch (key) {
    case '0':
		if(ifdone)
		task14answer.push_back('0');
		break;
    case '1':
		if(ifdone)task14answer.push_back('1');
		break;	
    case '2':
		if(ifdone)task14answer.push_back('2');
		break;
    case '3':
		if(ifdone)task14answer.push_back('3');
		break;
    case '4':
		if(ifdone)task14answer.push_back('4');
		break;
    case '5':
		if(ifdone)task14answer.push_back('5');
		break;
    case '6':
		if(ifdone)task14answer.push_back('6');
		break;
    case '7':
		if(ifdone)task14answer.push_back('7');
		break;
    case '8':
		if(ifdone)task14answer.push_back('8');
		break;
    case '9':
		if(ifdone)task14answer.push_back('9');
		break;
    case 'e':
		if(ifdone)task14answer.push_back('e');
		break;
    case '+':
		if(ifdone)task14answer.push_back('+');
		break;
    case '-':
		if(ifdone)task14answer.push_back('-');
		break;
    case '.':
		if(ifdone)task14answer.push_back('.');
		break;
    case '\010':
		if(ifdone)
			if(task14answer.size()!=0)
			task14answer.pop_back();	
		break;
    case '\033':
      exit(0);
      break;
  }
  glutPostRedisplay();
}



int main(int argc, char **argv)
{
   time(&begin_time);
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowSize (2000, 1000);
   glutInitWindowPosition (0, 0);
   glutCreateWindow (argv[0]);
   init(argc, argv);
   glutReshapeFunc(reshape);
   glutDisplayFunc(display);
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
   glutPassiveMotionFunc(movement);
   glutKeyboardFunc(key);
   glutMainLoop();
   return 0;
}
