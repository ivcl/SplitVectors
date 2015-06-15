#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <string.h>
#include <vector>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "../src/svUtil.h"
#include "../src/ivview.h"
#include "../src/jcUtil/MGL.h"

using namespace __svl_lib;
using namespace std;
extern svScalar min_density;
extern svScalar max_density;
extern svScalar mag_distance;

extern float zangle_min;
extern float zangle_max;
extern svVector3 Widget_z_angle[4];
extern svVector3 Widget_bound_zangle[2];
extern float Widget_min_degree_zangle;
extern bool Widget_select_zangle[2];
extern float Widget_t_zangle[2];

extern float range_mag[2];
extern svVector3 Widget_z_mag[4];
extern svVector3 Widget_bound_mag[2];
extern float Widget_min_degree_mag;
extern bool Widget_select_mag[2];
extern float Widget_t_mag[2];

extern vector<double> Widget_xy_min[3];
extern vector<double> Widget_xy_max[3];
extern vector<bool> Widget_select_xy;
extern svVector3 Widget_xy_center;
extern svScalar Widget_xy_radius;
extern vector<double> xy_min;
extern vector<double> xy_max;
extern float Widget_xy_h;
extern float Widget_xy_w;
extern bool Widget_select_xy_whole;
extern double Widget_xy_theta[2];

float GetAngle(float p1, float p2);
void Reshape_Widget(int w, int h);
void Widget_Mag();
void Widget_Z_angle();
void Widget_XY();

extern bool Widget_flag;
