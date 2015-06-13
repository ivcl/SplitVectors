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
#include <iomanip>
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

#ifndef M_PI
#define M_PI 3.1415926
#endif

#define PI		M_PI
#define DTOR            (M_PI/180.0)
#define RTOD            (180.0 / M_PI) //57.2957795
#define CROSSPROD(p1,p2,p3) \
   p3.x = p1.y*p2.z - p1.z*p2.y; \
   p3.y = p1.z*p2.x - p1.x*p2.z; \
   p3.z = p1.x*p2.y - p1.y*p2.x

// bounding volume of the brain model
GLfloat Xminmax[2] = {50.774986, 182.695862};
GLfloat Yminmax[2] = {38.447567, 203.434158};
GLfloat Zminmax[2] = {5.271503, 135.014359};

int datafile = GL_FALSE;

GLint rot_x = 0, rot_y = 0, rot_z = 0;
GLfloat scale= 3.;

typedef struct {
    double x, y, z;
} XYZ;
typedef struct {
    double r, g, b;
} COLOUR;
typedef struct {
    unsigned char r, g, b, a;
} PIXELA;
typedef struct {
    XYZ vp;			/* View position           */
    XYZ vd;			/* View direction vector   */
    XYZ vu;			/* View up direction       */
    XYZ pr;			/* Point to rotate about   */
    double focallength;		/* Focal Length along vd   */
    double aperture;		/* Camera aperture         */
    double eyesep;		/* Eye separation          */
    int screenwidth, screenheight;
} CAMERA;
double dtheta = 1;
CAMERA camera;
XYZ origin = { 0.0, 0.0, 0.0 };

/*------------------------------------------------------------------------
 * Normalize:
 *   Input:
 *   Output:
 *   Description:
 *      handy math routine to calculate vector normal.
 */
static void Normalise(XYZ * p)
{
    double length;

    length = sqrt(p->x * p->x + p->y * p->y + p->z * p->z);
    if (length != 0) {
		p->x /= length;
		p->y /= length;
		p->z /= length;
    } else {
		p->x = 0;
		p->y = 0;
		p->z = 0;
    }
}

/*------------------------------------------------------------------------
 *  initCamera
 *    Input:
 *    Output:
 *    Description:
 *       camera setup
 *       FOV = 60;
 *       Focal_length = viewing distance = 460.1mm on the small 24" display
 *  
 */
static void initCamera()
{
    camera.aperture = 60;
    camera.focallength = 460.1;
    camera.eyesep = camera.focallength / 60;  // for comfortable viewing
                                              // the denominator can be
                                              // any number between 20 and 30.
    camera.pr = origin;

    camera.vp.x = 0;
    camera.vp.y = 0;
    camera.vp.z = (Zminmax[0]+Zminmax[1]) * 6.0;
    camera.vd.x = -camera.vp.x;
    camera.vd.y = -camera.vp.y;
    camera.vd.z = -camera.vp.z;

    camera.vu.x = 0;
    camera.vu.y = 1;
    camera.vu.z = 0;


    camera.screenwidth = 1920;
    camera.screenheight = 1080;
}

QDOTglyph *myDraw;
dataSet *myData;

Trackball trackball;
GLfloat m[16];
int image_width=1000;
int image_height=1000;

int mouse_x,mouse_y;

int num_page = -1;
int num_task = 0;
int num_mode = 0;
int squareorder[4][4]= {1,2,3,4,
			2,1,4,3,
			3,4,1,2,
			4,3,2,1};
int num_data[8];
int mode_order[4];//0,1,3,2 split vector linear log text


bool ifpause = false;
int ifnext = 0;
bool ifshowanswer = false;
bool ifconfident = false;
bool ifselectanswer = false;

bool ifstereo;

    GLdouble vmat[16], pmat[16];
    GLint    viewport[4];

    GLdouble vmat_left[16], pmat_left[16];
    GLint    viewport_left[4];

    GLdouble vmat_right[16], pmat_right[16];
    GLint    viewport_right[4];

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
char *timename;
char *confidentname;

int datacount = -1;
int dataorder[32];
int binaryanswer[64];
int stereoorder[8];

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
int mousestate;

float rotatefactor[4];
float zoomfactor;
float panfactor[4];
char keystate;

double timefornext;

int page_order[196]=
{0,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,  
1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,  
1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,    
1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,  
0,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,  
1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,    
1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,  
1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,  
0,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,  
1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,    
1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,  
1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,    
3
};
int page_count = 0;
bool page_next = false;

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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, (GLsizei) width, (GLsizei) height);
    camera.screenwidth = width;
    camera.screenheight = height;

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
ReadFile()
{
	int n;
	ifstream infile;
	infile.open("../data/dataorder");
	for(int i=0;i<32;i++)
	{
		infile>>n;
		if(n%2==0)
			dataorder[i] = ((n/2)/4+1)*10 + ((n/2)%4+1);
		else
			dataorder[i] = ((n/2)/4+5)*10 + ((n/2)%4+1);
	}
	infile.close();



	myData->SetTask1("../data/task1_data","pointorder");
	myData->SetTask2("../data/task2_data","pointorder");
	myData->SetTask3("../data/task3_data","pointorder");
	myData->SetTask4("../data/task4_data","pointorder");

	infile.open("../data/binaryorder_mono");
	int j=0;
	for(int i=0;i<64;i++)
	{
		if(dataorder[j]<50)
		{	
			infile>>n;
			binaryanswer[i] = n%2;
			
		}

		j++;
		if(i==32)j=0;
	}//cerr<<endl;
	infile.close();

	infile.open("../data/binaryorder_stereo");
	j=0;
	for(int i=0;i<64;i++)
	{
		if(dataorder[j]>50)
		{
			infile>>n;
			binaryanswer[i] = n%2;
		}
		j++;
		if(i==32)j=0;
	}//cerr<<endl;
	infile.close();

	//for(int i=0;i<64;i++)
	//	cerr<<binaryanswer[i]<<" ";cerr<<endl;
}
void 
Draw_Page()
{
    float x, y;
    char str[40];
    char *tmps;
    x = viewport[2]*4.5/5.-50.;	
    y = viewport[3]*9./10.;
	glColor3f(0.,0.,0.);
    tmps = strdup("next");
	glRasterPos2f(x+50., y-20.);
	sprintf(str,"%s",tmps);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);	

    if(movex>=x&&movex<=x+150.
	&&movey>=y-50.&&movey<=y)
	glColor3f(1.,1.,1.);
    else
   	 glColor3f(0.5,0.5,0.5);

    glBegin(GL_QUADS);
    glVertex2f(x+5.,y-5.);
    glVertex2f(x+150.-5.,y-5.);
    glVertex2f(x+150.-5.,y-50.+5.);
    glVertex2f(x+5.,y-50.+5.);
    glEnd();	

    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
    glVertex2f(x+5.,y);
    glVertex2f(x+150.,y);
    glVertex2f(x+150,y-50.+5.);
    glVertex2f(x+5.,y-50.+5.);
    glEnd();	

    glColor3f(0.8,0.8,0.8);
    glBegin(GL_QUADS);
    glVertex2f(x,y);
    glVertex2f(x+150.,y);
    glVertex2f(x+150.,y-50.);
    glVertex2f(x,y-50.);
    glEnd();

}

