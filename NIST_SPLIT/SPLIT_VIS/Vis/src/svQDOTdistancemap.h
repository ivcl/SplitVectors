/**
  by Henan Zhao
  10/26/2014
  Elements
 */

#ifndef __SV_QDOT_distancemap_H
#define __SV_QDOT_distancemap_H

#include "svType.h"
#include "svVectorMatrix.h"
#include "svArray.h"
#include "svUtil.h"
#include "./EMclustering/CIEColor2.h"

#include "./jcUtil/MGL.h"
#include "./jcUtil/MArray.h"
#include "./jcUtil/MVectorMatrix.h"

#ifndef M_PI
#define M_PI 3.1415926
#endif
using namespace   __em_lib;
namespace __svl_lib {

class svQDOTdistancemap
{
  public:
    svQDOTdistancemap();
    ~svQDOTdistancemap(){delete [] SpiralColor;	if(dataColor!=NULL)
	{
			dataColor[0].free();
		delete [] dataColor;
		dataColor = NULL;
	}
	}

	void SetColors(int n);
	void SetColors(int n, int ln, vector<double> hue, int *idx, svVector3Array *pos, svScalarArray *vel, int zlayer, float z, double *vr, double *vg, double *vb);
	void SetColors(svVector3Array *pos,double * hue,svScalarArray *vel2, float z, char *file, int zlayer, int n);

    //mag: [0, 1]
	
	void Render2dLegend();

	void RenderPlane(int xnum,int ynum,int zlayer,float xl,float yl,
		int mouse_x, int mouse_y, float scale,float zoom, float xpan, float ypan,
		float r, int w, int h);
	void RenderOnePlane(svVector3Array pos, int w, int h,float xl, float yl,
		int mouse_x, int mouse_y, bool global);		
	void RenderText(int n);

	void Render(svVector3Array *pos,svScalarArray *vel2, bool *in,int zlayer, float xd, float yd, float z); 
    void Render(svVector3Array *pos,svScalarArray *vel,bool *in,int zlayer, float xd, float yd); 
    void RenderPlaneComparision(svVector3Array *pos,svScalarArray *vel,bool *in,int zlayer, float xd, float yd); 

    void RenderLegend();
	void RenderColorLegend();

	void SelectPoint(float x, float y,svVector3Array *pos,svScalarArray *vel2,int zlayer, float xd, float yd, float z);

	void SelectBox(int x, int y);
	void MoveBox_bymouse(int x, int y);		
	void MoveBox_byslider();

	int SetSelectedLayer(int n){selected_layer = n;}

	int GetSelectedLayer(){return selected_layer;}
	int GetSelectedPoint(){return selected_p;}
	svVector3 GetSelectedPos(){return selected_point;}

	void GetSelectXY(int &x, int &y){x = select_x;y = select_y;}
	int GetSelectIndex(){return select_index;}

 // protected:
   //void cleanup();

private:
	svVector3Array *dataColor;
	svVector3 *SpiralColor;
	svVector3 *SpiralColor_order;
	int Color_Num;
	int selected_layer;
	int selected_p;
	svVector3 selected_point;

	int num_l;
	int num_h;
	bool Selected;
	svVector3 lb;
	svVector3 rb;
	int mouse_x;
	int mouse_y;

	int select_x;
	int select_y;
	int select_index;
	svVector3 select_col;
    //GLint list;

};

}

#endif // __SV_QDOT_distancemap_H
