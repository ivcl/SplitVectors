
#include "svException.h"
#include "svUtil.h"
#include "svArray.h"

#include "CIEColor.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string.h>
#include <vector>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

namespace __svl_lib{

class Readdata{
public:
	Readdata(){dataSize=0;databound=NULL;datapos=NULL;data2dpos=NULL;datacol=NULL;datafa=NULL;dataorder=NULL;multiplecol=NULL;divergecol=NULL;singlecol=NULL;}
	~ Readdata();

	void ProcessData(char *infname, char *outfname);

	void New(char *infname);
	void Removedata(double bound);
	void ReOrder();

	void SetColorDegree(int n){colordegree = (maxfa-minfa)/(n-1);numofcolor=n;}
	void SetImageWidth(int w, int h){iwidth = (double)w;iheight=(double)h;row_num=linenum/iwidth+1;
					x_d = (double)row_num * iwidth/(double)linenum;cerr<<row_num<<" "<<x_d<<endl;}

	void NewMultipleColors();
	void NewDivergingColors();
	void NewSingleColors();
	void SetColors();

	void Generate(int type);
	void Generate_Bar(int type);
	void Render_OrderBar(int type, int order_type);

	void RenderMultipleLegend();
	void RenderDivergingLegend();
	void RenderSingleLegend();
	void Render(int n);

protected:
	void cleanup();
	void cleanup_color();
private:
	svVector3Array *datapos;
	svVector3Array *data2dpos;
	svVector3Array *datacol;
	svScalarArray *datafa;
	svInt *dataorder;
	svIntArray *dataorder_y;
	svIntArray *databound;
	svInt *datafiberin;
	int dataSize;	

	svVector3 *multiplecol;
	svVector3 *divergecol;
	svVector3 *singlecol;

	int linenum;
	svScalar colordegree;
	int numofcolor;

	svScalar maxfa;
	svScalar minfa;

	svVector3 lb;
	svVector3 rb;
	svVector3 lb2;
	svVector3 rb2;

	int row_num;
	double iwidth;
	double iheight;
	double x_d;
};

}