void 
Draw_Done()
{
    float x, y;
    char str[40];
    char *tmps;
    x = viewport[2]*4.5/5.-50.;	
    y = viewport[3]*9./10.;

	glColor3f(0.,0.,0.);
    tmps = strdup("done");
	glRasterPos2f(x+50., y-20.);
	sprintf(str,"%s",tmps);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);	

    if(movex>=x&&movex<=x+150.
	&&movey>=y-50.&&movey<=y)
	glColor3f(1.,1.,1.);
    else
   	 glColor3f(0.5,0.5,0.5);

    glBegin(GL_QUADS);
    glVertex2f(x+5.,y-5.);
    glVertex2f(x+150.-5.,y-5.);
    glVertex2f(x+150.-5.,y-50.+5.);
    glVertex2f(x+5.,y-50.+5.);
    glEnd();	

    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
    glVertex2f(x+5.,y);
    glVertex2f(x+150.,y);
    glVertex2f(x+150,y-50.+5.);
    glVertex2f(x+5.,y-50.+5.);
    glEnd();	

    glColor3f(0.8,0.8,0.8);
    glBegin(GL_QUADS);
    glVertex2f(x,y);
    glVertex2f(x+150.,y);
    glVertex2f(x+150.,y-50.);
    glVertex2f(x,y-50.);
    glEnd();
}

void 
Draw_Label()
{

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
	char stra[100];
	//cerr<<image_width/3.<<" "<<image_height/2.<<endl;
	for(int i=0;i<task14answer.size();i++)
	{
		//cerr<<task14answer[i]<<" ";
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, task14answer[i]);
		stra[i] = task14answer[i];
	}	

	if(task14answer.size()!=0)
	{	ifselectanswer=true;
		useranswer = atof(stra);
	}
	else
		useranswer = 0.;
	glColor3f(1.,1.,1.);
	glBegin(GL_QUADS);
	glVertex2f(image_width/3.,image_height/2.-image_height/60.);
	glVertex2f(image_width/3.,image_height/2.+image_height/120.);
	glVertex2f(image_width/3.+image_width/10.,image_height/2.+image_height/120.);
	glVertex2f(image_width/3.+image_width/10.,image_height/2.-image_height/60.);
	glEnd();
}

void 
Draw_End()
{

	
	glColor3f(1.,1.,1.);
    float x, y;
    char str[40];
    char *tmps = strdup("Thanks.");
    x = viewport[2]/2.;	
    y = viewport[3]/2.;
    glRasterPos2f(x, y);
    sprintf(str,"%s",tmps);
    for(int i=0;i<strlen(str);i++)
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);	
    glRasterPos2f(x, y+100.);
    sprintf(str,"All the tasks are done.");
    for(int i=0;i<strlen(str);i++)
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);	


	x = viewport[2]/2.;	
	y = y + 200.;
    glColor3f(0.,0.,0.);
    tmps = strdup("Exit");
	glRasterPos2f(x+50., y-20.);
	sprintf(str,"%s",tmps);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);	

    if(movex>=x&&movex<=x+150.
	&&movey>=y-50.&&movey<=y)
	glColor3f(1.,1.,1.);
    else
   	 glColor3f(0.5,0.5,0.5);

    glBegin(GL_QUADS);
    glVertex2f(x+5.,y-5.);
    glVertex2f(x+150.-5.,y-5.);
    glVertex2f(x+150.-5.,y-50.+5.);
    glVertex2f(x+5.,y-50.+5.);
    glEnd();	

    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
    glVertex2f(x+5.,y);
    glVertex2f(x+150.,y);
    glVertex2f(x+150,y-50.+5.);
    glVertex2f(x+5.,y-50.+5.);
    glEnd();	

    glColor3f(0.8,0.8,0.8);
    glBegin(GL_QUADS);
    glVertex2f(x,y);
    glVertex2f(x+150.,y);
    glVertex2f(x+150.,y-50.);
    glVertex2f(x,y-50.);
    glEnd();

    if(mousex>=x&&mousex<=x+150.
	&&mousey>=y-50.&&mousey<=y)
	exit(0);
}

