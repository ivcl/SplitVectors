#include <assert.h>
#include <ctime>
#include <unistd.h>
#include <time.h>       /* time_t, struct tm, time, localtime */

#include "../src/glui-2.35/src/include/GL/glui.h"
#include "../src/svQDOTNorm.h"
#include "../src/svContour.h"
#include "../src/svCut.h"
#include "../src/svOutline.h"
#include "../src/svMagnitude.h"
#include "../src/svArrowGlyph.h"
#include "../src/svSlice.h"
#include "../src/svROIbox.h"
#include "../src/svANN.h"
#include "../src/svQDOTdistancefunction.h"
#include "../src/svQDOTdistancemap.h"
#include "../src/svPrimitive.h"
#include "../src/ivTrackball.h"
#include "../src/EMclustering/dataProcess.h"

#include "File.h"

#ifndef M_PI 
#define M_PI 3.1415926
#endif

//using namespace __svl_lib;
//using namespace __em_lib;
//using namespace std;

GLuint window, right_window, left_window;
GLuint sub_width=500,sub_height=500,image_width=1000,image_height=1000;

GLdouble vmat[16], pmat[16];
GLint    viewport[4];

GLint mouse_state=-1;
GLint mouse_button=-1;
view3d view_info;

char *InputFile1;
char *InputFile2;

svQDOTNorm *qdotField[2];
svContour *iso[2];
svContour *projection_iso[2];
svCut * cutting_mag[2];
svCut * cutting_vel[2];
svMagnitude *qdotMag;
svOutline *qdotOutline[2];
svArrowGlyph *qdotIsoArrow[2];
svArrowGlyph *qdotIsoArrow2[2];
svSlice *qdotSlice[2];
svROIbox *qdotBox[2];
svANN *qdotann;
svANN *projectionann[2];
QDOTdistancefunction *qdotdistance[2];
svQDOTdistancemap * qdotmap[2];
svPrimitive *qdotprimitive[2];
Cluster *qdotcluster[2];
Cluster *isocluster[2];

char leftdatastring[50];
char rightdatastring[50];

GLint nx, ny, nz;
Trackball trackball[2];
Trackball trackball_twod[2];
Trackball trackball_legend[2];

svVector3 lb[2], rb[2];
svVector3 l[2], r[2];//interested region
svVector3 ll[2];
svFloat zmax[2]; // drawing range (zmin, zmax); 
                    // initialize it to the bounding box size

svBool enablesample = false;
svBool enableline = true;
svBool enableline2 = true;

int twod_mouse_x;
int twod_mouse_y;
int twod_mouse_y2;
int mouse_x;
int mouse_y;
int mouse_x2;
int mouse_y2;

int datasize[2];
float zoomin = 0;

GLfloat m[2][16];
GLfloat m1[2][16];
GLfloat mleft[2][16];
__svl_lib::Matrix4f zoomintwod[2];
__svl_lib::Matrix4f panintwod[2];


float *TwoDx[2];
float *TwoDy[2];
int TwoDnum[2];
float mindistance;
bool *inbox[2];
bool *inbox2[2];

bool *inbox_contour[2];
bool *inbox_cluster_contour[2];
bool *inbox_mag[2];
bool *inbox_vel[2];
bool *inbox_mag2[2];
bool *inbox_vel2[2];
bool *inbox_mag_cluster[2];
bool *inbox_vel_cluster[2];
bool *inbox_mag_projection[2];
bool *inbox_vel_projection[2];
bool *inbox_mag_projection_cluster[2];
bool *inbox_vel_projection_cluster[2];

float twodzoom[2];// = 1.;
float twotranx[2] ;//= 0.;
float twotrany[2];// = 0.;

svVector3 *slicePos[2];
float ROIden[2];

int mousestop = 0;
bool ifstop = false;

char vtkout[50];

float mag_scale;

//to make sure 2d data full of windows.
float distance_scale[2];

int enable_sync = 0;
int enable_data_sync = 1;

void Delete_Init(int data_type);
void InitData(int data_type);
void InitMag(int data_type);
void InitIso(int data_type);
void InitRender(int data_type);
void InitClusters(int data_type);
void InitArrows(int data_type);

void CaptureScreen(char *file);
void MagnitudeEncoding(int n);
void LoadFile();
void Mag_ROIClustering(int n, int data_type);
void Vel_ROIClustering(int n, int data_type);
void SetContourColor(int data_type);
void Vel_Contour_ROI_Clustering(int n, int data_type);
void Vel_Contour_Clustering(int data_type);
void Vel_ROI_Clustering(int n, int data_type);
void Vel_Clustering(int data_type);
void Mag_Contour_ROI_Clustering(int n, int data_type);
void Mag_Contour_Clustering(int data_type);
void Mag_ROI_Clustering(int n, int data_type);
void Mag_Clustering(int data_type);
void Semi_Mag(int data_type);
void PlaneComparison(int data_type);
void ContoursGeneration(vector<float> ra,int data_type);



int MagEncode = 10;
int Colorby_id = 11;
int Original_id = 12;
int Cluster_id = 13;
int MagSemi_id = 14;
int VelSemi_id = 15;
int Projection_id  = 16;
int MagCluster_id = 17;
int VelCluster_id = 18;
int Twod_id = 19;
int Map1_id = 20;
int Map2_id = 21;
int N_p = 22;
int MagMax_id = 23;
int MagMin_id = 24;
int Loadfile_id = 25;
int gui_id = 26;
int Magnum_id1 = 27;
int Velnum_id1 = 28;
int Magnumin_id1 = 29;
int Velnumin_id1 = 30;
int Z_refer_id = 31;
int Savefile_id = 32;
int Contour_id = 33;
int ROI_id = 34;
int Leftdrop_id = 35;
int Rightdrop_id = 36;
int Zlayer_id = 37;
int Zlayer_text_id = 40;
int Zanglemin_id = 38;
int Zanglemin_text_id = 41;
int Zanglemax_id = 39;
int Zanglemax_text_id = 42;
int Vector_id = 43;
int SavePicture_id = 44;
int Projectionaggregation_id = 45;
int ROIBox_id = 46;
int Rightdata_id = 47;
int Leftdata_id = 48;
int Syncdata_id = 49;
int Magnum_id2 = 127;
int Velnum_id2 = 128;
int Magnumin_id2 = 129;
int Velnumin_id2 = 130;
int Leftpop_id = 50;
int Rightpop_id = 51;
int Leftvtk_id = 54;
int Rightvtk_id = 55;
int Leftselect_id = 52;
int Rightselect_id = 53;

svVector3 oneplane_selectp[2];
GLUI *glui;
GLUI *glui_data_1;
GLUI *glui_data_2;
GLUI *glui_load_data;
GLUI *glui_left;
GLUI *glui_right;
GLUI_StaticText *obj_text_zlayer;
GLUI_StaticText *obj_text_zanglemin;
GLUI_StaticText *obj_text_zanglemax;
GLUI_Scrollbar *sb_zlayer;
GLUI_Scrollbar *sb_zanglemin;
GLUI_Scrollbar *sb_zanglemax;
GLUI *glui_left_pop;
GLUI *glui_right_pop;
GLUI_List *lefthah;
GLUI_List *righthah;
GLUI_List *leftvtk;
GLUI_List *rightvtk;

svScalar min_density;
svScalar max_density;
svScalar mag_distance;

float zangle_min;
float zangle_max;
 svVector3 Widget_z_angle[4];
 svVector3 Widget_bound_zangle[2];
 float Widget_min_degree_zangle;
 bool Widget_select_zangle[2];
 float Widget_t_zangle[2];

 float range_mag[2];
 svVector3 Widget_z_mag[4];
 svVector3 Widget_bound_mag[2];
 float Widget_min_degree_mag;
 bool Widget_select_mag[2];
 float Widget_t_mag[2];

 vector<double> Widget_xy_min[3];
 vector<double> Widget_xy_max[3];
 vector<bool> Widget_select_xy;
 svVector3 Widget_xy_center;
 svScalar Widget_xy_radius;
 vector<double> xy_min;
 vector<double> xy_max;
 float Widget_xy_h;
 float Widget_xy_w;
 bool Widget_select_xy_whole = false;
 double Widget_xy_theta[2];
bool Widget_flag;

//-------parameters in File.h--------------
vector<string> datastring;
vector<string> folderstring;
char DataDir[2][50];
char datafile[2][50];
char ContourFile2[2][50];
char VTKfile[2][200];
char LogFile[50];
char tmpfiledir[50];
char picturedir[50];

 int left_display_type;
 int right_display_type;

 int magnitude_encoding;
 int projectionaggregation;
 int enable_roi;
 float zmin;
 float vector_x;
 float vector_y;
 float vector_z;
 int lightness_degree;
 vector<float> contour_ratio[2];
 int magnum[2];
 int velnum[2];
 int magnumin[2];
 int velnumin[2];
 float num_magnitude_clusters[2];
 float num_magnitude_clusters_in[2];
 float num_orientation_clusters[2];
 float num_orientation_clusters_in[2];

 int enable_cuttingplane;
 int enable_box;
 int enable_zplane;
 int enable_projection;
 int enable_contour;
 int enable_maglegend1;
 int enable_maglegend2;
 int enable_maglegend3;
 int enable_vellegend1;
 int enable_vellegend2;
 int enable_vellegend3;
 int enable_sum;
 int enable_slider;
 int enable_bounding;
 int enable_2dlegend;
 int enable_select2d;

 float transformation_mat[2][16];

 int left_data_type;
 int right_data_type;

 float arrow_base[2];
 float num_segment_arrow[2];
 float z_standard[2];
char data_string[2][50];

void InitLight()
{
  //setting of lighting
  GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8, .4};
  GLfloat mat_specular[] = { 1, 1, 1, 1 };
  GLfloat mat_shininess[] = { 20.0 };
 // GLfloat light_position[] = { 0, 0, 60, 0.0 };
  GLfloat light_position[] = { 0.,0.,rb[0][2]*4, 0.0 };
  GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };
  glClearColor(0.0, 0.0, 0.0, 0.0);

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);

}

void
main_reshape(int width,  int height) 
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height,-10,10);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    sub_width = width/2.0;
    sub_height = height;

    glutSetWindow(left_window);
    glutPositionWindow(0., 0.);
    glutReshapeWindow(sub_width, sub_height);

    glutSetWindow(right_window);
    glutPositionWindow(sub_width, 0.);
    glutReshapeWindow(sub_width, sub_height);

    image_width = width;
    image_height = height;

    mag_scale = (fabs(view_info.eye[2]) * tan(M_PI*view_info.view_angle/(2.*180)) * 2.)/image_height;
    qdotMag->SetMagScalar(mag_scale);
}

void
main_display(void)
{
    glClearColor((float)0.5, (float)0.5,(float)0.5, 1.);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glFlush();
    glutSwapBuffers();
}

void left_reshape(int width, int height)
{
	glViewport (0., 0., width, height);
  	glMatrixMode(GL_PROJECTION);
  	glLoadIdentity();

        gluPerspective(view_info.view_angle,
                   view_info.aspect_ratio * width/height,
                   0.1,
                   1000);
	glMatrixMode(GL_MODELVIEW);
  	glLoadIdentity();
        gluLookAt(0,0,view_info.eye[2],
              0,0,0,
              0, 1, 0);

      glGetIntegerv (GL_VIEWPORT, qdotOutline[0]->viewport);
      glGetDoublev (GL_MODELVIEW_MATRIX, qdotOutline[0]->mvmatrix);
      glGetDoublev (GL_PROJECTION_MATRIX, qdotOutline[0]->projmatrix);

      glGetIntegerv (GL_VIEWPORT, qdotBox[0]->viewport);
      glGetDoublev (GL_MODELVIEW_MATRIX, qdotBox[0]->mvmatrix);
      glGetDoublev (GL_PROJECTION_MATRIX, qdotBox[0]->projmatrix);

      glGetIntegerv (GL_VIEWPORT, qdotSlice[0]->viewport);
      glGetDoublev (GL_MODELVIEW_MATRIX, qdotSlice[0]->mvmatrix);
      glGetDoublev (GL_PROJECTION_MATRIX, qdotSlice[0]->projmatrix);

      glGetIntegerv (GL_VIEWPORT, qdotOutline[1]->viewport);
      glGetDoublev (GL_MODELVIEW_MATRIX, qdotOutline[1]->mvmatrix);
      glGetDoublev (GL_PROJECTION_MATRIX, qdotOutline[1]->projmatrix);

      glGetIntegerv (GL_VIEWPORT, qdotBox[1]->viewport);
      glGetDoublev (GL_MODELVIEW_MATRIX, qdotBox[1]->mvmatrix);
      glGetDoublev (GL_PROJECTION_MATRIX, qdotBox[1]->projmatrix);\

      glGetIntegerv (GL_VIEWPORT, qdotSlice[1]->viewport);
      glGetDoublev (GL_MODELVIEW_MATRIX, qdotSlice[1]->mvmatrix);
      glGetDoublev (GL_PROJECTION_MATRIX, qdotSlice[1]->projmatrix);

      glGetIntegerv (GL_VIEWPORT, viewport);
      glGetDoublev (GL_MODELVIEW_MATRIX,vmat);
      glGetDoublev (GL_PROJECTION_MATRIX, pmat);

        glClearColor(0.5,0.5,0.5,1);

	for(int i=0;i<2;i++)
	{
		qdotOutline[i]->width=height;
		qdotBox[i]->width=height;
		qdotSlice[i]->width = height;
	}

	qdotMag->width = height;
	Reshape_Widget(image_width,image_height);

}

//Render
void contour_display(int display_type, int data_type)
{
	trackball[data_type].getMatrix().getValue(m[data_type]);
	int sc[datasize[data_type]];
	int tmpsc[9]={-1,-1,-1,-1,-1,-1,-1,-1,-1};
	//Render 3D objects.
        glPushMatrix();
	glMultMatrixf(m[data_type]);
	
	if(enable_projection && display_type == 0)
	{
		if(projectionaggregation)
			cutting_mag[data_type]->RenderCutting(zmin,inbox_contour[data_type]);
		else
			cutting_mag[data_type]->RenderCutting(zmin,inbox_mag_projection[data_type]);
	}
	else if(enable_projection && display_type == 1)
	{
		if(projectionaggregation)
			cutting_vel[data_type]->RenderCutting(zmin,inbox_contour[data_type]);
		else
			cutting_vel[data_type]->RenderCutting(zmin,inbox_mag_projection[data_type]);
	}

	if(display_type==0 && enable_zplane)
	{
		qdotcluster[data_type]->mag_display(inbox_mag[data_type]);
	}
	else if(display_type == 1 && enable_zplane)
	{
		qdotcluster[data_type]->vel_display(inbox_vel[data_type]);
	}
	
	if(display_type==0 && enable_contour)
	{
		qdotIsoArrow[data_type]->RenderArrow(inbox_contour[data_type],tmpsc,9, enablesample,true,sc);
	}
	else if(display_type==1 && enable_contour)
	{
		qdotIsoArrow2[data_type]->RenderArrow(inbox_contour[data_type],tmpsc,9, enablesample,true,sc);
	}

	if(enable_cuttingplane)
	{
		qdotSlice[data_type]->RenderSlice(qdotBox[data_type]->stop1, qdotBox[data_type]->stop2);
	}

	if(enable_bounding)
	{
		qdotOutline[data_type]->DrawAxis(l[data_type],r[data_type]);
		qdotprimitive[data_type]-> DrawAxis_scale(image_width/2);
		qdotOutline[data_type]->DrawXYZ();
	}

	if(enable_box)
	{
		qdotBox[data_type]->RenderROIbox();
	}
   	glPopMatrix();

	//magnitude legend
	if(display_type==0)
	{
	    char str[50];char str1[50];
	    glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();
	    gluOrtho2D(0.0,(GLfloat) viewport[2], 0.0, (GLfloat) viewport[3]);
	    glMatrixMode(GL_MODELVIEW);
	    glLoadIdentity();
	    glDisable(GL_LIGHTING);

		sprintf(str,"original data");
		glColor3f(1.,1.,1.);
		glRasterPos2f(40.,qdotcluster[data_type]->GetML()*20.+20.);
		for(int j=0;j<strlen(str);j++)
		     glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

		glPushMatrix();
		glTranslatef(40.,20.,0);
		glScalef(10.,20.,1.);
		if(enable_maglegend1&&enable_zplane)
		{
			qdotcluster[data_type]->DrawMagLegend();
		}
		glPopMatrix();

		if(enable_maglegend2 && enable_contour)
		{
			sprintf(str,"contour data");
			glColor3f(1.,1.,1.);
			glRasterPos2f(200.,isocluster[data_type]->GetML()*20.+20.);
			for(int j=0;j<strlen(str);j++)
			     glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
		
			glPushMatrix();
			glTranslatef(200.,20.,0);
			glScalef(10.,20.,1.);
			if(enable_maglegend2 && enable_contour)
			{
				isocluster[data_type]->DrawMagLegend();
			}
			glPopMatrix();

			sprintf(str,"data in ROI box");
			glColor3f(1.,1.,1.);
			glRasterPos2f(360.,isocluster[data_type]->GetMLin()*20.+20.);
			for(int j=0;j<strlen(str);j++)
			     glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

			glPushMatrix();
			glTranslatef(360.,20.,0);
			glScalef(10.,20.,1.);
			if(enable_maglegend3&&enable_contour)
			{
				isocluster[data_type]->DrawMagLegend_in();
			}
			glPopMatrix();
		}
		    glMatrixMode(GL_PROJECTION);
		    glLoadMatrixd(pmat);
		    glMatrixMode(GL_MODELVIEW);
		    glLoadMatrixd(vmat);
	}
	//orientation legend
	else if(display_type == 1)
	{
		if(enable_vellegend1&&enable_zplane)
		{
			glViewport (0, 0., (GLfloat)viewport[2]/8., (GLfloat)viewport[3]/4.);
		  	glMatrixMode(GL_PROJECTION);
		  	glLoadIdentity();
			gluPerspective(view_info.view_angle,
				   view_info.aspect_ratio * ((GLfloat)viewport[2]/8)/((GLfloat)viewport[3]/4.),
				    50,
				   1000);
			glMatrixMode(GL_MODELVIEW);
		  	glLoadIdentity();
			gluLookAt(0,0,view_info.eye[2],
			      0,0,0,
			      0, 1, 0);

			if(enable_sync)
				trackball[data_type].getRotateMatrix().getValue(m1[data_type]);
			else
				trackball_legend[data_type].getRotateMatrix().getValue(m1[data_type]);
			glColor3f(1,1,1);
			char vel[13]={'o','r','i','g','i','n','a','l',' ','d', 'a','t','a'};
			glColor3f(1,1,1);
			glRasterPos3f(-30.,30.,0.);
			for(int i=0;i<13;i++)
			{
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, vel[i]);
			}

			glPushMatrix();
			glMultMatrixf(m1[data_type]);
			//glRotatef(-90.,1.,0.,0.);
			glScalef(20,20,20);
			qdotcluster[data_type]->DrawVelLegend(-1);
			glPopMatrix();
		}

		if(enable_contour&&enable_vellegend2)
		{
			glViewport ((GLfloat)viewport[2]/8., 0., (GLfloat)viewport[2]/8., (GLfloat)viewport[3]/4.);
		  	glMatrixMode(GL_PROJECTION);
		  	glLoadIdentity();
			gluPerspective(view_info.view_angle,
				   view_info.aspect_ratio * ((GLfloat)viewport[2]/8)/((GLfloat)viewport[3]/4.),
				    50,
				   1000);
			glMatrixMode(GL_MODELVIEW);
		  	glLoadIdentity();
			gluLookAt(0,0,view_info.eye[2],
			      0,0,0,
			      0, 1, 0);

			if(enable_sync)
				trackball[data_type].getRotateMatrix().getValue(m1[data_type]);
			else
				trackball_legend[data_type].getRotateMatrix().getValue(m1[data_type]);

			glColor3f(1,1,1);
			char vel2[12]={'c','o','n','t','o','u','r',' ','d', 'a','t','a'};
			glColor3f(1,1,1);
			glRasterPos3f(-30.,30.,0.);
			for(int i=0;i<12;i++)
			{
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, vel2[i]);
			}

			glPushMatrix();
			glMultMatrixf(m1[data_type]);
			//glRotatef(-90.,1.,0.,0.);
			glScalef(20,20,20);
			isocluster[data_type]->DrawVelLegend(-1);
			glPopMatrix();
	
		   	glMatrixMode(GL_PROJECTION);
		    	glLoadMatrixd(pmat);
		    	glMatrixMode(GL_MODELVIEW);
		    	glLoadMatrixd(vmat);
		}

		if(enable_contour&&enable_vellegend3)
		{
			glViewport ((GLfloat)viewport[2]/4., 0., (GLfloat)viewport[2]/8., (GLfloat)viewport[3]/4.);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(view_info.view_angle,
					  view_info.aspect_ratio * ((GLfloat)viewport[2]/8)/((GLfloat)viewport[3]/4.),
					    50,
					   1000);
			glMatrixMode(GL_MODELVIEW);
			  glLoadIdentity();
			gluLookAt(0,0,view_info.eye[2],
				     0,0,0,
				     0, 1, 0);

			if(enable_sync)
				trackball[data_type].getRotateMatrix().getValue(m1[data_type]);
			else
				trackball_legend[data_type].getRotateMatrix().getValue(m1[data_type]);

			glColor3f(1,1,1);
			char vel2[15]={'d','a','t','a',' ','i','n',' ','R', 'O','I',' ', 'b', 'o','x'};
			glColor3f(1,1,1);
			glRasterPos3f(-30.,30.,0.);
			for(int i=0;i<15;i++)
			{
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, vel2[i]);
			}

			glPushMatrix();
			glMultMatrixf(m1[data_type]);
			//glRotatef(-90.,1.,0.,0.);
			glScalef(20,20,20);
			isocluster[data_type]->DrawVelLegend_in();
			glPopMatrix();

			glMatrixMode(GL_PROJECTION);
			glLoadMatrixd(pmat);
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixd(vmat);
		}

		glViewport (0., 0., (GLfloat)viewport[2], (GLfloat)viewport[3]);
				
	}

}

