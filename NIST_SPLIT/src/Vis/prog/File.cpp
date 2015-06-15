#include "File.h"
//http://www.cplusplus.com/forum/beginner/9173/
void ListFile(char *dir)
{
	datastring.clear();

      DIR *pDIR;
        struct dirent *entry;
	int i=0;
        if( pDIR=opendir(dir) ){
                while(entry = readdir(pDIR)){
                        if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 )
			{
			datastring.push_back(entry->d_name);
                        //cout << entry->d_name << "\n";
			cout <<datastring[i]<<endl;
			i++;
			}
                }
                closedir(pDIR);
        }	
}
//http://stackoverflow.com/questions/5043403/listing-only-folders-in-directory
void ListFolder(char *dir2)
{
    DIR *dir = opendir(dir2);

	folderstring.clear();

    struct dirent *entry = readdir(dir);

    while (entry != NULL)
    {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
			folderstring.push_back( entry->d_name);
		   // printf("%s\n", entry->d_name);
	}

        entry = readdir(dir);
    }

    closedir(dir);

   // return 0;
}

void InitFile(char *file)
{
 	ifstream inparameters(file);

  	string tmp;

	if(inparameters.fail())
	{
		cerr<<"No input parameter file"<<endl;
		exit(-1);
	}

	float store;
	
	inparameters>>tmp;//data file name
	//cerr<<tmp<<endl;
	inparameters>>DataDir[0];
	//cerr<<DataDir[data_type]<<endl;
	inparameters>>datafile[0];
	sprintf(data_string[0], "%s", datafile[0]);
	inparameters>>DataDir[1];
	//cerr<<DataDir[data_type]<<endl;
	inparameters>>datafile[1];
	sprintf(data_string[1], "%s", datafile[1]);
	
	inparameters>>tmp;//vtk file
	//cerr<<tmp<<endl;
	inparameters>>VTKfile[0];
	//cerr<<VTKfile[data_type]<<endl;
	inparameters>>VTKfile[1];
	//cerr<<VTKfile[data_type]<<endl;

	inparameters>>tmp;//log file
	//cerr<<tmp<<endl;
	inparameters>>LogFile;

	inparameters>>tmp;//picture file
	//cerr<<tmp<<endl;
	inparameters>>picturedir;

	inparameters>>tmp;//temp file
	//cerr<<tmp<<endl;
	inparameters>>tmpfiledir;

	inparameters>>tmp;//left window
	//cerr<<tmp<<endl;
	inparameters>>store;
	left_display_type= (int)store;

	inparameters>>tmp;//right window
	inparameters>>store;
	right_display_type= (int)store;

        inparameters>>tmp;//magnitude encoding
        inparameters>>store;
	magnitude_encoding=(int)store;

        inparameters>>tmp;//projection aggregation
        inparameters>>store;
	projectionaggregation=(int)store;

        inparameters>>tmp;//opaque data inside ROI box
        inparameters>>store;
	enable_roi = (int)store;

        inparameters>>tmp;//reference layer
        inparameters>>zmin;
	z_standard[0]=zmin;
	z_standard[1] = zmin;

        inparameters>>tmp;//input vector
        inparameters>>vector_x>>vector_y>>vector_z;

        inparameters>>tmp;// enable to select one item in 2d map
        inparameters>>store;
	enable_select2d = (int)store;

        inparameters>>tmp;// resulotion of lightness
        inparameters>>store;
	lightness_degree = 180./store;

        inparameters>>tmp; // contours
        inparameters>>store;
	float store2;
	for(int i=0;i<(int)store;i++)
	{
		inparameters>>store2;
		contour_ratio[0].push_back(store2);
	}
        inparameters>>store;
	//float store2;
	for(int i=0;i<(int)store;i++)
	{
		inparameters>>store2;
		contour_ratio[1].push_back(store2);
	}

        inparameters>>tmp; // # of mag clusters
        inparameters>>num_magnitude_clusters[0];
	magnum[0]= (int)num_magnitude_clusters[0];
        inparameters>>num_magnitude_clusters[1];
	magnum[1]= (int)num_magnitude_clusters[1];

        inparameters>>tmp; // # of vel clusters
        inparameters>>num_orientation_clusters[0];
	velnum[0] = (int)num_orientation_clusters[0];
        inparameters>>num_orientation_clusters[1];
	velnum[1] = (int)num_orientation_clusters[1];

        inparameters>>tmp; // # of mag clusters roi
	cerr<<tmp<<endl;
        inparameters>>num_magnitude_clusters_in[0];
	magnumin[0] = (int)num_magnitude_clusters_in[0];
        inparameters>>num_magnitude_clusters_in[1];
	magnumin[1] = (int)num_magnitude_clusters_in[1];

        inparameters>>tmp; // # of vel clusters roi
        inparameters>>num_orientation_clusters_in[0];
	velnumin[0] = (int)num_orientation_clusters_in[0];	
        inparameters>>num_orientation_clusters_in[1];
	velnumin[1] = (int)num_orientation_clusters_in[1];	

        inparameters>>tmp;//visible
        inparameters>>tmp;//cutting plane
        inparameters>>store;
	enable_cuttingplane = (int)store;

        inparameters>>tmp;//roi box
        inparameters>>store;
	enable_box = (int)store;

        inparameters>>tmp;//z plane
        inparameters>>store;
	enable_zplane = (int)store;

        inparameters>>tmp;//projection
        inparameters>>store;
	enable_projection = (int)store;

        inparameters>>tmp;//contour
        inparameters>>store;
	enable_contour = (int)store;

        inparameters>>tmp;//mag legend ori
        inparameters>>store;
	enable_maglegend1 = (int)store;

        inparameters>>tmp;//mag legend contour
        inparameters>>store;
	enable_maglegend2 = (int)store;

        inparameters>>tmp; //mag legend roi
        inparameters>>store;
	enable_maglegend3 = (int)store;

        inparameters>>tmp;//vel legend ori
        inparameters>>store;
	enable_vellegend1 = (int)store;

        inparameters>>tmp;//vel legend contour
        inparameters>>store;
	enable_vellegend2 = (int)store;

        inparameters>>tmp;// vel legend roi
        inparameters>>store;
	enable_vellegend3 = (int)store;

        inparameters>>tmp;//sum
        inparameters>>store;
        enable_sum= (int)store;

        inparameters>>tmp;//slider
        inparameters>>store;
	enable_slider = (int)store;

        inparameters>>tmp;//bounding box
        inparameters>>store;
	enable_bounding = (int)store;

        inparameters>>tmp;//2d legend
        inparameters>>store;
	enable_2dlegend = (int)store;

        inparameters>>tmp;//visible mag range (!!!!!!!!!!)
	//cerr<<tmp<<endl;
        inparameters>>range_mag[0]>>range_mag[1];
	//cerr<<range_mag[0]<<" "<<range_mag[1]<<endl;

        inparameters>>tmp;//visible xy angle range (!!!!!!!!!!!!!!)
        inparameters>>store>>store2;
	if(xy_min.size()==0)
	{
		xy_min.push_back(store);
		xy_max.push_back(store2);
		xy_min[0] = xy_min[0]/180. * M_PI;
		xy_max[0] = xy_max[0]/180. * M_PI;
	}

        inparameters>>tmp;//visible z angle range (!!!!!!!!!!!!!!!!!!)
        inparameters>>zangle_min>>zangle_max;
	zangle_min = zangle_min + 90;
	zangle_max = zangle_max + 90;

        inparameters>>tmp;//transformation matrix
	for(int i=0;i<16;i++)
	{
		inparameters>>transformation_mat[0][i];
	}	
        //inparameters>>tmp;//transformation matrix
	for(int i=0;i<16;i++)
	{
		inparameters>>transformation_mat[1][i];
	}
	
  	inparameters.close();

	num_segment_arrow[1] = 4;
	arrow_base[1]	=0.25;
}
/*
void InitFile(char *file, int data_type)
{
	for(int i=0;i<strlen(file);i++)
	{
		data_string[data_type][i] = file[i];
	}

 	ifstream inparameters(file);

  	string tmp;

	if(inparameters.fail())
	{
		cerr<<"No input parameter file"<<endl;
		exit(-1);
	}

	float store;
	
	inparameters>>tmp;//data file name
	//cerr<<tmp<<endl;
	inparameters>>DataDir[data_type];
	//cerr<<DataDir[data_type]<<endl;
	inparameters>>datafile[data_type];
	
	inparameters>>tmp;//vtk file
	//cerr<<tmp<<endl;
	inparameters>>VTKfile[data_type];
	//cerr<<VTKfile[data_type]<<endl;
	inparameters>>tmp;//log file
	//cerr<<tmp<<endl;
	inparameters>>LogFile[data_type];

	inparameters>>tmp;//picture file
	//cerr<<tmp<<endl;
	inparameters>>picturedir[data_type];

	inparameters>>tmp;//temp file
	//cerr<<tmp<<endl;
	inparameters>>tmpfiledir[data_type];

	inparameters>>tmp;//left window
	//cerr<<tmp<<endl;
	inparameters>>store;
	left_display_type= (int)store;

	inparameters>>tmp;//right window
	inparameters>>store;
	right_display_type= (int)store;

        inparameters>>tmp;//magnitude encoding
        inparameters>>store;
	magnitude_encoding=(int)store;

        inparameters>>tmp;//projection aggregation
        inparameters>>store;
	projectionaggregation=(int)store;

        inparameters>>tmp;//opaque data inside ROI box
        inparameters>>store;
	enable_roi = (int)store;

        inparameters>>tmp;//reference layer
        inparameters>>zmin;
	z_standard[data_type]=zmin;

        inparameters>>tmp;//input vector
        inparameters>>vector_x>>vector_y>>vector_z;

        inparameters>>tmp;// enable to select one item in 2d map
        inparameters>>store;
	enable_select2d = (int)store;

        inparameters>>tmp;// resulotion of lightness
        inparameters>>store;
	lightness_degree = 180./store;

        inparameters>>tmp; // contours
        inparameters>>store;
	float store2;
	for(int i=0;i<(int)store;i++)
	{
		inparameters>>store2;
		contour_ratio[data_type].push_back(store2);
	}

        inparameters>>tmp; // # of mag clusters
        inparameters>>num_magnitude_clusters[data_type];
	magnum= (int)num_magnitude_clusters[data_type];

        inparameters>>tmp; // # of vel clusters
        inparameters>>num_orientation_clusters[data_type];
	velnum = (int)num_orientation_clusters[data_type];

        inparameters>>tmp; // # of mag clusters roi
	cerr<<tmp<<endl;
        inparameters>>num_magnitude_clusters_in[data_type];
	magnumin = (int)num_magnitude_clusters_in[data_type];

        inparameters>>tmp; // # of vel clusters roi
        inparameters>>num_orientation_clusters_in[data_type];
	velnumin = (int)num_orientation_clusters_in[data_type];	

        inparameters>>tmp;//visible
        inparameters>>tmp;//cutting plane
        inparameters>>store;
	enable_cuttingplane = (int)store;

        inparameters>>tmp;//roi box
        inparameters>>store;
	enable_box = (int)store;

        inparameters>>tmp;//z plane
        inparameters>>store;
	enable_zplane = (int)store;

        inparameters>>tmp;//projection
        inparameters>>store;
	enable_projection = (int)store;

        inparameters>>tmp;//contour
        inparameters>>store;
	enable_contour = (int)store;

        inparameters>>tmp;//mag legend ori
        inparameters>>store;
	enable_maglegend1 = (int)store;

        inparameters>>tmp;//mag legend contour
        inparameters>>store;
	enable_maglegend2 = (int)store;

        inparameters>>tmp; //mag legend roi
        inparameters>>store;
	enable_maglegend3 = (int)store;

        inparameters>>tmp;//vel legend ori
        inparameters>>store;
	enable_vellegend1 = (int)store;

        inparameters>>tmp;//vel legend contour
        inparameters>>store;
	enable_vellegend2 = (int)store;

        inparameters>>tmp;// vel legend roi
        inparameters>>store;
	enable_vellegend3 = (int)store;

        inparameters>>tmp;//sum
        inparameters>>store;
        enable_sum= (int)store;

        inparameters>>tmp;//slider
        inparameters>>store;
	enable_slider = (int)store;

        inparameters>>tmp;//bounding box
        inparameters>>store;
	enable_bounding = (int)store;

        inparameters>>tmp;//2d legend
        inparameters>>store;
	enable_2dlegend = (int)store;

        inparameters>>tmp;//visible mag range (!!!!!!!!!!)
        inparameters>>range_mag[data_type][0]>>range_mag[data_type][1];

        inparameters>>tmp;//visible xy angle range (!!!!!!!!!!!!!!)
        inparameters>>store>>store2;
	if(xy_min.size()==0)
	{
		xy_min.push_back(store);
		xy_max.push_back(store2);
		xy_min[0] = xy_min[0]/180. * M_PI;
		xy_max[0] = xy_max[0]/180. * M_PI;
	}

        inparameters>>tmp;//visible z angle range (!!!!!!!!!!!!!!!!!!)
        inparameters>>zangle_min>>zangle_max;
	zangle_min = zangle_min + 90;
	zangle_max = zangle_max + 90;

        inparameters>>tmp;//transformation matrix
	for(int i=0;i<16;i++)
	{
		inparameters>>transformation_mat[data_type][i];
	}	
	
  	inparameters.close();

	num_segment_arrow[1] = 4;
	arrow_base[1]	=0.25;
}*/
/*void InitFile2(char *file, int data_type)
{
	for(int i=0;i<strlen(file);i++)
	{
		data_string[data_type][i] = file[i];
	}

 	ifstream inparameters(file);

  	string tmp;

	if(inparameters.fail())
	{
		cerr<<"No input parameter file"<<endl;
		exit(-1);
	}

	float store,store2, store3;
	
	inparameters>>tmp;//data file name
	//cerr<<tmp<<endl;
	inparameters>>DataDir[data_type];
	//cerr<<DataDir[data_type]<<endl;
	inparameters>>datafile[data_type];
	
	inparameters>>tmp;//vtk file
	//cerr<<tmp<<endl;
	inparameters>>VTKfile[data_type];
	//cerr<<VTKfile[data_type]<<endl;
	inparameters>>tmp;//log file
	//cerr<<tmp<<endl;
	inparameters>>LogFile[data_type];

	inparameters>>tmp;//picture file
	//cerr<<tmp<<endl;
	inparameters>>picturedir[data_type];

	inparameters>>tmp;//temp file
	//cerr<<tmp<<endl;
	inparameters>>tmpfiledir[data_type];

	inparameters>>tmp;//left window
	//cerr<<tmp<<endl;
	inparameters>>store;
	//left_display_type= (int)store;

	inparameters>>tmp;//right window
	inparameters>>store;
	//right_display_type= (int)store;

        inparameters>>tmp;//magnitude encoding
        inparameters>>store;
	//magnitude_encoding=(int)store;

        inparameters>>tmp;//projection aggregation
        inparameters>>store;
	//projectionaggregation=(int)store;

        inparameters>>tmp;//opaque data inside ROI box
        inparameters>>store;
	//enable_roi = (int)store;

        inparameters>>tmp;//reference layer
        inparameters>>store;

        inparameters>>tmp;//input vector
        inparameters>>store>>store2>>store3;

        inparameters>>tmp;// enable to select one item in 2d map
        inparameters>>store;
	//enable_select2d = (int)store;

        inparameters>>tmp;// resulotion of lightness
        inparameters>>store;
	//lightness_degree = 180./store;

        inparameters>>tmp; // contours
        inparameters>>store;
	contour_ratio[data_type].clear();
	for(int i=0;i<(int)store;i++)
	{
		inparameters>>store2;
		contour_ratio[data_type].push_back(store2);
	}

        inparameters>>tmp; // # of mag clusters
        inparameters>>store;//num_magnitude_clusters[data_type];
	//magnum= (int)num_magnitude_clusters[data_type];

        inparameters>>tmp; // # of vel clusters
        inparameters>>store;//num_orientation_clusters[data_type];
	//velnum = (int)num_orientation_clusters[data_type];

        inparameters>>tmp; // # of mag clusters roi
	cerr<<tmp<<endl;
        inparameters>>store;//num_magnitude_clusters_in[data_type];
	//magnumin = (int)num_magnitude_clusters_in[data_type];

        inparameters>>tmp; // # of vel clusters roi
        inparameters>>store;//num_orientation_clusters_in[data_type];
	//velnumin = (int)num_orientation_clusters_in[data_type];	

        inparameters>>tmp;//visible
        inparameters>>tmp;//cutting plane
        inparameters>>store;
	//enable_cuttingplane = (int)store;

        inparameters>>tmp;//roi box
        inparameters>>store;
	//enable_box = (int)store;

        inparameters>>tmp;//z plane
        inparameters>>store;
	//enable_zplane = (int)store;

        inparameters>>tmp;//projection
        inparameters>>store;
	//enable_projection = (int)store;

        inparameters>>tmp;//contour
        inparameters>>store;
	//enable_contour = (int)store;

        inparameters>>tmp;//mag legend ori
        inparameters>>store;
	//enable_maglegend1 = (int)store;

        inparameters>>tmp;//mag legend contour
        inparameters>>store;
	//enable_maglegend2 = (int)store;

        inparameters>>tmp; //mag legend roi
        inparameters>>store;
	//enable_maglegend3 = (int)store;

        inparameters>>tmp;//vel legend ori
        inparameters>>store;
	//enable_vellegend1 = (int)store;

        inparameters>>tmp;//vel legend contour
        inparameters>>store;
	//enable_vellegend2 = (int)store;

        inparameters>>tmp;// vel legend roi
        inparameters>>store;
	//enable_vellegend3 = (int)store;

        inparameters>>tmp;//sum
        inparameters>>store;
        //enable_sum= (int)store;

        inparameters>>tmp;//slider
        inparameters>>store;
	//enable_slider = (int)store;

        inparameters>>tmp;//bounding box
        inparameters>>store;
	//enable_bounding = (int)store;

        inparameters>>tmp;//2d legend
        inparameters>>store;
	//enable_2dlegend = (int)store;

        inparameters>>tmp;//visible mag range (!!!!!!!!!!)
        inparameters>>store>>store2;//range_mag[data_type][0]>>range_mag[data_type][1];

        inparameters>>tmp;//visible xy angle range (!!!!!!!!!!!!!!)
        inparameters>>store>>store2;
	if(xy_min.size()==0)
	{
		xy_min.push_back(store);
		xy_max.push_back(store2);
		xy_min[0] = xy_min[0]/180. * M_PI;
		xy_max[0] = xy_max[0]/180. * M_PI;
	}

        inparameters>>tmp;//visible z angle range (!!!!!!!!!!!!!!!!!!)
        inparameters>>store>>store2;//zangle_min>>zangle_max;
	//zangle_min = zangle_min + 90;
	//zangle_max = zangle_max + 90;

        inparameters>>tmp;//transformation matrix
	for(int i=0;i<16;i++)
	{
		inparameters>>transformation_mat[data_type][i];
	}	
	
  	inparameters.close();

	//num_segment_arrow[1] = 4;
	//arrow_base[1]	=0.25;
}*/
void SavetoFile(char *file)
{
	float store;
	ofstream outfile(file);

	outfile<<"#Data"<<endl;
	outfile<<DataDir[0]<<endl;
	outfile<<datafile[0]<<endl;
	outfile<<DataDir[1]<<endl;
	outfile<<datafile[1]<<endl;

	outfile<<"#VTK_Data"<<endl;
	outfile<<VTKfile[0]<<endl;
	outfile<<VTKfile[1]<<endl;

	outfile<<"#Log"<<endl;
	outfile<<LogFile<<endl;
	
	outfile<<"#Pictures"<<endl;
	outfile<<picturedir<<endl;

	outfile<<"#Temp_File"<<endl;
	outfile<<tmpfiledir<<endl;

	outfile<<"#Left_window(0:magnitude,1:orientation,2:2D_map)"<<endl;
	store = left_display_type;
	outfile<<store<<endl;

	outfile<<"#Right_window(0:magnitude,1:orientation,2:2D_map)"<<endl;
	store = right_display_type;
	outfile<<store<<endl;

	outfile<<"#Magnitude_Encoding(0:Linear,1:SplitVectors)"<<endl;
	store = magnitude_encoding;
	outfile<<store<<endl;

	outfile<<"#Projection_of_Aggregation(1:on,0:off)"<<endl;
	store = projectionaggregation;
	outfile<<store<<endl;

 	outfile<<"#Semitransparent_data_outside_ROI_Box(1:on,0:off)"<<endl;
	store = enable_roi;
	outfile<<store<<endl;

	outfile<<"#Z_layer"<<endl;
	store = zmin;
	outfile<<store<<endl;

	outfile<<"#Input_Vector"<<endl;
	outfile<<vector_x<<" "<<vector_y<<" "<<vector_z<<endl;

	outfile<<"#Enable_select_one_item(1:on,0:off)"<<endl;
	store = enable_select2d;
	outfile<<store<<endl;

	outfile<<"#Min_degree_of_brightness"<<endl;
	store=   lightness_degree;
	outfile<<store<<endl;

	outfile<<"#Ratio_for_Contours_Generation"<<endl;
	store = contour_ratio[0].size();
	outfile<<store<<" ";
	for(int i=0;i<contour_ratio[0].size();i++)
	{
		outfile<<contour_ratio[0][i]<<" ";
	}
	outfile<<endl;	
	store = contour_ratio[1].size();
	outfile<<store<<" ";
	for(int i=0;i<contour_ratio[1].size();i++)
	{
		outfile<<contour_ratio[1][i]<<" ";
	}
	outfile<<endl;	

	outfile<<"#Num_of_Magnitude_Clusters"<<endl;
	outfile<<num_magnitude_clusters[0]<<endl;
	outfile<<num_magnitude_clusters[1]<<endl;

	outfile<<"#Num_of_Orientation_Clusters"<<endl;
	outfile<<num_orientation_clusters[0]<<endl;
	outfile<<num_orientation_clusters[1]<<endl;

	outfile<<"#Num_of_Magnitude_Clusters(ROI)"<<endl;
	outfile<<num_magnitude_clusters_in[0]<<endl;
	outfile<<num_magnitude_clusters_in[1]<<endl;

	outfile<<"#Num_of_Orientation_Clusters(ROI)"<<endl;
	outfile<<num_orientation_clusters_in[0]<<endl;
	outfile<<num_orientation_clusters_in[1]<<endl;

	outfile<<"#Visible"<<endl;
	outfile<<"#Cutting_Plane(1:on,0:off)"<<endl;
	store = enable_cuttingplane;
	outfile<<store<<endl;

	outfile<<"#ROI_Box(1:on,0:off)"<<endl;
	store = enable_box;
	outfile<<store<<endl;

	outfile<<"#Z_Plane(1:on,0:off)"<<endl;
	store = enable_zplane;
	outfile<<store<<endl;

	outfile<<"#Contour_Projection(1:On,0:off)"<<endl;
	store = enable_projection;
	outfile<<store<<endl;

	outfile<<"#Contour(1:On,0:off)"<<endl;
	store = enable_contour;
	outfile<<store<<endl;

	outfile<<"#Magnitude_Legend_(original)(1:On,0:off)"<<endl;
	store = enable_maglegend1;
	outfile<<store<<endl;

	outfile<<"#Magnitude_Legend_(contour)(1:On,0:off)"<<endl;
	store = enable_maglegend2;
	outfile<<store<<endl;

	outfile<<"#Magnitude_Legend_(ROI)(1:On,0:off)"<<endl;
	store = enable_maglegend3;
	outfile<<store<<endl;

	outfile<<"#Orientation_Legend_(original)(1:On,0:off)"<<endl;
	store = enable_vellegend1;
	outfile<<store<<endl;

	outfile<<"#Orientation_Legend_(contour)(1:On,0:off)"<<endl;
	store =  enable_vellegend2;
	outfile<<store<<endl;

	outfile<<"#Orientation_Legend_(ROI)(1:On,0:off)"<<endl;
	store = enable_vellegend3;
	outfile<<store<<endl;

	outfile<<"#Sum_(of_magnitude)(1:On,0:off)"<<endl;
	store = enable_sum;
	outfile<<store<<endl;

	outfile<<"#Slider(1:On,0:off)"<<endl;
	store = enable_slider;
	outfile<<store<<endl;

	outfile<<"#Bounding_Box(1:On,0:off)"<<endl;
	store = enable_bounding;
	outfile<<store<<endl;

	outfile<<"#2D_Legend(1:On,0:off)"<<endl;
	store = enable_2dlegend;
	outfile<<store<<endl;

	outfile<<"#Visible_range_of_magnitude"<<endl;
	outfile<<range_mag[0]<<" "<<range_mag[1]<<endl;

	outfile<<"#Visible_xy_angle_of_orientation"<<endl;
	outfile<<xy_min[0]<<" "<<xy_max[0]<<endl;

	outfile<<"#Visible_z_angle_of_orientation"<<endl;
	outfile<<zangle_min<<" "<<zangle_max<<endl;

	outfile<<"#Transformation_Matrix"<<endl;
	for(int i=0;i<16;i++)
	{
		outfile<<transformation_mat[0][i]<<" ";
		if((i+1)%4==0)
			outfile<<endl;
	}
	for(int i=0;i<16;i++)
	{
		outfile<<transformation_mat[1][i]<<" ";
		if((i+1)%4==0)
			outfile<<endl;
	}
	outfile.close();

}