void 
Draw_Time()
{
    float x, y;
    char str[40];
    char *tmps = strdup("pause");
    x = viewport[2]*4.5/5.-50.;	
    y = viewport[3]/10.;

	glColor3f(0.,0.,0.);
    if(!ifpause)
    	tmps = strdup("pause");
    else
    	tmps = strdup("resume");
	glRasterPos2f(x+50., y-20.);
	sprintf(str,"%s",tmps);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);	

    if(movex>=x&&movex<=x+150.
	&&movey>=y-50.&&movey<=y)
	glColor3f(1.,1.,1.);
    else
   	 glColor3f(0.5,0.5,0.5);

    glBegin(GL_QUADS);
    glVertex2f(x+5.,y-5.);
    glVertex2f(x+150.-5.,y-5.);
    glVertex2f(x+150.-5.,y-50.+5.);
    glVertex2f(x+5.,y-50.+5.);
    glEnd();	

    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
    glVertex2f(x+5.,y);
    glVertex2f(x+150.,y);
    glVertex2f(x+150,y-50.+5.);
    glVertex2f(x+5.,y-50.+5.);
    glEnd();	

    glColor3f(0.8,0.8,0.8);
    glBegin(GL_QUADS);
    glVertex2f(x,y);
    glVertex2f(x+150.,y);
    glVertex2f(x+150.,y-50.);
    glVertex2f(x,y-50.);
    glEnd();

}
void
Draw_Logvalue()
{
	float a;
	char str[50];
    	float x = viewport[2]*4.5/5.-50.;	
    	float y = viewport[3]/10.+20.;
	for(int i=1;i<10;i++)
	{
		a = log10(i);
		sprintf(str,"Log10(%d) = %.2f",i,a);
		glRasterPos2f(x,y+i*20);
		for(int i=0;i<strlen(str);i++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);			
	}
}
void Draw_OTHER()
{	glColor3f(1.,1.,1.);
	char str[40];
	char *s;
	if(mode_order[num_mode] == 0)
	{
		s = strdup("Encoding Method: Linear");
		sprintf(str,"%s",s);
	}
	else if(mode_order[num_mode] == 1)
	{
		s = strdup("Encoding Method: SplitVector");
		sprintf(str,"%s",s);
	}
	if(mode_order[num_mode] == 2)
	{
		s = strdup("Encoding Method: Text");
		sprintf(str,"%s",s);
	}
	if(mode_order[num_mode] == 3)
	{
		s = strdup("Encoding Method: Logarithm");
		sprintf(str,"%s",s);
	}
  	float  x = viewport[2]*3.7/5.;	
    	float  y = viewport[3]- viewport[3]/20.;
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
    x = viewport[2]/20.-50.;	
    y = viewport[3]*9./10.;

	glColor3f(0.,0.,0.);
    tmps = strdup("previous");
	glRasterPos2f(x+50., y-20.);
	sprintf(str,"%s",tmps);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);	

    if(movex>=x&&movex<=x+150.
	&&movey>=y-50.&&movey<=y)
	glColor3f(1.,1.,1.);
    else
   	 glColor3f(0.5,0.5,0.5);

    glBegin(GL_QUADS);
    glVertex2f(x+5.,y-5.);
    glVertex2f(x+150.-5.,y-5.);
    glVertex2f(x+150.-5.,y-50.+5.);
    glVertex2f(x+5.,y-50.+5.);
    glEnd();	

    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
    glVertex2f(x+5.,y);
    glVertex2f(x+150.,y);
    glVertex2f(x+150,y-50.+5.);
    glVertex2f(x+5.,y-50.+5.);
    glEnd();	

    glColor3f(0.8,0.8,0.8);
    glBegin(GL_QUADS);
    glVertex2f(x,y);
    glVertex2f(x+150.,y);
    glVertex2f(x+150.,y-50.);
    glVertex2f(x,y-50.);
    glEnd();	

}

void 
Draw_Correct()
{
    float x, y;
    char str[40];
    char *tmps;
    x = viewport[2]*4.5/5.-50.;	
    y = viewport[3]*7.5/10.;

	glColor3f(0.,0.,0.);
    tmps = strdup("answer");
	glRasterPos2f(x+50., y-20.);
	sprintf(str,"%s",tmps);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);	

    if(movex>=x&&movex<=x+150.
	&&movey>=y-50.&&movey<=y)
	glColor3f(1.,1.,1.);
    else
   	 glColor3f(0.5,0.5,0.5);

    glBegin(GL_QUADS);
    glVertex2f(x+5.,y-5.);
    glVertex2f(x+150.-5.,y-5.);
    glVertex2f(x+150.-5.,y-50.+5.);
    glVertex2f(x+5.,y-50.+5.);
    glEnd();	

    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
    glVertex2f(x+5.,y);
    glVertex2f(x+150.,y);
    glVertex2f(x+150,y-50.+5.);
    glVertex2f(x+5.,y-50.+5.);
    glEnd();	

    glColor3f(0.8,0.8,0.8);
    glBegin(GL_QUADS);
    glVertex2f(x,y);
    glVertex2f(x+150.,y);
    glVertex2f(x+150.,y-50.);
    glVertex2f(x,y-50.);
    glEnd();

}