void GetMatrix(int data_type)
{
	__svl_lib::Matrix4f trans2 = trackball[data_type].getMatrix();
	//t = trans2;// * t * t3 * t2;
	GLfloat mm[16];
	trans2.getValue(mm);
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			qdotOutline[data_type]->tb_transform[i][j] =mm[j+i*4];
			qdotBox[data_type]->tb_transform[i][j] = mm[j+i*4];
			qdotSlice[data_type]->tb_transform[i][j] = mm[j+i*4];
		}
	}
}

void twodglyph_display(int data_type)
{
	// 2D measurement maps
	//glDisable(GL_DEPTH_TEST);
	    glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();
	    gluOrtho2D(0.0,(GLfloat) viewport[2], 0.0, (GLfloat) viewport[3]);
	    glMatrixMode(GL_MODELVIEW);
	    glLoadIdentity();
	    glDisable(GL_LIGHTING);

		glPushMatrix();

		glTranslatef(image_width/4.,image_height/2.,0);
		glTranslatef(twotranx[data_type],twotrany[data_type],0.);
		glScalef(twodzoom[data_type],twodzoom[data_type],twodzoom[data_type]);
		//glScalef(distance_scale[data_type],distance_scale[data_type],distance_scale[data_type]);
	
		qdotmap[data_type]->Render(qdotdistance[data_type]->GetLayerPos(),
				qdotdistance[data_type]->GetLayerDis(),
				inbox_vel2[data_type],
				qdotField[data_type]->GetZlayer(),
				qdotdistance[data_type]->GetXd(),
				qdotdistance[data_type]->GetYd(), 
				z_standard[data_type]);
		
		glPopMatrix();
	
		glPushMatrix();
		glTranslatef(20.,20.,0.);
		glScalef(20,20,1.);
		if(enable_2dlegend)
		{
			qdotmap[data_type]->Render2dLegend();
		}	
		glPopMatrix();
	
		glColor3f(1.,1.,1.);
		char s[50];
		sprintf(s,"Distance Comparison");
		glRasterPos2f(40.,image_height - 20.);
		for(int ii=0;ii<strlen(s);ii++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s[ii]);

		glMatrixMode(GL_PROJECTION);
		glLoadMatrixd(pmat);
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixd(vmat);

		//orientation clustering legend
		glViewport ((GLfloat)viewport[2]/2., 0., (GLfloat)viewport[2]/8., (GLfloat)viewport[3]/4.);
	  	glMatrixMode(GL_PROJECTION);
	  	glLoadIdentity();
		gluPerspective(view_info.view_angle,
		           view_info.aspect_ratio * ((GLfloat)viewport[2]/8)/((GLfloat)viewport[3]/4.),
		            50,
		           1000);
		glMatrixMode(GL_MODELVIEW);
	  	glLoadIdentity();
		gluLookAt(0,0,view_info.eye[2],
		      0,0,0,
		      0, 1, 0);

		trackball_legend[data_type].getRotateMatrix().getValue(m1[data_type]);
		glColor3f(1,1,1);
		char vel[13]={'o','r','i','g','i','n','a','l',' ','d', 'a','t','a'};
		glColor3f(1,1,1);
		glRasterPos3f(-30.,30.,0.);
		for(int i=0;i<13;i++)
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, vel[i]);
		}

		glPushMatrix();
		glMultMatrixf(m1[data_type]);
		glScalef(20,20,20);
		qdotcluster[data_type]->DrawVelLegend(qdotmap[data_type]->GetSelectIndex());
		glPopMatrix();

	   	glMatrixMode(GL_PROJECTION);
	    	glLoadMatrixd(pmat);
	    	glMatrixMode(GL_MODELVIEW);
	    	glLoadMatrixd(vmat);


	glViewport (0., 0., (GLfloat)viewport[2], (GLfloat)viewport[3]);
   	//glutSwapBuffers();
}

void Display_text(int data_type)
{ 
	qdotMag->RenderMagnitude(image_width/2., image_height);

	glDisable(GL_DEPTH_TEST);
	glColor3f(1,1,1);
	char str[50];
	if(enable_slider)
	{
		sprintf(str,"Orientation");
		glRasterPos2f(40., image_height-20.);
		for(int j=0;j<strlen(str);j++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[j]);

		Widget_Z_angle();
		Widget_XY();
		Widget_Mag();

		glColor3f(1,1,1);
		//char str[50];
		sprintf(str,"Magnitude");
		glRasterPos2f(40., image_height-20);
			for(int j=0;j<strlen(str);j++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[j]);
	}


}

void Display_ROI(int data_type)
{
	char str[50];

	if(enable_sum && enable_box)
	{
		char str1[50];
		sprintf(str1, "sum of (charge density)");
		sprintf(str,"%.2f",ROIden[data_type]);
	
		glColor3f(1.,1.,1.);
		glRasterPos2f(40.,image_height-90.);
		for(int j=0;j<strlen(str1);j++)
		     glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str1[j]);
		for(int j=0;j<strlen(str);j++)
		     glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
	}



}

void left_display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(left_display_type ==0 || left_display_type == 1)
	{
		contour_display(left_display_type, left_data_type);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
		twodglyph_display(left_data_type);
	}

	if(left_data_type!=right_data_type)
	{
	    glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();
	    gluOrtho2D(0.0,(GLfloat) viewport[2], 0.0, (GLfloat) viewport[3]);
	    glMatrixMode(GL_MODELVIEW);
	    glLoadIdentity();
	    glDisable(GL_LIGHTING);
		Display_ROI(left_data_type);
	    glMatrixMode(GL_PROJECTION);
	    glLoadMatrixd(pmat);
	    glMatrixMode(GL_MODELVIEW);
	    glLoadMatrixd(vmat);
	}
	    glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();
	    gluOrtho2D(0.0,(GLfloat) viewport[2], 0.0, (GLfloat) viewport[3]);
	    glMatrixMode(GL_MODELVIEW);
	    glLoadIdentity();
	    glDisable(GL_LIGHTING);
		glColor3f(0.,0.,0.);
		glLineWidth(2.);
		glBegin(GL_LINES);
		glVertex2f(viewport[2],0.);
		glVertex2f(viewport[2],(GLfloat) viewport[3]);
		glEnd();
		glLineWidth(1.);

		glColor3f(1.,1.,1.);
		char str1[200];
		char str[50];
		sprintf(str1, "Data %d: %s ", left_data_type, datafile[left_data_type]);
		glRasterPos2f(40.,image_height-60.);
		for(int j=0;j<strlen(str1);j++)
		     glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str1[j]);		

	    glMatrixMode(GL_PROJECTION);
	    glLoadMatrixd(pmat);
	    glMatrixMode(GL_MODELVIEW);
	    glLoadMatrixd(vmat);

	glFlush();
   	glutSwapBuffers();
}
void right_display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(right_display_type==0 || right_display_type==1)
	{
		//cerr<<"right"<<endl;
		contour_display(right_display_type,right_data_type);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
		twodglyph_display(right_data_type);
	}

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,(GLfloat) viewport[2], 0.0, (GLfloat) viewport[3]);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_LIGHTING);

	Display_text(right_data_type);
	glDisable(GL_DEPTH_TEST);
	Display_ROI(right_data_type);

		glColor3f(0.,0.,0.);
		glLineWidth(2.);
		glBegin(GL_LINES);
		glVertex2f(0.,0.);
		glVertex2f(0.,(GLfloat) viewport[3]);
		glEnd();
		glLineWidth(1.);

		glColor3f(1.,1.,1.);
		char str1[200];
		sprintf(str1, "Data %d: %s", right_data_type, datafile[right_data_type]);
		glRasterPos2f(40.,image_height-60.);
		for(int j=0;j<strlen(str1);j++)
		     glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str1[j]);	

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(pmat);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(vmat);

	glFlush();
   	glutSwapBuffers();
}

void redisplay_all(void)
{
    glutSetWindow(window);
    //main_reshape(image_width, image_height);
    glutPostRedisplay();

    glutSetWindow(left_window);
    //left_reshape(sub_width,sub_height);
    glutPostRedisplay();

    glutSetWindow(right_window);
    //left_reshape(sub_width,sub_height);
    glutPostRedisplay();
}

bool gui_open = true;

void control_left_pop(int control)
{
	if(control == Leftselect_id)
	{
		int item = lefthah->get_current_item();
		for(int i=0;i<datastring.size();i++)
		{
			if(i == item)
			{	
				char *str = new char[datastring[i].length()+1];
				strcpy(str,datastring[i].c_str());
				sprintf(leftdatastring, "%s", str);
				delete [] str;
				break;
			}
		}
	        DIR *pDIR;
		struct dirent *entry;
		int ii=0;
		int count = 0;
		char str2[100];
		char vtkstr[100];
		sprintf(vtkstr, "../../data/vtkdata/%s", leftdatastring);
		cerr<<"vtkstr "<<vtkstr<<endl;
		if( pDIR=opendir(vtkstr) )
		{
		        while(entry = readdir(pDIR))
			{
		                if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 )
				{
		                	//cout << entry->d_name << "\n";
					ii++;
					if(ii==2)
					{	
						count = 0 ;
						char str[100];
						sprintf(str, "%s", entry->d_name);
						cerr<<str<<endl;
						for(int i=0;i<strlen(str);i++)
						{
							if(str[i]!=str2[i])
							{
								break;
							}
							else
								count++;
						}
						break;
					}
					sprintf(str2, "%s", entry->d_name);
				}
		        }
		        closedir(pDIR);
		}
		cerr<<str2<<" "<<count<<endl;
		char vtkname1[100];
		for(int i=0;i<count;i++)
			vtkname1[i] = str2[i];
		for(int i=count;i<100;i++)
			vtkname1[i] = '\0';
		cerr<<vtkname1<<endl;
		char vtkname[100];
		sprintf(vtkname, "%s/%s", vtkstr, vtkname1);

		cerr<<vtkname<<endl;
		cerr<<leftdatastring<<endl;

		cerr<<leftdatastring<<" "<<data_string[0]<<" "<<data_string[1]<<endl;
		if(!strcmp(data_string[0],leftdatastring))
		{
			left_data_type = 0;
		}
		else if(!strcmp(data_string[1],leftdatastring))
		{
			left_data_type = 1;
		}
		else 
		{
			if(left_data_type == right_data_type)
			{
				left_data_type = 1 - left_data_type;
			}
			
			sprintf(data_string[left_data_type],"%s",leftdatastring);
			sprintf(datafile[left_data_type], "%s",data_string[left_data_type]);
			sprintf(VTKfile[left_data_type], "%s", vtkname);

			Delete_Init(left_data_type);
			qdotIsoArrow[1-left_data_type]->SetArrows(enablesample,true,qdotMag->GetMag(iso[1-left_data_type]->GetDensity(), iso[1-left_data_type]->GetPointnum(), image_width/2., image_height));
			qdotIsoArrow2[1-left_data_type]->SetArrows(enablesample,true,qdotMag->GetMag(iso[1-left_data_type]->GetDensity(), iso[1-left_data_type]->GetPointnum(), image_width/2., image_height));	
		}
		glui_left_pop->hide();	
	}

	redisplay_all();
}

void control_right_pop(int control)
{
	if(control == Rightselect_id)
	{
		int item = righthah->get_current_item();
		for(int i=0;i<datastring.size();i++)
		{
			if(i == item)
			{	
				char *str = new char[datastring[i].length()+1];
				strcpy(str,datastring[i].c_str());
				sprintf(rightdatastring, "%s", str);
				delete [] str;
				break;
			}
		}

	        DIR *pDIR;
		struct dirent *entry;
		int ii=0;
		int count = 0;
		char str2[100];
		char vtkstr[100];
		sprintf(vtkstr, "../../data/vtkdata/%s", rightdatastring);
		cerr<<"vtkstr "<<vtkstr<<endl;
		if( pDIR=opendir(vtkstr) )
		{
		        while(entry = readdir(pDIR))
			{
		                if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 )
				{
		                	//cout << entry->d_name << "\n";
					ii++;
					if(ii==2)
					{	
						count = 0 ;
						char str[100];
						sprintf(str, "%s", entry->d_name);
						cerr<<str<<endl;
						for(int i=0;i<strlen(str);i++)
						{
							if(str[i]!=str2[i])
							{
								//count++;
								break;
							}
							else
								count++;
						}
						break;
					}
					sprintf(str2, "%s", entry->d_name);
				}
		        }
		        closedir(pDIR);
		}
		cerr<<str2<<" "<<count<<endl;
		char vtkname1[100];
		for(int i=0;i<count;i++)
			vtkname1[i] = str2[i];
		for(int i=count;i<100;i++)
			vtkname1[i] = '\0';
		cerr<<vtkname1<<endl;
		char vtkname[100];
		sprintf(vtkname, "%s/%s", vtkstr, vtkname1);
		//delete [] vtkname1;

		cerr<<vtkname<<endl;
		cerr<<rightdatastring<<endl;

		cerr<<rightdatastring<<" "<<data_string[0]<<" "<<data_string[1]<<endl;

		if(!strcmp(data_string[0],rightdatastring))
		{
			right_data_type = 0;
		}
		else if(!strcmp(data_string[1],rightdatastring))
		{
			right_data_type = 1;
		}
		else 
		{
			if(left_data_type == right_data_type)
			{
				right_data_type = 1 - right_data_type;
			}
			sprintf(data_string[right_data_type],"%s",rightdatastring);
			sprintf(datafile[right_data_type],"%s",rightdatastring);
			strcpy(VTKfile[right_data_type], vtkname);
			cerr<<VTKfile[right_data_type]<<endl;

			Delete_Init(right_data_type);	
			qdotIsoArrow[1-right_data_type]->SetArrows(enablesample,true,qdotMag->GetMag(iso[1-right_data_type]->GetDensity(), iso[1-right_data_type]->GetPointnum(), image_width/2., image_height));
			qdotIsoArrow2[1-right_data_type]->SetArrows(enablesample,true,qdotMag->GetMag(iso[1-right_data_type]->GetDensity(), iso[1-right_data_type]->GetPointnum(), image_width/2., image_height));
			
		}

		glui_right_pop->hide();
	}

	redisplay_all();
}

void control_left(int control)
{
	if(control == Leftdata_id)
	{

		//cerr<<datastring.size()<<endl;
		for(int i=0;i<datastring.size();i++)
		{
			lefthah->delete_item(i);		
		}		
		ListFile(DataDir[0]);
		for(int i=0;i<datastring.size();i++)
		{
			char *str = new char[datastring[i].length()+1];
			strcpy(str,datastring[i].c_str());
			//cerr<<str<<endl;
			lefthah->add_item(i,str);		
			delete [] str;
		}

		glui_left_pop->show();
	}

	redisplay_all();
}

void control_right(int control)
{
	if(control == Rightdata_id)
	{

		//cerr<<datastring.size()<<endl;
		for(int i=0;i<datastring.size();i++)
		{
			righthah->delete_item(i);		
		}		
		ListFile(DataDir[0]);
		for(int i=0;i<datastring.size();i++)
		{
			char *str = new char[datastring[i].length()+1];
			strcpy(str,datastring[i].c_str());
			//cerr<<str<<endl;
			righthah->add_item(i,str);		
			delete [] str;
		}


		glui_right_pop->show();
	}

	redisplay_all();
}

void control_data1(int control)
{
	if(control == Magnum_id1)
	{
		num_magnitude_clusters[0] = magnum[0];
		Mag_Clustering(0);	
		Mag_Contour_Clustering(0);
		SetContourColor(0);			
		
	}
	else if(control == Velnum_id1)
	{

			num_orientation_clusters[0] = velnum[0];
			Vel_Clustering(0);	
			Vel_Contour_Clustering(0);
			SetContourColor(0);	
		
	}
	else if(control == Magnumin_id1)
	{
			num_magnitude_clusters_in[0] = magnumin[0];	
			Mag_Contour_ROI_Clustering(magnumin[0],0);	
			SetContourColor(0);	
				
	}
	else if(control == Velnumin_id1)
	{

			num_orientation_clusters_in[0] = velnumin[0];	
			Vel_Contour_ROI_Clustering(velnumin[0],0);
			SetContourColor(0);
			
	}
	redisplay_all();
}

