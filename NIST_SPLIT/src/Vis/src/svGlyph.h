
/*
 * svGlyph
 */

#ifndef __SV_GLYPH_H
#define __SV_GLYPH_H

#include "svArray.h"
#include "svVectorField.h"
#include "svParticle.h"
#include "svPrimitive.h"
#include "svLut.h"

#define DEFAULT_GLYPH_RADIUS 0.02
#define DEFAULT_GLYPH_HEIGHT 0.08
#define DEFAULT_GLYPH_FREQUENCY 1
#define DEFAULT_GLYPH_SECTION_NUM 5

namespace __svl_lib {

class svGlyph : public svPrimitive
{
 public:
  svGlyph();
  svGlyph(svScalarArray ypos, svScalarArray zpos, svVector3Array dir);
  svGlyph(svVector3Array *vec3in, 
          svVector4Array *color4in, 
		  svInt _seed_num){};
  svGlyph(svVector3Array *vec3in, svInt _seed_num){};
  svGlyph(svChar *dir, svChar *inf);
  svGlyph(svVector3 _lbbox, svVector3 _rbbox);
  virtual ~svGlyph();

  void   clean(); // clean memory

// Generate
  virtual void Generate(); 
  virtual void Generate(svVector3Array *vec3in);
  virtual void Generate(svVector3Array *vec3in, svVector4Array *color4in);

  virtual void Render();
  virtual void RenderStore()=0;     // using vertex array
  virtual void RenderStoreDirect(svBool)=0;
  virtual void RenderStoreDirect(svFloat, svFloat, svBool)=0;

  virtual void RemovePoint(svInt lineIndex);
  // Not implemented: remove a glyph

  virtual void SaveToFile(char *fname); // save for later rendering

  // Enables
  virtual void EnableColor();
  virtual void EnableColor(svInt colorModel, svFloat minValue, svFloat maxValue);
  // colorModel: SV_GRAY, SV_LOCS
  
  virtual void EnableLineWidth(svFloat minlw, svFloat maxlw);

  virtual void SetLineColorFromVec3(svVector3Array *vec3in);
  virtual void SetLut(const svLut & newlut)
  {}; // Not implemented
  virtual void SetGlyphFrequency(int fr) {glyphFrequency = fr;}
  virtual void SetGlyphScale(svFloat sc) {}; // Not implemented

  virtual void GetColor(svVector4Array *colorArray) const 
  {}; // Not implemented
  virtual void GetLineWidth(svScalarArray *lineWidth) const 
  {}; // Not implemented
  virtual void GetPosition(svVector3Array *glyphpos) const 
  {};// Not implemented

  virtual void Replace(svUint glyph_num, 
                svScalar x, svScalar y, svScalar z){}; 
  // Not imeplemented
  svVector3Array *GetGlyphs() const {return glyphPos;}

  virtual void SetEnableGlyphScale(svBool newState) 
  {ENABLE_GLYPH_SCALE=newState;}
  virtual svBool GetGlyphScale() const {return ENABLE_GLYPH_SCALE;}

 private:
 protected:
  //Dec 2103: 
  //   make it pure virtual so a rendering method
  //   can have its own way to build the display list
  virtual void  BuildDisplayListFromStore()=0;
  virtual void  RebuildDisplayList()=0;

  // data attributes (set when the dataset is in except glyphPos[1])
  svVector3Array  *glyphPos;   // positions (0: is the original location: 
                          //            1: changes with the drawing
						  //               attributes)
  svVector3Array  *glyphDir;   // direction
  svScalar   *glyphMag;   // magnitute
  svScalar   *glyphScale; // glyph drawing size

  svScalar   glyphMagRange[2]; // min: glyphMagRange(0) 
                                   // max: glyphMagRange(1)
								   // min and max in each layer

  // drawing attributes (can be modified later based on drawing flags)
  svInt          glyphFrequency;  // glyph density
  svVector4Array *glyphColors; // glyph colors
  svScalarArray  *glyphWidth;  // glyph size

  svChar *infile;  // pre-stored glyph data; read in and render directly

  svBool  ENABLE_GLYPH_SCALE;  // if true, will scale the 
                               // length of the glyphs;
							   // default: false
};

}

#endif // __SV_GLYPH_H