void
Draw_Answer()
{
	glColor3f(1.,1.,0.);
   float x = viewport[2]*2.5/5.-50.;	
   float y = viewport[3]/2.;
	char str[40];
	if(num_task==0)
		correctanswer = myDraw->GetCorrectAnswer(num_data[num_page],0,0);
	else if(num_task==1)
		correctanswer = myDraw->GetCorrectAnswer(num_data[num_page],0,1);
	else if(num_task==2)
		correctanswer = myDraw->GetCorrectAnswer(binaryanswer[datacount-8],1-binaryanswer[datacount-8],2);
	else if(num_task==3)
		correctanswer = myDraw->GetCorrectAnswer(binaryanswer[datacount-12],1-binaryanswer[datacount-12],3);
	if(num_task==0||num_task==1)
		sprintf(str,"The correct answer is %.2e",correctanswer);
	else
	{
		if(correctanswer==0)
			sprintf(str,"The correct answer is A");
		else
			sprintf(str,"The correct answer is B");
	}
	glRasterPos2f(x,y);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
}
void Draw_Level()
{

	glColor3f(1.,1.,1.);
	char str[100];
	char *s = strdup("Please select your confidence level for this task.");
	sprintf(str,"%s",s);
	//cerr<<str<<endl;

  	float x = viewport[2]/3.;
    	float y = viewport[3]/2.+viewport[3]/20.;
	glRasterPos2f(x,y);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);	

	s = strdup("least");
	sprintf(str,"%s",s);
	glRasterPos2f(x+ image_width/50.,y+ image_height/10.);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);	

	y = y + image_height/20.;
	for(int i=0;i<7;i++)
	{
		x = x + image_width/40.;
		if(mousex>=x-image_width/160.&&mousex<=x+image_width/80.
		&&mousey>=y-image_height/80.&&mousey<=y+image_height/40.)
		{
			ifconfident = true;
			confidentlevel=i+1;//cerr<<confidentlevel<<endl;
		}

		glColor3f(0.,0.,0.);
		sprintf(str,"%d",i+1);
		glRasterPos2f(x,y);
		for(int j=0;j<strlen(str);j++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[j]);

		if(confidentlevel == i+1)
			glColor3f(1.,1.,1.);
		else if(movex>=x-image_width/160.&&movex<=x+image_width/80.
		&&movey>=y-image_height/80.&&movey<=y+image_height/40.)
			glColor3f(1.,1.,1.);
		else
		   	glColor3f(0.5,0.5,0.5);

		    glBegin(GL_QUADS);
			glVertex2f(x-image_width/160.+5.,y+image_height/80.-5.);
			glVertex2f(x+image_width/80.-5.,y+image_height/80.-5.);
			glVertex2f(x+image_width/80.-5.,y-image_height/40.+5.);
			glVertex2f(x-image_width/160.+5.,y-image_height/40.+5.);
		    glEnd();	

		    glColor3f(0.2,0.2,0.2);
		    glBegin(GL_QUADS);
			glVertex2f(x-image_width/160.+5.,y+image_height/80.);
			glVertex2f(x+image_width/80.,y+image_height/80.);
			glVertex2f(x+image_width/80.,y-image_height/40.+5.);
			glVertex2f(x-image_width/160.+5.,y-image_height/40.+5.);
		    glEnd();	

		    glColor3f(0.8,0.8,0.8);
		glBegin(GL_QUADS);
		glVertex2f(x-image_width/160.,y+image_height/80.);
		glVertex2f(x+image_width/80.,y+image_height/80.);
		glVertex2f(x+image_width/80.,y-image_height/40.);
		glVertex2f(x-image_width/160.,y-image_height/40.);
		glEnd();

	}
	glColor3f(1.,1.,1.);
	s = strdup("most");
	sprintf(str,"%s",s);
	glRasterPos2f(x-+ image_width/150.,viewport[3]/2.+viewport[3]/20.+ image_height/10.);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);	

}

void
Draw_Task()
{
	glColor3f(1.,1.,1.);
	char str[200];

	if(num_task==0)
		sprintf(str, "Task 1. What is the magnitude at point A?");
	else if(num_task==1)
		sprintf(str, "Task 2. What is the ratio between the vectors at points A and B?");
	else if(num_task==2)
		sprintf(str, "Task 3. Which magnitude is larger, point A or point B?");
	else if(num_task==3)
		sprintf(str, "Task 4. Which angle is larger,  between the two angles marked by two vectors at each of A and B?");

  	float x = viewport[2]/3.;	
    	float y = viewport[3]/2.;
	glRasterPos2f(x,y);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);	
    x = viewport[2]*4.5/5.-50.;	
    y = viewport[3]*9./10.;
    char *tmps;
    x = viewport[2]*4.5/5.-50.;	
    y = viewport[3]*9./10.;
	glColor3f(0.,0.,0.);
    tmps = strdup("next");
	glRasterPos2f(x+50., y-20.);
	sprintf(str,"%s",tmps);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);	

    if(movex>=x&&movex<=x+150.
	&&movey>=y-50.&&movey<=y)
	glColor3f(1.,1.,1.);
    else
   	 glColor3f(0.5,0.5,0.5);

    glBegin(GL_QUADS);
    glVertex2f(x+5.,y-5.);
    glVertex2f(x+150.-5.,y-5.);
    glVertex2f(x+150.-5.,y-50.+5.);
    glVertex2f(x+5.,y-50.+5.);
    glEnd();	

    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
    glVertex2f(x+5.,y);
    glVertex2f(x+150.,y);
    glVertex2f(x+150,y-50.+5.);
    glVertex2f(x+5.,y-50.+5.);
    glEnd();	

    glColor3f(0.8,0.8,0.8);
    glBegin(GL_QUADS);
    glVertex2f(x,y);
    glVertex2f(x+150.,y);
    glVertex2f(x+150.,y-50.);
    glVertex2f(x,y-50.);
    glEnd();
	
}

