/**
 *  svLineGlyphQDOT
 *    plot line glyph on an irregular grid
 *    
 */

#ifndef __SV_LINE_GLYPH_QDOT_H
#define __SV_LINE_GLYPH_QDOT_H

#include "svGlyph.h"

namespace __svl_lib {

class svLineGlyphQDOT : public svGlyph
{
 public:
  // by default: generate uniform scaled line glyphs
  svLineGlyphQDOT(svVector3 _lbbox, svVector3 _rbbox, int dataSize,
          svVector3 *posIn, svVector3 *velIn,
		  svScalar *denIn,
		  svVector4 *color4In);

  // read line glyph field saved in a file 
  // will set: 
  //     glyphPos  (read from infname)
  //     glyphColors  (read from infname)
  //     glyphFrequency = 1
  //     glyphScale = 1
  svLineGlyphQDOT(svChar *dir, svChar *infname);
  
  virtual ~svLineGlyphQDOT();

  virtual void BuildDisplayListFromStore();
  virtual void RebuildDisplayList();
  //virtual void Generate();

  virtual void Print();
  virtual void init();
  //virtual void Render();
  virtual void RenderStore();
  virtual void RenderStoreDirect(svBool enableDot);
  virtual void RenderStoreDirect(svFloat zmin, svFloat zmax, svBool enableDot);
  virtual void RenderStoreDirect(svFloat zmin, svFloat zmax);

  virtual void Draw3DAxis();
  virtual void DrawAxisWidgets(Matrix4f m,GLenum mode);
  virtual void DrawContourWidgets(Matrix4f m,GLenum mode);

  void ProcessHit(GLint hits, GLuint buffer[]);

  void SetAxisWidgetPos(svVector3 l, svVector3 r);
  void SetContourWidgetPos(svVector3 l);
  void SetAxisWidgetCol();
  void SetContourDen(svScalar num){ContourPointDen = num;}
  int PickWidgets(Matrix4f m, svVector3 l, svVector3 r, int x, int y, int w, int h);
  void MoveWidgets(Matrix4f m, int x, int y, svVector3 &l, svVector3 &r);
  svVector3 GetContourPos(){svVector3 tmp; tmp[0]=ContourWidgetPos[0][0];tmp[1]=ContourWidgetPos[1][1];tmp[2]=ContourWidgetPos[2][2];return tmp;}

  virtual void SetGlyphScale(svFloat sc);
  // set glyphScale to a global scale

  virtual void SetGlyphScale(svFloat sc, svFloat zmin, svFloat zmax);
  // set glyphScale to a local scale for data in (zmin, zmax)

  // save for later rendering
  virtual void SaveToFile(char *dir, char *fname, int freq); 

  int green;
  int hitflag;
  int hits;
  float wwz2;
  float ix,iy,iz;
  float offx;
  int transflag;

  int AxisWidget;

  bool contour;


 private:
  svVector3 AxisWidgetPos[6];
  svVector3 AxisWidgetCol[6];
  svVector3 ContourWidgetPos[3];
  svVector3 ContourWidgetCol[3];
  svVector3 ContourPointPos;
  svVector3 ContourPointCol;
  svScalar ContourPointDen;
 protected:
};
}
#endif // __SV_LINE_GLYPH_H