void control_data2(int control)
{
	if(control == Magnum_id2)
	{
		num_magnitude_clusters[1] = magnum[1];
		Mag_Clustering(1);	
		Mag_Contour_Clustering(1);
		SetContourColor(1);			
		
	}
	else if(control == Velnum_id2)
	{

			num_orientation_clusters[1] = velnum[1];
			Vel_Clustering(1);	
			Vel_Contour_Clustering(1);
			SetContourColor(1);	
		
	}
	else if(control == Magnumin_id2)
	{
			num_magnitude_clusters_in[1] = magnumin[1];	
			Mag_Contour_ROI_Clustering(magnumin[1],1);	
			SetContourColor(1);	
				
	}
	else if(control == Velnumin_id2)
	{

			num_orientation_clusters_in[1] = velnumin[1];	
			Vel_Contour_ROI_Clustering(velnumin[1],1);
			SetContourColor(1);
			
	}
	redisplay_all();
}

void control_whole(int control)
{
	int s,e;
	s = 0; e = 2;

	if(control == MagEncode)
	{
		MagnitudeEncoding(magnitude_encoding+1);
	}
	else if(control == Loadfile_id)
	{
		LoadFile();
	}
	else if(control == Savefile_id)
	{
		for(int i=0;i<2;i++)
		{
			trackball[i].getMatrix().getValue(m[i]);	
			for(int j=0;j<16;j++)
			{
				transformation_mat[i][j] = m[i][j];
			}	
			char str[100];
			time_t rawtime;
			struct tm * timeinfo;
			time (&rawtime);
			timeinfo = localtime (&rawtime);

			sprintf(str, "%s/config_%s",LogFile,asctime(timeinfo));
			SavetoFile(str);
		}
	}
	else if(control == SavePicture_id)
	{
		for(int i=0;i<2;i++)
		{
			time_t rawtime;
			struct tm * timeinfo;

			time (&rawtime);
			timeinfo = localtime (&rawtime);

			char str[50];
			sprintf(str,"%s/%s.ppm", picturedir,asctime(timeinfo));

			CaptureScreen(str);
		}
	}
	else if(control == Projection_id)
	{
		if(enable_projection)
		{
			for(int ii=s;ii<e;ii++)
			{
				svVector4 *tmpcolor = new svVector4[iso[ii]->GetPointnum()];
				double *tmpr = new double[iso[ii]->GetPointnum()];
				double *tmpg = new double[iso[ii]->GetPointnum()];
				double *tmpb = new double[iso[ii]->GetPointnum()];
				if(projectionaggregation == true)
				{
					isocluster[ii]->GetMagColors(tmpr, tmpg, tmpb);
					for(int i=0;i<iso[ii]->GetPointnum();i++)
					{
						tmpcolor[i][0] = tmpr[i];
						tmpcolor[i][1] = tmpg[i];
						tmpcolor[i][2] = tmpb[i];
						tmpcolor[i][3] = 255.;
					}
					cutting_mag[ii]->SetColors(tmpcolor);
				}
				else
				{
					delete [] tmpcolor;
					delete [] tmpr;
					delete [] tmpg;
					delete [] tmpb;
					tmpcolor = new svVector4[projection_iso[ii]->GetPointnum()];
					tmpr = new double[projection_iso[ii]->GetPointnum()];
					tmpg = new double[projection_iso[ii]->GetPointnum()];
					tmpb = new double[projection_iso[ii]->GetPointnum()];

					isocluster[ii]->GetMagColors(projection_iso[ii]->GetPointnum(),
							inbox_mag_projection_cluster[ii],
							projection_iso[ii]->GetDoubleDensity(),tmpr, tmpg, tmpb);
					for(int i=0;i<projection_iso[ii]->GetPointnum();i++)
					{
						tmpcolor[i][0] = tmpr[i];
						tmpcolor[i][1] = tmpg[i];
						tmpcolor[i][2] = tmpb[i];
						tmpcolor[i][3] = 255.;
					}

					cutting_mag[ii]->SetColors(tmpcolor);
				}

				delete [] tmpcolor;
				delete [] tmpr;
				delete [] tmpg;
				delete [] tmpb;
				tmpcolor = new svVector4[iso[ii]->GetPointnum()];
				tmpr = new double[iso[ii]->GetPointnum()];
				tmpg = new double[iso[ii]->GetPointnum()];
				tmpb = new double[iso[ii]->GetPointnum()];

				isocluster[ii]->GetVelColors(tmpr, tmpg, tmpb);
				for(int i=0;i<iso[ii]->GetPointnum();i++)
				{
					tmpcolor[i][0] = tmpr[i];
					tmpcolor[i][1] = tmpg[i];
					tmpcolor[i][2] = tmpb[i];
					tmpcolor[i][3] = 255.;
				}
				if(projectionaggregation)
					cutting_vel[ii]->SetColors(tmpcolor);
				else
					cutting_vel[ii]->SetColors(projectionann[ii]->ANN_individual(projection_iso[ii]->GetVecDir(),
						projection_iso[ii]->GetPointnum(), 
						tmpcolor, 
						inbox_cluster_contour[ii], 
						inbox_vel_projection_cluster[ii]) );

				delete [] tmpcolor;
				delete [] tmpr;
				delete [] tmpg;
				delete [] tmpb;
			}
		}
		

	}
	else if(control == Map2_id)
	{

		for(int i=s;i<e;i++)
			PlaneComparison(i);
	}
	else if(control == Z_refer_id)
	{	

		for(int i=s;i<e;i++)
		{
			z_standard[i] = zmin;
			PlaneComparison(i);
		}
	}
/*	else if(control == Magnum_id)
	{
		for(int i=s;i<e;i++)
		{
			num_magnitude_clusters[i] = magnum;
			Mag_Clustering(i);	
			Mag_Contour_Clustering(i);
			SetContourColor(i);			
		}	
	}
	else if(control == Velnum_id)
	{

		for(int i=s;i<e;i++)
		{
			num_orientation_clusters[i] = velnum;
			Vel_Clustering(i);	
			Vel_Contour_Clustering(i);
			SetContourColor(i);	
		}
	}
	else if(control == Magnumin_id)
	{

		for(int i=s;i<e;i++)
		{
			num_magnitude_clusters_in[i] = magnumin;	
			Mag_Contour_ROI_Clustering(magnumin,i);	
			SetContourColor(i);	
		}		
	}
	else if(control == Velnumin_id)
	{

		for(int i=s;i<e;i++)
		{
			num_orientation_clusters_in[i] = velnumin;	
			Vel_Contour_ROI_Clustering(velnumin,i);
			SetContourColor(i);
		}	
	}*/
	else if(control == ROI_id)
	{

		for(int i=s;i<e;i++)
		{
			Semi_Mag(i);
		}
	}
	else if(control == Zlayer_id)
	{
		int i;
		if(lb[1][2] >= lb[0][2])
		{
			i = 0;
		}
		else
			i = 1;
		
		int j = (int)((zmin-lb[i][2])/((rb[i][2] - lb[i][2])/(qdotField[i]->GetZlayer()-1.)));
		zmin = lb[i][2] +  ((rb[i][2] - lb[i][2])/(qdotField[i]->GetZlayer()-1.)) * j;

		if(zmin<lb[1-i][2])
		{
			zmin= lb[1-i][2];
		}	
		else if(zmin<=rb[1-i][2])
		{
			zmin= zmin;
		}
		else
		{
			zmin = rb[1-i][2];
		}

		qdotcluster[1-i]->SetZ(zmin);
		qdotcluster[i]->SetZ(zmin);

		char str[20];
		sprintf(str,"%0.2f",zmin);
		obj_text_zlayer->set_text(str);	
		
	}
	else if(control == Contour_id) //need to be improved
	{
		contour_ratio[0].clear();
		contour_ratio[1].clear();
		float store2;
		//cerr>>store2;
		contour_ratio[0].push_back(store2);
		contour_ratio[1].push_back(store2);
		int i;
		ContoursGeneration(contour_ratio[i],i);
		Mag_Contour_Clustering(i);
		Mag_Contour_ROI_Clustering((int)num_magnitude_clusters_in[i],i);		
		Vel_Contour_Clustering(i);
		Vel_Contour_ROI_Clustering((int)num_orientation_clusters_in[i],i);
		Semi_Mag(i);
		SetContourColor(i);
	
	}
	else if(control == Projectionaggregation_id)
	{

		if(projectionaggregation)
		{
			for(int i=s;i<e;i++)
			{
				cutting_mag[i]->SetCut(iso[i]->GetPositon(),NULL);
				cutting_vel[i]->SetCut(iso[i]->GetPositon(),NULL);
				SetContourColor(i);
				Semi_Mag(i);
			}
		}
		else
		{
			for(int i=s;i<e;i++)
			{
				cutting_mag[i]->SetCut(projection_iso[i]->GetPositon(),NULL);
				cutting_vel[i]->SetCut(projection_iso[i]->GetPositon(),NULL);
				
				SetContourColor(i);

				delete [] inbox_mag_projection[i];
				delete [] inbox_vel_projection[i];
				inbox_mag_projection[i] = new bool[projection_iso[i]->GetPointnum()];
				inbox_vel_projection[i] = new bool[projection_iso[i]->GetPointnum()];
				delete [] inbox_mag_projection_cluster[i];
				delete [] inbox_vel_projection_cluster[i];
				inbox_mag_projection_cluster[i] = new bool[projection_iso[i]->GetPointnum()];
				inbox_vel_projection_cluster [i]= new bool[projection_iso[i]->GetPointnum()];

				Semi_Mag(i);
			}

		}
				
	}	
	else if(control == ROIBox_id)
	{
		if(enable_box)
		{
			for(int i=s;i<e;i++)
			{
				ROIden[i] = qdotBox[i]->GetROIden(qdotField[i]->GetScalar());	

				inbox_cluster_contour[i] = qdotBox[i]->InBox(iso[i]->GetVecPos(),iso[i]->GetPointnum());
				inbox_mag_cluster[i] = qdotBox[i]->InBox(qdotField[i]->GetSortPos(),qdotField[i]->GetDataSize());
			
				Mag_Contour_ROI_Clustering((int)num_magnitude_clusters_in[i],i);
		
				inbox_vel_cluster[i] = qdotBox[i]->InBox(qdotField[i]->GetSortPos(),
									qdotField[i]->GetDataSize());
				inbox_mag_projection_cluster[i] = qdotBox[i]->InBox(projection_iso[i]->GetVecPos(),
									projection_iso[i]->GetPointnum());
				inbox_vel_projection_cluster[i] = qdotBox[i]->InBox(projection_iso[i]->GetVecPos(),
									projection_iso[i]->GetPointnum());
			
				Vel_Contour_ROI_Clustering((int)num_orientation_clusters_in[i],i);

				SetContourColor(i);
				Semi_Mag(i);
			}
		}
		else 
		{
			for(int j=0;j<2;j++)
			{
				for(int i=0;i<iso[j]->GetPointnum();i++)
				{
					inbox_cluster_contour[j][i] = 0;
					inbox_mag_cluster[j][i] = 0;
					inbox_vel_cluster[j][i] = 0;
				}
				for(int i=0;i<projection_iso[j]->GetPointnum();i++)
				{
					inbox_mag_projection_cluster[j][i] = 0;
					inbox_vel_projection_cluster[j][i] = 0;
				}
			

				Mag_Contour_ROI_Clustering((int)num_magnitude_clusters_in[j],j);
				Vel_Contour_ROI_Clustering((int)num_orientation_clusters_in[j],j);
				SetContourColor(j);
				Semi_Mag(j);
			}
		}

	}
	else if(control == Syncdata_id && enable_data_sync)
	{

		qdotBox[1-left_data_type]->SetPoints(qdotBox[left_data_type]->GetIntersectionp(),
								qdotBox[left_data_type]->GetWidget()); 
		qdotSlice[1-left_data_type]->SetPoints(qdotSlice[left_data_type]->GetWidget(),
							qdotSlice[left_data_type]->GetDir(),
							qdotSlice[left_data_type]->GetDir2());

		qdotBox[1-left_data_type]->SetDir2(qdotSlice[1-left_data_type]->GetDir2());
		qdotBox[1-left_data_type]->SetDir(qdotSlice[1-left_data_type]->GetDir()); 
		qdotSlice[1-left_data_type]->SetPlaneXYZ();
		

	}
	redisplay_all();
}

void Slider_mouse(int button, int state, int x, int y)
{
		if((mouse_button == GLUT_LEFT_BUTTON) && (mouse_state==GLUT_DOWN)) 
		{
			mouse_y = y;
			mouse_x = x;
			mouse_y2 = y;
			mouse_x2 = x;

			if(!qdotMag->SELECTED) {qdotMag->Find_Nearest(x, y);}
			if(!qdotMag->SELECTED)
			{
				if(x>=Widget_z_mag[0][0]&&x<=Widget_z_mag[1][0]
				&& (image_height-y)>=Widget_z_mag[0][1]+Widget_t_mag[0]
				&& (image_height-y)<=Widget_z_mag[1][1]+Widget_t_mag[0])
				{
					Widget_select_mag[0] = true;//cerr<<Widget_select_mag[0]<<endl;
					Widget_flag = true;
				}
				else if(x>=Widget_z_mag[2][0]&&x<=Widget_z_mag[3][0]
				&& (image_height-y)>=Widget_z_mag[2][1]+Widget_t_mag[1]
				&& (image_height-y)<=Widget_z_mag[3][1]+Widget_t_mag[1])
				{
					Widget_select_mag[1] = true;
					Widget_flag = true;
				}
			}
			else
			{	
				Widget_flag = true;
			}
			if(!Widget_select_mag[0]&&!Widget_select_mag[1]&&!qdotMag->SELECTED)
			{//cerr<<"B"<<endl;
				if(x>=Widget_z_angle[0][0]&&x<=Widget_z_angle[1][0]
				&& (image_height-y)>=Widget_z_angle[0][1]+Widget_t_zangle[0]
				&& (image_height-y)<=Widget_z_angle[1][1]+Widget_t_zangle[0])
				{
					Widget_select_zangle[0] = true;
					Widget_flag = true;
					if(x>=Widget_z_angle[2][0]&&x<=Widget_z_angle[3][0]
					&& (image_height-y)>=Widget_z_angle[2][1]+Widget_t_zangle[1]
					&& (image_height-y)<=Widget_z_angle[3][1]+Widget_t_zangle[1])
					{
						if(zangle_min == 0.);
						{
							Widget_select_zangle[1] = true;
							Widget_flag = true;
							Widget_select_zangle[0] = false;
						}
					}

				}
				else if(x>=Widget_z_angle[2][0]&&x<=Widget_z_angle[3][0]
				&& (image_height-y)>=Widget_z_angle[2][1]+Widget_t_zangle[1]
				&& (image_height-y)<=Widget_z_angle[3][1]+Widget_t_zangle[1])
				{
					Widget_select_zangle[1] = true;
					Widget_flag = true;
				}
				else
				{
					//bool flag = false;
					//cerr<<"A"<<endl;
					for(int i=0;i<Widget_xy_min[0].size();i++)
					{//cerr<<x<<" "<<Widget_xy_min[0][i]+Widget_xy_w-5.<<" "<<Widget_xy_min[0][i]+Widget_xy_w+5.<<" "<<image_height-y<<" "<<Widget_xy_min[1][i]+Widget_xy_h-5.<<" "<<Widget_xy_min[1][i]+Widget_xy_h+5.<<endl;
						if(x>=Widget_xy_min[0][i]+Widget_xy_w-5.
						&&x<=Widget_xy_min[0][i]+Widget_xy_w+5.
						&&(image_height-y)>=Widget_xy_min[1][i]+Widget_xy_h-5.
						&&(image_height-y)<=Widget_xy_min[1][i]+Widget_xy_h+5.)
						{
							Widget_select_xy[i] = true;
							Widget_select_xy_whole=true;
							Widget_flag = true;
							Widget_xy_theta[0] = x - Widget_xy_center[0] - Widget_xy_w;
							Widget_xy_theta[1] = image_height - y - Widget_xy_center[1] - Widget_xy_h;
							float sum = 	Widget_xy_theta[0]*Widget_xy_theta[0]
								+ 	Widget_xy_theta[1]*Widget_xy_theta[1];
							sum = sqrt(sum);
							Widget_xy_theta[0] = Widget_xy_radius * Widget_xy_theta[0]/sum;
							Widget_xy_theta[1] = Widget_xy_radius * Widget_xy_theta[1]/sum;	
							//cerr<<Widget_xy_theta[0]<<" "<<Widget_xy_theta[1]<<endl;		
							break;
						}
					}
					if(!Widget_select_xy_whole)
					for(int i=0;i<Widget_xy_max[0].size();i++)
					{
						if(x>=Widget_xy_max[0][i]+Widget_xy_w-5.
						&&x<=Widget_xy_max[0][i]+Widget_xy_w+5.
						&&(image_height-y)>=Widget_xy_max[1][i]+Widget_xy_h-5.
						&&(image_height-y)<=Widget_xy_max[1][i]+Widget_xy_h+5.)
						{
							Widget_select_xy[i+Widget_xy_min[0].size()] = true;
							Widget_select_xy_whole=true;
							Widget_flag = true;
							Widget_xy_theta[0] = x - Widget_xy_center[0] - Widget_xy_w;
							Widget_xy_theta[1] = image_height - y - Widget_xy_center[1] - Widget_xy_h;
							float sum = 	Widget_xy_theta[0]*Widget_xy_theta[0]
								+ 	Widget_xy_theta[1]*Widget_xy_theta[1];
							sum = sqrt(sum);
							Widget_xy_theta[0] = Widget_xy_radius * Widget_xy_theta[0]/sum;
							Widget_xy_theta[1] = Widget_xy_radius * Widget_xy_theta[1]/sum;	
							//cerr<<Widget_xy_theta[0]<<" "<<Widget_xy_theta[1]<<endl;
							break;
						}
					}
				}
			}
		}
		if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		{
			if(qdotMag->SELECTED==GL_TRUE)
			{
				enablesample = false;
				for(int i=0;i<2;i++)
				{
					qdotIsoArrow[i]->SetArrows(enablesample,true,
							qdotMag->GetMag(iso[i]->GetDensity(), 
							iso[i]->GetPointnum(), image_width/2., image_height));
					qdotIsoArrow2[i]->SetArrows(enablesample,true,
							qdotMag->GetMag(iso[i]->GetDensity(), 
							iso[i]->GetPointnum(), image_width/2., image_height));
				}
				//cerr<<"It is done to redraw the contours."<<endl;
			}
			else if(Widget_select_mag[0]||Widget_select_mag[1])
			{
				for(int i=0;i<2;i++)
				{
					Semi_Mag(i);
				}
			}
			else if((Widget_select_xy_whole||Widget_select_zangle[0]||Widget_select_zangle[1]))
			{
				for(int i=0;i<2;i++)
				{
					Semi_Mag(i);
				}
			}
			qdotMag->SELECTED = GL_FALSE;
			qdotMag->NEAR = GL_FALSE;	
			Widget_select_mag[0] = false;
			Widget_select_mag[1] = false;
			Widget_select_zangle[0]=false;
			Widget_select_zangle[1] = false;
			Widget_select_xy_whole = false;
			Widget_flag = false;
			for(int i=0;i<Widget_select_xy.size();i++)
				Widget_select_xy[i] = false;
		}
   	//redisplay_all();
}