void
DrawBinary()
{

	glColor3f(1.,1.,1.);
	char str[40];
	char *input = strdup("Please select your answer.");
	sprintf(str,"%s",input);
	glRasterPos2f(image_width*4/5.,image_height/2.-image_height/18.);
	for(int i=0;i<strlen(str);i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
	}	

	float x,y;
	x = image_width*4.5/5.;
	y = image_height/2.-image_height/36.;
	for(int i=0;i<3;i++)
	{
		if(mousex>=x&&mousex<=x+image_width/30.
		&&mousey>=y&&mousey<=y+image_height/40.)
		{
			ifselectanswer = true;
			useranswer = i;
		}
		glColor3f(0.,0.,0.);
		if(i==0)
			sprintf(str,"A");
		else if(i==1)
			sprintf(str,"B");
		else if(i==2)
			sprintf(str,"Same");
		if(i!=2)
		glRasterPos2f((x + x+image_width/30.)/2.-image_width/280.,(y+ y+image_height/40.)/2.+image_height/160.);
		else
		glRasterPos2f(x+image_width/280.,(y+ y+image_height/40.)/2.+image_height/160.);
		for(int j=0;j<strlen(str);j++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[j]);	

		if(useranswer == i&&ifselectanswer==true)
			glColor3f(1.,1.,1.);
		else if(movex>=x&&movex<=x+image_width/30.
			&&movey>=y&&movey<=y+image_height/40.)
			glColor3f(1.,1.,1.);
		else
			glColor3f(0.5,0.5,0.5);

		    glBegin(GL_QUADS);
			glVertex2f(x+5.,y+5.);
			glVertex2f(x+5.,y+image_height/40.-5.);
			glVertex2f(x+image_width/30.-5.,y+image_height/40.-5.);
			glVertex2f(x+image_width/30.-5.,y+5.);
		    glEnd();	

		    glColor3f(0.2,0.2,0.2);
		    glBegin(GL_QUADS);
			glVertex2f(x+5.,y+5.);
			glVertex2f(x+5.,y+image_height/40.);
			glVertex2f(x+image_width/30.,y+image_height/40.);
			glVertex2f(x+image_width/30.,y+5.);
		    glEnd();	

		    glColor3f(0.8,0.8,0.8);
		glBegin(GL_QUADS);
		glVertex2f(x,y);
		glVertex2f(x,y+image_height/40.);
		glVertex2f(x+image_width/30.,y+image_height/40.);
		glVertex2f(x+image_width/30.,y);
		glEnd();
		y = y + image_height/25.;
	}
	
}

void
Draw_2D()
{
	if(ifnext!=3&&ifnext!=0)
	{
		if(num_task==0)
		{
			if(ifnext==1){Draw_Done();if(mode_order[num_mode]==1)myDraw->RenderDistributeLegend();}
			myDraw->RenderTask1(myData->GetTask1((dataorder[datacount]/10 - 1)* 4+dataorder[datacount]%10-1),viewport[2],viewport[3]);
		}
		else if(num_task==1)
		{
			myDraw->RenderTask2(viewport[2],viewport[3]);
			if(ifnext==1){Draw_Done();if(mode_order[num_mode]==1)myDraw->RenderDistributeLegend();}
		}
		else if(num_task==2)
		{
			myDraw->RenderTask3(binaryanswer[datacount],1-binaryanswer[datacount],viewport[2],viewport[3]);
			if(ifnext==1){Draw_Done();if(mode_order[num_mode]==1)myDraw->RenderDistributeLegend();}
		}
		else if(num_task==3)
		{
			myDraw->RenderTask4(binaryanswer[datacount+32],1-binaryanswer[datacount+32],viewport[2],viewport[3]);
			if(ifnext==1){Draw_Done();if(mode_order[num_mode]==1)myDraw->RenderDistributeLegend();}
		}
		
		if((num_task==0||num_task==3)&&ifnext==2)
		{
			{Draw_Page();}
		}
		else if((num_task==1||num_task==2)&&ifnext==2)
		{
			{Draw_Page();}
		}


		Draw_Time();
		Draw_OTHER();
		myDraw->RenderQuestionN(viewport[2],viewport[3]);
		if(mode_order[num_mode]+1==Log)
			Draw_Logvalue();
		if(ifnext==1&&!ifpause)
		{
			if(num_task==2||num_task==3)
				DrawBinary();
		}
		else if(ifnext==2)
		{	
			if(!ifpause)
			{
				if(num_task==0||num_task==1)
					Draw_Label();
				Draw_Level();
			}
		}
	}
	else if(ifnext==3)
		Draw_End();
	else if(ifnext==0)
	{Draw_Time();	Draw_Task();}

}

void
Draw_Everything()
{
        glClearColor(0.5,0.5,0.5,1.0);
	//glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	trackball.getMatrix().getValue(m);

	if(ifnext!=3&&ifnext!=0)
	{
		if(ifnext==1&&!ifpause)
		{
			glPushMatrix();
			glMultMatrixf(m);
			myDraw->RenderBounding();
			//myDraw->Render();
			myDraw->RenderCylindernocolor();
			myDraw->RenderLines();
			glPopMatrix();
			myDraw->RenderMarker(m);
			if(num_task==3)
				myDraw->RenderTask4Marker(m);
			if(mode_order[num_mode]+1==Text)
				myDraw->RenderText(m);

		}

	}

       // glutSwapBuffers();
}

