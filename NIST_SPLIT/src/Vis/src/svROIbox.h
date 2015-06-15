#ifndef __SV_ROIBOX_H
#define __SV_ROIBOX_H

#include <vector>

#include "svType.h"
#include "svVectorMatrix.h"
#include "svArray.h"

#include "./jcUtil/MGL.h"
#include "./jcUtil/MArray.h"
#include "./jcUtil/MVectorMatrix.h"

using namespace std;

namespace __svl_lib {

class svROIbox
{
 public:
  svROIbox(svVector3 l, svVector3 r,int dataSize,
          svVector3 *posIn, svScalar *denIn);  
  ~svROIbox(){cleanup();}

  void SetData(svVector3 l, svVector3 r,int dataSize,
          svVector3 *posIn, svScalar *denIn);  

  void RenderLines(int p0, int p1, int l, svVector3 c);
  void RenderROIbox();
  void Reset();
  void WholeField();

  svVector3 CrossProduct(svVector3 u, svVector3 v);
  float DotProduct(svVector3 u, svVector3 v);
  bool InBox(svVector3 p1, svVector3 p2, svVector3 p, svVector3 point);
  bool InBox(svVector3 p, svVector3 point);
  bool *InBox(svVector3 *pos, int datasize);
  vector<bool> InBox2(svVector3 *pos, int datasize);
  //vector<vector<bool> > InBox(vector<vector<double> > pos[3], int zlayer);
  //http://math.stackexchange.com/questions/406864/intersection-of-two-lines-in-vector-form
  svVector3 Intersection2Lines(svVector3 d1,svVector3 p1,svVector3 d2,svVector3 p2);
  svVector3 Plane2Plane(svVector3 d1,svVector3 p1,svVector3 d2,svVector3 p2,svVector3 ld,svVector3 lp);
  svVector3 Plane2Plane(svVector3 d1,svVector3 p1,svVector3 d2,svVector3 p2);
  bool Line2Plane(svVector3 p0, svVector3 l0, svVector3 l1, svVector3 l, svVector3 n, svVector3 &r);

  void SetOutBoundary();
  bool ifreverse();//for order of intersection points

  svIntArray * GetSelectedIndex();
  int GetSelectedNum(){return selectedNum;}

  void move_ctrlpoint(int x, int y);
  void SetLocation(GLdouble wld_x, GLdouble wld_y,GLdouble wld_z);
  void SetVerticalDir(svVector3 v);
  void SetDir(svVector3 v);
  void SetDir2(svVector3 v);
  void SetPoints(svVector3 *ip, svVector3 *w);

  float GetROIden(float scalar);
  svVector3 *GetIntersectionp(){return intersectionp;};
  svVector3 *GetWidget(){return widgetp;}

  void testbounding();

  void Find_Nearest(int x, int y);
  bool Find_Near(int x, int y);
  GLboolean invert_matrix(const GLfloat *m, GLfloat *out);

  svInt glyphFrequency;  
  int width;
  bool SELECTED;
  bool NEAR;
  GLfloat tb_transform[4][4];

  GLint viewport[4];
  GLdouble mvmatrix[16];
  GLdouble projmatrix[16];
  bool stop1;
  bool stop2;

	void SetZlayer(int z){zlayer = z;}

 private:
  svVector3Array  *glyphPos;  
  svScalar *glyphMag;  

  svIntArray *glyphIndex; 
  int selectedNum;

  svVector3 widgetp[11];
  //0,4,5,6,7,8,9,10 the 8 intersetion points
  //1 the middle widget on the bottom
  //2,3 z values

  svVector3 intersectionp[8];
  svVector3 upx[2];
  svVector3 upy[2];

  float widgetxl;
  float widgetyl;

  svVector3 dirx;
  svVector3 diry;

  GLdouble wx[11];
  GLdouble wy[11];
  GLdouble wz[11];
  int ni;

  svVector3 lb;	
  svVector3 rb;

  svVector3 intersection_out_boundary[12][2];
  int intersection_out_num[12];
  bool intersectionp_out[8];

	int zlayer;

 protected:
  void cleanup();
};

}
#endif // __SV_ROIBOX_H
