#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "svROIbox.h"
#include <string.h>

using namespace std;

namespace __svl_lib
{
svROIbox::svROIbox(svVector3 l, svVector3 r,int dataSize,
          svVector3 *posIn, svScalar *denIn)
{


	glyphPos = NULL;
	glyphMag = NULL;
	glyphIndex = NULL;

	SetData(l,r,dataSize,posIn,denIn);
}

void svROIbox::SetData(svVector3 l, svVector3 r,int dataSize,
          svVector3 *posIn, svScalar *denIn)
{
	if(glyphPos!=NULL)
	{
		glyphPos[0].free();
		delete [] glyphPos;
		glyphPos = NULL;
		delete [] glyphMag;
	}

	lb = l;rb = r;

	int i;

	glyphPos = new svVector3Array[1];
	glyphMag = new svScalar[dataSize];
	glyphIndex = NULL;

	for(i=0;i<dataSize;i++)
	{
		glyphPos[0].add(posIn[i]);
		glyphMag[i] = denIn[i];
	}

	SELECTED=GL_FALSE;
	
	float xl = rb[0] - lb[0];
	float yl = rb[1] - lb[1];	

	intersectionp[0][0]=lb[0]+xl/4.;intersectionp[0][1]=lb[1]+yl/4.;intersectionp[0][2]=lb[2];
	intersectionp[1][0]=lb[0]+xl*3./4.;intersectionp[1][1]=lb[1]+yl/4.;intersectionp[1][2]=lb[2];
	intersectionp[2][0]=lb[0]+xl*3./4.;intersectionp[2][1]=lb[1]+yl*3./4.;intersectionp[2][2]=lb[2];
	intersectionp[3][0]=lb[0]+xl/4.;intersectionp[3][1]=lb[1]+yl*3/4.;intersectionp[3][2]=lb[2];

	intersectionp[4][0]=lb[0]+xl/4.;intersectionp[4][1]=lb[1]+yl/4.;intersectionp[4][2]=rb[2];
	intersectionp[5][0]=lb[0]+xl*3./4.;intersectionp[5][1]=lb[1]+yl/4.;intersectionp[5][2]=rb[2];
	intersectionp[6][0]=lb[0]+xl*3./4.;intersectionp[6][1]=lb[1]+yl*3./4.;intersectionp[6][2]=rb[2];
	intersectionp[7][0]=lb[0]+xl/4.;intersectionp[7][1]=lb[1]+yl*3/4.;intersectionp[7][2]=rb[2];

	widgetp[0] = intersectionp[0];
	for(int i=4;i<11;i++)
		widgetp[i] = intersectionp[i-3];
	widgetp[1] = (intersectionp[0] + intersectionp[2])/2.;
	widgetp[2] = lb;
	widgetp[3] = lb;widgetp[3][2] = rb[2];

	selectedNum = 0;

	dirx = normalize(intersectionp[1] - intersectionp[0]);
	diry = normalize(intersectionp[3] - intersectionp[0]);

	stop1 = false;
	stop2 = false;

	for(int i=0;i<8;i++)
	{
		intersectionp_out[i] = false;
	}
	for(int i=0;i<12;i++)
	{
		intersection_out_num[i] = 0;
	}
}
void svROIbox::Reset()
{
	SELECTED=GL_FALSE;
	
	float xl = rb[0] - lb[0];
	float yl = rb[1] - lb[1];	

	intersectionp[0][0]=lb[0]+xl/4.;intersectionp[0][1]=lb[1]+yl/4.;intersectionp[0][2]=lb[2];
	intersectionp[1][0]=lb[0]+xl*3./4.;intersectionp[1][1]=lb[1]+yl/4.;intersectionp[1][2]=lb[2];
	intersectionp[2][0]=lb[0]+xl*3./4.;intersectionp[2][1]=lb[1]+yl*3./4.;intersectionp[2][2]=lb[2];
	intersectionp[3][0]=lb[0]+xl/4.;intersectionp[3][1]=lb[1]+yl*3/4.;intersectionp[3][2]=lb[2];

	intersectionp[4][0]=lb[0]+xl/4.;intersectionp[4][1]=lb[1]+yl/4.;intersectionp[4][2]=rb[2];
	intersectionp[5][0]=lb[0]+xl*3./4.;intersectionp[5][1]=lb[1]+yl/4.;intersectionp[5][2]=rb[2];
	intersectionp[6][0]=lb[0]+xl*3./4.;intersectionp[6][1]=lb[1]+yl*3./4.;intersectionp[6][2]=rb[2];
	intersectionp[7][0]=lb[0]+xl/4.;intersectionp[7][1]=lb[1]+yl*3/4.;intersectionp[7][2]=rb[2];

	widgetp[0] = intersectionp[0];
	for(int i=4;i<11;i++)
		widgetp[i] = intersectionp[i-3];
	widgetp[1] = (intersectionp[0] + intersectionp[2])/2.;
	widgetp[2] = lb;
	widgetp[3] = lb;widgetp[3][2] = rb[2];

	selectedNum = 0;

	dirx = normalize(intersectionp[1] - intersectionp[0]);
	diry = normalize(intersectionp[3] - intersectionp[0]);

	stop1 = false;
	stop2 = false;

	for(int i=0;i<8;i++)
	{
		intersectionp_out[i] = false;
	}
	for(int i=0;i<12;i++)
	{
		intersection_out_num[i] = 0;
	}
}
void svROIbox::WholeField()
{
	SELECTED=GL_FALSE;
	
	float xl = rb[0] - lb[0];
	float yl = rb[1] - lb[1];	

	intersectionp[0][0]=lb[0];intersectionp[0][1]=lb[1];intersectionp[0][2]=lb[2];
	intersectionp[1][0]=lb[0]+xl*4./4.;intersectionp[1][1]=lb[1];intersectionp[1][2]=lb[2];
	intersectionp[2][0]=lb[0]+xl*4./4.;intersectionp[2][1]=lb[1]+yl*4./4.;intersectionp[2][2]=lb[2];
	intersectionp[3][0]=lb[0];intersectionp[3][1]=lb[1]+yl*4./4.;intersectionp[3][2]=lb[2];

	intersectionp[4][0]=lb[0];intersectionp[4][1]=lb[1];intersectionp[4][2]=rb[2];
	intersectionp[5][0]=lb[0]+xl*4./4.;intersectionp[5][1]=lb[1];intersectionp[5][2]=rb[2];
	intersectionp[6][0]=lb[0]+xl*4./4.;intersectionp[6][1]=lb[1]+yl*4./4.;intersectionp[6][2]=rb[2];
	intersectionp[7][0]=lb[0];intersectionp[7][1]=lb[1]+yl*4./4.;intersectionp[7][2]=rb[2];

	widgetp[0] = intersectionp[0];
	for(int i=4;i<11;i++)
		widgetp[i] = intersectionp[i-3];
	widgetp[1] = (intersectionp[0] + intersectionp[2])/2.;
	widgetp[2] = lb;
	widgetp[3] = lb;widgetp[3][2] = rb[2];

	selectedNum = 0;

	dirx = normalize(intersectionp[1] - intersectionp[0]);
	diry = normalize(intersectionp[3] - intersectionp[0]);

	stop1 = false;
	stop2 = false;

	for(int i=0;i<8;i++)
	{
		intersectionp_out[i] = false;
	}
	for(int i=0;i<12;i++)
	{
		intersection_out_num[i] = 0;
	}
}
void svROIbox::RenderLines(int p0, int p1, int l, svVector3 c)//need to be improved!!!!!
{
	 glEnable(GL_LINE_SMOOTH);
	 glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
   	 glEnable(GL_BLEND);
   	 glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if(intersectionp_out[p0]==false&&intersectionp_out[p1]==false)
	{
		glColor3f(c[0],c[1],c[2]);
		glLineWidth(1);
		glBegin(GL_LINES);
		glVertex3f(intersectionp[p0][0],intersectionp[p0][1],intersectionp[p0][2]);
		glVertex3f(intersectionp[p1][0],intersectionp[p1][1],intersectionp[p1][2]);
		glEnd();
	}
	else if((intersectionp_out[p0]==false&&intersectionp_out[p1]==true))
	{
		glColor4f(c[0],c[1],c[2],0.2);
		glLineWidth(0.1);
		glBegin(GL_LINES);
		glVertex3f(intersectionp[p1][0],intersectionp[p1][1],intersectionp[p1][2]);
		glVertex3f(intersection_out_boundary[l][0][0],intersection_out_boundary[l][0][1],
			intersection_out_boundary[l][0][2]);
		glEnd();

		glColor3f(c[0],c[1],c[2]);
		glLineWidth(1.);
		glBegin(GL_LINES);
		glVertex3f(intersectionp[p0][0],intersectionp[p0][1],intersectionp[p0][2]);
		glVertex3f(intersection_out_boundary[l][0][0],intersection_out_boundary[l][0][1],
			intersection_out_boundary[l][0][2]);
		glEnd();
	}
	else if(intersectionp_out[p0]==true&&intersectionp_out[p1]==false)
	{
		glColor3f(c[0],c[1],c[2]);
		glLineWidth(1);
		glBegin(GL_LINES);
		glVertex3f(intersectionp[p1][0],intersectionp[p1][1],intersectionp[p1][2]);
		glVertex3f(intersection_out_boundary[l][0][0],intersection_out_boundary[l][0][1],
			intersection_out_boundary[l][0][2]);
		glEnd();
		glColor4f(c[0],c[1],c[2],0.2);
		glLineWidth(0.1);
		glBegin(GL_LINES);
		glVertex3f(intersectionp[p0][0],intersectionp[p0][1],intersectionp[p0][2]);
		glVertex3f(intersection_out_boundary[l][0][0],intersection_out_boundary[l][0][1],
			intersection_out_boundary[l][0][2]);
		glEnd();
	}
	else if((intersectionp_out[p0]==true&&intersectionp_out[p1]==true)
		&&intersection_out_num[l]==0)
	{
		glColor4f(c[0],c[1],c[2],0.2);
		glLineWidth(0.1);
		glBegin(GL_LINES);
		glVertex3f(intersectionp[p0][0],intersectionp[p0][1],intersectionp[p0][2]);
		glVertex3f(intersectionp[p1][0],intersectionp[p1][1],intersectionp[p1][2]);
		glEnd();
	}
	else if((intersectionp_out[p0]==true&&intersectionp_out[p1]==true)
		&&intersection_out_num[l]==2)
	{
		glColor4f(c[0],c[1],c[2],0.2);
		glLineWidth(0.1);
		glBegin(GL_LINES);
		glVertex3f(intersectionp[p0][0],intersectionp[p0][1],intersectionp[p0][2]);
		glVertex3f(intersection_out_boundary[l][0][0],intersection_out_boundary[l][0][1],
			intersection_out_boundary[l][0][2]);
		glEnd();
		glColor4f(c[0],c[1],c[2],0.2);
		glLineWidth(0.1);
		glBegin(GL_LINES);
		glVertex3f(intersectionp[p1][0],intersectionp[p1][1],intersectionp[p1][2]);
		glVertex3f(intersection_out_boundary[l][1][0],intersection_out_boundary[l][1][1],
			intersection_out_boundary[l][1][2]);
		glEnd();
		glColor3f(c[0],c[1],c[2]);
		glLineWidth(1);
		glBegin(GL_LINES);
		glVertex3f(intersection_out_boundary[l][0][0],intersection_out_boundary[l][0][1],
			intersection_out_boundary[l][0][2]);
		glVertex3f(intersection_out_boundary[l][1][0],intersection_out_boundary[l][1][1],
			intersection_out_boundary[l][1][2]);
		glEnd();
	}
}
void svROIbox::RenderROIbox()
{
	char str[50];

	glColor3f(1.,1.,1.);

	glPointSize(7.);	
	glBegin(GL_POINTS);
	for(int i=0;i<11;i++)
	{
		if(ni==i&&(SELECTED==GL_TRUE||NEAR==GL_TRUE))
			glColor3f(1.,1.,0.);
		else
			glColor3f(1.,1.,1.);

		glVertex3f(widgetp[i][0],widgetp[i][1],widgetp[i][2]);
	}
	glEnd();
	glPointSize(1.);

	/*for(int i=0;i<8;i++)
	{
		sprintf(str, "%d",i);
		glRasterPos3f(intersectionp[i][0],intersectionp[i][1],intersectionp[i][2]);
		for(int j=0;j<strlen(str);j++)
		       	 glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
	}*/

	//glColor3f(1.,1.,1.);
	for(int i=2;i<=3;i++)
        {
		sprintf(str, "%.2f", widgetp[i][2]);
		glRasterPos3f(widgetp[i][0],widgetp[i][1],widgetp[i][2]);
		for(int j=0;j<strlen(str);j++)
		       	 glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
	}

	/*
	glBegin(GL_LINE_LOOP);
	for(int i=4;i<8;i++)
		glVertex3f(intersectionp[i][0],intersectionp[i][1],intersectionp[i][2]);
	glEnd();*/

	glColor3f(1.,1.,1.);

	RenderLines(0,1,0,svVector3(1.,1.,1.));
	RenderLines(1,2,1,svVector3(217./255.,240./255.,163./255.));
	RenderLines(2,3,2,svVector3(1.,1.,1.));
	RenderLines(3,0,3,svVector3(217./255.,240./255.,163./255.));
	RenderLines(0,4,4,svVector3(1.,1.,1.));
	RenderLines(1,5,5,svVector3(1.,1.,1.));
	RenderLines(2,6,6,svVector3(1.,1.,1.));
	RenderLines(3,7,7,svVector3(1.,1.,1.));
	RenderLines(4,5,8,svVector3(1.,1.,1.));
	RenderLines(5,6,9,svVector3(217./255.,240./255.,163./255.));
	RenderLines(6,7,10,svVector3(1.,1.,1.));
	RenderLines(7,4,11,svVector3(217./255.,240./255.,163./255.));
/*
	//need to be improved!!!!!
	if(intersectionp_out[0]==false&&intersectionp_out[1]==false)
	{
		glBegin(GL_LINES);
		glVertex3f(intersectionp[0][0],intersectionp[0][1],intersectionp[0][2]);
		glVertex3f(intersectionp[1][0],intersectionp[1][1],intersectionp[1][2]);
		glEnd();
	}
	else if((intersectionp_out[0]==false&&intersectionp_out[1]==true)
		|| (intersectionp_out[0]==true&&intersectionp_out[1]==false))
	{
		glLineWidth(0.1);
		glBegin(GL_LINES);
		glVertex3f(intersectionp[0][0],intersectionp[0][1],intersectionp[0][2]);
		glVertex3f(intersectionp[1][0],intersectionp[1][1],intersectionp[1][2]);
		glEnd();
	}
		
	glBegin(GL_LINES);
	glVertex3f(intersectionp[4][0],intersectionp[4][1],intersectionp[4][2]);
	glVertex3f(intersectionp[5][0],intersectionp[5][1],intersectionp[5][2]);
	glEnd();

	glColor3f(1.,1.,1.);
	glBegin(GL_LINES);
	glVertex3f(intersectionp[2][0],intersectionp[2][1],intersectionp[2][2]);
	glVertex3f(intersectionp[3][0],intersectionp[3][1],intersectionp[3][2]);
	glEnd();	
	glBegin(GL_LINES);
	glVertex3f(intersectionp[6][0],intersectionp[6][1],intersectionp[6][2]);
	glVertex3f(intersectionp[7][0],intersectionp[7][1],intersectionp[7][2]);
	glEnd();

	glColor3f(217./255.,240./255.,163./255.);
	glBegin(GL_LINES);
	glVertex3f(intersectionp[0][0],intersectionp[0][1],intersectionp[0][2]);
	glVertex3f(intersectionp[3][0],intersectionp[3][1],intersectionp[3][2]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(intersectionp[4][0],intersectionp[4][1],intersectionp[4][2]);
	glVertex3f(intersectionp[7][0],intersectionp[7][1],intersectionp[7][2]);
	glEnd();

	glColor3f(217./255.,240./255.,163./255.);
	glBegin(GL_LINES);
	glVertex3f(intersectionp[2][0],intersectionp[2][1],intersectionp[2][2]);
	glVertex3f(intersectionp[1][0],intersectionp[1][1],intersectionp[1][2]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(intersectionp[6][0],intersectionp[6][1],intersectionp[6][2]);
	glVertex3f(intersectionp[5][0],intersectionp[5][1],intersectionp[5][2]);
	glEnd();

	glColor3f(1.,1.,1.);
	glBegin(GL_LINES);
	for(int i=0;i<4;i++)
	{
		glVertex3f(intersectionp[i+4][0],intersectionp[i+4][1],intersectionp[i+4][2]);
		glVertex3f(intersectionp[i][0],intersectionp[i][1],intersectionp[i][2]);	
	}
	glEnd();
*/
	glLineWidth(1.);
}

svVector3 svROIbox::CrossProduct(svVector3 u, svVector3 v)
{
	svVector3 c;

	c[0]=u[1]*v[2] - u[2]*v[1];
	c[1]=u[2]*v[0] - u[0]*v[2];
	c[2]=u[0]*v[1] - u[1]*v[0];

	c = normalize(c);

	return c;
}

float svROIbox::DotProduct(svVector3 u, svVector3 v)
{
	float d;

	d = u[0]*v[0] + u[1]*v[1] + u[2]*v[2];

	return d;
}

bool svROIbox::InBox(svVector3 p1, svVector3 p2, svVector3 p,svVector3 point)
{
	//p-----------p1
	//|\
	//| \n
	//p2

	svVector3 n = CrossProduct(p2-p, p1-p);

	svVector3 tmp = point - p;

	float tmpd = DotProduct(tmp,n);

	bool in;
	if(tmpd>=0)in = false;
	else in = true;

	return in;
}

bool *svROIbox::InBox(svVector3 *pos, int datasize)
{
	int i;
	bool *in = new bool[datasize];

	svVector3 tmpl=intersectionp[0];
	svVector3 tmpr=intersectionp[0];

	svVector3 z;
	z[0]=0.;z[1]=0.;z[2]=1.;

	for(i=1;i<8;i++)
	{
		if(intersectionp[i][0]>=tmpr[0])tmpr[0]=intersectionp[i][0];
		if(intersectionp[i][0]<=tmpl[0])tmpl[0]=intersectionp[i][0];
		if(intersectionp[i][1]>=tmpr[1])tmpr[1]=intersectionp[i][1];
		if(intersectionp[i][1]<=tmpl[1])tmpl[1]=intersectionp[i][1];
	}

	tmpl[2] = widgetp[2][2];
	tmpr[2] = widgetp[3][2];

	bool tmpin[4];
	//cerr<<tmpl[0]<<" "<<tmpr[0]<<endl;
	//cerr<<tmpl[1]<<" "<<tmpr[1]<<endl;	//cerr<<tmpl[2]<<" "<<tmpr[2]<<endl;
	for(i=0;i<datasize;i++)
	{
		if(pos[i][0]>=tmpl[0]&&pos[i][0]<=tmpr[0]
		&&pos[i][1]>=tmpl[1]&&pos[i][1]<=tmpr[1]
		&&pos[i][2]>=tmpl[2]&&pos[i][2]<=tmpr[2])
		{
			tmpin[0]=InBox(intersectionp[0], intersectionp[5],
				intersectionp[1],pos[i]);
			tmpin[1]=InBox(intersectionp[1], intersectionp[6],  
				intersectionp[2],pos[i]);
			tmpin[2]=InBox(intersectionp[2], intersectionp[7], 
				intersectionp[3],pos[i]);
			tmpin[3]=InBox(intersectionp[3], intersectionp[4], 
				intersectionp[0],pos[i]);

			if(tmpin[0]==true&&tmpin[1]==true&&tmpin[2]==true&&tmpin[3]==true)
			{
				in[i] = true;
			}
			else
				in[i] = false;
		}
		else
			in[i] = false;
	}

	return in;
}
vector<bool> svROIbox::InBox2(svVector3 *pos, int datasize)
{
	int i;
	vector<bool> in;
	in.resize(datasize);

	svVector3 tmpl=intersectionp[0];
	svVector3 tmpr=intersectionp[0];

	svVector3 z;
	z[0]=0.;z[1]=0.;z[2]=1.;

	for(i=1;i<8;i++)
	{
		if(intersectionp[i][0]>=tmpr[0])tmpr[0]=intersectionp[i][0];
		if(intersectionp[i][0]<=tmpl[0])tmpl[0]=intersectionp[i][0];
		if(intersectionp[i][1]>=tmpr[1])tmpr[1]=intersectionp[i][1];
		if(intersectionp[i][1]<=tmpl[1])tmpl[1]=intersectionp[i][1];
	}

	tmpl[2] = widgetp[2][2];
	tmpr[2] = widgetp[3][2];

	bool tmpin[4];
	//cerr<<tmpl[0]<<" "<<tmpr[0]<<endl;
	//cerr<<tmpl[1]<<" "<<tmpr[1]<<endl;	//cerr<<tmpl[2]<<" "<<tmpr[2]<<endl;
	for(i=0;i<datasize;i++)
	{
		if(pos[i][0]>=tmpl[0]&&pos[i][0]<=tmpr[0]
		&&pos[i][1]>=tmpl[1]&&pos[i][1]<=tmpr[1]
		&&pos[i][2]>=tmpl[2]&&pos[i][2]<=tmpr[2])
		{
			tmpin[0]=InBox(intersectionp[0], intersectionp[5],
				intersectionp[1],pos[i]);
			tmpin[1]=InBox(intersectionp[1], intersectionp[6],  
				intersectionp[2],pos[i]);
			tmpin[2]=InBox(intersectionp[2], intersectionp[7], 
				intersectionp[3],pos[i]);
			tmpin[3]=InBox(intersectionp[3], intersectionp[4], 
				intersectionp[0],pos[i]);

			if(tmpin[0]==true&&tmpin[1]==true&&tmpin[2]==true&&tmpin[3]==true)
			{
				in[i] = true;
			}
			else
				in[i] = false;
		}
		else
			in[i] = false;
	}

	return in;
}/*
vector<vector<bool> > svROIbox::InBox(vector<vector<double> > pos[3], int zlayer)
{
	int i;
	//bool *in = new bool[datasize];
	vector<vector<bool> >in;
	in.resize(zlayer);

	svVector3 tmpl=intersectionp[0];
	svVector3 tmpr=intersectionp[0];

	svVector3 z;
	z[0]=0.;z[1]=0.;z[2]=1.;

	for(i=1;i<8;i++)
	{
		if(intersectionp[i][0]>=tmpr[0])tmpr[0]=intersectionp[i][0];
		if(intersectionp[i][0]<=tmpl[0])tmpl[0]=intersectionp[i][0];
		if(intersectionp[i][1]>=tmpr[1])tmpr[1]=intersectionp[i][1];
		if(intersectionp[i][1]<=tmpl[1])tmpl[1]=intersectionp[i][1];
	}

	tmpl[2] = widgetp[2][2];
	tmpr[2] = widgetp[3][2];

	bool tmpin[4];
	//cerr<<tmpl[0]<<" "<<tmpr[0]<<endl;
	//cerr<<tmpl[1]<<" "<<tmpr[1]<<endl;	//cerr<<tmpl[2]<<" "<<tmpr[2]<<endl;
	for(i=0;i<zlayer;i++)
	{
		for(int j=0;j<pos[0][i].size();j++)
		{
			if(pos[0][i][j]>=tmpl[0]&&pos[0][i][j]<=tmpr[0]
			&&pos[1][i][j]>=tmpl[1]&&pos[1][i][j]<=tmpr[1]
			&&pos[2][i][j]>=tmpl[2]&&pos[2][i][j]<=tmpr[2])
			{
				svVector3 p;
				p[0] = pos[0][i][j];
				p[1] = pos[1][i][j];
				p[2] = pos[2][i][j];
				tmpin[0]=InBox(intersectionp[0], intersectionp[5],
					intersectionp[1],p);
				tmpin[1]=InBox(intersectionp[1], intersectionp[6],  
					intersectionp[2],p);
				tmpin[2]=InBox(intersectionp[2], intersectionp[7], 
					intersectionp[3],p);
				tmpin[3]=InBox(intersectionp[3], intersectionp[4], 
					intersectionp[0],p);

				if(tmpin[0]==true&&tmpin[1]==true&&tmpin[2]==true&&tmpin[3]==true)
				{
					in[i].push_back(true);
				}
				else
					in[i].push_back(false);
			}
			else
				in[i].push_back(false);
		}
	}

	return in;
}*/
bool svROIbox::InBox(svVector3 p, svVector3 point)
{
	float tmpd = DotProduct(p,point);

	bool in;
	if(tmpd>=0)in = false;
	else in = true;

	return in;
}

svIntArray * svROIbox:: GetSelectedIndex()
{
	if(glyphIndex!=NULL)
	{
		glyphIndex[0].free();
		delete [] glyphIndex;
		glyphIndex = NULL;
	}

	glyphIndex = new svIntArray[1];
	selectedNum = 0;

	svVector3 tmpl=intersectionp[0];
	svVector3 tmpr=intersectionp[0];
	int i;
	svVector3 z;
	z[0]=0.;z[1]=0.;z[2]=1.;

	for(i=1;i<4;i++)
	{
		if(intersectionp[i][0]>=tmpr[0])tmpr[0]=intersectionp[i][0];
		if(intersectionp[i][0]<=tmpl[0])tmpl[0]=intersectionp[i][0];
		if(intersectionp[i][1]>=tmpr[1])tmpr[1]=intersectionp[i][1];
		if(intersectionp[i][1]<=tmpl[1])tmpl[1]=intersectionp[i][1];
	}

	tmpl[2] = widgetp[9][2];
	tmpr[2] = widgetp[8][2];

	int sample = glyphPos[0].size();
	bool tmpin[4];

	for(i=0;i<sample;i++)
	{
		if(glyphPos[0][i][0]>=tmpl[0]&&glyphPos[0][i][0]<=tmpr[0]
		&&glyphPos[0][i][1]>=tmpl[1]&&glyphPos[0][i][1]<=tmpr[1]
		&&glyphPos[0][i][2]>=tmpl[2]&&glyphPos[0][i][2]<=tmpr[2])
		{
			tmpin[0]=InBox(intersectionp[0], intersectionp[1]+z,
				intersectionp[1],glyphPos[0][i]);
			tmpin[1]=InBox(intersectionp[2]+z,intersectionp[3],  
				intersectionp[2],glyphPos[0][i]);
			tmpin[2]=InBox(intersectionp[3]+z, intersectionp[0], 
				intersectionp[3],glyphPos[0][i]);
			tmpin[3]=InBox(intersectionp[1], intersectionp[2]+z, 
				intersectionp[2],glyphPos[0][i]);

			if(tmpin[0]==true&&tmpin[1]==true&&tmpin[2]==true&&tmpin[3]==true)
			{
				glyphIndex[0].add(i);
				selectedNum++;
			}
		}
	}

	return glyphIndex;
}

float svROIbox::GetROIden(float scalar)
{
	int i;
	int datasize = glyphPos[0].size();
	svVector3 tmpl=intersectionp[0];
	svVector3 tmpr=intersectionp[0];

	svVector3 z;
	z[0]=0.;z[1]=0.;z[2]=1.;

	for(i=1;i<8;i++)
	{
		if(intersectionp[i][0]>=tmpr[0])tmpr[0]=intersectionp[i][0];
		if(intersectionp[i][0]<=tmpl[0])tmpl[0]=intersectionp[i][0];
		if(intersectionp[i][1]>=tmpr[1])tmpr[1]=intersectionp[i][1];
		if(intersectionp[i][1]<=tmpl[1])tmpl[1]=intersectionp[i][1];
	}

	tmpl[2] = widgetp[2][2];
	tmpr[2] = widgetp[3][2];

	bool tmpin[4];
	float den = 0;

	for(i=0;i<datasize;i++)
	{
		if(glyphPos[0][i][0]>=tmpl[0]&&glyphPos[0][i][0]<=tmpr[0]
		&&glyphPos[0][i][1]>=tmpl[1]&&glyphPos[0][i][1]<=tmpr[1]
		&&glyphPos[0][i][2]>=tmpl[2]&&glyphPos[0][i][2]<=tmpr[2])
		{
			tmpin[0]=InBox(intersectionp[0], intersectionp[5],
				intersectionp[1],glyphPos[0][i]);
			tmpin[1]=InBox(intersectionp[1], intersectionp[6],  
				intersectionp[2],glyphPos[0][i]);
			tmpin[2]=InBox(intersectionp[2], intersectionp[7], 
				intersectionp[3],glyphPos[0][i]);
			tmpin[3]=InBox(intersectionp[3], intersectionp[4], 
				intersectionp[0],glyphPos[0][i]);

			if(tmpin[0]==true&&tmpin[1]==true&&tmpin[2]==true&&tmpin[3]==true)
			{
				den = den + glyphMag[i];
			}
		}
	}
	float tmppow = 1;
	for(int iii=0;iii<scalar;iii++)
		tmppow = tmppow * 10.;
	
	den = den/tmppow;//pow(10.,scalar);
	return den;	
}

void svROIbox::cleanup()
{
	if(glyphPos!=NULL)
	{
		glyphPos[0].free();
		delete [] glyphPos;
		glyphPos=NULL;
	}
	
	delete [] glyphMag;
}
void svROIbox::move_ctrlpoint(int x, int y)
{
    GLdouble xx, yy, zz;
    GLdouble wld_x, wld_y, wld_z;       /* world coordinate */
    GLdouble tx, ty, tz;
    GLfloat invert_tb[16], tm[16];
    int i, j, k;


    //glGetIntegerv (GL_VIEWPORT, viewport);
    //glGetDoublev (GL_MODELVIEW_MATRIX, mvmatrix);
    //glGetDoublev (GL_PROJECTION_MATRIX, projmatrix);

    xx = (GLdouble)x;
    yy = (GLdouble)width-(GLdouble)y;
    zz = wz[ni];

    gluUnProject(xx, yy, zz,
                mvmatrix, projmatrix, viewport,
                &tx, &ty, &tz);
    k=0;
    for(i=0; i<4; i++)
        for(j=0; j<4; j++)
            tm[k++]=tb_transform[i][j];

    if(!invert_matrix(tm, invert_tb)) {
        printf("Erron in inverting transform matrix\n");
        exit(0);
    }

    wld_x = tx * invert_tb[0] +
            ty * invert_tb[4] +
            tz * invert_tb[8] +
            invert_tb[12];

    wld_y = tx * invert_tb[1] +
            ty * invert_tb[5] +
            tz * invert_tb[9] +
            invert_tb[13];

    wld_z = tx * invert_tb[2] +
            ty * invert_tb[6] +
            tz * invert_tb[10] +
            invert_tb[14];

    SetLocation(wld_x, wld_y,wld_z);
}

svVector3 svROIbox::Intersection2Lines(svVector3 d1,svVector3 p1,svVector3 d2,svVector3 p2)
{
	float x1 = p1[0];
	float y1 = p1[1];
	float u1 = d1[0];
	float v1 = d1[1];
	float x2 = p2[0];
	float y2 = p2[1];
	float u2 = d2[0];
	float v2 = d2[1];

	float a;
	a = (-v2*(x2-x1)+u2*(y2-y1))/(-u1*v2+v1*u2);

	svVector3 result;

	result[2] = p1[2];
	result[0] = x1 + a*u1;
	result[1] = y1 + a*v1;

	return result;
}

bool svROIbox::ifreverse()
{
	svVector3 u;
	u = intersectionp[1] - intersectionp[0];
	svVector3 v;
	v = intersectionp[3] - intersectionp[0];
	svVector3 w;
	w = CrossProduct(u,v);
	if(w[2]>0)
		return true;
	else
		return false;
}

void svROIbox::SetLocation(GLdouble wld_x, GLdouble wld_y,GLdouble wld_z)
{
	svVector3 vx,vy,vz;
	vx[0]=1.;vx[1]=0.;vx[2]=0.;
	vy[0]=0.;vy[1]=1.;vy[2]=0.;
	vz[0]=0.;vz[1]=0.;vz[2]=1.;
	svVector3 tmpp;
	float deltal;
	float tmpl;
	float deltax,deltay;
	svVector3 d;
	svVector3 p1,p2,p3,p4,p5,p6,p7;

	svVector3 store[12];
	
	if(ni==0||ni==7)
	{
		d[0] = wld_x - widgetp[ni][0];
		d[1] = wld_y - widgetp[ni][1];
		d[2] = 0.;
		d = normalize(d);
		deltal = sqrt((wld_x-widgetp[ni][0])*(wld_x-widgetp[ni][0])
				+(wld_y-widgetp[ni][1])*(wld_y-widgetp[ni][1]));
		deltax = DotProduct(dirx,d) * deltal;
		deltay = DotProduct(diry,d) * deltal;
		p1 = intersectionp[1] + diry * deltay;
		p2 = intersectionp[3] + dirx * deltax;	
		p3 = intersectionp[5] + diry * deltay;
		p4 = intersectionp[7] + dirx * deltax;
		if(ni==0)
			p5 = intersectionp[4] + dirx * deltax;
		else
			p5 = intersectionp[0] + dirx * deltax;
		p5 = p5 + diry * deltay;	
		/*if(p1[0]>=lb[0]&&p1[0]<=rb[0]&&p1[1]>=lb[1]&&p1[1]<=rb[1]
		&&p2[0]>=lb[0]&&p2[0]<=rb[0]&&p2[1]>=lb[1]&&p2[1]<=rb[1]
		&&p3[0]>=lb[0]&&p3[0]<=rb[0]&&p3[1]>=lb[1]&&p3[1]<=rb[1]
		&&p4[0]>=lb[0]&&p4[0]<=rb[0]&&p4[1]>=lb[1]&&p4[1]<=rb[1]
		&&p5[0]>=lb[0]&&p5[0]<=rb[0]&&p5[1]>=lb[1]&&p5[1]<=rb[1]
		&&wld_x>=lb[0]&&wld_x<=rb[0]&&wld_y>=lb[1]&&wld_y<=rb[1])
		{*/
		store[0] = widgetp[0];
		store[1] = intersectionp[0];
		store[2] = intersectionp[4];
		store[3] = widgetp[7];
		store[4] = intersectionp[4];
		store[5] = intersectionp[0];
		store[6] = intersectionp[1];
		store[7] = intersectionp[3];
		store[8] = intersectionp[5];
		store[9] = intersectionp[7];
		store[10] = widgetp[1];
		if(ni==0)
		{
			widgetp[0] = intersectionp[0] + dirx * deltax;
			widgetp[0] = widgetp[0] + diry * deltay;
			intersectionp[0] = widgetp[0];
			intersectionp[4] = p5;
		}
		else
		{
			widgetp[7] = intersectionp[4] + dirx * deltax;
			widgetp[7] = widgetp[7] + diry * deltay;
			intersectionp[4] = widgetp[7];
			intersectionp[0] = p5;
		}
		intersectionp[1] = p1;
		intersectionp[3] = p2;
		intersectionp[5] = p3;
		intersectionp[7] = p4;
		widgetp[1] = (intersectionp[0] + intersectionp[2])/2.;

		if(!ifreverse())
		{
			widgetp[0]=store[0];
			intersectionp[0]=store[1];
			intersectionp[4]=store[2];
			widgetp[7]=store[3];
			intersectionp[4]=store[4];
			intersectionp[0]=store[5];
			intersectionp[1]=store[6];
			intersectionp[3]=store[7];
			intersectionp[5]=store[8];
			intersectionp[7]=store[9];
			widgetp[1]=store[10];
		}

		//}
	}
	else if(ni==4||ni==8)
	{
		d[0] = wld_x - widgetp[ni][0];
		d[1] = wld_y - widgetp[ni][1];
		d[2] = 0.;
		d = normalize(d);
		deltal = sqrt((wld_x-widgetp[ni][0])*(wld_x-widgetp[ni][0])
				+(wld_y-widgetp[ni][1])*(wld_y-widgetp[ni][1]));
		deltax = DotProduct(dirx,d) * deltal;
		deltay = DotProduct(diry,d) * deltal;

		p1 = intersectionp[0] + diry * deltay;
		p2 = intersectionp[2] + dirx * deltax;	
		p3 = intersectionp[4] + diry * deltay;
		p4 = intersectionp[6] + dirx * deltax;
		if(ni==4)
			p5 = intersectionp[5] + dirx * deltax;
		else
			p5 = intersectionp[1] + dirx * deltax;
		p5 = p5 + diry * deltay;

		store[0] = widgetp[4];
		store[1] = intersectionp[1];
		store[2] = intersectionp[5];
		store[3] = widgetp[8];
		store[4] = intersectionp[5];
		store[5] = intersectionp[1];
		store[6] = intersectionp[0];
		store[7] = intersectionp[2];
		store[8] = intersectionp[4];
		store[9] = intersectionp[6];
		store[10] = widgetp[1];
		if(ni==4)
		{
			widgetp[4] = intersectionp[1] + dirx * deltax;
			widgetp[4] = widgetp[4] + diry * deltay;
			intersectionp[1] = widgetp[4];
			intersectionp[5] = p5;
		}
		else
		{
			widgetp[8] = intersectionp[5] + dirx * deltax;
			widgetp[8] = widgetp[8] + diry * deltay;
			intersectionp[5] = widgetp[8];
			intersectionp[1] = p5;
		}
		intersectionp[0] = p1;
		intersectionp[2] = p2;
		intersectionp[4] = p3;
		intersectionp[6] = p4;
		widgetp[1] = (intersectionp[0] + intersectionp[2])/2.;

		if(!ifreverse())
		{
			widgetp[4]=store[0];
			intersectionp[1]=store[1];
			intersectionp[5]=store[2];
			widgetp[8]=store[3];
			intersectionp[5]=store[4];
			intersectionp[1]=store[5];
			intersectionp[0]=store[6];
			intersectionp[2]=store[7];
			intersectionp[4]=store[8];
			intersectionp[6]=store[9];
			widgetp[1]=store[10];
		}
	}
	else if(ni==5||ni==9)
	{
		d[0] = wld_x - widgetp[ni][0];
		d[1] = wld_y - widgetp[ni][1];
		d[2] = 0.;
		d = normalize(d);
		deltal = sqrt((wld_x-widgetp[ni][0])*(wld_x-widgetp[ni][0])+(wld_y-widgetp[ni][1])*(wld_y-widgetp[ni][1]));
		deltax = DotProduct(dirx,d) * deltal;
		deltay = DotProduct(diry,d) * deltal;

		p1 = intersectionp[3] + diry * deltay;
		p2 = intersectionp[1] + dirx * deltax;	
		p3 = intersectionp[7] + diry * deltay;
		p4 = intersectionp[5] + dirx * deltax;
		if(ni==5)
			p5 = intersectionp[6] + dirx * deltax;
		else
			p5 = intersectionp[2] + dirx * deltax;
		p5 = p5 + diry * deltay;

		store[0] = widgetp[5];
		store[1] = intersectionp[2];
		store[2] = intersectionp[6];
		store[3] = widgetp[9];
		store[4] = intersectionp[6];
		store[5] = intersectionp[2];
		store[6] = intersectionp[3];
		store[7] = intersectionp[1];
		store[8] = intersectionp[7];
		store[9] = intersectionp[5];
		store[10] = widgetp[1];

		if(ni==5)
		{
			widgetp[5] = intersectionp[2] + dirx * deltax;
			widgetp[5] = widgetp[5] + diry * deltay;
			intersectionp[2] = widgetp[5];
			intersectionp[6] = p5;
		}
		else
		{
			widgetp[9] = intersectionp[6] + dirx * deltax;
			widgetp[9] = widgetp[9] + diry * deltay;
			intersectionp[6] = widgetp[9];
			intersectionp[2] = p5;
		}
		intersectionp[3] = p1;
		intersectionp[1] = p2;
		intersectionp[7] = p3;
		intersectionp[5] = p4;
		
		widgetp[1] = (intersectionp[0] + intersectionp[2])/2.;
		if(!ifreverse())
		{
			widgetp[5]=store[0];
			intersectionp[2]=store[1];
			intersectionp[6]=store[2];
			widgetp[9]=store[3];
			intersectionp[6]=store[4];
			intersectionp[2]=store[5];
			intersectionp[3]=store[6];
			intersectionp[1]=store[7];
			intersectionp[7]=store[8];
			intersectionp[5]=store[9];
			widgetp[1]=store[10];
		}
	}
	else if(ni==6||ni==10)
	{
		d[0] = wld_x - widgetp[ni][0];
		d[1] = wld_y - widgetp[ni][1];
		d[2] = 0.;
		d = normalize(d);
		deltal = sqrt((wld_x-widgetp[ni][0])*(wld_x-widgetp[ni][0])
				+(wld_y-widgetp[ni][1])*(wld_y-widgetp[ni][1]));
		deltax = DotProduct(dirx,d) * deltal;
		deltay = DotProduct(diry,d) * deltal;

		p1 = intersectionp[2] + diry * deltay;
		p2 = intersectionp[0] + dirx * deltax;	
		p3 = intersectionp[6] + diry * deltay;
		p4 = intersectionp[4] + dirx * deltax;
		if(ni==6)
			p5 = intersectionp[7] + dirx * deltax;
		else
			p5 = intersectionp[3] + dirx * deltax;
		p5 = p5 + diry * deltay;

		store[0] = widgetp[6];
		store[1] = intersectionp[3];
		store[2] = intersectionp[7];
		store[3] = widgetp[10];
		store[4] = intersectionp[7];
		store[5] = intersectionp[3];
		store[6] = intersectionp[2];
		store[7] = intersectionp[0];
		store[8] = intersectionp[6];
		store[9] = intersectionp[4];
		store[10] = widgetp[4];
		if(ni==6)
		{
			widgetp[6] = intersectionp[3] + dirx * deltax;
			widgetp[6] = widgetp[6] + diry * deltay;
			intersectionp[3] = widgetp[6];
			intersectionp[7] = p5;
		}
		else
		{
			widgetp[10] = intersectionp[7] + dirx * deltax;
			widgetp[10] = widgetp[10] + diry * deltay;
			intersectionp[7] = widgetp[10];
			intersectionp[3] = p5;
		}
		intersectionp[2] = p1;
		intersectionp[0] = p2;
		intersectionp[6] = p3;
		intersectionp[4] = p4;

		widgetp[1] = (intersectionp[0] + intersectionp[2])/2.;
		if(!ifreverse())
		{
			widgetp[6]=store[0];
			intersectionp[3]=store[1];
			intersectionp[7]=store[2];
			widgetp[10]=store[3];
			intersectionp[7]=store[4];
			intersectionp[3]=store[5];
			intersectionp[2]=store[6];
			intersectionp[0]=store[7];
			intersectionp[6]=store[8];
			intersectionp[4]=store[9];
			widgetp[1]=store[10];
		}
	}
	else if(ni==1)
	{
		deltax = wld_x - widgetp[1][0];
		deltay = wld_y - widgetp[1][1];
		svVector3 p[8];
		bool flag=true;
		for(int i=0;i<8;i++)
		{
			p[i] = intersectionp[i];
			p[i][0] = p[i][0] + deltax;
			p[i][1] = p[i][1] + deltay; 
			//if(!(p[i][0]>=lb[0]&&p[i][0]<=rb[0]
			//&&p[i][1]>=lb[1]&&p[i][1]<=rb[1]))
			//{flag = false;break;}
		}
		if(flag)
		{
			for(int i=0;i<8;i++)
			{
				intersectionp[i] = p[i];
			}	
			widgetp[0] = intersectionp[0];
			widgetp[1][0]=wld_x;widgetp[1][1]=wld_y;	
		}
	}
	else if(ni==2)
	{
		if(wld_z>=lb[2]&&wld_z<=widgetp[3][2])
		{
			int j = (wld_z-lb[2])/((rb[2]-lb[2])/(zlayer-1));
			float l = lb[2] + (float)j * ((rb[2]-lb[2])/(zlayer-1));
			cerr<<j<<endl;
			widgetp[2][2] = l;//wld_z; 
			for(int i=0;i<4;i++)
			{
				intersectionp[i][2] =l;// wld_z;
			}
			widgetp[0] = intersectionp[0];
			widgetp[1][2] = l;//wld_z;
		}
	}
	else if(ni==3)
	{
		if(wld_z>=widgetp[2][2]&&wld_z<=rb[2])
		{
			int j = (rb[2] - wld_z)/((rb[2]-lb[2])/(zlayer-1));
			float l = rb[2] - (float)j * ((rb[2]-lb[2])/(zlayer-1));

			widgetp[3][2] = l;//wld_z; 
			for(int i=4;i<8;i++)
			{
				intersectionp[i][2] = l;//wld_z;
			}
			widgetp[0] = intersectionp[0];
		}
	}

	for(int i=4;i<11;i++)
		widgetp[i] = intersectionp[i-3];
	widgetp[0] = intersectionp[0];

	SetOutBoundary();
}

svVector3 svROIbox::Plane2Plane(svVector3 d1,svVector3 p1,svVector3 d2,svVector3 p2,svVector3 ld,svVector3 lp)
{
	svVector3 d = CrossProduct(d1,d2);
	float c1,c2;
	float h1,h2;
	h1 = DotProduct(d1,p1);
	h2 = DotProduct(d2,p2);
	
	c1 = (h1 - h2 *DotProduct(d1,d2))/(1-DotProduct(d1,d2)*DotProduct(d1,d2));
	c2 = (h2 - h1 *DotProduct(d1,d2))/(1-DotProduct(d1,d2)*DotProduct(d1,d2));

	svVector3 p;
	p = c1*d1 + c2*d2;


	svVector3 r=Intersection2Lines(d,p,ld,lp);	
	return r;
}
svVector3 svROIbox::Plane2Plane(svVector3 d1,svVector3 p1,svVector3 d2,svVector3 p2)
{
	svVector3 l0,l;
	svVector3 p0,n;
	n = d1;
	p0 = p1;
	l0 = widgetp[3];
	l[0]=0.;l[1]=1.;l[2]=0.;

	float d;
	svVector3 p;


	if(DotProduct(l,n)!=0)
	{
		d = DotProduct((p0 - l0),n)/DotProduct(l,n);
		p = d *l +l0;
	}
	else
	{
		l[0]=1.;l[1]=0.;l[2]=0.;
		d = DotProduct((p0 - l0),n)/DotProduct(l,n);
		p = d *l +l0;
	}

	return p;
}
bool svROIbox::Line2Plane(svVector3 p0, svVector3 l0, svVector3 l1, svVector3 l, svVector3 n, svVector3 &r)
{
	/*p0[0]=17;p0[1]=17;p0[2]=17;
	n[0]=1;n[1]=0;n[2]=0;
	l0[0]=-30.;l0[1]=-6.9;l0[2]=-10;
	l1[0]=30.;l1[1]=-6.9;l1[2]=-10;
	svVector3 tmpl = l1 - l0;
	l = normalize(tmpl);
	cerr<<l[0]<<" "<<l[1]<<" "<<l[2]<<endl;	*/

	//---
	svVector3 tmpl0 = l0;
	tmpl0[0] = (tmpl0[0] + l1[0])/2.;
	tmpl0[1] = (tmpl0[1] + l1[1])/2.;
	tmpl0[2] = (tmpl0[2] + l1[2])/2.;
	//---

	float d;
	svVector3 tmp11;
	tmp11[0] = p0[0] - tmpl0[0];
	tmp11[1] = p0[1] - tmpl0[1];
	tmp11[2] = p0[2] - tmpl0[2];
	d = DotProduct(tmp11, n);

	if(DotProduct(l,n)==0)
	{
		r = r;
		return false;
	}

	d = d/DotProduct(l,n);

	svVector3 tmpr = r;

	r[0] = d * l[0];r[1] = d * l[1];r[2] = d * l[2];
	r[0] = r[0] + tmpl0[0];r[1] = r[1] + tmpl0[1];r[2] = r[2] + tmpl0[2];
	//cerr<<r[0]<<" "<<r[1]<<" "<<r[2]<<endl;

	svVector3 tmp1;
	tmp1[0] = l0[0] - r[0];tmp1[1] = l0[1] - r[1];tmp1[2] = l0[2] - r[2];
	svVector3 tmp2;
	tmp2[0] = l1[0] - r[0];tmp2[1] = l1[1] - r[1];tmp2[2] = l1[2] - r[2];

	if(DotProduct(tmp1,tmp2)<0.
	&& r[0]>=lb[0] && r[0]<=rb[0]
	&& r[1]>=lb[1] && r[1]<=rb[1]
	&& r[2]>=lb[2] && r[2]<=rb[2]
	&& !(fabs(r[0]-l0[0])<=0.00001&&fabs(r[1]-l0[1])<=0.00001&&fabs(r[2]-l0[2])<=0.00001)
	&& !(fabs(r[0]-l1[0])<=0.00001&&fabs(r[1]-l1[1])<=0.00001&&fabs(r[2]-l1[2])<=0.00001))
	{//cerr<<r[0]<<" "<<r[1]<<" "<<r[2]<<endl;
		return true;
	}
	else
	{
		r = tmpr;
		return false;
	}
}
void svROIbox::SetDir(svVector3 v)//01, 23 (dirx)
{
	v = normalize(v);
	if(fabs(v[0])<=0.00001&&fabs(v[1])<=0.00001&&fabs(v[2]-1.)<=0.00001)
		{ v[1]=0.005;v[2]=0.99;}

//cerr<<"dir1 "<<v[0]<<" "<<v[1]<<" "<<v[2]<<" "<<diry[0]<<" "<<diry[1]<<" "<<diry[2]<<endl;
	//v[0]=0.995037;v[1]= 0;v[2]= 0.0995037;

	v = normalize(v);//cerr<<"dir1 "<<v[0]<<" "<<v[1]<<" "<<v[2]<<" "<<diry[0]<<" "<<diry[1]<<" "<<diry[2]<<endl;
	svVector3 u;
	svVector3 vz;

	vz[0]=0.;vz[1]=0.;vz[2]=1.;

	u = CrossProduct(v, vz);
	u = normalize(u);
	
	if(DotProduct(u,dirx)<0)
		u = -u;

	float deltal = sqrt((intersectionp[0][0]-intersectionp[1][0])*(intersectionp[0][0]-intersectionp[1][0])
			+ (intersectionp[0][1]-intersectionp[1][1])*(intersectionp[0][1]-intersectionp[1][1]));

	svVector3 p[6];

	//p[0] = intersectionp[1] - u * deltal;
	//p[1] = intersectionp[2] - u * deltal;
	p[0] = Intersection2Lines(u,intersectionp[1],diry,intersectionp[3]);
	p[1] = Intersection2Lines(u,intersectionp[2],diry,intersectionp[3]);
	p[2] = Plane2Plane(v,intersectionp[1],vz,widgetp[3]);
	p[3] = Plane2Plane(v,intersectionp[2],vz,widgetp[3]);
	//cerr<<v[0]<<" "<<v[1]<<" "<<v[2]<<endl;
	//cerr<<p[2][0]<<" "<<p[2][1]<<" "<<p[2][2]<<" "<<intersectionp[1][0]<<" "<<intersectionp[1][1]<<" "<<intersectionp[1][2]<<" "<<endl;
	//cerr<<p[3][0]<<" "<<p[3][1]<<" "<<p[3][2]<<" "<<intersectionp[2][0]<<" "<<intersectionp[2][1]<<" "<<intersectionp[2][2]<<" "<<endl;
	p[4] = Intersection2Lines(u,p[2],diry,intersectionp[6]);//ip5
	p[2] = Intersection2Lines(u,p[2],diry,intersectionp[7]);//ip4
	p[5] = Intersection2Lines(u,p[3],diry,intersectionp[6]);//ip6;
	p[3] = Intersection2Lines(u,p[3],diry,intersectionp[7]);//ip7
	//cerr<<p[2][0]<<" "<<p[2][1]<<" "<<p[2][2]<<endl;
	//cerr<<p[3][0]<<" "<<p[3][1]<<" "<<p[3][2]<<endl;
	//cerr<<"--------------------------------"<<endl;

	bool flag = true;

	/*for(int i=0;i<6;i++)
	{
		if(!(p[i][0]>=lb[0]&&p[i][0]<=rb[0]
		&&p[i][1]>=lb[1]&&p[i][1]<=rb[1]))
		{	flag = false;break;}
	}*/

	if(flag)
	{
		intersectionp[0] = p[0];
		intersectionp[3] = p[1]; 
		intersectionp[4] = p[2];
		intersectionp[7] = p[3]; 
		intersectionp[5] = p[4];
		intersectionp[6] = p[5];
		widgetp[0] = intersectionp[0];
		widgetp[1] = (intersectionp[0] + intersectionp[2])/2.;
		for(int i=4;i<11;i++)
			widgetp[i] = intersectionp[i-3];
		dirx = normalize(intersectionp[1]-intersectionp[0]);
		stop1 = false;
		
	}
	else 
		stop1 = true;

	SetOutBoundary();

}
void svROIbox::SetPoints(svVector3 *ip, svVector3 *w)
{
	for(int i=0;i<8;i++)
	{
		intersectionp[i] = ip[i];
	}
	for(int i=0;i<11;i++)
	{
		widgetp[i] = w[i];
	}
	SetOutBoundary();
}

void svROIbox::SetVerticalDir(svVector3 v)//01, 23 (dirx)
{
	v = normalize(v);
	svVector3 v2;
	v2[0]=0;v2[1]=0;v2[2]=0;
	if(v[0]==1)
		v2[1]=1;
	else
		v2[0]=1;

			svVector3 tmpp[8];
			tmpp[0][0]=lb[0]*3./4.;tmpp[0][1]=lb[1]*3./4.;tmpp[0][2]=lb[2];
			tmpp[1][0]=rb[0]*3./4.;tmpp[1][1]=lb[1]*3./4.;tmpp[1][2]=lb[2];
			tmpp[2][0]=rb[0]*3./4.;tmpp[2][1]=rb[1]*3./4.;tmpp[2][2]=lb[2];
			tmpp[3][0]=lb[0]*3./4.;tmpp[3][1]=rb[1]*3./4.;tmpp[3][2]=lb[2];

			tmpp[4][0]=lb[0]*3./4.;tmpp[4][1]=lb[1]*3./4.;tmpp[4][2]=rb[2];
			tmpp[5][0]=rb[0]*3./4.;tmpp[5][1]=lb[1]*3./4.;tmpp[5][2]=rb[2];
			tmpp[6][0]=rb[0]*3./4.;tmpp[6][1]=rb[1]*3./4.;tmpp[6][2]=rb[2];
			tmpp[7][0]=lb[0]*3./4.;tmpp[7][1]=rb[1]*3./4.;tmpp[7][2]=rb[2];

			if(v[1]==1)
			{
			intersectionp[0] = tmpp[0];
			intersectionp[1] = tmpp[1];
			intersectionp[2] = tmpp[2];
			intersectionp[3] = tmpp[3];
			intersectionp[4] = tmpp[4];
			intersectionp[5] = tmpp[5];
			intersectionp[6] = tmpp[6];
			intersectionp[7] = tmpp[7];
			}
			else
			{
			intersectionp[0] = tmpp[1];
			intersectionp[1] = tmpp[2];
			intersectionp[2] = tmpp[3];
			intersectionp[3] = tmpp[0];
			intersectionp[4] = tmpp[5];
			intersectionp[5] = tmpp[6];
			intersectionp[6] = tmpp[7];
			intersectionp[7] = tmpp[4];
			}

			widgetp[0] = intersectionp[0];
			widgetp[1] = (intersectionp[0] + intersectionp[2])/2.;
			widgetp[2] = lb;
			widgetp[3] = lb;widgetp[3][2]=rb[2];
			
			for(int i=4;i<11;i++)
				widgetp[i] = intersectionp[i-3];
			dirx = normalize(intersectionp[1]-intersectionp[0]);
			diry = normalize(intersectionp[3]-intersectionp[0]);


SetOutBoundary();

}
void svROIbox::SetOutBoundary()
{
	//01, 12, 23, 30, 04, 15, 26, 37, 45, 56, 67, 74

	int array[12][2];
	array[0][0] = 0;
	array[0][1] = 1;
	array[1][0] = 1;
	array[1][1] = 2;
	array[2][0] = 2;
	array[2][1] = 3;
	array[3][0] = 3;
	array[3][1] = 0;

	array[4][0] = 0;
	array[4][1] = 4;
	array[5][0] = 1;
	array[5][1] = 5;
	array[6][0] = 2;
	array[6][1] = 6;
	array[7][0] = 3;
	array[7][1] = 7;

	array[8][0] = 4;
	array[8][1] = 5;
	array[9][0] = 5;
	array[9][1] = 6;
	array[10][0] = 6;
	array[10][1] = 7;
	array[11][0] = 7;
	array[11][1] = 4;

	bool flag[8];
	for(int i=0;i<8;i++)
	{
		intersectionp_out[i] = false;
	}
	for(int i=0;i<12;i++)
	{
		intersection_out_num[i] = 0;
	}
	for(int i=0;i<8;i++)
	{
		if(intersectionp[i][0]<lb[0]
			|| intersectionp[i][0]>rb[0]
			|| intersectionp[i][1]<lb[1]
			|| intersectionp[i][1]>rb[1]
			|| intersectionp[i][2]<lb[2]
			|| intersectionp[i][2]>rb[2])
		{
			intersectionp_out[i] = true;
		}
	}
	int j = 0;
	for(int i=0;i<12;i++)
	{
		j = 0;
		if(intersectionp_out[array[i][0]]==true||
			 intersectionp_out[array[i][1]]==true)
		{
			bool f;	
			svVector3 tmpl;
			tmpl[0] = intersectionp[array[i][1]][0] - intersectionp[array[i][0]][0];
			tmpl[1] = intersectionp[array[i][1]][1] - intersectionp[array[i][0]][1];
			tmpl[2] = intersectionp[array[i][1]][2] - intersectionp[array[i][0]][2];
			svVector3 l = normalize(tmpl);
			//cerr<<l[0]<<" "<<l[1]<<" "<<l[2]<<endl;
			svVector3 n;

			n[0]=0;n[1]=0;n[2]=1;
			f = Line2Plane(svVector3(0.,0.,lb[2]), intersectionp[array[i][0]], intersectionp[array[i][1]],
				 l,n, intersection_out_boundary[i][j]);
			if(f==true)j++;


			n[0]=0;n[1]=1;n[2]=0;
			f = Line2Plane(svVector3(0.,lb[1],0.), intersectionp[array[i][0]], intersectionp[array[i][1]],
				 l,n, intersection_out_boundary[i][j]);
			if(f==true)j++;


			n[0]=1;n[1]=0;n[2]=0;
			f = Line2Plane(svVector3(lb[0],0.,0.), intersectionp[array[i][0]], intersectionp[array[i][1]],
				 l,n, intersection_out_boundary[i][j]);
			if(f==true)j++;

			n[0]=0;n[1]=0;n[2]=1;
			f = Line2Plane(svVector3(0.,0.,rb[2]), intersectionp[array[i][0]], intersectionp[array[i][1]],
				 l,n, intersection_out_boundary[i][j]);
			if(f==true)j++;


			n[0]=0;n[1]=1;n[2]=0;
			f = Line2Plane(svVector3(0.,rb[1],0.), intersectionp[array[i][0]], intersectionp[array[i][1]],
				 l,n, intersection_out_boundary[i][j]);
			if(f==true)j++;


			n[0]=1;n[1]=0;n[2]=0;
			f = Line2Plane(svVector3(rb[0],0.,0.), intersectionp[array[i][0]], intersectionp[array[i][1]],
				 l,n, intersection_out_boundary[i][j]);
			if(f==true)j++;
		}
		intersection_out_num[i] = j;
		if(j==2)
		{
			svVector3 tmp1;
			tmp1[0]= intersection_out_boundary[i][0][0] - intersectionp[array[i][0]][0];
			tmp1[1]= intersection_out_boundary[i][0][1] - intersectionp[array[i][0]][1];
			tmp1[2]= intersection_out_boundary[i][0][2] - intersectionp[array[i][0]][2];
			//tmp1 = normalize(tmp1);
			svVector3 tmp2;
			tmp2[0] = intersection_out_boundary[i][0][0] - intersection_out_boundary[i][1][0];
			tmp2[1] = intersection_out_boundary[i][0][1] - intersection_out_boundary[i][1][1];
			tmp2[2] = intersection_out_boundary[i][0][2] - intersection_out_boundary[i][1][2];
			//tmp2 = normalize(tmp2);
			svVector3 tmp3;
			//tmp3[0]=0;tmp3[1]=0;tmp3[2]=0;
			if(DotProduct(tmp1,tmp2)>0.)
			{
				tmp3 = intersection_out_boundary[i][0];
				intersection_out_boundary[i][0] = intersection_out_boundary[i][1];
				intersection_out_boundary[i][1] = tmp3;
			}
		}
		//cerr<<intersection_out_boundary[i][0][0]<<" "<<intersection_out_boundary[i][0][1]<<" "<<intersection_out_boundary[i][0][2]<<" "<<endl;
		//cerr<<intersection_out_boundary[i][1][0]<<" "<<intersection_out_boundary[i][1][1]<<" "<<intersection_out_boundary[i][1][2]<<" "<<endl;
	}	
	/*for(int i=0;i<12;i++)
	{
		cerr<<intersectionp[array[i][0]][0]<<" "<<intersectionp[array[i][0]][1]<<" "<<intersectionp[array[i][0]][2]<<" "<<intersectionp[array[i][1]][0]<<" "<<intersectionp[array[i][1]][1]<<" "<<intersectionp[array[i][1]][2]<<" "<<endl;
		for(int j=0;j<intersection_out_num[i];j++)
		cerr<<intersection_out_boundary[i][j][0]<<" "<<intersection_out_boundary[i][j][1]<<" "<<intersection_out_boundary[i][j][2]<<" ";
		cerr<<endl;
	}cerr<<"---"<<endl;*/
	/*bool flag[8];
	for(int i=0;i<8;i++)
	{
		flag[i] = false;
	}
	for(int i=0;i<12;i++)
	{
		intersection_out_boundary[i][0] = lb[0]-1;
		intersection_out_boundary[i][1] = lb[1]-1;
		intersection_out_boundary[i][2] = lb[2]-1;
	}
	for(int i=0;i<4;i++)
	{
		if(intersectionp[i][0]<lb[0]
			|| intersectionp[i][0]>rb[0]
			|| intersectionp[i][1]<lb[1]
			|| intersectionp[i][1]>rb[1]
			|| intersectionp[i][2]<lb[2]
			|| intersectionp[i][2]>rb[2])
		{
			flag[i] = true;
		}
		
		if(flag[i])
		{
			int i1,i2,i3;

			i3 = i + 4;
			if(i==0)
				i1 = 3;
			else
				i1 = i - 1;
			if(i==3)
				i2 = 0;
			else
				i2 = i + 1;
		
			//i3
			//i1 intersection_out_boundary[i1]
			for(int j=0;j>3;j++)
			{
				int ii;
				int kk;

				if(j==0){ii = i3;kk=i3;}
				else if(j==1) {ii = i1;kk=i1;}
				else if(j==2) {ii = i2;kk=i;} 

				if(flag[ii]==false)
				{
					if((intersectionp[i][0]-lb[0])*(intersectionp[ii][0]-lb[0])<0)
					{
						intersection_out_boundary[kk][0]=lb[0];
					}
					else if((intersectionp[i][0]-rb[0])*(intersectionp[ii][0]-rb[0])<0)
					{
						intersection_out_boundary[kk][0]=rb[0];
					}

					if((intersectionp[i][1]-lb[1])*(intersectionp[ii][1]-lb[1])<0)
					{
						intersection_out_boundary[kk][1]=lb[1];
					}
					else if((intersectionp[i][1]-rb[1])*(intersectionp[ii][1]-rb[1])<0)
					{
						intersection_out_boundary[kk][1]=rb[1];
					}

					if((intersectionp[i][2]-lb[2])*(intersectionp[ii][2]-lb[2])<0)
					{
						intersection_out_boundary[kk][2]=lb[2];
					}
					else if((intersectionp[i][2]-rb[2])*(intersectionp[ii][2]-rb[2])<0)
					{
						intersection_out_boundary[kk][2]=rb[2];
					}

					if(intersection_out_boundary[kk][0]!=lb[0]-1)
					{
						
					}
				}
			}

			
			
		}
	}*/
}

void svROIbox::SetDir2(svVector3 v)//diry
{
	if(fabs(v[0])<=0.00001&&fabs(v[1])<=0.00001&&fabs(v[2]-1.)<=0.00001)
		{ v[1]=0.005;v[2]=0.99;}

	v = normalize(v);
	svVector3 u;
	svVector3 vz;

	vz[0]=0.;vz[1]=0.;vz[2]=1.;

	u = CrossProduct(v, vz);
	u = normalize(u);
	
	if(DotProduct(u,dirx)<0)
		u = -u;

	float deltal = sqrt((intersectionp[0][0]-intersectionp[1][0])*(intersectionp[0][0]-intersectionp[1][0])
			+ (intersectionp[0][1]-intersectionp[1][1])*(intersectionp[0][1]-intersectionp[1][1]));

	svVector3 p[6];

	p[0] = Intersection2Lines(u,intersectionp[3],dirx,intersectionp[1]);
	p[1] = Intersection2Lines(u,intersectionp[2],dirx,intersectionp[1]);
	p[2] = Plane2Plane(v,intersectionp[3],vz,widgetp[3]);
	p[3] = Plane2Plane(v,intersectionp[2],vz,widgetp[3]);
	p[4] = Intersection2Lines(u,p[2],dirx,intersectionp[5]);//ip4
	p[2] = Intersection2Lines(u,p[2],dirx,intersectionp[6]);//ip7
	p[5] = Intersection2Lines(u,p[3],dirx,intersectionp[5]);//ip5
	p[3] = Intersection2Lines(u,p[3],dirx,intersectionp[6]);//ip6

	bool flag = true;

	/*for(int i=0;i<6;i++)
	{
		if(!(p[i][0]>=lb[0]&&p[i][0]<=rb[0]
		&&p[i][1]>=lb[1]&&p[i][1]<=rb[1]))
		{	flag = false;break;}
	}*/

	if(flag)
	{
		intersectionp[0] = p[0];
		intersectionp[1] = p[1]; 
		intersectionp[4] = p[4];
		intersectionp[7] = p[2]; 
		intersectionp[5] = p[5];
		intersectionp[6] = p[3];
		widgetp[0] = intersectionp[0];
		widgetp[1] = (intersectionp[0] + intersectionp[2])/2.;
		for(int i=4;i<11;i++)
			widgetp[i] = intersectionp[i-3];
		diry = normalize(intersectionp[3]-intersectionp[0]);

		stop2 = false;
	}
	else 
		stop2 = true;

	SetOutBoundary();
}
void svROIbox::Find_Nearest(int x, int y)
{
    int i, j;
    GLdouble td, dd;
    svVector3 p[11];

    //glGetIntegerv (GL_VIEWPORT, viewport);
   // glGetDoublev (GL_MODELVIEW_MATRIX, mvmatrix);
    //glGetDoublev (GL_PROJECTION_MATRIX, projmatrix);

	for(i=0;i<11;i++)
	{
		p[i][0]=0;p[i][1]=0;p[i][2]=0;	
	}

	for(i=0;i<11;i++)
	{
		p[i][0]  = widgetp[i][0] * tb_transform[0][0] +
			    widgetp[i][1] * tb_transform[1][0] +
			    widgetp[i][2] * tb_transform[2][0] +
			    tb_transform[3][0] + p[i][0];
                p[i][1]  = widgetp[i][0] * tb_transform[0][1] +
                            widgetp[i][1] * tb_transform[1][1] +
                            widgetp[i][2] * tb_transform[2][1] +
                            tb_transform[3][1] + p[i][1];
                p[i][2]  = widgetp[i][0] * tb_transform[0][2] +
                            widgetp[i][1] * tb_transform[1][2] +
                            widgetp[i][2] * tb_transform[2][2] +
                            tb_transform[3][2] + p[i][2];
	}
	
	for(i=0;i<11;i++)
	{
		gluProject((GLdouble)p[i][0],
                        (GLdouble)p[i][1],
                        (GLdouble)p[i][2],
                        mvmatrix, projmatrix, viewport,
                        &(wx[i]), &(wy[i]), &(wz[i]));
		wy[i] = (GLdouble)width-wy[i];
	}

        dd=9e+9;

    	ni = 0;
	for(i=0;i<11;i++)
	{
		td = ((GLdouble)x-wx[i]) *  ((GLdouble)x-wx[i]) +
		      ((GLdouble)y-wy[i]) * ((GLdouble)y-wy[i]);
		if(td<dd)
		{
			dd = td;
			ni = i;
		}
	}
	if(dd<=50.)SELECTED=GL_TRUE;
}
bool svROIbox::Find_Near(int x, int y)
{
    int i, j;
    GLdouble td, dd;
    svVector3 p[11];

    //glGetIntegerv (GL_VIEWPORT, viewport);
   // glGetDoublev (GL_MODELVIEW_MATRIX, mvmatrix);
    //glGetDoublev (GL_PROJECTION_MATRIX, projmatrix);

	for(i=0;i<11;i++)
	{
		p[i][0]=0;p[i][1]=0;p[i][2]=0;	
	}

	for(i=0;i<11;i++)
	{
		p[i][0]  = widgetp[i][0] * tb_transform[0][0] +
			    widgetp[i][1] * tb_transform[1][0] +
			    widgetp[i][2] * tb_transform[2][0] +
			    tb_transform[3][0] + p[i][0];
                p[i][1]  = widgetp[i][0] * tb_transform[0][1] +
                            widgetp[i][1] * tb_transform[1][1] +
                            widgetp[i][2] * tb_transform[2][1] +
                            tb_transform[3][1] + p[i][1];
                p[i][2]  = widgetp[i][0] * tb_transform[0][2] +
                            widgetp[i][1] * tb_transform[1][2] +
                            widgetp[i][2] * tb_transform[2][2] +
                            tb_transform[3][2] + p[i][2];
	}
	
	for(i=0;i<11;i++)
	{
		gluProject((GLdouble)p[i][0],
                        (GLdouble)p[i][1],
                        (GLdouble)p[i][2],
                        mvmatrix, projmatrix, viewport,
                        &(wx[i]), &(wy[i]), &(wz[i]));
		wy[i] = (GLdouble)width-wy[i];
	}

        dd=9e+9;

    	ni = 0;
	for(i=0;i<11;i++)
	{
		td = ((GLdouble)x-wx[i]) *  ((GLdouble)x-wx[i]) +
		      ((GLdouble)y-wy[i]) * ((GLdouble)y-wy[i]);
		if(td<dd)
		{
			dd = td;
			ni = i;
		}
	}
	if(dd<=100.)return true;
	else return false;
}
GLboolean svROIbox::invert_matrix( const GLfloat *m, GLfloat *out )
{
/* NB. OpenGL Matrices are COLUMN major. */
#define SWAP_ROWS(a, b) { GLfloat *_tmp = a; (a)=(b); (b)=_tmp; }
#define MAT(m,r,c) (m)[(c)*4+(r)]

 GLfloat wtmp[4][8];
 GLfloat m0, m1, m2, m3, s;
 GLfloat *r0, *r1, *r2, *r3;

 r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];