void 
display(void)
{

	glEnable(GL_DEPTH_TEST);

		glDrawBuffer(GL_BACK_LEFT);
   		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    		glDrawBuffer(GL_BACK_RIGHT);
    		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(ifstereo&&ifnext==1)
    	{	

  	    //glViewport(0, 0, (GLsizei) image_width, (GLsizei) image_height);
	    XYZ r;
	    double ratio, radians, wd2, ndfl;
	    double left, right, top, bottom;

	    /* Clip to avoid extreme stereo */
	    double near = camera.focallength / 5;
	    double far = (Yminmax[1]-Yminmax[2]) * 40.0;

	    /* Misc stuff */
	    ratio = camera.screenwidth / (double) camera.screenheight;
	    radians = DTOR * camera.aperture / 2.;
	    wd2 = near * tan(radians);
	    ndfl = near / camera.focallength;

	    /* Derive the two eye positions */
	    CROSSPROD(camera.vd, camera.vu, r);
	    Normalise(&r);
	    r.x *= camera.eyesep / 2.0;
	    r.y *= camera.eyesep / 2.0;
	    r.z *= camera.eyesep / 2.0;
		  svVector3 eye;
 		 eye[0]=0.;eye[1]=0.;eye[2]=near*2;
		  myDraw->SetEye(eye);
	    // left eye
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		left = -ratio * wd2 - 0.5 * camera.eyesep * ndfl;
		right = ratio * wd2 - 0.5 * camera.eyesep * ndfl;
		top = wd2;
		bottom = -wd2;
		glFrustum(left, right, bottom, top, near*2, far);

		glMatrixMode(GL_MODELVIEW);
		glDrawBuffer(GL_BACK_RIGHT);
		glLoadIdentity();
		gluLookAt(camera.vp.x + r.x, camera.vp.y + r.y, camera.vp.z + r.z,
			camera.vp.x + r.x + camera.vd.x,
			camera.vp.y + r.y + camera.vd.y,
			camera.vp.z + r.z + camera.vd.z,
			camera.vu.x, camera.vu.y, camera.vu.z);

		glPushMatrix();
		glTranslatef(0.,0.,-800);
		glScalef(220,220,220);
		Draw_Everything();
		glPopMatrix();

	    // right eye
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		left = -ratio * wd2 + 0.5 * camera.eyesep * ndfl;
		right = ratio * wd2 + 0.5 * camera.eyesep * ndfl;
		top = wd2;
		bottom = -wd2;
		glFrustum(left, right, bottom, top, near*2, far);

		glMatrixMode(GL_MODELVIEW);
		glDrawBuffer(GL_BACK_LEFT);
		glLoadIdentity();
		gluLookAt(camera.vp.x - r.x, camera.vp.y - r.y, camera.vp.z - r.z,
					camera.vp.x - r.x + camera.vd.x,
					camera.vp.y - r.y + camera.vd.y,
					camera.vp.z - r.z + camera.vd.z,
					camera.vu.x, camera.vu.y, camera.vu.z);

		glPushMatrix();
		glTranslatef(0.,0.,-800);
		glScalef(220,220,220);
		Draw_Everything();
		glPopMatrix();
	}
	else
	{
		glDrawBuffer(GL_BACK);
	  	glMatrixMode(GL_PROJECTION);
	  	glLoadIdentity();
		gluPerspective(45, 1.0*image_width/image_height,0.1, 10000);
		glMatrixMode(GL_MODELVIEW);
	  	glLoadIdentity();
		gluLookAt(0,0,5.5,
		      0,0,0,
		      0, 1, 0);
		  svVector3 eye;
 		 eye[0]=0.;eye[1]=0.;eye[2]=5.5;
		  myDraw->SetEye(eye);
		glPushMatrix();
		Draw_Everything();
		//Draw_2D();
		glPopMatrix();
	}
	
			glDrawBuffer(GL_BACK);
   	 glViewport(0, 0, (GLsizei) image_width, (GLsizei) image_height);
	 glMatrixMode(GL_PROJECTION);
	 glLoadIdentity();
	 gluOrtho2D(0.0,(GLfloat) image_width,(GLfloat) image_height,0.);
	 glMatrixMode(GL_MODELVIEW);
	 glLoadIdentity();
	 viewport[2] = image_width;
	 viewport[3] = image_height;
	 Draw_2D();

	glFlush();
       	glutSwapBuffers();
}

void
init(int argc, char **argv)
{
   myData=new dataSet();
   myDraw = new QDOTglyph();

  correctname = argv[4];
  answername = argv[3];
  timename = argv[5];
  confidentname = argv[6];
  //dataName = argv[2];
  dataDir = argv[1];
  //colorName = argv[5];
  //colorDir = argv[4];

	int block = atoi(argv[7]);
	if(block==1)
	{
		mode_order[0] = 1;
		mode_order[1] = 0;
		mode_order[2] = 3;
		mode_order[3] = 2;
	}	
	else if(block==2)
	{
		mode_order[0] = 0;
		mode_order[1] = 1;
		mode_order[2] = 2;
		mode_order[3] = 3;
	}
	else if(block==3)
	{
		mode_order[0] = 3;
		mode_order[1] = 2;
		mode_order[2] = 1;
		mode_order[3] = 0;
	}
	else if(block==4)
	{
		mode_order[0] = 2;
		mode_order[1] = 3;
		mode_order[2] = 0;
		mode_order[3] = 1;
	}

  myDraw = new QDOTglyph();
  myDraw->SetMode(mode_order[num_mode]+1);
  ReadFile();

  svVector3 eye;
  svVector3 center;
  center[0]=0.;center[1]=0.;center[2]=0.;
  eye[0]=0.;eye[1]=0.;eye[2]=5.5;
  trackball.setEye(eye);
  trackball.setFocus(center);
  trackball.SetLogName(argv[2]);
  namelog = argv[2];

  myDraw->SetEye(eye);

  trackball.setWindowSize(image_width,image_height);
  trackball.getMatrix().getValue(m);  
  confidentlevel = -1; 

  begin_time = GetTime();
  ifnext = 0;
  ifstereo = false;
}