void contour_mouse(int button, int state, int x, int y, int data_type)
{
	mouse_button = button;
	mouse_state = state;
	long s=0x00000000;
	mouse_x = x;
	mouse_y = y;
			s |= (button == GLUT_LEFT_BUTTON)   ? ((state == GLUT_DOWN) ? Trackball::LBUTTON_DOWN : Trackball::LBUTTON_UP) : 0;
	       		  s |= (button == GLUT_MIDDLE_BUTTON) ? ((state == GLUT_DOWN) ? Trackball::MBUTTON_DOWN : Trackball::MBUTTON_UP) : 0;
			  s |= (button == GLUT_RIGHT_BUTTON)  ? ((state == GLUT_DOWN) ? Trackball::RBUTTON_DOWN : Trackball::RBUTTON_UP) : 0;

			int key_state = glutGetModifiers();
			s |= (key_state & GLUT_ACTIVE_CTRL)  ? Trackball::CTRL_DOWN  : 0;
			s |= (key_state & GLUT_ACTIVE_ALT)   ? Trackball::ALT_DOWN   : 0;
			s |= (key_state & GLUT_ACTIVE_SHIFT) ? Trackball::SHIFT_DOWN : 0;

			 if (s & Trackball::BUTTON_DOWN){
				  trackball[data_type].mouseDown(s, x, y);
				if(enable_data_sync)
					trackball[1-data_type] = trackball[data_type];
			}

		if (s & Trackball::BUTTON_UP){
			 trackball[data_type].mouseUp(s, x, y);
			if(enable_data_sync)
 				trackball[1-data_type] = trackball[data_type];
		 }

		if((mouse_button == GLUT_LEFT_BUTTON) && (mouse_state==GLUT_DOWN)) 
		{

			GetMatrix(data_type);
			GetMatrix(1-data_type);

			if(!Widget_flag)	
			{
				if(!qdotBox[data_type]->SELECTED)
				{ 
					qdotBox[data_type]->Find_Nearest(x, y);
					if(qdotBox[data_type]->SELECTED)
						ifstop=true;
				}
				
			}
			if(!qdotBox[data_type]->SELECTED)
			{
				if(!qdotSlice[data_type]->SELECTED)
				{ 
					qdotSlice[data_type]->Find_Nearest(x, y);
				}
			}


			
		}
		if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		{
			mousestop = 0;
			ifstop = false;

			if(qdotBox[data_type]->SELECTED&&enable_box)
			{
				int s, e;
				if(enable_data_sync)
				{	
					s = 0;e = 2;
				}
				else
				{
					s = data_type; e = s + 1;
				}
				for(int i=s;i<e;i++)
				{
					ROIden[i] = qdotBox[i]->GetROIden(qdotField[i]->GetScalar());	

					inbox_cluster_contour[i] = qdotBox[i]->InBox(iso[i]->GetVecPos(),
										iso[i]->GetPointnum());
					inbox_mag_cluster[i] = qdotBox[i]->InBox(qdotField[i]->GetSortPos(),qdotField[i]->GetDataSize());

					Mag_Contour_ROI_Clustering((int)num_magnitude_clusters_in[i],i);
					inbox_vel_cluster[i] = qdotBox[i]->InBox(qdotField[i]->GetSortPos(),qdotField[i]->GetDataSize());
					inbox_mag_projection_cluster[i] = qdotBox[i]->InBox(projection_iso[i]->GetVecPos(),projection_iso[i]->GetPointnum());
					inbox_vel_projection_cluster[i] = qdotBox[i]->InBox(projection_iso[i]->GetVecPos(),projection_iso[i]->GetPointnum());

					Vel_Contour_ROI_Clustering((int)num_orientation_clusters_in[i],i);
					SetContourColor(i);
					Semi_Mag(i);
				}
			}

			if(qdotSlice[data_type]->SELECTED&&enable_cuttingplane)
 			{
			  if(qdotSlice[data_type]->SELECTED_WHITE)
			  {
				int s, e;
				if(enable_data_sync)
				{	
					s = 0;e = 2;
				}
				else
				{
					s = data_type; e = s + 1;
				}
				for(int ii=s;ii<e;ii++)
				{
					  mindistance = qdotSlice[ii]->GetSliceXY(qdotField[ii]->GetXYplane(),
							qdotField[ii]->GetXZplane(),qdotField[ii]->GetYZplane(),
							qdotField[ii]->GetXYsample(),qdotField[ii]->GetXZsample(),
							qdotField[ii]->GetYZsample());  
					  qdotSlice[ii]->GetCutPos(slicePos[ii]);
					  TwoDnum[ii] = qdotSlice[ii]->GetSelectedNum();
					char str[50];
					sprintf(str,"%s%s",tmpfiledir, datafile[data_type]);
					  qdotField[ii]->Savetofile(str, slicePos[ii], TwoDnum[ii]);//need to be improved!
					  projection_iso[ii]->NewContours();

					for(int j=0;j<contour_ratio[ii].size();j++)
					{
						double ra = contour_ratio[ii][j];
						cerr<<"The ratio is "<<ra<<endl;//cerr<<tmpvalue<<endl;
						for(int i=0;i<qdotField[ii]->GetZlayer();i++)
						{
							sprintf(vtkout, "%s%sprojection%d",tmpfiledir,datafile[ii],i);
							svScalar tmpd = ra*qdotField[ii]->GetDenMax(i);//cerr<<qdotField->GetDenMax(i)<<endl;
							//cerr<<tmpd<<endl;
							projection_iso[ii]->GetLineContour(vtkout, tmpd);
							if(projection_iso[ii]->Addnewcontour())
								projection_iso[ii]->SetZnum(i);
						}
					}

					if(!projectionaggregation)
					{
						cutting_mag[ii]->SetCut(projection_iso[ii]->GetPositon(),NULL);
						cutting_vel[ii]->SetCut(projection_iso[ii]->GetPositon(),NULL);	
					}
				}
			  }
				int s, e;
				if(enable_data_sync)
				{	
					s = 0;e = 2;
				}
				else
				{
					s = data_type; e = s + 1;
				}
				for(int ii=s;ii<e;ii++)
				{
				ROIden[ii] = qdotBox[ii]->GetROIden(qdotField[ii]->GetScalar());	

				inbox_cluster_contour[ii] = qdotBox[ii]->InBox(iso[ii]->GetVecPos(),iso[ii]->GetPointnum());
				inbox_mag_cluster[ii] = qdotBox[ii]->InBox(qdotField[ii]->GetSortPos(),qdotField[ii]->GetDataSize());

				Mag_Contour_ROI_Clustering((int)num_magnitude_clusters_in[ii],ii);
	
				inbox_vel_cluster[ii] = qdotBox[ii]->InBox(qdotField[ii]->GetSortPos(),qdotField[ii]->GetDataSize());
				inbox_mag_projection_cluster[ii] = qdotBox[ii]->InBox(projection_iso[ii]->GetVecPos(),projection_iso[ii]->GetPointnum());
				inbox_vel_projection_cluster[ii] = qdotBox[ii]->InBox(projection_iso[ii]->GetVecPos(),projection_iso[ii]->GetPointnum());

				Vel_Contour_ROI_Clustering((int)num_orientation_clusters_in[ii],ii);

				SetContourColor(ii);
				delete []inbox_mag_projection[ii];
				delete [] inbox_vel_projection[ii];
				inbox_mag_projection[ii] = new bool[projection_iso[ii]->GetPointnum()];
				inbox_vel_projection[ii] = new bool[projection_iso[ii]->GetPointnum()];
				delete []inbox_mag_projection_cluster[ii];
				delete [] inbox_vel_projection_cluster[ii];
				inbox_mag_projection_cluster[ii] = new bool[projection_iso[ii]->GetPointnum()];
				inbox_vel_projection_cluster[ii] = new bool[projection_iso[ii]->GetPointnum()];
				Semi_Mag(ii);
				}

		}


		

		qdotSlice[data_type]->SELECTED = GL_FALSE;
		qdotSlice[data_type]->SELECTED_WHITE = GL_FALSE;
		qdotSlice[data_type]->SELECTED_BLUE = GL_FALSE;
		qdotOutline[data_type]->SELECTED = GL_FALSE;
		qdotOutline[data_type]->SELECTED_CONTOUR = GL_FALSE;
		qdotBox[data_type]->SELECTED = GL_FALSE;		
	}

}


//improved!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
bool enable_sep = false;

void Slider_motion(int x, int y)
{
		if(Widget_select_mag[0])
		{
			float delta_y = -y + mouse_y;
			svVector3 Widget_center;
			Widget_center[1] = (Widget_z_mag[0][1]+Widget_z_mag[1][1])/2.+Widget_t_mag[0]+delta_y;
			svVector3 Widget_center2;
			Widget_center2[1] = (Widget_z_mag[2][1]+Widget_z_mag[3][1])/2.+Widget_t_mag[1];
			if(Widget_center[1] < Widget_bound_mag[0][1])
			{
				Widget_center[1] = Widget_bound_mag[0][1];
				delta_y = 0;
			}
			//cerr<<delta_y<<" "<<y<<" "<<mouse_y<<endl;
			if(Widget_center[1] >= Widget_bound_mag[0][1]
			&& Widget_center[1] <= Widget_center2[1])
			{
				Widget_t_mag[0] = Widget_t_mag[0] + delta_y;
				//Widget_center[1] = (Widget_z_mag[0][1]+Widget_z_mag[1][1])/2.+Widget_t_mag[0];
				range_mag[0] = Widget_center[1] - Widget_bound_mag[0][1];
				if(min_density==0)
					range_mag[0] =min_density+ mag_distance* range_mag[0]/(Widget_bound_mag[1][1]-Widget_bound_mag[0][1]);
				else
				range_mag[0] =log10(min_density)+ mag_distance* range_mag[0]/(Widget_bound_mag[1][1]-Widget_bound_mag[0][1]);
				range_mag[0] = pow(10., range_mag[0]);
				//range_mag[1][0] = range_mag[0][0];

			}
			//cerr<<zangle_min<<endl;
		}
		else if(Widget_select_mag[1])
		{
			float delta_y = -y + mouse_y;
			svVector3 Widget_center;
			Widget_center[1] = (Widget_z_mag[2][1]+Widget_z_mag[3][1])/2.+Widget_t_mag[1]+delta_y;
			svVector3 Widget_center2;
			Widget_center2[1] = (Widget_z_mag[0][1]+Widget_z_mag[1][1])/2.+Widget_t_mag[0];
			if(Widget_center[1] > Widget_bound_mag[1][1])
			{
				Widget_center[1] = Widget_bound_mag[1][1];
				delta_y = 0;
			}
			if(Widget_center[1] >= Widget_center2[1]
			&& Widget_center[1] <= Widget_bound_mag[1][1])
			{
				Widget_t_mag[1] = Widget_t_mag[1] + delta_y;
				//Widget_center[1] = (Widget_z_mag[2][1]+Widget_z_mag[3][1])/2.+Widget_t_mag[1];
				range_mag[1] = Widget_center[1] - Widget_bound_mag[0][1];
				//cerr<<mag_distance<<" "<<range_mag[1][1]<<" "<<Widget_bound_mag[1][1]-Widget_bound_mag[0][1]<<endl;
				if(min_density==0)
				range_mag[1] = min_density+ fabs(mag_distance)* range_mag[1]/(Widget_bound_mag[1][1]-Widget_bound_mag[0][1]);
				else
				range_mag[1] = log10(min_density)+ mag_distance* range_mag[1]/(Widget_bound_mag[1][1]-Widget_bound_mag[0][1]);
				range_mag[1] = pow(10., range_mag[1]);
				if(fabs(Widget_center[1] - Widget_bound_mag[1][1])<0.0001)
					range_mag[1] = max_density;
				//range_mag[0][1] = range_mag[1][1];
			}
		}
        	else if(qdotMag->SELECTED) 
		{
            		qdotMag->move_ctrlpoint(x, y, image_width/2., image_height);

			enablesample = true;
			for(int i=0;i<2;i++)
			{
			qdotIsoArrow[i]->SetArrows(enablesample,true,qdotMag->GetMag(iso[i]->GetDensity(), iso[i]->GetPointnum(), image_width/2., image_height));
			qdotIsoArrow2[i]->SetArrows(enablesample,true,qdotMag->GetMag(iso[i]->GetDensity(), iso[i]->GetPointnum(), image_width/2., image_height));
			}

        	}
		else if(Widget_select_zangle[0])
		{
			float delta_y = -y + mouse_y2;
			svVector3 Widget_center;
			Widget_center[0] = (Widget_z_angle[0][0]+Widget_z_angle[1][0])/2.+Widget_t_zangle[0]+delta_y;
			Widget_center[1] = (Widget_z_angle[0][1]+Widget_z_angle[1][1])/2.+Widget_t_zangle[0]+delta_y;
			svVector3 Widget_center2;
			Widget_center2[0] = (Widget_z_angle[2][0]+Widget_z_angle[3][0])/2.+Widget_t_zangle[1];
			Widget_center2[1] = (Widget_z_angle[2][1]+Widget_z_angle[3][1])/2.+Widget_t_zangle[1];

			if(Widget_center[1] >= Widget_bound_zangle[0][1]
			&& Widget_center[1] <= Widget_center2[1])
			{
				Widget_t_zangle[0] = Widget_t_zangle[0] + delta_y;
				Widget_center[1] = (Widget_z_angle[0][1]+Widget_z_angle[1][1])/2.+Widget_t_zangle[0];
				zangle_min = Widget_center[1] - Widget_bound_zangle[0][1];
				zangle_min = zangle_min * 180./(Widget_bound_zangle[1][1]-Widget_bound_zangle[0][1]);
			}
			//cerr<<zangle_min<<endl;
		}
		else if(Widget_select_zangle[1])
		{
			float delta_y = -y + mouse_y2;
			svVector3 Widget_center;
			Widget_center[0] = (Widget_z_angle[2][0]+Widget_z_angle[3][0])/2.+Widget_t_zangle[1]+delta_y;
			Widget_center[1] = (Widget_z_angle[2][1]+Widget_z_angle[3][1])/2.+Widget_t_zangle[1]+delta_y;
			svVector3 Widget_center2;
			Widget_center2[0] = (Widget_z_angle[0][0]+Widget_z_angle[1][0])/2.+Widget_t_zangle[0];
			Widget_center2[1] = (Widget_z_angle[0][1]+Widget_z_angle[1][1])/2.+Widget_t_zangle[0];

			if(Widget_center[1] >= Widget_center2[1]
			&& Widget_center[1] <= Widget_bound_zangle[1][1])
			{
				Widget_t_zangle[1] = Widget_t_zangle[1] + delta_y;
				Widget_center[1] = (Widget_z_angle[2][1]+Widget_z_angle[3][1])/2.+Widget_t_zangle[1];
				zangle_max = -Widget_center[1] + Widget_bound_zangle[1][1];
				zangle_max = 180. -  zangle_max * 180./(Widget_bound_zangle[1][1]-Widget_bound_zangle[0][1]);
			}
		}	

		else if(Widget_select_xy_whole)
		{

			int j;

			double theta[2], theta1[2];
			theta[0] = x - Widget_xy_center[0] - Widget_xy_w;
			theta[1] = image_height - y - Widget_xy_center[1] - Widget_xy_h;
			float sum = theta[0]*theta[0]
					+theta[1]*theta[1];
			sum = sqrt(sum);
			theta[0] = Widget_xy_radius * theta[0]/sum; 
			//theta1[0] = theta[0] - Widget_xy_theta[0];
			theta[1] = Widget_xy_radius * theta[1]/sum; 
			//theta1[1] = theta[1] - Widget_xy_theta[1];
			float delta_angle;
			float delta_rotate = GetAngle(theta[0],theta[1]) - GetAngle(Widget_xy_theta[0], Widget_xy_theta[1]);
			//cerr<<theta1[0]<<" "<<theta1[1]<<endl;	

			for(int i=0;i<Widget_select_xy.size();i++)
			{
				if(Widget_select_xy[i])
				{j=i;break;}
			}
			if(j>=Widget_xy_min[0].size())
			{
				
				j = j-Widget_xy_min[0].size();//cerr<<xy_max[j]<<" "<<xy_min[j]<<endl;
				float x0 = Widget_xy_max[0][j]*cos(delta_rotate) - Widget_xy_max[1][j]*sin(delta_rotate);// + theta1[0]
				float y0 = Widget_xy_max[0][j]*sin(delta_rotate) + Widget_xy_max[1][j]*cos(delta_rotate);// + theta1[1];		
				float xy = GetAngle(x0,y0);
				delta_angle = xy - xy_max[j];
				if(fabs(delta_angle)<M_PI&&!(fabs(xy_min[j])<=0.00001))
				{
					if((xy-xy_min[j])*(xy_max[j]-xy_min[j])>0.)
					{
						Widget_xy_max[0][j] = x0;//Widget_xy_radius * d[0];
						Widget_xy_max[1][j] = y0;//Widget_xy_radius * d[1];
						xy_max[j] = xy;//GetAngle(Widget_xy_max[0][j],Widget_xy_max[1][j] );
					}//cerr<<xy_max[j]<<" "<<xy_min[j]<<endl;
				}
				else if(fabs(xy_min[j])<=0.00001)
				{
					if(fabs(delta_angle)<M_PI)
					{
						Widget_xy_max[0][j] = x0;//Widget_xy_radius * d[0];
						Widget_xy_max[1][j] = y0;//Widget_xy_radius * d[1];
						xy_max[j] = xy;//GetAngle(Widget_xy_max[0][j],Widget_xy_max[1][j] );
					}					
				}
				else
				{
						Widget_xy_max[0][j] = x0;//Widget_xy_radius * d[0];
						Widget_xy_max[1][j] = y0;//Widget_xy_radius * d[1];
						xy_max[j] = xy;//GetAngle(Widget_xy_max[0][j],Widget_xy_max[1][j] );
				}

			}
			else
			{
				float x0 = Widget_xy_min[0][j]*cos(delta_rotate) - Widget_xy_min[1][j]*sin(delta_rotate);// + theta1[0];
				float y0 = Widget_xy_min[0][j]*sin(delta_rotate) + Widget_xy_min[1][j]*cos(delta_rotate);// + theta1[1];			
				float xy = GetAngle(x0,y0);
				delta_angle = xy - xy_min[j];
				if(fabs(delta_angle)<M_PI&&!(fabs(xy_max[j])<=0.00001))
				{
					if((xy-xy_max[j])*(xy_min[j]-xy_max[j])>0.)
					{
						Widget_xy_min[0][j] = x0;//Widget_xy_radius * d[0];
						Widget_xy_min[1][j] = y0;//Widget_xy_radius * d[1];
						xy_min[j] = xy;//GetAngle(Widget_xy_max[0][j],Widget_xy_max[1][j] );
					}//cerr<<xy_max[j]<<" "<<xy_min[j]<<endl;
				}
				else if(fabs(xy_max[j])<=0.00001)
				{
					if(fabs(delta_angle)<M_PI)
					{
						Widget_xy_min[0][j] = x0;//Widget_xy_radius * d[0];
						Widget_xy_min[1][j] = y0;//Widget_xy_radius * d[1];
						xy_min[j] = xy;//GetAngle(Widget_xy_max[0][j],Widget_xy_max[1][j] );
					}					
				}
				else
				{
						Widget_xy_min[0][j] = x0;//Widget_xy_radius * d[0];
						Widget_xy_min[1][j] = y0;//Widget_xy_radius * d[1];
						xy_min[j] = xy;//GetAngle(Widget_xy_max[0][j],Widget_xy_max[1][j] );
				}
			}
			Widget_xy_theta[0] = theta[0];
			Widget_xy_theta[1] = theta[1];		
	
	
		}

	mouse_y = y;
	mouse_x = x;
	mouse_y2 = y;
	mouse_x2 = x;
	
   	//redisplay_all();
}

