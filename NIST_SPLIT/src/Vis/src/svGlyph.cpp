/*  svGlyph
 */

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <fstream>
#include <string.h>
#include "svGlyph.h"
#include "svException.h"
#include "svUtil.h"

// TODO: add renderproperty

using namespace std;

namespace __svl_lib {

svGlyph::svGlyph()
  :svPrimitive(1)
{
  ENABLE_GLYPH_SCALE = false;
}

svGlyph::svGlyph(svScalarArray ypos, svScalarArray zpos, svVector3Array d)
    :svPrimitive(1)
{
  int ysize=ypos.size(), zsize=zpos.size();

  glyphPos = new svVector3Array[1];
  glyphDir = new svVector3Array[1];

  for(int zi=0; zi<zsize; zi++) {
    for(int yi=0; yi<ysize; yi++) {
      glyphPos[0].add(svVector3(0,ypos[yi],zpos[zi]));
      glyphDir[0].add(d[zi*ysize+yi].getValue());
    };// end for(yi)
  }; // end for(zi)

  glyphFrequency= DEFAULT_GLYPH_FREQUENCY;
  glyphColors = NULL;
  glyphWidth = NULL;

  ENABLE_GLYPH_SCALE=false;
}

svGlyph::svGlyph(svChar *dir, svChar *infname): svPrimitive(1)
{
  infile = new char[2056];
  sprintf(infile, "%s/%s", dir, infname);
  glyphPos = NULL;
  glyphDir=NULL;
  glyphFrequency= DEFAULT_GLYPH_FREQUENCY;
  glyphColors = NULL;
  glyphWidth = NULL;
  ENABLE_GLYPH_SCALE=false;
}

svGlyph::svGlyph(svVector3 _lbbox, svVector3 _rbbox) 
            : svPrimitive(1)
{
  SetBBox(_lbbox, _rbbox);

  glyphPos = NULL;
  glyphColors = NULL;
  glyphWidth = NULL;
  glyphDir = NULL;
  glyphFrequency= DEFAULT_GLYPH_FREQUENCY;
/*
  infile = strdup(inf);

  glyph_radius = DEFAULT_GLYPH_RADIUS;
  glyph_height = DEFAULT_GLYPH_HEIGHT;
  glyph_section_num = DEFAULT_GLYPH_SECTION_NUM;
  tube_radius = glyph_radius * 0.5;
*/
  ENABLE_GLYPH_SCALE=false;
}

void svGlyph::SetLineColorFromVec3(svVector3Array *vec3in)
{
 for(int i=0; i<seed_num; i++)
 {
     int size = vec3in[i].size();
     for(int j=0; j<size; j++) {
       svVector4 col = CalcLineColor(vec3in[i][j].getValue());
       glyphColors[i].add(col);
     }; // loop of size
 }; // loop of seed_num
}

void svGlyph::Generate(svVector3Array *vec3in)
{
  cerr << "calling svGlyph::Generate()" << endl;
  cerr << "seed_num = " << seed_num << endl;

  glyphPos = new svVector3Array[seed_num];
  glyphColors = new svVector4Array[seed_num];
  for(int i=0; i<seed_num; i++) // copy vec3in
  {
    int ss= vec3in[i].size();
    for(int j=0; j<ss; j++) {
      glyphPos[i].add(vec3in[i][j]);
    };
  }; // loop of seed_num
  SetLineColorFromVec3(vec3in);
}

void svGlyph::Generate(svVector3Array *vec3in, svVector4Array *color4in)
{
}

svGlyph::~svGlyph()
{
  clean();
  cleanDisplayList(SV_IMMEDIATE);
}

void svGlyph::clean()
{
  if (glyphPos != NULL) {
    for (int i=0; i<seed_num; i++)  
	glyphPos[i].free();
    delete [] glyphPos;
    glyphPos=NULL;
  };

  if (glyphDir!=NULL) {
    for(int i=0; i<seed_num; i++)
      glyphDir[i].free();
    delete [] glyphDir;
    glyphDir=NULL;
  };

  if (glyphMag!=NULL) delete [] glyphMag;
  if (glyphScale!=NULL) delete [] glyphScale;

  if (glyphWidth != NULL) {
    for (int i=0; i<seed_num; i++)  
	glyphWidth [i].free();
    delete [] glyphWidth;
    glyphWidth=NULL;
  }
  if (glyphColors != NULL) {
    for (int i=0; i<seed_num; i++)  
	glyphColors[i].free();
    delete [] glyphColors;
    glyphColors=NULL;
  };
}


//TODO
void svGlyph::RemovePoint(svInt seedIndex)
{
}


void svGlyph::EnableLineWidth(svFloat minlw, svFloat maxlw)
{
   // clean up
   if(glyphWidth!=NULL) 
   {
     for(int i=0; i<seed_num; i++)
       glyphWidth[i].free();
     delete [] glyphWidth;
     glyphWidth = NULL;
   };
  
  lut.SetLineWidthRange(minlw,maxlw);
  lut.SetScalarRange(0.0007, 1.5);

  clean();
  cleanDisplayList(SV_IMMEDIATE);

  // regenerate glyphs
  ENABLE_LINEWIDTH = true;
}


void svGlyph::EnableColor()
{
  EnableColor(SV_LOCS,0.007,0.15);
}

void svGlyph::EnableColor(svInt colorModel, svFloat minValue, svFloat maxValue)
{
  // set glyph color
  if(glyphColors!=NULL) 
  {
     for(int i=0; i<seed_num; i++)
       glyphColors[i].free();
     delete [] glyphColors;
     glyphColors= NULL;
  };

  //lut.SetColorModel(colorModel, 0.0007, 1.5);
  lut.SetColorModel(colorModel, minValue, maxValue);

  clean();
  cleanDisplayList(SV_IMMEDIATE);

  ENABLE_COLOR = true;
}

void svGlyph::Generate()
{
  BuildDisplayListFromStore();
  display_mode = SV_DISPLAYLIST;
  STILL_UPDATE = false;
}

/*
void svGlyph::RenderQDOT()
{
    glCallList(display_list);
}
*/

void svGlyph::Render()
{
  //if(STILL_UPDATE) return;

  //glColor3fv(render_property.color.getValue());
  if(display_mode == SV_IMMEDIATE) {
    //cerr << "calling svGlyph::Render: Generate() " << endl;
    Generate();
  }
  else if(display_mode == SV_DISPLAYLIST && glIsList(display_list)) {
    //cerr << "call svGlyph::Render: displaylist... " << endl;
    glCallList(display_list);
  }
  else if (display_mode == SV_STORE) 
  {
    //cerr << "call svGlyph::Render(): mode is SV_STORE" << endl;
    //BuildDisplayListFromStore();
    /*
    if(ENABLE_COLOR==true || ENABLE_LINEWIDTH==true)
      RenderStoreDirect();
    else
      RenderStoreDirect();
      */
      //RenderStore();
  }
}

/*
void svGlyph::RenderStoreDirect() const
{
}
void svGlyph::RenderStore() const
{
}
*/

void svGlyph::SaveToFile(char *fname)
{
  ofstream outf(fname);
  outf << seed_num << endl;
  for(int i=0; i<seed_num; i++)
  {
    int s = glyphPos[i].size();
    outf << s << endl;
    for(int j=0; j<s; j++)
    {
      outf << glyphPos[i][j][0] << "  "
           << glyphPos[i][j][1] << "  "
	       << glyphPos[i][j][2] << endl;
    };
  }
  outf.close();
}

}