void
RecordLog()
{
	long double time = GetTime();
	int task;
	int mode;
	int stereo;
	int confidentoutput;
	if(ifnext==0)
	{
		task = -1;
		mode = -1;
		stereo = -1;
	}
	else
	{
		task = num_task;
		mode = mode_order[num_mode];
		stereo = ifstereo;
	}
	if(ifnext==1)
	{
		trackball.GetRotateFactor(rotatefactor);
		trackball.GetZoomFactor(zoomfactor);
		trackball.GetPanFactor(panfactor);
	}	
	else
	{
		rotatefactor[0] = 0;
		rotatefactor[1] = 0;
		rotatefactor[2] = 0;
		rotatefactor[3] = 0;
		zoomfactor = 0;
		panfactor[0] = 0;
		panfactor[1] = 0;
	}
	float task4useranswer;
	float binaryanswer = -1;
	if(num_task==2||num_task==3)
	{
		if(ifselectanswer)
			binaryanswer = useranswer;
	}
	int n,n1,n2;

	std::ofstream log(namelog, std::ios_base::app | std::ios_base::out);

	log<<std::fixed << std::setprecision(8) <<time<<" ";
	log<<ifpause<<" ";
	log<<std::fixed << std::setprecision(2) <<ifnext<<" "
			<<task<<" "<<mode<<" "<<stereo<<" "<<mousebutton<<" "<<mousestate<<" "
		<<rotatefactor[0]<<" "<<rotatefactor[1]<<" "<<rotatefactor[2]<<" "<<rotatefactor[3]<<" "
		<<zoomfactor<<" "<<panfactor[0]<<" "<<panfactor[1]<<" ";//<<keystate<<" ";

	char stra[100];
	for(int i=0;i<task14answer.size();i++)
	{
		stra[i] = task14answer[i];
	}	

	if(task14answer.size()!=0)
	{	
		task4useranswer = atof(stra);
	}
	else
		task4useranswer = 0.;

	confidentoutput = confidentlevel;

	log<<task4useranswer<<" "<<binaryanswer<<" "<<confidentoutput<<endl;
	log.close();
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
    mousestate = state;

		s |= (button == GLUT_LEFT_BUTTON)   ? ((state == GLUT_DOWN) ? Trackball::LBUTTON_DOWN : Trackball::LBUTTON_UP) : 0;
		  s |= (button == GLUT_RIGHT_BUTTON)  ? ((state == GLUT_DOWN) ? Trackball::RBUTTON_DOWN : Trackball::RBUTTON_UP) : 0;

		int key_state = glutGetModifiers();
		//s |= (key_state & GLUT_ACTIVE_CTRL)  ? Trackball::CTRL_DOWN  : 0;
		s |= (key_state & GLUT_ACTIVE_ALT)   ? Trackball::ALT_DOWN   : 0;
		//s |= (key_state & GLUT_ACTIVE_SHIFT) ? Trackball::SHIFT_DOWN : 0;

	if((button == GLUT_LEFT_BUTTON) && (state==GLUT_DOWN))
	{
		mousex = (float)x;mousey = (float)y;
		if(x>=nextx&&x<=nextx+150.&&y>=viewport[3]/10.-50&&y<=viewport[3]/10.)
		{
			if(ifpause==false)
			{
				ifpause=true;
				if(ifnext==1)
					wholetime = GetTime()-begin_time + wholetime;
				//cerr<<GetTime()-wholetime<<endl;
				
			}
			else
			{
				ifpause=false;
				if(ifnext==1)
					begin_time = GetTime();
			}
		}
		else if(x>=nextx&&x<=nextx+150.&&y<=nexty&&y>=nexty-50.)
		{
			page_count++;
			page_next = true;
			if(ifnext==2)
			{
				if(!(ifselectanswer==true&&ifconfident==true))
				{	page_count--;page_next = false;}

			}	
			else if(ifnext==1)	
			{
				if((!(ifselectanswer==true)&&(num_task==2||num_task==3))
					||GetTime()-timefornext<3.)
				{	page_count--;page_next = false;}	
			}
			ifnext = page_order[page_count];//cerr<<ifnext<<" "<<page_count<<endl;
			if(ifnext == 1&&((ifconfident==true&&ifselectanswer==true)||page_next))
			{
				begin_time = GetTime();
				timefornext = GetTime();
				if(page_count!=1)
				{
					answerfile.open(answername,std::ios_base::app);
					answerfile<<useranswer<<endl;
					answerfile.close();
					task14answer.clear();		

					answerfile.open(confidentname,std::ios_base::app);
					answerfile<<confidentlevel<<endl;
					answerfile.close();
				}
				if(num_page==7)
				{
					num_page = 0;
					num_mode ++;
					myDraw->SetMode(mode_order[num_mode]+1);
				}
				else
					num_page++;
				myDraw->questionnum++;//cerr<<num_mode<<endl;

				datacount++;
				if(datacount==32)datacount=0;

				cerr<<num_mode<<" "<<num_page<<endl;
				if(num_task==0)
					sprintf(name,"task1_data/D%d%d",dataorder[datacount]/10,dataorder[datacount]%10);
				else if(num_task==1)
					sprintf(name,"task2_data/D%d%d",dataorder[datacount]/10,dataorder[datacount]%10);
				else if(num_task==2)
					sprintf(name,"task3_data/D%d%d",dataorder[datacount]/10,dataorder[datacount]%10);
				else if(num_task==3)
					sprintf(name,"task4_data/D%d%d",dataorder[datacount]/10,dataorder[datacount]%10);
				if(dataorder[datacount]>50)
					ifstereo = true;
				else
					ifstereo = false;
				//cerr<<ifstereo<<endl;

				ifselectanswer=false;
				myData->SetData(dataDir, name);
 				myDraw->SetData(myData->GetPos(),myData->GetDir(),myData->GetDen(),NULL,
						myData->GetNum(),myData->GetLeftBounding(),myData->GetRightBounding());
				l = myData->GetLeftBounding();
				r = myData->GetRightBounding();
				if(num_task==0)
				myDraw->SetTask1(myData->GetTask1((dataorder[datacount]/10 - 1)* 4+dataorder[datacount]%10-1));
				else if(num_task==1)
				myDraw->SetTask2(myData->GetTask21((dataorder[datacount]/10 - 1)* 4+dataorder[datacount]%10-1),myData->GetTask22((dataorder[datacount]/10 - 1)* 4+dataorder[datacount]%10-1));
				else if(num_task==2)
				myDraw->SetTask3(myData->GetTask31((dataorder[datacount]/10 - 1)* 4+dataorder[datacount]%10-1),myData->GetTask32((dataorder[datacount]/10 - 1)* 4+dataorder[datacount]%10-1));
				else if(num_task==3)
				{
					myDraw->SetTask4(myData->GetTask41((dataorder[datacount]/10 - 1)* 4+dataorder[datacount]%10-1),myData->GetTask42((dataorder[datacount]/10 - 1)* 4+dataorder[datacount]%10-1));
					int n1[12],n2[12];
					myData->GetTask4n1((dataorder[datacount]/10 - 1)* 4+dataorder[datacount]%10-1,n1);
					myData->GetTask4n2((dataorder[datacount]/10 - 1)* 4+dataorder[datacount]%10-1,n2);
					myDraw->SetTask4Neighbours(n1,n2);
				}
				myDraw->SetEndPos();
				myDraw->SetArrows();
				myDraw->ResetArrow();
				myDraw->GenerateLines();

				svVector3 center;
  				center[0]=0.;center[1]=0.;center[2]=0.;
  				trackball.setFocus(center);
				trackball.reset();

				confidentlevel = -1;
				ifconfident = false;
				time(&nowtime);

				ifshowanswer = false;
				ofstream outfile(correctname,std::ios_base::app | std::ios_base::out);
				if(num_task==0)
					outfile<<myDraw->GetCorrectAnswer(num_data[num_page],0,0)<<endl;
				else if(num_task==1)
					outfile<<myDraw->GetCorrectAnswer(num_data[num_page],0,1)<<endl;
				else if(num_task==2)
					outfile<<myDraw->GetCorrectAnswer(binaryanswer[datacount],1-binaryanswer[datacount],2)<<endl;
				else if(num_task==3)
					outfile<<myDraw->GetCorrectAnswer(binaryanswer[datacount+32],1-binaryanswer[datacount+32],3)<<endl;
				outfile.close();


			}
			else if(ifnext==0)
			{
				num_task++;
				num_mode=-1;
			}
			else if(ifnext==3)
			{
				answerfile.open(answername,std::ios_base::app);
				answerfile<<useranswer<<endl;
				answerfile.close();

				answerfile.open(confidentname,std::ios_base::app);
				answerfile<<confidentlevel<<endl;
				answerfile.close();
			}
			else if(ifnext ==2&&page_next==true)
			{
				wholetime = wholetime + GetTime() - begin_time;
				answerfile.open(timename,std::ios_base::app);
				answerfile<<wholetime<<endl;
				answerfile.close();
				wholetime = 0;
			}
		}

		else
		{
				if(ifnext==1)
				{
				  if (s & Trackball::BUTTON_DOWN){
				    trackball.mouseDown(s, x, y);
				  }

				  if (s & Trackball::BUTTON_UP){
				    trackball.mouseUp(s, x, y);
			  		}
				}
		}

	}
	else
	{

		if((button == GLUT_RIGHT_BUTTON) && (state==GLUT_DOWN))
		{
			if(ifnext==1)
			{  if (s & Trackball::BUTTON_DOWN){
			    trackball.mouseDown(s, x, y);
			  }

			  if (s & Trackball::BUTTON_UP){
			    trackball.mouseUp(s, x, y);
		  	}
			}
		}
	}
	RecordLog();
	glutPostRedisplay();
}

