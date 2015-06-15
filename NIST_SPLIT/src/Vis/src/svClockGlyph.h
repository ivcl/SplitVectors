
#ifndef __SV_CLOCK_GLYPH_H
#define __SV_CLOCK_GLYPH_H

#include "svGlyph.h"
#include <vector>

using namespace std;

namespace __svl_lib {

class svClockGlyph 
{
 public:
  // by default: generate uniform scaled line glyphs
  svClockGlyph(svVector3 l, svVector3 r,int dataSize,
          svVector3 *posIn, svVector3 *velIn,
		  svScalar *denIn,
		  svVector4 *color4In);

  ~svClockGlyph(){cleanup();}

  void SetData(int dataSize,
          svVector3 *posIn, svVector3 *velIn,
		  svScalar *denIn,
		  svVector4 *color4In);
  void SetMaxExp(float e){maxexp = e;}
  void GenerateDisplay(svVector3 l,svVector3 r);
  void GenerateDisplay(float *x, float *y, int *index, int num, float R);
  void GenerateDisplay(float *x, float *y, int num, float R);

  void RenderClock();
  void RenderClock(float *x, float *y, int num,float R);

  svInt          glyphFrequency;  // glyph density
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

  svVector4Array *glyphColors; // glyph colors

  svScalar maxMag;
  svVector3 lb;	
  svVector3 rb;
 GLuint theList;
 protected:
	void cleanup();
};
}
#endif // __SV_CLOCK_GLYPH_H
