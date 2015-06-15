/**
 *  svLineGlyphQDOT
 *    plot line glyph on an irregular grid
 *    
 */

#ifndef __SV_ICON_GLYPH_H
#define __SV_ICON_GLYPH_H

#include <vector>
#include "svGlyph.h"

using namespace std;

namespace __svl_lib {

class svIconGlyph 
{
 public:
  // by default: generate uniform scaled line glyphs
  svIconGlyph(svVector3 l, svVector3 r,int dataSize,
          svVector3 *posIn, svVector3 *velIn,
		  svScalar *denIn,
		  svVector4 *color4In1, svVector4 *color4In2);

  // read line glyph field saved in a file 
  // will set: 
  //     glyphPos  (read from infname)
  //     glyphColors  (read from infname)
  //     glyphFrequency = 1
  //     glyphScale = 1

  void SetData(int dataSize,
     svVector3 *posIn, svVector3 *velIn,  svScalar *denIn,
	 svVector4 *color4In1,svVector4 *color4In2);
  void SetWindow(svVector3 *p)
	{for(int i=0;i<4;i++)choosewindow[i]=p[i];}

  void SetMaxExp(float e){maxexp = e;}

  ~svIconGlyph(){cleanup();}

  void GenerateDisplay(svVector3 l,svVector3 r);
  void GenerateDisplay(float *x, float *y, int *index, int num);  
  void GenerateDisplay(float *x, float *y, int num, float R);

  void RenderIcon();
  void RenderIcon(float *x, float *y, int num, float R);
  void RenderOrientation(float *x, float *y, int num,float R);
  void RenderWindow();

  void RenderScale(int w, int h);

  svInt          glyphFrequency;  // glyph density
  int windowswidth;
  int windowsheight;
 private:
	float *thetax;
	float *thetay;
	float *thetaz;
	float *exp;
	float *coe;
	float maxexp;
  // data attributes (set when the dataset is in except glyphPos[1])
  svVector3Array  *glyphPos;   // positions (0: is the original location: 
                          //            1: changes with the drawing
						  //               attributes)
  svVector3Array  *glyphDir;   // direction
  svScalar   *glyphMag;   // magnitute
  svScalar   *glyphScale; // glyph drawing size

  svVector4Array *glyphColors1; // glyph colors
  svVector4Array *glyphColors2;

  svVector3 lb;	
  svVector3 rb;
  GLuint theList;

  svVector3 choosewindow[4];
  svVector3 widget;


 protected:
	void cleanup();
};
}
#endif // __SV_ICON_GLYPH_H
