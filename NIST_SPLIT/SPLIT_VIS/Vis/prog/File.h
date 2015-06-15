#include "Sliders.h"
#include <dirent.h>

//using namespace __svl_lib;
//using namespace std;

extern vector<string> datastring;
extern vector<string> folderstring;

extern char DataDir[2][50];
extern char datafile[2][50];
extern char ContourFile2[2][50];
extern char VTKfile[2][200];
extern char LogFile[50];
extern char tmpfiledir[50];
extern char picturedir[50];

extern int left_display_type;
extern int right_display_type;

extern int magnitude_encoding;
extern int projectionaggregation;
extern int enable_roi;
extern float zmin;
extern float vector_x;
extern float vector_y;
extern float vector_z;
extern int lightness_degree;
extern vector<float> contour_ratio[2];
extern int magnum[2];
extern int velnum[2];
extern int magnumin[2];
extern int velnumin[2];
extern float num_magnitude_clusters[2];
extern float num_magnitude_clusters_in[2];
extern float num_orientation_clusters[2];
extern float num_orientation_clusters_in[2];

extern int enable_cuttingplane;
extern int enable_box;
extern int enable_zplane;
extern int enable_projection;
extern int enable_contour;
extern int enable_maglegend1;
extern int enable_maglegend2;
extern int enable_maglegend3;
extern int enable_vellegend1;
extern int enable_vellegend2;
extern int enable_vellegend3;
extern int enable_sum;
extern int enable_slider;
extern int enable_bounding;
extern int enable_2dlegend;
extern int enable_select2d;

extern float transformation_mat[2][16];

extern int left_data_type;
extern int right_data_type ;

extern float arrow_base[2];
extern float num_segment_arrow[2];
extern float z_standard[2];

extern char data_string[2][50];

extern void ListFile(char *dir);
extern void ListFolder(char *dir2);
extern void InitFile(char *file);
//extern void InitFile(char *file, int data_type);
extern void InitFile2(char *file, int data_type);
extern void SavetoFile(char *file);
extern void SetWidget();
