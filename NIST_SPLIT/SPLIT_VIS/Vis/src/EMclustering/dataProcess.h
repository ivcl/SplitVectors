//velocity clustering need to be improved
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "emgm.h"
#include "colorsEM.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <math.h>
#include <vector>
#include <algorithm>

#ifndef PI
#define PI 3.1415926
#endif

using namespace std;

namespace __em_lib{

class Cluster{

public:
	Cluster(){dataSize=0;zmin=0;qdotem=new EMclustering();qdotem_in=new EMclustering();widget_t_l=0;
		widget_t_h=0;widget_select[0]=false;widget_select[1]=false;}
	~Cluster(){cleanup();}

	void vel_display(bool *in);
	void DrawVelLegend(int n);
	void DrawMagLegend_in();
	void DrawVelLegend_in();
	void mag_display(bool *in);
	void DrawMagLegend();
	void vel_display2();

	void mag_display2();

	void SetMagNum(int n){clusternum_mag[0]=n;}
	void SetVelNum(int n){clusternum_vel[0]=n;}
	void SetMaginNum(int n){clusternum_mag_in[0]=n;}
	void SetVelinNum(int n){clusternum_vel_in[0]=n;}
	void ReadColors(int mode);
	void SetROIColors();
	void MagProcess();
	void VelProcess(bool existing);
	void MaginProcess();
	void VelinProcess();

	void RenderWidget();

	void SetEMdata(vector<vector<double> >p[3], vector<vector<double> > v[3], vector<vector<double> > d, int zValue,
		int num_mag_cluster);
	void SetEMdata2(vector<vector<double> >p[3], vector<vector<double> > v[3], vector<vector<double> > d, int zValue,
		int num_mag_cluster);//not detect symmetry
	void GetVelClusters(int c1, int c2);
	void GetVelClusters();
	void GetMagClusters();
	void GetROIVelClusters(vector<bool> in);
	//void GetROIVelClusters2(vector<bool> in);//no detect symmetry
	void GetROIMagClusters(vector<bool> in);

	void SetZ(float z){zmin = z;}
	
	void GetMagClusters_file(char *file);
	void GetVelClusters_file(char *file);
	void Savemagtofile(char *file);
	void Saveveltofile(char *file);
	
	void GetMagColors(double *r, double *g, double *b);
	void GetMagColors(int n, bool *in, double *d, double *r, double *g, double *b);
	void GetVelColors(double *r, double *g, double *b);
	void GetVelHue(double *h);
	vector<double> GetOrderHue();

	bool scompare1(int a, int b);
	bool scompare2(int a, int b);
	vector<double> magrange[2];
	vector<double>magrange_in[2];
	void SetScale(float s){d_scale = s;}
	double GetML(){return m_length;}
	double GetMLin(){return m_length_in;}

	int *GetIDXMag();//{return IDX_Mag;}
	int *GetIDXVel();//{return IDX_Vel;}
	//vector<double> GetVelHue();

	void GetMagRange();
	void selectwidget(int x, int y, int x2, int y2);
	void movewidget(int x,int y);
	double GetRangeMag1(){return range_mag[0];}
	double GetRangeMag2(){return range_mag[1];}

	void cleanup();
	void cleanup_nocolor();

	bool widget_select[2];
private:
	int dataSize;

	int *IDX_Mag;
	int *IDX_Vel;
	int *IDX_Mag_In;
	int *IDX_Vel_In;

	int Index_Vel[2];

	vector<int> sym_index;
	
	vector<vector<double> > position;
	vector<vector<double> > velocity;
	vector<vector<double> > velocity2;
	vector<double> density;
	double max_density;
	double min_density;
	double max_density_in;
	double min_density_in;

	vector<double> density_unique;	
	vector<int> density_index;//unique index
	vector<vector<double> > velocity_unique;
	vector<vector<int> > unique_index;//for velocity
	vector<vector<int> > unique_index2;//for densityity
	vector<int> velocity_index;//unique index
	vector<vector<int> > velocity_density;
	vector<vector<int> > velocity_density_in;

	int clusternum_error_mag;
	int clusternum_error_mag_in;
	float whole_min_degree;
	vector<double> density_color[3];
	vector<double> density_color_in[3];

	vector<double> velocity_center[3];
	vector<double> velocity_center_in[3];
	vector<double> velocity_color[3];
	vector<double> velocity_hue;
	vector<double> velocity_color_in[3];
	vector<double> velocity_hue_in;
	vector<int> velocity_order;
	vector<int> velocity_order_in;

	int clusternum_mag[1];
	int clusternum_vel[1];
	int clusternum_mag_in[1];
	int clusternum_vel_in[1];

	vector<double> spiralcolors_mag[3];
	vector<double> spiralcolors_vel[3];
	vector<double> spiralcolors_vel_hue;
	vector<double> spiralcolors_mag_in[3];
	vector<double> spiralcolors_vel_in[3];
	vector<double> spiralcolors_vel_in_hue;

	vector<int>IDX_sort_mag;
	vector<int>IDX_sort_mag_in;
	vector<int>IDX_mag_order;
	vector<int>IDX_mag_order_in;

	float zmin;
	double d_scale;
	double m_length;
	double m_length_in;

	EMclustering *qdotem;
	EMclustering *qdotem_in;

	vector<double> min_degree_mag;
	double widget_t_l;
	double widget_t_h;
	double widget_l[3];
	double widget_h[3];
	double range_mag[2];

	int mouse_x;
	int mouse_y;
};

}