 r0[0] = MAT(m,0,0), r0[1] = MAT(m,0,1),
 r0[2] = MAT(m,0,2), r0[3] = MAT(m,0,3),
 r0[4] = 1.0, r0[5] = r0[6] = r0[7] = 0.0,

 r1[0] = MAT(m,1,0), r1[1] = MAT(m,1,1),
 r1[2] = MAT(m,1,2), r1[3] = MAT(m,1,3),
 r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0,

 r2[0] = MAT(m,2,0), r2[1] = MAT(m,2,1),
 r2[2] = MAT(m,2,2), r2[3] = MAT(m,2,3),
 r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0,

 r3[0] = MAT(m,3,0), r3[1] = MAT(m,3,1),
 r3[2] = MAT(m,3,2), r3[3] = MAT(m,3,3),
 r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;

 /* choose pivot - or die */
 if (fabs(r3[0])>fabs(r2[0])) SWAP_ROWS(r3, r2);
 if (fabs(r2[0])>fabs(r1[0])) SWAP_ROWS(r2, r1);
 if (fabs(r1[0])>fabs(r0[0])) SWAP_ROWS(r1, r0);
 if (0.0 == r0[0])  return GL_FALSE;

 /* eliminate first variable     */
 m1 = r1[0]/r0[0]; m2 = r2[0]/r0[0]; m3 = r3[0]/r0[0];
 s = r0[1]; r1[1] -= m1 * s; r2[1] -= m2 * s; r3[1] -= m3 * s;
 s = r0[2]; r1[2] -= m1 * s; r2[2] -= m2 * s; r3[2] -= m3 * s;
 s = r0[3]; r1[3] -= m1 * s; r2[3] -= m2 * s; r3[3] -= m3 * s;
 s = r0[4];
 if (s != 0.0) { r1[4] -= m1 * s; r2[4] -= m2 * s; r3[4] -= m3 * s; }
 s = r0[5];
 if (s != 0.0) { r1[5] -= m1 * s; r2[5] -= m2 * s; r3[5] -= m3 * s; }
 s = r0[6];
 if (s != 0.0) { r1[6] -= m1 * s; r2[6] -= m2 * s; r3[6] -= m3 * s; }
 s = r0[7];
 if (s != 0.0) { r1[7] -= m1 * s; r2[7] -= m2 * s; r3[7] -= m3 * s; }

