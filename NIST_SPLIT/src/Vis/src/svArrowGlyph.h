/**
 *  svLineGlyphQDOT
 *    plot line glyph on an irregular grid
 *    
 */

#ifndef __SV_ARROW_GLYPH_H
#define __SV_ARROW_GLYPH_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <vector>
#include "svGlyph.h"

using namespace std;

namespace __svl_lib {

class svArrowGlyph 
{
 public:
  // by default: generate uniform scaled line glyphs
  svArrowGlyph(svVector3 l, svVector3 r,int dataSize,
          svVector3 *posIn, svVector3 *velIn,
		  svScalar *denIn,
			svScalar *exp, svScalar *coe,
		  svVector4 *color4In1, svVector4 *color4In2,
		  int num, float R);
  
  ~svArrowGlyph(){cleanup();}

//http://math.hws.edu/eck/cs424/notes2013/opengl-c/color-cube-of-spheres.c
  void createSphereArraysAndVBOs(bool enablesample);
  void SetData(svVector3 l, svVector3 r,int dataSize,
		     svVector3 *posIn, svVector3 *velIn,  svScalar *denIn,
			svScalar *exp, svScalar *coe,
			 svVector4 *color4In1,svVector4 *color4In2,
			 int num,float R);
  void SetColors(svVector4 *color4In1,svVector4 *color4In2);

  
  void SetArrows(bool enablesample, bool enablelines, svScalar *size);
  void SetSamples(int *sample, int num);
  
  void SetEncodeMode(int m){encode_mode=m;}
  void SetArrowBase(float r){ARROW_BASE = r;}

  void Render(bool *inbox);//line and dots
  void drawSphereDirectWithDataFromArrays(bool enablesample) ;
  void RenderArrow(bool *inbox, int *sc, int num, bool enablesample, bool enablelines, int *index);//arrows with/without lines

  svInt glyphFrequency;  // glyph density
	int NUM_SEG;
	float ARROW_BASE;

 private:
  svVector3Array  *glyphPos;   
  svVector3Array  *glyphDir;   // direction
  svScalar   *glyphMag;   // magnitute
  svScalar *glyphExp;
  svScalar *glyphCoe;
  svVector3Array *glyphLegend;

	svScalar *glyphradius;

  svVector4Array *glyphColors1; // Colors of lines
  svVector4Array *glyphColors2; //Colors of arrows

  svVector3Array  *glyphArrowPos; //position of arrow heads
  svVector3Array  *glyphArrowNorm; //normalized vectors of arrows


  vector<int> sample_index;

  svVector3 lb;	
  svVector3 rb;

  float *sphereVertexArray; // Pointer to the vertex coords, for use with glDrawArrays
  float *sphereNormalArray; // Pointer to the normal vectors, for use with glDrawArrays



  int encode_mode;//1 is direct mapping; 2 is splitvector; 3 is logarithm

 protected:
	void cleanup();
};
}
#endif // __SV_ARROW_GLYPH_H