void
contour_motion(int x, int y, int data_type)
{
	    if((mouse_button == GLUT_LEFT_BUTTON) && (mouse_state==GLUT_DOWN))
	    {
		if(Widget_flag)
		{	
			//cerr<<"1"<<endl;
        	}
		else if(qdotBox[data_type]->SELECTED&&enable_box)
		{
			//cerr<<"2"<<endl;
			qdotBox[data_type]->move_ctrlpoint(x, y); 
			if(enable_data_sync)
			{
				qdotBox[1-data_type]->SetPoints(qdotBox[data_type]->GetIntersectionp(),
								qdotBox[data_type]->GetWidget()); 

			}
			 
		}
		else if(qdotSlice[data_type]->SELECTED&&enable_cuttingplane)
		{
			//cerr<<"3"<<endl;
			ifstop = true;
			qdotSlice[data_type]->move_ctrlpoint(x, y);
			if(enable_data_sync)
			{
				qdotSlice[1-data_type]->SetPoints(qdotSlice[data_type]->GetWidget(),
							qdotSlice[data_type]->GetDir(),
							qdotSlice[data_type]->GetDir2());
			}							
			if(qdotSlice[data_type]->SELECTED_BLUE)
			{
				qdotBox[data_type]->SetDir2(qdotSlice[data_type]->GetDir2());
				if(enable_data_sync)
					qdotBox[1-data_type]->SetDir2(qdotSlice[1-data_type]->GetDir2());
			}
			else
			{
				  qdotBox[data_type]->SetDir(qdotSlice[data_type]->GetDir()); 
				  qdotSlice[data_type]->SetPlaneXYZ();
				if(enable_data_sync)		
				{
				  qdotBox[1-data_type]->SetDir(qdotSlice[1-data_type]->GetDir()); 
				  qdotSlice[1-data_type]->SetPlaneXYZ();
				}
			}
		}
		else 
		{
			//cerr<<"4 "<<enable_sync<<endl;

				//cerr<<"5"<<endl;
				trackball_legend[data_type].mouseMotion(x, y);
				//cerr<<"!!"<<endl;
				trackball[data_type].mouseMotion(x, y);
			
			if(enable_data_sync)
			{	
					trackball_legend[1-data_type]=trackball_legend[data_type];
					trackball[1-data_type] = trackball[data_type];			

			}
		}
	   }
	   else
	   {
			trackball_legend[data_type].mouseMotion(x, y);
			trackball[data_type].mouseMotion(x, y);
			if(enable_data_sync)
			{	
					trackball_legend[1-data_type]=trackball_legend[data_type];
					trackball[1-data_type] = trackball[data_type];			

			}
	   }


   // redisplay_all();

}

int twod_mouse_state;

void
twodglyph_mouse(int button, int state, int x, int y, int data_type)
{
	mouse_button = button;mouse_state = state;
	twod_mouse_state = state;
	long s=0x00000000;

		  s |= (button == GLUT_LEFT_BUTTON)   ? ((state == GLUT_DOWN) ? Trackball::LBUTTON_DOWN : Trackball::LBUTTON_UP) : 0;
       		  s |= (button == GLUT_MIDDLE_BUTTON) ? ((state == GLUT_DOWN) ? Trackball::MBUTTON_DOWN : Trackball::MBUTTON_UP) : 0;
		  s |= (button == GLUT_RIGHT_BUTTON)  ? ((state == GLUT_DOWN) ? Trackball::RBUTTON_DOWN : Trackball::RBUTTON_UP) : 0;

		int key_state = glutGetModifiers();
		s |= (key_state & GLUT_ACTIVE_CTRL)  ? Trackball::CTRL_DOWN  : 0;
		s |= (key_state & GLUT_ACTIVE_ALT)   ? Trackball::ALT_DOWN   : 0;
		s |= (key_state & GLUT_ACTIVE_SHIFT) ? Trackball::SHIFT_DOWN : 0;


				  if (s & Trackball::BUTTON_DOWN)
				{
				    trackball_twod[data_type].mouseDown(s, x, y);
				    trackball_legend[data_type].mouseDown(s, x, y);
				if(enable_data_sync)
				{	
					trackball_legend[1-data_type]=trackball_legend[data_type];
					trackball_twod[1-data_type] = trackball_twod[data_type];			

				}
				  }

 				if (s & Trackball::BUTTON_UP)
				{
			   		 trackball_twod[data_type].mouseUp(s, x, y);
			   		 trackball_legend[data_type].mouseUp(s, x, y);
					if(enable_data_sync)
					{	
					trackball_legend[1-data_type]=trackball_legend[data_type];
					trackball_twod[1-data_type] = trackball_twod[data_type];			

					}
				}

			if(button == GLUT_LEFT_BUTTON
			&&state==GLUT_DOWN)
			{
				twod_mouse_x = x;
				twod_mouse_y = y;

				float s_x;
				float s_y;
				s_x = (float)x -image_width/4.;
				s_y = image_height - y;
				s_y = (float)s_y - image_height/2.;
				s_x = s_x - twotranx[data_type];
				s_y = s_y - twotrany[data_type];
				s_x = (s_x/twodzoom[data_type]);///distance_scale[data_type];
				s_y = (s_y/twodzoom[data_type]);///distance_scale[data_type];
				if(enable_select2d)
				qdotmap[data_type]->SelectPoint(s_x, s_y, qdotdistance[data_type]->GetLayerPos(),qdotdistance[data_type]->GetLayerDis(),
		qdotField[data_type]->GetZlayer(),qdotdistance[data_type]->GetXd(),qdotdistance[data_type]->GetYd(), z_standard[data_type]);

	
			}

	//redisplay_all();
}

void 
twodglyph_motion(int x, int y, int data_type)
{
	if((mouse_button == GLUT_LEFT_BUTTON) && (mouse_state==GLUT_DOWN))
	    {
			if(twod_mouse_state==GLUT_DOWN
			&&twod_mouse_y>=image_height*7./8. 
			&& twod_mouse_y<=image_height
			&& !Widget_flag)
			{
				trackball_legend[data_type].mouseMotion(x, y);
			}
				trackball_twod[data_type].mouseMotion(x, y);	
			panintwod[data_type] = trackball_twod[data_type].getPanMatrix();	
			twotranx[data_type] = panintwod[data_type][0][3]/2.;
			twotrany[data_type] = panintwod[data_type][1][3]/2.;
			if(enable_data_sync)
			{
				trackball_legend[1-data_type]=trackball_legend[data_type];
				trackball_twod[1-data_type]=trackball_twod[data_type];	
				panintwod[1-data_type] = trackball_twod[1-data_type].getPanMatrix();	
				twotranx[1-data_type] = panintwod[1-data_type] [0][3]/2.;
				twotrany[1-data_type] = panintwod[1-data_type] [1][3]/2.;			
			}
			//cerr<<twotranx<<" "<<twotrany<<endl;
		
			
	   }
	   else
	   {
			if(twod_mouse_state==GLUT_DOWN
			&&twod_mouse_y>=image_height*7./8. 
			&& twod_mouse_y<=image_height
			&& !Widget_flag)
			{
				trackball_legend[data_type].mouseMotion(x, y);
			}
			trackball_twod[data_type].mouseMotion(x, y);
			zoomintwod[data_type] = trackball_twod[data_type].getZoomMatrix();
			twodzoom[data_type] = zoomintwod[data_type][2][3]/10.;
			if(twodzoom[data_type]<1) twodzoom[data_type]=1;
			if(enable_data_sync)
			{
				trackball_legend[1-data_type]=trackball_legend[data_type];
				trackball_twod[1-data_type]=trackball_twod[data_type];
				zoomintwod[1-data_type] = trackball_twod[1-data_type].getZoomMatrix();
				twodzoom[1-data_type] = zoomintwod[1-data_type][2][3]/10.;
				if(twodzoom[1-data_type]<1) twodzoom[1-data_type]=1;
			}

	    }
	twod_mouse_y2 = y;
}

void Slider_movement(int x, int y)
{
	if(qdotMag->Find_Near(x, y))
		qdotMag->NEAR = GL_TRUE;
	else	qdotMag->NEAR = GL_FALSE;


		//cerr<<x<<" "<<y<<" "<<Widget_z[0][0]<<" "<<Widget_z[0][1]<<endl;

	if(x>=Widget_z_mag[0][0]&&x<=Widget_z_mag[1][0]
	&& (image_height-y)>=Widget_z_mag[0][1]+Widget_t_mag[0]
	&& (image_height-y)<=Widget_z_mag[1][1]+Widget_t_mag[0])
	{
		Widget_select_mag[0] = true;
	}
	else
		Widget_select_mag[0] = false;
	
	if(x>=Widget_z_mag[2][0]&&x<=Widget_z_mag[3][0]
	&& (image_height-y)>=Widget_z_mag[2][1]+Widget_t_mag[1]
	&& (image_height-y)<=Widget_z_mag[3][1]+Widget_t_mag[1])
	{
		Widget_select_mag[1] = true;
	}
	else
		Widget_select_mag[1] = false;

	if(x>=Widget_z_angle[0][0]&&x<=Widget_z_angle[1][0]
	&& (image_height-y)>=Widget_z_angle[0][1]+Widget_t_zangle[0]
	&& (image_height-y)<=Widget_z_angle[1][1]+Widget_t_zangle[0])
	{
		Widget_select_zangle[0] = true;
	}
	else
		Widget_select_zangle[0] = false;
	
	if(x>=Widget_z_angle[2][0]&&x<=Widget_z_angle[3][0]
	&& (image_height-y)>=Widget_z_angle[2][1]+Widget_t_zangle[1]
	&& (image_height-y)<=Widget_z_angle[3][1]+Widget_t_zangle[1])
	{
		Widget_select_zangle[1] = true;
	}
	else
		Widget_select_zangle[1] = false;

   	//redisplay_all();
}

void 
contour_movement(int x, int y, int data_type)
{

	if(qdotBox[data_type]->Find_Near(x, y))
		qdotBox[data_type]->NEAR = GL_TRUE;
	else	qdotBox[data_type]->NEAR = GL_FALSE;


   	//redisplay_all();
}


void left_mouse(int button, int state, int x, int y)
{
	if(left_display_type==0 || left_display_type==1)
	{
		contour_mouse(button, state, x, y, left_data_type);
	}
	else if(left_display_type==2)
	{
		twodglyph_mouse(button, state, x, y, left_data_type);
	}

 	redisplay_all();
}

void right_mouse(int button, int state, int x, int y)
{

	if(right_display_type==0 || right_display_type==1)
	{
		contour_mouse(button, state, x, y, right_data_type);
	}
	else if(right_display_type==2)
	{
		twodglyph_mouse(button, state, x, y, right_data_type);
	}
	Slider_mouse(button,state,x,y);

 	redisplay_all();
}

void left_motion(int x, int y)
{
	if(left_display_type==0 || left_display_type==1)
	{
		contour_motion(x, y, left_data_type);
	}
	else if(left_display_type==2)
	{
		twodglyph_motion(x, y, left_data_type);
	}

 	redisplay_all();
}

void right_motion(int x, int y)
{
	if(right_display_type==0 || right_display_type==1)
	{
		contour_motion(x, y, right_data_type);
	}
	else if(right_display_type==2)
	{
		twodglyph_motion(x, y, right_data_type);
	}
	Slider_motion(x,y);

 	redisplay_all();
}

void right_movement(int x,int y)
{

	if(right_display_type==0 || right_display_type==1)
	{
		contour_movement(x, y, right_data_type);

	}
	Slider_movement(x,y);

 	redisplay_all();
}

void left_movement(int x,int y)
{
	if(left_display_type==0 || left_display_type==1)
	{
		contour_movement(x, y, left_data_type);
	}

 	redisplay_all();
}

void XYZ()
{/*
			qdotSlice->SetPlaneXYZ();
			mindistance = qdotSlice->GetSliceXY(qdotField->GetXYplane(),qdotField->GetXZplane(),qdotField->GetYZplane(),
			qdotField->GetXYsample(),qdotField->GetXZsample(),qdotField->GetYZsample());  
			 qdotSlice->GetCutPos(slicePos);

			TwoDx = qdotSlice->GetPlanex();
			TwoDy = qdotSlice->GetPlaney();
			TwoDnum = qdotSlice->GetSelectedNum();//cerr<<TwoDnum<<endl;
				  svVector3 maxpos;
				  svVector3 minpos;
				  maxpos[0]=-9e+9;maxpos[1]=-9e+9;
				  minpos[0]=9e+9;minpos[1]=9e+9;

				  for(int i=0;i<TwoDnum;i++)	
				  {	
					sliceDir[i]=qdotField->Interpolation(sliceDen[i],  sliceExp[i], sliceCoe[i], slicePos[i]);	
					if(TwoDx[i]>maxpos[0])maxpos[0]=TwoDx[i];
					if(TwoDx[i]<minpos[0])minpos[0]=TwoDx[i];
					if(TwoDy[i]>maxpos[1])maxpos[1]=TwoDy[i];
					if(TwoDy[i]<minpos[1])minpos[1]=TwoDy[i];

					//cerr<<TwoDx[i]<<" ";	
				  }//cerr<<endl;
					
			 qdotBox->SetDir2(qdotSlice->GetDir2());  
			  ROIden = qdotBox->GetROIden(qdotField->GetScalar());	*/
}


void 
main_key(unsigned char key, int x, int y)
{
  switch (key) {
	case 'H':
		for(int i=0;i<2;i++)
		{
		trackball[i].reset();
		trackball_legend[i].reset();
		trackball_legend[i].setFocus(svVector3(0,0,0));
  		trackball[i].setFocus(qdotField[i]->GetCenter());
		twotranx[i]=0;
		twotrany[i]=0;
		twodzoom[i]=1;
		if(enable_cuttingplane)
			qdotSlice[i]->Reset();
		qdotBox[i]->Reset();
		}
		XYZ();
		//ResetFile();
		break;
	case 'h':
		for(int i=0;i<2;i++)
		{
		trackball[i].reset();
		trackball_legend[i].reset();
		trackball_legend[i].setFocus(svVector3(0,0,0));
  		trackball[i].setFocus(qdotField[i]->GetCenter());
		twotranx[i]=0;
		twotrany[i]=0;
		twodzoom[i]=1;
		if(enable_cuttingplane)
			qdotSlice[i]->Reset();
		
		qdotBox[i]->Reset();
		}
		XYZ();
		break;
	case 'w':
		Widget_xy_min[0].push_back(Widget_xy_center[0]+Widget_xy_radius);
		Widget_xy_min[1].push_back(Widget_xy_center[1]);
		Widget_xy_max[0].push_back(Widget_xy_center[0]);
		Widget_xy_max[1].push_back(Widget_xy_center[1]+Widget_xy_radius);
		Widget_select_xy.push_back(false);
		Widget_select_xy.push_back(false);
		xy_min.push_back(0.);
		xy_max.push_back(M_PI/2.);
		break;
	case 'O':
	case 'o':
		LoadFile();
		break;	
	case 'Z':
	case 'z':
		{
			qdotSlice[0]->XY_vertical();	
			qdotBox[0]->SetDir2(qdotSlice[0]->GetDir2()); 			  
			qdotBox[0]->SetDir(qdotSlice[0]->GetDir()); //cerr<<"z"<<endl;
			qdotSlice[1]->XY_vertical();	
			qdotBox[1]->SetDir2(qdotSlice[1]->GetDir2()); 			  
			qdotBox[1]->SetDir(qdotSlice[1]->GetDir()); //cerr<<"z"<<endl;
			XYZ();
			
		}break;
	case 'X':
	case 'x':
	{	
		qdotSlice[0]->YZ_vertical();				  
		qdotBox[0]->SetVerticalDir(qdotSlice[0]->GetDir()); 
		qdotSlice[1]->YZ_vertical();				  
		qdotBox[1]->SetVerticalDir(qdotSlice[1]->GetDir()); 
		XYZ();
			
		}break;
	case 'r':
	case 'R':
		if(enable_sync==0)enable_sync = 1;
		else enable_sync = 0;
		break;
	case 'Y':
	case 'y':
	{				
		qdotSlice[0]->XZ_vertical();	 
		qdotBox[0]->SetVerticalDir(qdotSlice[0]->GetDir());
		qdotSlice[1]->XZ_vertical();	 
		qdotBox[1]->SetVerticalDir(qdotSlice[1]->GetDir());
			XYZ();
		}break;

	break;

    case '\033':
    case 'q':
    case 'Q':
	//Restore();
      exit(0);
      break;
  }
	redisplay_all();
}

void CaptureScreen(char *file)
{
	int w = image_width;
	int h = image_height;

  char *image_buf = new char [w*h*3];

  glPixelStorei(GL_PACK_ALIGNMENT, 1);

  glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, image_buf);



  FILE* fp;

  if (NULL != (fp = fopen(file, "wb"))){

    // Write the 'header' information

    fprintf(fp, "P6 %d %d 255\n", w, h);

    for (int i=h-1; i >= 0; --i)

    {

       // write binary data

       fwrite(image_buf+3*i*w, sizeof(unsigned char), 3*w, fp);

    }

    fclose(fp);

  }

  delete [] image_buf;

}
void MagnitudeEncoding(int n)
{
	qdotMag->SetEncodeMode(n-1);
	if(qdotField[0]->GetMaxDen() > qdotField[1]->GetMaxDen())
	{
		qdotMag->SetMag(qdotField[0]->GetDensity(), qdotField[0]->GetDataSize(), 
				image_width/2., image_height);
	}
	else
	{
		qdotMag->SetMag(qdotField[1]->GetDensity(), qdotField[1]->GetDataSize(), 
				image_width/2., image_height);
	}

	for(int i=0;i<2;i++)
	{
		qdotIsoArrow[i]->SetEncodeMode(n);
		qdotIsoArrow[i]->SetArrows(enablesample,enableline,
					qdotMag->GetMag(iso[i]->GetDensity(), iso[i]->GetPointnum(), 
							image_width/2., image_height));
		qdotIsoArrow2[i]->SetEncodeMode(n);
		qdotIsoArrow2[i]->SetArrows(enablesample,enableline,
						qdotMag->GetMag(iso[i]->GetDensity(), iso[i]->GetPointnum(), 
							image_width/2., image_height));	
	}
}

