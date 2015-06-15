#ifndef __SV_SLICE_H
#define __SV_SLICE_H

#include <vector>

#include "svType.h"
#include "svVectorMatrix.h"
#include "svArray.h"

#include "./jcUtil/MGL.h"
#include "./jcUtil/MArray.h"
#include "./jcUtil/MVectorMatrix.h"

using namespace std;

namespace __svl_lib {

class svSlice 
{
 public:
  svSlice(svVector3 l, svVector3 r,int dataSize,
          svVector3 *posIn);  
  ~svSlice(){cleanup();}

  void SetData(svVector3 l, svVector3 r,int dataSize,
          svVector3 *posIn);  
  
  //void SetMinDistance(svVector3 d){mindistance = d;}

  void RenderSlice(bool stop1, bool stop2);

  svIntArray * GetSelectedIndex();
  float GetSliceXY(float *x, float *y, int &num);
  float GetSampleSlice();
  
  float GetSliceXY(svVector3 *xy,svVector3 *xz,svVector3 *yz,
		int xyn, int xzn, int yzn);
  float GetSampleSlice(svVector3 *xy,svVector3 *xz,svVector3 *yz,
		int xyn, int xzn, int yzn);

  int GetSelectedNum(){return selectedNum;}
  int GetSampleNum(){return sampleNum;}
  int *GetIndex(){int *in = new int[selectedNum];for(int i=0;i<selectedNum;i++)in[i]=glyphIndex[0][i];return in;}
  void GetCutPos(svVector3 *pos){for(int i=0;i<selectedNum;i++)pos[i]=cutPos[i];}
  void GetCutSamplePos(svVector3 *pos){for(int i=0;i<sampleNum;i++)pos[i]=cutSamplePos[i];}

  float PointtoLine(svVector3 p1, svVector3 p2, svVector3 p);
  float PointtoLine2(svVector3 a, svVector3 n, svVector3 p);
  float GetDot(svVector3 a, svVector3 b);
  svVector3 CrossProduct(svVector3 u, svVector3 v);
  bool pointinpolygon(float x,float y);

  void Intersection();
  void SetPlaneXYZ();
  void InitChooseBox(float w, float h);
  void SetChooseBox(float scale);
  void moveChooseBox(float wld_x,float wld_y,float wld_z);
  void SetBoxCenter(float x, float y);
  void SetBoxShape(float ratio);
  void SetBoxSide(float w, float h){initialside[0]=w;initialside[1]=h;initboxcenter=planecenter;}
  svVector3 GetBoxCenter();
  svVector3 *GetBoxPoints();
  svVector3 GetMaxPlane(){return rb_plane;}
  svVector3 GetMinPlane(){return lb_plane;}

  float GetScale(int w, int h);

  float *GetPlanex(){return planex;}
  float *GetPlaney(){return planey;}
 
  svVector3 GetDir(){return dir;}
  svVector3 GetDir2(){return dir2;}
 
  float GetAngle1();
  float GetAngle2();

  void XY_vertical();
  void XZ_vertical();
  void YZ_vertical();
  void Reset();


  void SetPoints(svVector3 *w, svVector3 d1, svVector3 d2);
  void move_ctrlpoint(int x, int y);
  void Find_Nearest(int x, int y);
  GLboolean invert_matrix(const GLfloat *m, GLfloat *out);

  svVector3 * GetWidget(){return widgetp;}

  svInt glyphFrequency;  
  int width;
  bool SELECTED;
  bool SELECTED_WHITE;
  bool SELECTED_BLUE;
  GLfloat tb_transform[4][4];
    GLint viewport[4];
    GLdouble mvmatrix[16], projmatrix[16];

 private:
  svVector3Array  *glyphPos;    

  svIntArray *glyphIndex; 
  int selectedNum;
  int sampleNum;
  float mindistance;

  svVector3 widgetp[5];
  float widgetl;
  svVector3 dir;
  svVector3 dir2;

  GLdouble wx[5];
  GLdouble wy[5];
  GLdouble wz[5];
  int ni;

  svVector3 * cutPos;
  svVector3 *cutSamplePos;
  float *planex;
  float *planey;

  float side;

  svVector3 lb;	
  svVector3 rb;
  GLuint theList;

  svVector3 intersectionp[12];
  svVector3 intersectionplanep[6];
  svVector3 lb_plane;
  svVector3 rb_plane;
  int intersectionnum;
  int intersectionnum2;


  svVector3 choosebox[4];
  float chooseboxside[2];
  float initialside[2];
  float increasesize;
  svVector3 chooseboxcenter;
  svVector3 initboxcenter;
  svVector3 initchoosebox[4];

  svVector3 planexyz[3];
  svVector3 planecenter;
  
 // svVector3 mindistance;
  
 protected:
  void cleanup();
};

}
#endif // __SV_SLICE_H