 /* choose pivot - or die */
 if (fabs(r3[1])>fabs(r2[1])) SWAP_ROWS(r3, r2);
 if (fabs(r2[1])>fabs(r1[1])) SWAP_ROWS(r2, r1);
 if (0.0 == r1[1])  return GL_FALSE;

 /* eliminate second variable */
 m2 = r2[1]/r1[1]; m3 = r3[1]/r1[1];
 r2[2] -= m2 * r1[2]; r3[2] -= m3 * r1[2];
 r2[3] -= m2 * r1[3]; r3[3] -= m3 * r1[3];
 s = r1[4]; if (0.0 != s) { r2[4] -= m2 * s; r3[4] -= m3 * s; }
 s = r1[5]; if (0.0 != s) { r2[5] -= m2 * s; r3[5] -= m3 * s; }
 s = r1[6]; if (0.0 != s) { r2[6] -= m2 * s; r3[6] -= m3 * s; }
 s = r1[7]; if (0.0 != s) { r2[7] -= m2 * s; r3[7] -= m3 * s; }

 /* choose pivot - or die */
 if (fabs(r3[2])>fabs(r2[2])) SWAP_ROWS(r3, r2);
 if (0.0 == r2[2])  return GL_FALSE;

 /* eliminate third variable */
 m3 = r3[2]/r2[2];
 r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
 r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6],
 r3[7] -= m3 * r2[7];
 /* last check */
 if (0.0 == r3[3]) return GL_FALSE;

 s = 1.0/r3[3];              /* now back substitute row 3 */
 r3[4] *= s; r3[5] *= s; r3[6] *= s; r3[7] *= s;

 m2 = r2[3];                 /* now back substitute row 2 */
 s  = 1.0/r2[2];
 r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
 r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
 m1 = r1[3];
 r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1,
 r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;
 m0 = r0[3];
 r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0,
 r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;

 m1 = r1[2];                 /* now back substitute row 1 */
 s  = 1.0/r1[1];
 r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1),
 r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);
 m0 = r0[2];
 r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0,
 r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;

 m0 = r0[1];                 /* now back substitute row 0 */
 s  = 1.0/r0[0];
 r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0),
 r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);

 MAT(out,0,0) = r0[4]; MAT(out,0,1) = r0[5],
 MAT(out,0,2) = r0[6]; MAT(out,0,3) = r0[7],
 MAT(out,1,0) = r1[4]; MAT(out,1,1) = r1[5],
 MAT(out,1,2) = r1[6]; MAT(out,1,3) = r1[7],
 MAT(out,2,0) = r2[4]; MAT(out,2,1) = r2[5],
 MAT(out,2,2) = r2[6]; MAT(out,2,3) = r2[7],
 MAT(out,3,0) = r3[4]; MAT(out,3,1) = r3[5],
 MAT(out,3,2) = r3[6]; MAT(out,3,3) = r3[7];

 return GL_TRUE;

#undef MAT
#undef SWAP_ROWS
}
}