void SetClusterColor_contour(int data_type)
{
		svVector4 *tmpcolor = new svVector4[iso[data_type]->GetPointnum()];
		double *tmpr = new double[iso[data_type]->GetPointnum()];
		double *tmpg = new double[iso[data_type]->GetPointnum()];
		double *tmpb = new double[iso[data_type]->GetPointnum()];
		if(enable_projection && projectionaggregation)
		{
			isocluster[data_type]->GetMagColors(tmpr, tmpg, tmpb);
			for(int i=0;i<iso[data_type]->GetPointnum();i++)
			{
				tmpcolor[i][0] = tmpr[i];
				tmpcolor[i][1] = tmpg[i];
				tmpcolor[i][2] = tmpb[i];
				tmpcolor[i][3] = 255.;
			}
			cutting_mag[data_type]->SetColors(tmpcolor);
		}
		else if(enable_projection && !projectionaggregation)
		{
			delete [] tmpcolor;
			delete [] tmpr;
			delete [] tmpg;
			delete [] tmpb;
			tmpcolor = new svVector4[projection_iso[data_type]->GetPointnum()];
			tmpr = new double[projection_iso[data_type]->GetPointnum()];
			tmpg = new double[projection_iso[data_type]->GetPointnum()];
			tmpb = new double[projection_iso[data_type]->GetPointnum()];
			isocluster[data_type]->GetMagColors(projection_iso[data_type]->GetPointnum(),
							inbox_mag_projection_cluster[data_type],
							projection_iso[data_type]->GetDoubleDensity(),
							tmpr, tmpg, tmpb);
			for(int i=0;i<projection_iso[data_type]->GetPointnum();i++)
			{
				tmpcolor[i][0] = tmpr[i];
				tmpcolor[i][1] = tmpg[i];
				tmpcolor[i][2] = tmpb[i];
				tmpcolor[i][3] = 255.;
			}
			cutting_mag[data_type]->SetColors(tmpcolor);
		}

		delete [] tmpcolor;
		delete [] tmpr;
		delete [] tmpg;
		delete [] tmpb;
		tmpcolor = new svVector4[iso[data_type]->GetPointnum()];
		tmpr = new double[iso[data_type]->GetPointnum()];
		tmpg = new double[iso[data_type]->GetPointnum()];
		tmpb = new double[iso[data_type]->GetPointnum()];
		isocluster[data_type]->GetMagColors(tmpr, tmpg, tmpb);
		for(int i=0;i<iso[data_type]->GetPointnum();i++)
		{
			tmpcolor[i][0] = tmpr[i];
			tmpcolor[i][1] = tmpg[i];
			tmpcolor[i][2] = tmpb[i];
			tmpcolor[i][3] = 255.;
		}
		qdotIsoArrow[data_type]->SetColors(tmpcolor,tmpcolor);

		isocluster[data_type]->GetVelColors(tmpr, tmpg, tmpb);
		for(int i=0;i<iso[data_type]->GetPointnum();i++)
		{
			tmpcolor[i][0] = tmpr[i];
			tmpcolor[i][1] = tmpg[i];
			tmpcolor[i][2] = tmpb[i];
			tmpcolor[i][3] = 255.;
		}
		qdotIsoArrow2[data_type]->SetColors(tmpcolor,tmpcolor);//cerr<<"1"<<endl;
		if(enable_projection&& projectionaggregation)
		{
			cutting_vel[data_type]->SetColors(tmpcolor);
		}
		else if (!projectionaggregation)
		{
			cutting_vel[data_type]->SetColors(projectionann[data_type]->ANN_individual(projection_iso[data_type]->GetVecDir(),
						projection_iso[data_type]->GetPointnum(), 
						tmpcolor, 
						inbox_cluster_contour[data_type], 
						inbox_vel_projection_cluster[data_type]));
		}

		delete [] tmpcolor;
		delete [] tmpr;
		delete [] tmpg;
		delete [] tmpb;
	
}

void SetContourColor(int data_type)
{
	SetClusterColor_contour(data_type);	
}

void ContoursGeneration(vector<float> ra, int data_type)
{
	#ifdef _VTK
	iso[data_type]->NewContours();
	for(int j=0;j<ra.size();j++)
	{
		cerr<<"The ratio to generate contours is "<<ra[j]<<endl;//cerr<<tmpvalue<<endl;
		for(int i=0;i<qdotField[data_type]->GetZlayer();i++)
		{
			sprintf(vtkout, "%s%d.vtk",VTKfile[data_type],i);
			svScalar tmpd = ra[j]*qdotField[data_type]->GetDenMax(i);
			iso[data_type]->Get2DContour(vtkout, tmpd);
			if(iso[data_type]->Addnewcontour())
				iso[data_type]->SetZnum(i);
		}
	}

 	projection_iso[data_type]->NewContours();
	for(int j=0;j<ra.size();j++)
	{
		cerr<<"The ratio is "<<ra[j]<<endl;//cerr<<tmpvalue<<endl;
		for(int i=0;i<qdotField[data_type]->GetZlayer();i++)
		{
			sprintf(vtkout, "%s%sprojection%d",tmpfiledir,datafile[data_type], i);
			svScalar tmpd = ra[j]*qdotField[data_type]->GetDenMax(i);//cerr<<qdotField->GetDenMax(i)<<endl;
			projection_iso[data_type]->GetLineContour(vtkout, tmpd);
			if(projection_iso[data_type]->Addnewcontour())
				projection_iso[data_type]->SetZnum(i);
		}
	}

	datasize[data_type] = iso[data_type]->GetPointnum();//cerr<<datasize<<endl;
	if(datasize[data_type])
	{
		if(projectionaggregation)
			cutting_mag[data_type]->SetCut(iso[data_type]->GetPositon(),NULL);
		else
			cutting_mag[data_type]->SetCut(projection_iso[data_type]->GetPositon(),NULL);
		if(projectionaggregation)
			cutting_vel[data_type]->SetCut(iso[data_type]->GetPositon(),NULL);
		else
			cutting_vel[data_type]->SetCut(projection_iso[data_type]->GetPositon(),NULL);

		qdotIsoArrow[data_type]->SetData(lb[data_type], rb[data_type],iso[data_type]->GetPointnum(),
			iso[data_type]->GetVecPos(),
			   iso[data_type]->GetVecDir(),iso[data_type]->GetDensity(),
			  iso[data_type]->GetExponent(),iso[data_type]->GetCoefficient(),
			NULL,NULL,
			iso[data_type]->NUM_SEG,iso[data_type]->ROI_ARROW_BASE);
		  qdotIsoArrow[data_type]->SetSamples(iso[data_type]->GetSample(),iso[data_type]->GetSampleNum());
		  qdotIsoArrow[data_type]->SetArrows(enablesample,enableline2,
			qdotMag->GetMag(iso[data_type]->GetDensity(), iso[data_type]->GetPointnum(), 
			image_width/2., image_height));

		qdotIsoArrow2[data_type]->SetData(lb[data_type], rb[data_type],iso[data_type]->GetPointnum(),
			iso[data_type]->GetVecPos(),
			   iso[data_type]->GetVecDir(),iso[data_type]->GetDensity(),
			  iso[data_type]->GetExponent(),iso[data_type]->GetCoefficient(),
			NULL,NULL,
			iso[data_type]->NUM_SEG,iso[data_type]->ROI_ARROW_BASE);
		  qdotIsoArrow2[data_type]->SetSamples(iso[data_type]->GetSample(),iso[data_type]->GetSampleNum());
		  qdotIsoArrow2[data_type]->SetArrows(enablesample,enableline2,
			qdotMag->GetMag(iso[data_type]->GetDensity(), iso[data_type]->GetPointnum(), 
			image_width/2., image_height));

		projectionann[data_type]->SetData(iso[data_type]->GetPointnum(), iso[data_type]->GetVecDir());

		delete [] inbox_mag_projection[data_type];
		delete [] inbox_vel_projection[data_type];
		inbox_mag_projection[data_type] = new bool[projection_iso[data_type]->GetPointnum()];
		inbox_vel_projection[data_type] = new bool[projection_iso[data_type]->GetPointnum()];
		delete [] inbox_mag_projection_cluster[data_type];
		delete [] inbox_vel_projection_cluster[data_type];
		inbox_mag_projection_cluster[data_type] = new bool[projection_iso[data_type]->GetPointnum()];
		inbox_vel_projection_cluster[data_type] = new bool[projection_iso[data_type]->GetPointnum()];
		Semi_Mag(data_type);
		ROIden[data_type] = qdotBox[data_type]->GetROIden(qdotField[data_type]->GetScalar());
		cerr<<"It is done to generate a new contour."<<endl;
	   }

  	isocluster[data_type]->SetEMdata2(iso[data_type]->GetSortVectorPos(),iso[data_type]->GetSortVectorVel(), 
		iso[data_type]->GetSortVectorDen(), iso[data_type]->GetZlayer(), num_magnitude_clusters[data_type]);
	#else
		cerr<<"Sorry cannot find VTK"<<endl;
	#endif
}

void Mag_Contour_ROI_Clustering(int n, int data_type)
{
	vector<bool> roi2;// = qdotBox->InBox2(iso->GetVecPos(),iso->GetPointnum());

	for(int i=0;i<iso[data_type]->GetPointnum();i++)
	{
		roi2.push_back(inbox_cluster_contour[data_type][i]);
	}

	isocluster[data_type]->SetMaginNum(n);
  	isocluster[data_type]->SetROIColors();
	isocluster[data_type]->GetROIMagClusters(roi2);
	roi2.resize(0);	
}
void Vel_Contour_ROI_Clustering(int n, int data_type)
{
	vector<bool> roi2;// = qdotBox->InBox2(iso->GetVecPos(),iso->GetPointnum());

	for(int i=0;i<iso[data_type]->GetPointnum();i++)
	{
		roi2.push_back(inbox_cluster_contour[data_type][i]);
	}

	isocluster[data_type]->SetVelinNum(n);
  	isocluster[data_type]->SetROIColors();
	isocluster[data_type]->GetROIVelClusters(roi2);
	roi2.resize(0);	
}
void Mag_Contour_Clustering(int data_type)
{
	char str[50];
	sprintf(str,"%s%s_%d_contour_mag",tmpfiledir, datafile[data_type],(int)num_magnitude_clusters[data_type]);
	ifstream infile(str);
	bool flag = false;
	if(infile.is_open())
	{flag = true;}
	if(flag)
	{//cerr<<str<<endl;
		infile.close();
		isocluster[data_type]->SetMagNum((float)magnum[data_type]);
		isocluster[data_type]->ReadColors(2);
		isocluster[data_type]->GetMagClusters_file(str);//cerr<<str<<endl;
	}
	else
	{
		isocluster[data_type]->SetMagNum((float)magnum[data_type]);//num_magnitude_clusters[data_type]);
		isocluster[data_type]->ReadColors(2);
		isocluster[data_type]->GetMagClusters();
		isocluster[data_type]->Savemagtofile(str);//cerr<<str<<endl;
	}
}
void Vel_Contour_Clustering(int data_type)
{
	char str[50];
	sprintf(str,"%s%s_%d_contour_vel",tmpfiledir, datafile[data_type],(int)num_orientation_clusters[data_type]);
	ifstream infile(str);
	bool flag = false;
	if(infile.is_open())
	{flag = true;}
	if(flag)
	{
		infile.close();
		isocluster[data_type]->SetVelNum(num_orientation_clusters[data_type]);
		isocluster[data_type]->ReadColors(2);
		isocluster[data_type]->GetVelClusters_file(str);
	}
	else
	{
		isocluster[data_type]->SetVelNum(num_orientation_clusters[data_type]);
		isocluster[data_type]->ReadColors(2);
		isocluster[data_type]->GetVelClusters();
		isocluster[data_type]->Saveveltofile(str);
	}
}

void Vel_Clustering(int data_type)
{
	char str[50];
	sprintf(str,"%s%s_%d_vel",tmpfiledir, datafile[data_type],(int)num_orientation_clusters[data_type]);
	ifstream infile(str);
	bool flag = false;
	if(infile.is_open())
	{flag = true;}
	if(flag)
	{
		infile.close();
		qdotcluster[data_type]->SetVelNum(num_orientation_clusters[data_type]);
		qdotcluster[data_type]->ReadColors(1);
		qdotcluster[data_type]->GetVelClusters_file(str);

	}
	else
	{
		qdotcluster[data_type]->SetVelNum(num_orientation_clusters[data_type]);
		qdotcluster[data_type]->ReadColors(1);
		qdotcluster[data_type]->GetVelClusters();
		qdotcluster[data_type]->Saveveltofile(str);
	}
}
void Mag_Clustering(int data_type)
{
	char str[50];
	sprintf(str,"%s%s_%d_mag",tmpfiledir, datafile[data_type],(int)num_magnitude_clusters[data_type]);
	ifstream infile(str);
	bool flag = false;
	if(infile.is_open())
	{flag = true;}
	if(flag)
	{//cerr<<str<<endl;
		infile.close();
		qdotcluster[data_type]->SetMagNum(num_magnitude_clusters[data_type]);
		qdotcluster[data_type]->ReadColors(1);
		qdotcluster[data_type]->GetMagClusters_file(str);//cerr<<str<<endl;
	}
	else
	{//cerr<<str<<endl;
		qdotcluster[data_type]->SetMagNum(num_magnitude_clusters[data_type]);
		qdotcluster[data_type]->ReadColors(1);
		qdotcluster[data_type]->GetMagClusters();
		qdotcluster[data_type]->Savemagtofile(str);//cerr<<str<<endl;
	}
}

void PlaneComparison(int data_type)
{	
		svVector3 p;
		p[0]=vector_x;p[1]=vector_y;p[2]=vector_z;
		p = normalize(p);
		int n =qdotField[data_type]->GetLayer(zmin);
		int *n1 = new int[qdotField[data_type]->GetDataSize()];
		int *n2 =  new int[qdotField[data_type]->GetDataSize()];
	  	qdotann->ANN_montecarlo(qdotField[data_type]->GetDataSize(),
					qdotField[data_type]->GetSplitPos(),
					qdotField[data_type]->GetZlayer(), zmin,
					n1,n2, p);
		qdotdistance[data_type]->SetNei(n1, n2);
		delete [] n1;
		delete [] n2;
		qdotdistance[data_type]->SetNeighbours_Multiplelayer();//cerr<<"!"<<endl;
		double *h = new double[qdotField[data_type]->GetDataSize()];
		qdotcluster[data_type]->GetVelHue(h);//cerr<<"!"<<endl;

		double *tmpr = new double[qdotField[data_type]->GetDataSize()];
		double *tmpg = new double[qdotField[data_type]->GetDataSize()];
		double *tmpb = new double[qdotField[data_type]->GetDataSize()];
		qdotcluster[data_type]->GetVelColors(tmpr, tmpg, tmpb);//cerr<<"!"<<endl;

		qdotmap[data_type]->SetColors(num_orientation_clusters[data_type], 
			lightness_degree, qdotcluster[data_type]->GetOrderHue(),
			qdotcluster[data_type]->GetIDXVel(),
			qdotdistance[data_type]->GetLayerPos(),
			qdotdistance[data_type]->GetLayerDis(),
			qdotField[data_type]->GetZlayer(),zmin,
			tmpr, tmpg, tmpb);//cerr<<"!"<<endl;

		delete [] h;
		delete [] tmpr;
		delete [] tmpg;
		delete [] tmpb;
}

void Semi_Mag(int data_type)
{
	bool *in1 = new bool[datasize[data_type]];
	bool *in2 = new bool[qdotField[data_type]->GetDataSize()];
	bool *in6 = new bool[projection_iso[data_type]->GetPointnum()];
	in1 = iso[data_type]->GetInsidePos_Mag(range_mag[0], range_mag[1]);
	in2 = qdotField[data_type]->GetInsidePos_Mag(range_mag[0], range_mag[1]);
	in6 = projection_iso[data_type]->GetInsidePos_Mag(range_mag[0], range_mag[1]);

	bool *in3 = new bool[datasize[data_type]];
	bool *in4 = new bool[qdotField[data_type]->GetDataSize()];
	bool *in7 = new bool[projection_iso[data_type]->GetPointnum()];
	in3 = iso[data_type]->GetInsidePos_Vel(xy_min, xy_max, 180 - zangle_max, 180 - zangle_min);
	in4 = qdotField[data_type]->GetInsidePos_Vel(xy_min, xy_max, 180 - zangle_max, 180- zangle_min);
	in7 = projection_iso[data_type]->GetInsidePos_Vel(xy_min, xy_max, 180 - zangle_max, 180 - zangle_min);

	bool *in5 = new bool[datasize[data_type]];
	bool *in8 = new bool[projection_iso[data_type]->GetPointnum()];
	if(enable_roi&&enable_box)
	{
		in5 = qdotBox[data_type]->InBox(iso[data_type]->GetVecPos(),iso[data_type]->GetPointnum());
		in8 = qdotBox[data_type]->InBox(projection_iso[data_type]->GetVecPos(),projection_iso[data_type]->GetPointnum());
	}
	else
	{
		for(int i=0;i<datasize[data_type];i++)
			in5[i] = true;
		for(int i=0;i<projection_iso[data_type]->GetPointnum();i++)
		{
			in8[i] = true;
		}
	}

	for(int i=0;i<datasize[data_type];i++)
	{
		if(in1[i]&&in3[i]&&in5[i])
		{
			inbox_contour[data_type][i] = 1;
		}
		else
		{
			inbox_contour[data_type][i] = 0;
		}
	}

	for(int i=0;i<projection_iso[data_type]->GetPointnum();i++)
	{
		if(in6[i]&&in7[i]&&in8[i])
		{
			inbox_mag_projection[data_type][i] = 1;
			inbox_vel_projection[data_type][i] = 1;
		}
		else
		{
			inbox_mag_projection[data_type][i] = 0;
			inbox_vel_projection[data_type][i] = 0;
		}		
	}

	for(int i=0;i<qdotField[data_type]->GetDataSize();i++)
	{
		if(in2[i]&&in4[i])
		{

			inbox_mag2[data_type][i] = 1;
			inbox_vel2[data_type][i] = 1;
		}
		else
		{
			inbox_mag2[data_type][i] = 0;
			inbox_vel2[data_type][i] = 0;
		}
	}

	delete [] in1;
	delete [] in2;
	delete [] in3;
	delete [] in4;
	delete [] in5;
	delete [] in6;
	delete [] in7;
	delete [] in8;
}

void LoadFile()
{/*
	contour_ratio[0].clear();
	contour_ratio[1].clear();

	//InitFile();

	ContoursGeneration(contour_ratio[1]);

	Mag_Clustering();	
	Mag_Contour_Clustering();
	Mag_Contour_ROI_Clustering((int)num_magnitude_clusters_in);	
	Vel_Clustering();	
	Vel_Contour_Clustering();
	Vel_Contour_ROI_Clustering((int)num_orientation_clusters_in);
	Semi_Mag();
	SetContourColor();

	qdotField->PlaneNeighbors(z_standard[1],plane_neighbor+1);
	PlaneComparison();*/

}


void New_data(int data_type)
{
	qdotField[data_type] = new svQDOTNorm();
	iso[data_type] = new svContour();
  	qdotMag = new svMagnitude();
}