void motion(int x, int y)
{
	if(mousebutton!=GLUT_MIDDLE_BUTTON)
		trackball.mouseMotion(x, y);
	RecordLog();
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
	keystate = key;
  switch (key) {
    case '0':
		if(ifnext==2&&(num_task==0||num_task==1))
		task14answer.push_back('0');
		break;
    case '1':
		if(ifnext==2&&(num_task==0||num_task==1))task14answer.push_back('1');
		break;	
    case '2':
		if(ifnext==2&&(num_task==0||num_task==1))task14answer.push_back('2');
		break;
    case '3':
		if(ifnext==2&&(num_task==0||num_task==1))task14answer.push_back('3');
		break;
    case '4':
		if(ifnext==2&&(num_task==0||num_task==1))task14answer.push_back('4');
		break;
    case '5':
		if(ifnext==2&&(num_task==0||num_task==1))task14answer.push_back('5');
		break;
    case '6':
		if(ifnext==2&&(num_task==0||num_task==1))task14answer.push_back('6');
		break;
    case '7':
		if(ifnext==2&&(num_task==0||num_task==1))task14answer.push_back('7');
		break;
    case '8':
		if(ifnext==2&&(num_task==0||num_task==1))task14answer.push_back('8');
		break;
    case '9':
		if(ifnext==2&&(num_task==0||num_task==1))task14answer.push_back('9');
		break;
    case 'e':
		if(ifnext==2&&(num_task==0||num_task==1))task14answer.push_back('e');
		break;
    case 'E':
		if(ifnext==2&&(num_task==0||num_task==1))task14answer.push_back('e');
		break;
    case '+':
		if(ifnext==2&&(num_task==0||num_task==1))task14answer.push_back('+');
		break;
    case '-':
		if(ifnext==2&&(num_task==0||num_task==1))task14answer.push_back('-');
		break;
    case '.':
		if(ifnext==2&&(num_task==0||num_task==1))task14answer.push_back('.');
		break;
    case 'h':
		trackball.reset();
		break;
	case 'H':
		trackball.reset();
		break;
    case '\010':
		if(ifnext==2&&(num_task==0||num_task==1))
			if(task14answer.size()!=0)
			task14answer.pop_back();	
		break;
    case '\033':
      exit(0);
      break;
  }
  RecordLog();
  glutPostRedisplay();
}



int main(int argc, char **argv)
{
   time(&begin_time);
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);//|GLUT_STEREO);//O
   glutInitWindowSize (2000, 1000);
   glutInitWindowPosition (0, 0);
   glutCreateWindow (argv[0]);
   glutFullScreen();   
   initCamera();
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