void SetWidget()
{
	Widget_t_mag[0] =  (log10(range_mag[0]) - log10(min_density))/mag_distance*(Widget_bound_mag[1][1]-Widget_bound_mag[0][1]);
	Widget_t_mag[1] = - (-log10(range_mag[1]) + log10(max_density))/mag_distance*(Widget_bound_mag[1][1]-Widget_bound_mag[0][1]);
	Widget_t_zangle[0] = zangle_min/180. * (Widget_bound_zangle[1][1]-Widget_bound_zangle[0][1]);
	Widget_t_zangle[1] = - (180.-zangle_max)/180. * (Widget_bound_zangle[1][1]-Widget_bound_zangle[0][1]);
	float x0 = cos(xy_max[0])*Widget_xy_radius;
	float y0 = sin(xy_max[0])*Widget_xy_radius;
	Widget_xy_max[0][0] = Widget_xy_center[0]+x0;
	Widget_xy_max[1][0] = Widget_xy_center[1]+y0;
	x0 = cos(xy_min[0])*Widget_xy_radius;
	y0 = sin(xy_min[0])*Widget_xy_radius;
	Widget_xy_min[0][0] = Widget_xy_center[0]+x0;
	Widget_xy_min[1][0] = Widget_xy_center[1]+y0;
}