void New_Render(int data_type)
{
	qdotBox[data_type] = new svROIbox(lb[data_type],rb[data_type],qdotField[data_type]->GetDataSize(),
		  			qdotField[data_type]->GetPosition(), qdotField[data_type]->GetDensity());
	qdotprimitive[data_type] = new svPrimitive();
	qdotSlice[data_type] = new svSlice(lb[data_type],rb[data_type],qdotField[data_type]->GetDataSize(),
				qdotField[data_type]->GetPosition());
	projection_iso[data_type] = new svContour();
	projectionann[data_type] = new svANN();
	qdotOutline[data_type] = new svOutline(lb[data_type], rb[data_type], image_width);

	cutting_mag[data_type] = new svCut(iso[data_type]->GetPositon(),NULL);
	cutting_vel[data_type] = new svCut(iso[data_type]->GetPositon(),NULL);	
}

void New_Cluster(int data_type)
{
	qdotcluster[data_type] = new Cluster();
	isocluster[data_type] = new Cluster();
	qdotdistance[data_type] = new QDOTdistancefunction();
	qdotmap[data_type] = new svQDOTdistancemap();
}

void New_Arrow(int data_type)
{
	qdotIsoArrow[data_type] = new svArrowGlyph(lb[data_type], rb[data_type],iso[data_type]->GetPointnum(),
						iso[data_type]->GetVecPos(),
		   				iso[data_type]->GetVecDir(),iso[data_type]->GetDensity(),
		  				iso[data_type]->GetExponent(),iso[data_type]->GetCoefficient(),
						NULL,NULL,
						num_segment_arrow[1],arrow_base[1]);
	qdotIsoArrow2[data_type] = new svArrowGlyph(lb[data_type], rb[data_type],iso[data_type]->GetPointnum(),
					iso[data_type]->GetVecPos(),
		   			iso[data_type]->GetVecDir(),iso[data_type]->GetDensity(),
		 			iso[data_type]->GetExponent(),iso[data_type]->GetCoefficient(),
					NULL,NULL,
					num_segment_arrow[1],arrow_base[1]);
}

void InitData(int data_type)
{
	//qdotField[data_type] = new svQDOTNorm();

	qdotField[data_type]->New(DataDir[data_type], datafile[data_type], (float)0.00); 
	int dataSize[data_type+1];
	dataSize[data_type]= qdotField[data_type]->GetDataSize();
	cerr << "... DataSize = " << dataSize[data_type] << endl;
	qdotField[data_type]->GetBoundingBox(&lb[data_type], &rb[data_type]);
	cerr << "... Get bounding box " << endl;
	qdotField[data_type]->SortData();

	inbox_mag_cluster[data_type] = new bool[dataSize[data_type]];
	inbox_vel_cluster[data_type] = new bool[dataSize[data_type]];


	  
	inbox_mag[data_type] = new bool[dataSize[data_type]];
	inbox_vel[data_type] = new bool[dataSize[data_type]];
	inbox_mag2[data_type] = new bool[dataSize[data_type]];
	inbox_vel2[data_type] = new bool[dataSize[data_type]];

	svVector3 center = qdotField[data_type]->GetCenter();
	cerr << "center : " << center[0] << " " 
	       << center[1] << " " << center[2] << endl;
	center.getValue(view_info.coi);

	float x, y, z;
	qdotField[data_type]->GetPhysicalDimension(&x,&y, &z);
	cerr << "physical dimensions: " << x << " " << y << " " << z << endl;

	view_info.eye[0] = x;  // x/2.0
	view_info.eye[1] = y;
	view_info.eye[2] = z*4.0;

	l[data_type][0] = lb[data_type][0]+2;l[data_type][1] = lb[data_type][1]+2;l[data_type][2] = -10;
	r[data_type][0] = rb[data_type][0]; r[data_type][1] = rb[data_type][1]; r[data_type][2] = 2;
	ll[data_type][0] =0;
	ll[data_type][1] =0;
	ll[data_type][2] =0;

	//zmin = qdotField[data_type]->GetMinDen();
	zmax[data_type] = rb[data_type][2];z_standard[data_type] = zmin;

	for(int i=0;i<qdotField[data_type]->GetDataSize();i++)
	{
		inbox_mag_cluster[data_type][i] = true;
		inbox_mag[data_type][i] = true;
	}
	for(int i=0;i<qdotField[data_type]->GetDataSize();i++)
	{
		inbox_vel_cluster[data_type][i] = true;
		inbox_vel[data_type][i] = true;
	}

}

void InitMag(int data_type)
{
	qdotMag->SCALE=qdotField[data_type]->GetScalar();
	qdotMag->SetEncodeMode(magnitude_encoding);
	mag_scale = (fabs(view_info.eye[2]) * tan(M_PI*view_info.view_angle/(2.*180)) * 2.)/image_height;
	qdotMag->SetMagScalar(mag_scale);
	qdotMag->SetMag(qdotField[data_type]->GetDensity(), qdotField[data_type]->GetDataSize(), 
			image_width/2., image_height);
}

void InitIso(int data_type)
{
	//iso[data_type] = new svContour();

	iso[data_type]->ROI_ARROW_BASE=arrow_base[1];
	iso[data_type]->NUM_SEG=num_segment_arrow[1];

	iso[data_type]->SetZlayer(qdotField[data_type]->GetZlayer());
	cerr<<"....Create Contours...."<<endl;
	iso[data_type]->NewContours();
	#ifdef _VTK

	for(int ii=0;ii<contour_ratio[data_type].size();ii++)
	{
		svScalar ra = contour_ratio[data_type][ii];
		cerr<<"The ratio is "<<ra<<endl;//cerr<<tmpvalue<<endl;
		for(int j=0;j<qdotField[data_type]->GetZlayer();j++)
		{
			sprintf(vtkout, "%s%d.vtk",VTKfile[data_type],j);
			//cerr<<VTKfile[data_type]<<" "<<vtkout<<endl;
			double tmpd = ra*qdotField[data_type]->GetDenMax(j);//cerr<<qdotField->GetDenMax(i)<<endl;
			//cerr<<tmpd<<endl;
			iso[data_type]->Get2DContour(vtkout, tmpd);
			if(iso[data_type]->Addnewcontour())
				iso[data_type]->SetZnum(j);
		}
	}
	  
	#endif
	cerr<<"Contours are generated successfully..."<<endl;
	iso[data_type]->SetScalar(qdotField[data_type]->GetScalar());

	inbox_cluster_contour[data_type] = new bool[iso[data_type]->GetPointnum()];
	inbox_contour[data_type] = new bool[iso[data_type]->GetPointnum()];
	datasize[data_type] = iso[data_type]->GetPointnum();

}

void InitRender(int data_type)
{
	int dataSize = qdotField[data_type]->GetDataSize();

	/*qdotBox[data_type] = new svROIbox(lb[data_type],rb[data_type],qdotField[data_type]->GetDataSize(),
		  			qdotField[data_type]->GetPosition(), qdotField[data_type]->GetDensity());
	qdotprimitive[data_type] = new svPrimitive();
	qdotSlice[data_type] = new svSlice(lb[data_type],rb[data_type],dataSize,qdotField[data_type]->GetPosition());
	projection_iso[data_type] = new svContour();
	projectionann[data_type] = new svANN();
	qdotOutline[data_type] = new svOutline(lb[data_type], rb[data_type], image_width);

	cutting_mag[data_type] = new svCut(iso[data_type]->GetPositon(),NULL);
	cutting_vel[data_type] = new svCut(iso[data_type]->GetPositon(),NULL);

*/

	qdotBox[data_type]->SetZlayer(qdotField[data_type]->GetZlayer());
	ROIden[data_type] = qdotBox[data_type]->GetROIden(qdotField[data_type]->GetScalar());

	qdotprimitive[data_type]->SetBBox(lb[data_type],rb[data_type]);
	
	slicePos[data_type] = new svVector3[dataSize];
	mindistance = qdotSlice[data_type]->GetSliceXY(qdotField[data_type]->GetXYplane(),
								qdotField[data_type]->GetXZplane(),
								qdotField[data_type]->GetYZplane(),
								qdotField[data_type]->GetXYsample(),
								qdotField[data_type]->GetXZsample(),
								qdotField[data_type]->GetYZsample());  
	qdotSlice[data_type]->GetCutPos(slicePos[data_type]);
	TwoDnum[data_type] = qdotSlice[data_type]->GetSelectedNum();
	char str[50];
	sprintf(str,"%s%s",tmpfiledir, datafile[data_type]);
        qdotField[data_type]->Savetofile(str, slicePos[data_type], TwoDnum[data_type]);

	projection_iso[data_type]->SetZlayer(qdotField[data_type]->GetZlayer());
	projection_iso[data_type]->NewContours();
	for(int i=0;i<contour_ratio[data_type].size();i++)
	{
		svScalar ra = contour_ratio[data_type][i];
		for(int i=0;i<qdotField[data_type]->GetZlayer();i++)
		{
			sprintf(vtkout, "%s%sprojection%d",tmpfiledir, datafile[data_type],i);
			svScalar tmpd = ra*qdotField[data_type]->GetDenMax(i);//cerr<<qdotField->GetDenMax(i)<<endl;
			projection_iso[data_type]->GetLineContour(vtkout, tmpd);
			if(projection_iso[data_type]->Addnewcontour())
				projection_iso[data_type]->SetZnum(i);
		}
	}
	projection_iso[data_type]->SetScalar(qdotField[data_type]->GetScalar());  

	cutting_mag[data_type]->SetCutRegion(lb[data_type],rb[data_type],-1); 
	cutting_vel[data_type]->SetCutRegion(lb[data_type],rb[data_type],-1); 
	if(!projectionaggregation)
	{
		cutting_vel[data_type]->SetCut(projection_iso[data_type]->GetPositon(),NULL);
		cutting_mag[data_type]->SetCut(projection_iso[data_type]->GetPositon(),NULL);
	}

	inbox_mag_projection[data_type] = new bool[projection_iso[data_type]->GetPointnum()];
	inbox_vel_projection[data_type] = new bool[projection_iso[data_type]->GetPointnum()];
	inbox_mag_projection_cluster[data_type] = new bool[projection_iso[data_type]->GetPointnum()];
	inbox_vel_projection_cluster[data_type] = new bool[projection_iso[data_type]->GetPointnum()];
	inbox_mag_projection_cluster[data_type] = qdotBox[data_type]->InBox(projection_iso[data_type]->GetVecPos(),projection_iso[data_type]->GetPointnum());
	inbox_vel_projection_cluster[data_type] = qdotBox[data_type]->InBox(projection_iso[data_type]->GetVecPos(),projection_iso[data_type]->GetPointnum());
	projectionann[data_type]->SetData(iso[data_type]->GetPointnum(), iso[data_type]->GetVecDir());

	qdotOutline[data_type]->SetAxisWidgetPos(l[data_type],r[data_type]);
	qdotOutline[data_type]->SetContourWidgetPos(ll[data_type]);


}

//call after ROI box setup
void InitClusters(int data_type)
{
	/*qdotcluster[data_type] = new Cluster();
	isocluster[data_type] = new Cluster();
	qdotdistance[data_type] = new QDOTdistancefunction();
	qdotmap[data_type] = new svQDOTdistancemap();*/

	qdotcluster[data_type]->SetMagNum( num_magnitude_clusters[data_type]);	//cerr<<"qdot"<<endl;
 	qdotcluster[data_type]->SetVelNum( num_orientation_clusters[data_type]);	//cerr<<"qdot"<<endl;
	qdotcluster[data_type]->ReadColors(1);	//cerr<<"qdot"<<endl;
	qdotcluster[data_type]->SetScale(qdotField[data_type]->GetScalar());	//cerr<<"qdot"<<endl;
	qdotcluster[data_type]->SetEMdata(qdotField[data_type]->GetSortVectorPos(),
					qdotField[data_type]->GetSortVectorVel(), 
					qdotField[data_type]->GetSortVectorDen(), 
					qdotField[data_type]->GetZlayer(), 
					num_magnitude_clusters[data_type]);	//cerr<<"qdot"<<endl;
	qdotcluster[data_type]->SetZ(zmin);
	//cerr<<"qdot"<<endl;

	isocluster[data_type]->SetMagNum(num_magnitude_clusters[data_type]);
	isocluster[data_type]->SetVelNum(num_orientation_clusters[data_type]);
	isocluster[data_type]->ReadColors(2);
	isocluster[data_type]->SetMaginNum(num_magnitude_clusters_in[data_type]);
	isocluster[data_type]->SetVelinNum(num_orientation_clusters_in[data_type]);
	isocluster[data_type]->SetROIColors();
	isocluster[data_type]->SetScale(qdotField[data_type]->GetScalar());
	isocluster[data_type]->SetEMdata2(iso[data_type]->GetSortVectorPos(),
					iso[data_type]->GetSortVectorVel(), 
					iso[data_type]->GetSortVectorDen(), 
					iso[data_type]->GetZlayer(), 
					num_magnitude_clusters[data_type]);
	//cerr<<"qdot"<<endl;
	inbox_cluster_contour[data_type]= qdotBox[data_type]->InBox(iso[data_type]->GetVecPos(),
									iso[data_type]->GetPointnum());
	//cerr<<"qdot"<<endl;
	Mag_Clustering(data_type);	//cerr<<"qdot"<<endl;	
	Mag_Contour_Clustering(data_type);	//cerr<<"qdot"<<endl;	
	Mag_Contour_ROI_Clustering((int)num_magnitude_clusters_in[data_type],data_type);		//cerr<<"qdot"<<endl;	
	Vel_Clustering(data_type);			//cerr<<"qdot"<<endl;
	Vel_Contour_Clustering(data_type);		//cerr<<"qdot"<<endl;
	Vel_Contour_ROI_Clustering((int)num_orientation_clusters_in[data_type],data_type);		//cerr<<"qdot"<<endl;	

	qdotdistance[data_type]->SetData(qdotField[data_type]->GetSplitPos(),qdotField[data_type]->GetSplitVel(),
			qdotField[data_type]->GetDataSize(),qdotField[data_type]->GetZlayer(),lb[data_type],rb[data_type]);
	//qdotdistance[data_type]->ReadNei(datafile[data_type]);//!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//cerr<<"qdot"<<endl;
	svVector3 p;
	p[0]=vector_x;p[1]=vector_y;p[2]=vector_z;
	p = normalize(p);
	int n =qdotField[data_type]->GetLayer(zmin);
	int *n1 = new int[qdotField[data_type]->GetDataSize()];
	int *n2 =  new int[qdotField[data_type]->GetDataSize()];
	qdotann->ANN_montecarlo(qdotField[data_type]->GetDataSize(),
					qdotField[data_type]->GetSplitPos(),
					qdotField[data_type]->GetZlayer(), zmin,
					n1,n2, p);
	qdotdistance[data_type]->SetNei(n1, n2);
	delete [] n1;
	delete [] n2;
	cerr<<"done1"<<endl;
	qdotdistance[data_type]->SetNeighbours_Multiplelayer();
	cerr<<"done2"<<endl;
	double *h = new double[qdotField[data_type]->GetDataSize()];
	qdotcluster[data_type]->GetVelHue(h);

	char str[50];
	double *tmpr = new double[qdotField[data_type]->GetDataSize()];
	double *tmpg = new double[qdotField[data_type]->GetDataSize()];
	double *tmpb = new double[qdotField[data_type]->GetDataSize()];
	qdotcluster[data_type]->GetVelColors(tmpr, tmpg, tmpb);

	qdotmap[data_type]->SetColors(num_orientation_clusters[data_type], lightness_degree, 
				qdotcluster[data_type]->GetOrderHue(),
				qdotcluster[data_type]->GetIDXVel(),
				qdotdistance[data_type]->GetLayerPos(),
				qdotdistance[data_type]->GetLayerDis(),
				qdotField[data_type]->GetZlayer(),z_standard[data_type],
				tmpr, tmpg, tmpb);
	delete [] h;
	delete [] tmpr;
	delete [] tmpg;
	delete [] tmpb;		
	oneplane_selectp[data_type] = qdotmap[data_type]->GetSelectedPos();

	float rr = (float) image_width/(float) image_height;
	float distance_width_scale = (image_width/(qdotdistance[data_type]->GetXlength()*(qdotdistance[data_type]->GetXl()+qdotdistance[data_type]->GetXd()*4)))/2.;
	float distance_height_scale = (image_height/(qdotdistance[data_type]->GetYlength()*(qdotdistance[data_type]->GetYl()+qdotdistance[data_type]->GetYd()*4)));
	if(distance_width_scale> distance_height_scale)
		distance_scale[data_type] = distance_height_scale;
	else
		distance_scale[data_type] = distance_width_scale;
	//cerr<<"dis"<<distance_scale[data_type]<<endl;		
}

void InitArrows(int data_type)
{
	 /* qdotIsoArrow[data_type] = new svArrowGlyph(lb[data_type], rb[data_type],iso[data_type]->GetPointnum(),
						iso[data_type]->GetVecPos(),
		   				iso[data_type]->GetVecDir(),iso[data_type]->GetDensity(),
		  				iso[data_type]->GetExponent(),iso[data_type]->GetCoefficient(),
						NULL,NULL,
						num_segment_arrow[1],arrow_base[1]);*/
	  qdotIsoArrow[data_type]->SetEncodeMode((int)magnitude_encoding+1);
	  qdotIsoArrow[data_type]->SetSamples(iso[data_type]->GetSample(),iso[data_type]->GetSampleNum());
	  qdotIsoArrow[data_type]->SetArrows(enablesample,enableline2,
				qdotMag->GetMag(iso[data_type]->GetDensity(), iso[data_type]->GetPointnum(), 
							image_width/2., image_height));
	qdotIsoArrow[data_type]->createSphereArraysAndVBOs(enablesample); 

	 /* qdotIsoArrow2[data_type] = new svArrowGlyph(lb[data_type], rb[data_type],iso[data_type]->GetPointnum(),
					iso[data_type]->GetVecPos(),
		   			iso[data_type]->GetVecDir(),iso[data_type]->GetDensity(),
		 			iso[data_type]->GetExponent(),iso[data_type]->GetCoefficient(),
					NULL,NULL,
					num_segment_arrow[1],arrow_base[1]);*/
	  qdotIsoArrow2[data_type]->SetEncodeMode((int)magnitude_encoding+1);
	  qdotIsoArrow2[data_type]->SetSamples(iso[data_type]->GetSample(),iso[data_type]->GetSampleNum());
	  qdotIsoArrow2[data_type]->SetArrows(enablesample,enableline2,qdotMag->GetMag(iso[data_type]->GetDensity(), 
						iso[data_type]->GetPointnum(), 
						image_width/2., image_height));
	qdotIsoArrow2[data_type]->createSphereArraysAndVBOs(enablesample); 
}

