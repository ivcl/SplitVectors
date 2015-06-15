#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#ifdef __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif

#include "svType.h"
#include "svVectorMatrix.h"
#include "svArray.h"

#include "./jcUtil/MGL.h"
#include "./jcUtil/MArray.h"
#include "./jcUtil/MVectorMatrix.h"

#include <string.h>

using namespace __svl_lib;
using namespace std;

#define checkImageWidth 512
#define checkImageHeight 512
#include <stdlib.h>
#include <stdio.h>

namespace __svl_lib {

class svQDOT2dmapping{
	
public:
    svQDOT2dmapping(){dataSize = 0;selected_index1=-1;selected_index2=-1;}
	~svQDOT2dmapping(){cleanup();};

	void initcolors();

    void SetScale(int w, int h){image_width=(float)w;image_height=(float)h;}
    void SetTransformation(GLfloat *m, int n);

	//Before inputting parameters, please make sure all parameters are normalized

	void SetColor(svVector4Array *col, svVector4Array *col1, svVector4Array *col2,
			int zlayer);
	void SetColor();

    void RenderBars(svVector3Array *pos,svScalarArray *vel,
		svScalarArray *exp, svScalarArray *coe,
		int zlayer,
		float bw, float bh);
    void RenderSizeColor(svVector3Array *pos,svScalarArray *vel,
		svScalarArray *exp, svScalarArray *coe,
		int zlayer,
		float bw, float bh);
    void RenderColorColor(svVector3Array *pos,svScalarArray *vel,
		svScalarArray *exp, svScalarArray *coe,
		int zlayer,
		float bw, float bh);
    void RenderSizeOrientation(svVector3Array *pos,svScalarArray *vel,
		svScalarArray *exp, svScalarArray *coe,
		int zlayer,
		float bw, float bh);
    void RenderSizeLightness(svVector3Array *pos,svScalarArray *vel,
		svScalarArray *exp, svScalarArray *coe,
		int zlayer,
		float bw, float bh);
    void RenderLengthColor(svVector3Array *pos,svScalarArray *vel,
		svScalarArray *exp, svScalarArray *coe,
		int zlayer,
		float bw, float bh);
    void RenderLengthLightness(svVector3Array *pos,svScalarArray *vel,
		svScalarArray *exp, svScalarArray *coe,
		int zlayer,
		float bw, float bh);
    void RenderLengthOrientation(svVector3Array *pos,svScalarArray *vel,
		svScalarArray *exp, svScalarArray *coe,
		int zlayer,
		float bw, float bh);

    void RenderColorLength(svVector3Array *pos,svScalarArray *vel,
		svScalarArray *exp, svScalarArray *coe,
		int zlayer,
		float bw, float bh);
    void RenderLightnessLength(svVector3Array *pos,svScalarArray *vel,
		svScalarArray *exp, svScalarArray *coe,
		int zlayer,
		float bw, float bh);
    void RenderTextureLength(svVector3Array *pos,svScalarArray *vel,
		svScalarArray *exp, svScalarArray *coe,
		int zlayer,
		float bw, float bh);
    void RenderAngleLength(svVector3Array *pos,svScalarArray *vel,
		svScalarArray *exp, svScalarArray *coe,
		int zlayer,
		float bw, float bh);

    void RenderColorArea(svVector3Array *pos,svScalarArray *vel,
		svScalarArray *exp, svScalarArray *coe,
		int zlayer,
		float bw, float bh);
    void RenderLightnessArea(svVector3Array *pos,svScalarArray *vel,
		svScalarArray *exp, svScalarArray *coe,
		int zlayer,
		float bw, float bh);
    void RenderTextureArea(svVector3Array *pos,svScalarArray *vel,
		svScalarArray *exp, svScalarArray *coe,
		int zlayer,
		float bw, float bh);
    void RenderAngleArea(svVector3Array *pos,svScalarArray *vel,
		svScalarArray *exp, svScalarArray *coe,
		int zlayer,
		float bw, float bh);

    void RenderLightnessColor(svVector3Array *pos,svScalarArray *vel,
		svScalarArray *exp, svScalarArray *coe,
		int zlayer,
		float bw, float bh);
    void RenderTextureColor(svVector3Array *pos,svScalarArray *vel,
		svScalarArray *exp, svScalarArray *coe,
		int zlayer,
		float bw, float bh);
    void RenderAngleColor(svVector3Array *pos,svScalarArray *vel,
		svScalarArray *exp, svScalarArray *coe,
		int zlayer,
		float bw, float bh);

    void RenderLightnessLightness(svVector3Array *pos,svScalarArray *vel,
		svScalarArray *exp, svScalarArray *coe,
		int zlayer,
		float bw, float bh);
    void RenderTextureColorLightness(svVector3Array *pos,svScalarArray *vel,
		svScalarArray *exp, svScalarArray *coe,
		int zlayer,
		float bw, float bh);
    void RenderAngleColorLightness(svVector3Array *pos,svScalarArray *vel,
		svScalarArray *exp, svScalarArray *coe,
		int zlayer,
		float bw, float bh);

    void RenderLengthTexture(svVector3Array *pos,svScalarArray *vel,
		svScalarArray *exp, svScalarArray *coe,
		int zlayer,
		float bw, float bh);
    void RenderAreaTexture(svVector3Array *pos,svScalarArray *vel,
		svScalarArray *exp, svScalarArray *coe,
		int zlayer,
		float bw, float bh);
    void RenderColorTexture(svVector3Array *pos,svScalarArray *vel,
		svScalarArray *exp, svScalarArray *coe,
		int zlayer,
		float bw, float bh);
    void RenderLightnessTexture(svVector3Array *pos,svScalarArray *vel,
		svScalarArray *exp, svScalarArray *coe,
		int zlayer,
		float bw, float bh);
    void RenderAngleTexture(svVector3Array *pos,svScalarArray *vel,
		svScalarArray *exp, svScalarArray *coe,
		int zlayer,
		float bw, float bh);


    void RenderMarker();
    void RenderBarsLegend(float scale);

	void RenderOrientationLegend();
	void RenderSizeLegend();
	void RenderSizeLegend2();
	void RenderColorLegend();
	void RenderBrightLegend();

    void SelectPoint(int x, int y);

    void ResetMarker(){selected_index1=-1;selected_index2=-1;}
    void SetMarker(int i1, int i2){selected_index1=i1;selected_index2=i2;}

    int GetAnswer(){return selected_index1;}

	void reset(){selected_index1=-1;}

protected:
	void cleanup();
private:
	svVector3 *dataPos;
	svScalar *dataDir;
	svScalar *dataExp;
	svScalar *dataSig;
	svVector4Array *dataCol;
    svVector4Array *dataSigCol;
    svVector4Array *dataExpCol;
	svInt dataSize;

	svVector3Array *initcolor;
	svVector3Array *initlightness;

	float block_width;
	float block_height;

    float image_width;
    float image_height;

    svScalar scale_value;

    int selected_index1;
    int selected_index2;

	svVector4 brewcolors[18];

    GLfloat tb_transform[4][4];

    GLubyte checkImage[19][checkImageHeight][checkImageWidth][4];
    GLuint texName[19];
};
}