void Delete_Init(int data_type)
{
	delete [] inbox_mag_cluster[data_type];
	delete [] inbox_vel_cluster[data_type];
	delete [] inbox_mag[data_type];
	delete [] inbox_vel[data_type];
	delete [] inbox_mag2[data_type];
	delete [] inbox_vel2[data_type];
	delete [] inbox_cluster_contour[data_type];
	delete [] inbox_contour[data_type];
	delete [] inbox_mag_projection[data_type];
	delete [] inbox_vel_projection[data_type];;
	delete [] inbox_mag_projection_cluster[data_type];
	delete [] inbox_vel_projection_cluster[data_type];
	delete [] slicePos[data_type];

	cerr<<"delete"<<endl;

  	//InitFile2(data_string[data_type], data_type);
	InitData(data_type);
	//if(fabs(qdotField[data_type]->GetMaxDen() - max_density)<0.0001)
	//	InitMag(data_type);

	if(qdotField[0]->GetMaxDen() > qdotField[1]->GetMaxDen())
	{
		double min_d = qdotField[0]->GetMinDen_Nonzero() ;
		double max_d = qdotField[0]->GetMaxDen() ;
		min_density = min_d;
		max_density = max_d;
		InitMag(0);
	}
	else
	{
		InitMag(1);
		double min_d = qdotField[1]->GetMinDen_Nonzero() ;
		double max_d = qdotField[1]->GetMaxDen() ;
		min_density = min_d;
		max_density = max_d;
	}

	if(min_density!=0)
		mag_distance = log10(max_density)  - log10(min_density);
	else
		mag_distance = log10(max_density);

	cerr<<"data"<<endl;

	InitIso(data_type);
	cerr<<"iso"<<endl;

	qdotBox[data_type]->SetData(lb[data_type],rb[data_type],qdotField[data_type]->GetDataSize(),
		  			qdotField[data_type]->GetPosition(), qdotField[data_type]->GetDensity());
	qdotSlice[data_type]->SetData(lb[data_type],rb[data_type],qdotField[data_type]->GetDataSize(),
				qdotField[data_type]->GetPosition());
	qdotOutline[data_type]->SetOutline(lb[data_type], rb[data_type], image_width);
	cutting_mag[data_type]->SetCut(iso[data_type]->GetPositon(),NULL);
	cutting_vel[data_type]->SetCut(iso[data_type]->GetPositon(),NULL);
	InitRender(data_type);
	cerr<<"render"<<endl;

	InitClusters(data_type);//!!!!!!!!!!!

	cerr<<"cluster"<<endl;

	qdotIsoArrow[data_type]->SetData(lb[data_type], rb[data_type],iso[data_type]->GetPointnum(),
			iso[data_type]->GetVecPos(),
			   iso[data_type]->GetVecDir(),iso[data_type]->GetDensity(),
			  iso[data_type]->GetExponent(),iso[data_type]->GetCoefficient(),
			NULL,NULL,
			iso[data_type]->NUM_SEG,iso[data_type]->ROI_ARROW_BASE);
	qdotIsoArrow2[data_type]->SetData(lb[data_type], rb[data_type],iso[data_type]->GetPointnum(),
			iso[data_type]->GetVecPos(),
			   iso[data_type]->GetVecDir(),iso[data_type]->GetDensity(),
			  iso[data_type]->GetExponent(),iso[data_type]->GetCoefficient(),
			NULL,NULL,
			iso[data_type]->NUM_SEG,iso[data_type]->ROI_ARROW_BASE);
	InitArrows(data_type);
	cerr<<"arrow"<<endl;

	Semi_Mag(data_type);
	SetContourColor(data_type);  

	if(enable_data_sync)
	{

		qdotBox[data_type]->SetPoints(qdotBox[1-data_type]->GetIntersectionp(),
								qdotBox[1-data_type]->GetWidget()); 
		qdotSlice[data_type]->SetPoints(qdotSlice[1-data_type]->GetWidget(),
							qdotSlice[1-data_type]->GetDir(),
							qdotSlice[1-data_type]->GetDir2());

		qdotBox[data_type]->SetDir2(qdotSlice[1-data_type]->GetDir2());
		qdotBox[data_type]->SetDir(qdotSlice[1-data_type]->GetDir()); 
		qdotSlice[data_type]->SetPlaneXYZ();
	}
}

void init()
{

  InitFile(InputFile1);

  min_density = 9e+9;
  max_density = -9e+9;
  qdotann = new svANN();
  enable_sync = 0;

  if(!strcmp(data_string[0], data_string[1]))
  {
	left_data_type = 0;
	right_data_type = 0;
  }
  else
  {
	left_data_type = 0;
	right_data_type = 1;
  }
  
  for(int i=0;i<2;i++)
  {
	  New_data(i);
	  InitData(i);
  }


	if(qdotField[0]->GetMaxDen() > qdotField[1]->GetMaxDen())
	{
		double min_d = qdotField[0]->GetMinDen_Nonzero() ;
		double max_d = qdotField[0]->GetMaxDen() ;
		min_density = min_d;
		max_density = max_d;
		InitMag(0);
	}
	else
	{
		InitMag(1);
		double min_d = qdotField[1]->GetMinDen_Nonzero() ;
		double max_d = qdotField[1]->GetMaxDen() ;
		min_density = min_d;
		max_density = max_d;
	}

	if(min_density!=0)
		mag_distance = log10(max_density)  - log10(min_density);
	else
		mag_distance = log10(max_density);

  for(int i=0;i<2;i++)
  {	
	InitIso(i);
	New_Render(i);
	InitRender(i);
	New_Cluster(i);
	InitClusters(i);
	New_Arrow(i);
	InitArrows(i);
	  Semi_Mag(i);
	  SetContourColor(i);  
  
 }
	Widget_t_zangle[0] = 0;
	Widget_t_zangle[1] = 0;
	Widget_t_mag[0] = 0;
	Widget_t_mag[1] = 0;
	Widget_select_mag[0] = false;
	Widget_select_mag[1] = false;

	Widget_xy_center[0] = 0;
	Widget_xy_center[1] = 0.;
	Widget_xy_radius = 25.;
	Widget_xy_min[0].push_back(Widget_xy_center[0]+Widget_xy_radius);
	Widget_xy_min[1].push_back(Widget_xy_center[1]);
	Widget_xy_max[0].push_back(Widget_xy_center[0]);
	Widget_xy_max[1].push_back(Widget_xy_center[1]-Widget_xy_radius);
	Widget_select_xy.push_back(false);
	Widget_select_xy.push_back(false);

	Reshape_Widget(image_width,image_height);
 	SetWidget();
	//z_standard[0] = zmin;
	//z_standard[1] = zmin;
	//cerr<<" "<<log10(range_mag[0][0])<<" "<<log10(min_density)<<" "<<mag_distance<<" "<<Widget_t_mag[0]<<" "<<Widget_bound_mag[1][1]<<" "<<" "<<Widget_bound_mag[0][1]<<endl;
 
  for(int i=0;i<2;i++)
{
 svVector3 center;
  center[0] = 0;
	center[1] =0;
	center[2] = 0;

  trackball[i].setEye(view_info.eye);
  trackball[i].setFocus(center);
  trackball[i].setWindowSize(image_width/2.,image_height);
  trackball[i].getMatrix().getValue(m[i]);

	__svl_lib::Matrix4f trans2(transformation_mat[i]);
	trackball[i].setMatrix(trans2);

  trackball_twod[i].setFocus(center);
  trackball_twod[i].setWindowSize(image_width/2.,image_height);

  trackball_legend[i].setFocus(center);
  trackball_legend[i].setWindowSize(image_width/2.,image_height);
}

	for(int i=0;i<2;i++)
	{
	twotranx[i] = 0;
	twotrany[i] = 0;
	twodzoom[i] = 1;
	}

  InitLight();
}



int
main(int argc, char** argv)
{
    //srand(12345);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInit(&argc, argv);

    glutInitWindowSize(1000,1000);
    glutInitWindowPosition(0, 0);


  if(argc == 2)
  {
	InputFile1 = strdup(argv[1]);
	//InputFile2 = strdup(argv[2]);
    	init();
  }
  else
  {
	cerr << "Usage:  ./qdotVis  [Original Data DirName] [Original Data FileName] " << endl;  
	exit(0);
  }



    window = glutCreateWindow("SPLIT");
    glutReshapeFunc(main_reshape);
    glutDisplayFunc(main_display);
    glutKeyboardFunc(main_key);

    left_window = glutCreateSubWindow(window, 500, 0, 500, 1000);

    GLUI_Master.set_glutReshapeFunc(left_reshape);
    GLUI_Master.set_glutDisplayFunc(left_display);
    GLUI_Master.set_glutMouseFunc(left_mouse);
    glutMotionFunc(left_motion);
    glutPassiveMotionFunc(left_movement);
    glutKeyboardFunc(main_key);

    right_window = glutCreateSubWindow(window, 500, 0, 500, 1000);
    GLUI_Master.set_glutReshapeFunc(left_reshape);
    GLUI_Master.set_glutDisplayFunc(right_display);
    GLUI_Master.set_glutMouseFunc(right_mouse);
    glutMotionFunc(right_motion);
    glutPassiveMotionFunc(right_movement);
    glutKeyboardFunc(main_key);
	
//----------------------

	glui_data_1 = GLUI_Master.create_glui("Data1 GLUI");
	glui_data_2 = GLUI_Master.create_glui("Data2 GLUI");

	GLUI_Panel *obj_panel_data1 = glui_data_1->add_panel("# of clusters");
	GLUI_EditText *edittext_data1_1 = glui_data_1->add_edittext_to_panel(obj_panel_data1 , "magnitude", GLUI_EDITTEXT_INT, &magnum[0],Magnum_id1, control_data1);
	GLUI_EditText *edittext_data1_2 = glui_data_1->add_edittext_to_panel(obj_panel_data1 ,"Orientation", GLUI_EDITTEXT_INT, &velnum[0],Velnum_id1, control_data1);
	GLUI_EditText *edittext_data1_3 = glui_data_1->add_edittext_to_panel(obj_panel_data1 ,"ROI magnitude", GLUI_EDITTEXT_INT, &magnumin[0],Magnumin_id1, control_data1);
	GLUI_EditText *edittext_data1_4 = glui_data_1->add_edittext_to_panel(obj_panel_data1 ,"ROI orietation", GLUI_EDITTEXT_INT, &velnumin[0],Velnumin_id1, control_data1);

	GLUI_Panel *obj_panel_data2 = glui_data_2->add_panel("# of clusters");
	GLUI_EditText *edittext_data2_1 = glui_data_2->add_edittext_to_panel(obj_panel_data2 , "magnitude", GLUI_EDITTEXT_INT, &magnum[1],Magnum_id2, control_data2);
	GLUI_EditText *edittext_data2_2 = glui_data_2->add_edittext_to_panel(obj_panel_data2 ,"Orientation", GLUI_EDITTEXT_INT, &velnum[1],Velnum_id2, control_data2);
	GLUI_EditText *edittext_data2_3 = glui_data_2->add_edittext_to_panel(obj_panel_data2 ,"ROI magnitude", GLUI_EDITTEXT_INT, &magnumin[1],Magnumin_id2, control_data2);
	GLUI_EditText *edittext_data2_4 = glui_data_2->add_edittext_to_panel(obj_panel_data2 ,"ROI orietation", GLUI_EDITTEXT_INT, &velnumin[1],Velnumin_id2, control_data2);

	glui = GLUI_Master.create_glui("SPLIT_GLUI");
	glui->set_main_gfx_window(window);
	GLUI_Panel *obj_panel_whole = glui->add_panel("Magnitude Encoding");
	GLUI_RadioGroup *group_whole =  glui->add_radiogroup_to_panel(obj_panel_whole,&magnitude_encoding,MagEncode,control_whole);
	glui->add_radiobutton_to_group(group_whole,"Linear");
	glui->add_radiobutton_to_group(group_whole,"SplitVectors");
	//new GLUI_Checkbox(glui,"Show Projection",&enable_projection,Projection_id,control_whole);
	new GLUI_Checkbox(glui,"Projection of Aggregation",&projectionaggregation,Projectionaggregation_id,control_whole);
	//new GLUI_Checkbox(glui,"Show Clusters",&enable_clustering,Cluster_id,control_whole);
	new GLUI_Checkbox(glui,"Semitransparent data outside ROI Box",&enable_roi,ROI_id,control_whole);

	GLUI_Panel *obj_panel_zlayer = glui->add_panel("Z Layer");
	char str[20];
	sprintf(str,"%0.2f",zmin);
	obj_text_zlayer = glui->add_statictext_to_panel(obj_panel_zlayer,str);
	glui->add_column_to_panel(obj_panel_zlayer,true);
	sb_zlayer = new GLUI_Scrollbar(obj_panel_zlayer,"zlayer",GLUI_SCROLL_HORIZONTAL,&zmin,Zlayer_id,control_whole);
	sb_zlayer->set_float_limits(lb[0][2],rb[0][2] );

	GLUI_Panel *obj_panel_twodglyph_3 = glui->add_panel("Distance Comparison");
	GLUI_Panel *obj_panel_twodglyph_5 = glui->add_panel_to_panel(obj_panel_twodglyph_3,"Vector");
	GLUI_EditText *vectorx = glui->add_edittext_to_panel(obj_panel_twodglyph_5, "x", GLUI_EDITTEXT_FLOAT, &vector_x, Vector_id, control_whole);
	GLUI_EditText *vectory = glui->add_edittext_to_panel(obj_panel_twodglyph_5, "y", GLUI_EDITTEXT_FLOAT, &vector_y, Vector_id, control_whole);
	GLUI_EditText *vectorz = glui->add_edittext_to_panel(obj_panel_twodglyph_5, "z", GLUI_EDITTEXT_FLOAT, &vector_z, Vector_id, control_whole);
	
	new GLUI_Checkbox(obj_panel_twodglyph_5, "Select one point", &enable_select2d);
	GLUI_EditText *edittext5 = glui->add_edittext_to_panel(obj_panel_twodglyph_3 ,"Min degree of brightness: 180/", GLUI_EDITTEXT_INT, &lightness_degree);
	GLUI_Button *updatebutton = glui->add_button_to_panel(obj_panel_twodglyph_3 ,"Update", Z_refer_id,control_whole);
/*
	GLUI_Panel *obj_panel_4 = glui->add_panel("# of clusters");
	GLUI_EditText *edittext = glui->add_edittext_to_panel(obj_panel_4 , "magnitude", GLUI_EDITTEXT_INT, &magnum,Magnum_id, control_whole);
	GLUI_EditText *edittext2 = glui->add_edittext_to_panel(obj_panel_4 ,"Orientation", GLUI_EDITTEXT_INT, &velnum,Velnum_id, control_whole);
//	magnumin = (int)num_magnitude_clusters_in;
//	velnumin = (int)num_orientation_clusters_in;
	GLUI_EditText *edittext3 = glui->add_edittext_to_panel(obj_panel_4 ,"ROI magnitude", GLUI_EDITTEXT_INT, &magnumin,Magnumin_id, control_whole);
	GLUI_EditText *edittext4 = glui->add_edittext_to_panel(obj_panel_4 ,"ROI orietation", GLUI_EDITTEXT_INT, &velnumin,Velnumin_id, control_whole);*/

	GLUI_Panel *panel_visible = glui->add_panel("Visible");
	new GLUI_Checkbox(panel_visible, "Cutting Plane", &enable_cuttingplane);
	new GLUI_Checkbox(panel_visible, "ROI Box", &enable_box,ROIBox_id, control_whole);
	new GLUI_Checkbox(panel_visible, "Z Plane", &enable_zplane);
	new GLUI_Checkbox(panel_visible, "Projection",&enable_projection,Projection_id,control_whole);
	new GLUI_Checkbox(panel_visible, "Contour", &enable_contour);
	new GLUI_Checkbox(panel_visible, "Magnitude Legend (original)", &enable_maglegend1);	
	new GLUI_Checkbox(panel_visible, "Magnitude Legend (contour)", &enable_maglegend2);
	new GLUI_Checkbox(panel_visible, "Magnitude Legend (ROI)", &enable_maglegend3);
	new GLUI_Checkbox(panel_visible, "Orientation Legend (original)", &enable_vellegend1);
	new GLUI_Checkbox(panel_visible, "Orientation Legend (contour)", &enable_vellegend2);
	new GLUI_Checkbox(panel_visible, "Orientation Legend (ROI)", &enable_vellegend3);
	new GLUI_Checkbox(panel_visible, "Sum (of magnitude)", &enable_sum);
	new GLUI_Checkbox(panel_visible, "Sliders", &enable_slider);
	new GLUI_Checkbox(panel_visible, "Bounding Box", &enable_bounding);
	new GLUI_Checkbox(panel_visible, "2D legend",&enable_2dlegend);
	new GLUI_Checkbox(panel_visible, "Sync(sphere)",&enable_sync);
	new GLUI_Checkbox(panel_visible, "Sync(Data)",&enable_data_sync, Syncdata_id, control_whole);

	new GLUI_Button(glui, "Generate contours", Contour_id,control_whole);
	new GLUI_Button(glui, "Load File", Loadfile_id,control_whole);
	new GLUI_Button(glui, "Save to File", Savefile_id,control_whole);
	new GLUI_Button(glui, "Save to Picture", SavePicture_id,control_whole);

	glui_left = GLUI_Master.create_glui_subwindow( left_window,GLUI_SUBWINDOW_TOP);
	GLUI_Listbox *glui_left_drop = new GLUI_Listbox(glui_left,"Display",&left_display_type,Leftdrop_id,control_left);
	glui_left_drop->add_item(0,"Magnitude");
	glui_left_drop->add_item(1,"Orientation");
	glui_left_drop->add_item(2,"Distance Comparison");
	glui_left->add_column(true);
	new GLUI_Button(glui_left,"Load data",Leftdata_id,control_left);

	glui_left_pop = GLUI_Master.create_glui("Data Selection");
	GLUI_StaticText *obj_text_left = glui_left_pop->add_statictext("Data");
	lefthah = new GLUI_List(glui_left_pop,true,Leftpop_id,control_left_pop);
	lefthah->set_w(250);
	new GLUI_Button(glui_left_pop, "OK", Leftselect_id,control_left_pop);
	glui_left_pop->hide();

	glui_right = GLUI_Master.create_glui_subwindow( right_window,GLUI_SUBWINDOW_TOP);
	GLUI_Listbox *glui_right_drop = new GLUI_Listbox(glui_right,"Display",&right_display_type,Rightdrop_id,control_right);
	glui_right_drop->add_item(0,"Magnitude");
	glui_right_drop->add_item(1,"Orientation");
	glui_right_drop->add_item(2,"Distance Comparison");
	glui_right->add_column(true);
	new GLUI_Button(glui_right,"Load data",Rightdata_id,control_right);

	glui_right_pop = GLUI_Master.create_glui("Data Selection");
	GLUI_StaticText *obj_text_right = glui_right_pop->add_statictext("Data");
	righthah = new GLUI_List(glui_right_pop,true,Rightpop_id,control_right_pop);
	righthah->set_w(250);
	new GLUI_Button(glui_right_pop,"OK", Rightselect_id, control_right_pop);
	glui_right_pop->hide();

	//GLUI_EditText *edittextright = glui_right->add_edittext("Data (directory of config file)", GLUI_EDITTEXT_TEXT, &rightdatastring);
	//glui_right->add_column(true);
	//new GLUI_Button(glui_right,"Update data",Rightdata_id, control_right);

    redisplay_all();	
    glutMainLoop();
	//Restore();

    return 0;
}
