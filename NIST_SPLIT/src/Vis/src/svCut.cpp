#include "svCut.h"

#include <cmath>


#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

namespace __svl_lib {

svCut::svCut(svVector3Array *pos, svVector4Array *col)
{
	int samplesize = pos[0].size();

	wholepos = new svVector3Array[2];
	wholecol = new svVector4Array[1];

	for(int i=0;i<samplesize;i++)
	{
		wholepos[0].add(pos[0][i]);
		wholepos[1].add(pos[1][i]);
		if(col!=NULL)
			wholecol[0].add(col[0][i]);
		else
		{
			svVector4 c;
			c[0]=1;c[1]=1;c[2]=1;c[3]=1;
			wholecol[0].add(c);
		}
	}

	projectionpos = new svVector3Array[2];
	projectioncol = new svVector4Array[1];
	projectionlable = new svInt[samplesize];

	for(int i=0;i<samplesize;i++)
	{
		projectionpos[0].add(wholepos[0][i]);
		projectionpos[1].add(wholepos[1][i]);
		projectioncol[0].add(wholecol[0][i]);
		projectionlable[i] = 0;
	}

	Frequency = 1;
	ONE = false;
	OTHER = false;
}

void svCut::SetCut(svVector3Array *pos, svVector4Array *col)
{
	cleanup();

	int samplesize = pos[0].size();	
	projectionnum = samplesize;
	//cerr<<samplesize<<" "<<col[0].size()<<endl;

	wholepos = new svVector3Array[2];
	wholecol = new svVector4Array[1];

	for(int i=0;i<samplesize;i++)
	{
		wholepos[0].add(pos[0][i]);
		wholepos[1].add(pos[1][i]);
		if(col!=NULL)
			wholecol[0].add(col[0][i]);
		else
		{
			svVector4 c;
			c[0]=1;c[1]=1;c[2]=1;c[3]=1;
			wholecol[0].add(c);
		}
	}

	projectionpos = new svVector3Array[2];
	projectioncol = new svVector4Array[1];
	projectionlable = new svInt[samplesize];

	for(int i=0;i<samplesize;i++)
	{
		projectionpos[0].add(wholepos[0][i]);
		projectionpos[1].add(wholepos[1][i]);
		projectioncol[0].add(wholecol[0][i]);
		projectionlable[i] = 0;
	}
	Frequency = 1;
	//cerr<<samplesize<<endl;

}

void svCut::SetColors(svVector4 *col)
{
	if(projectioncol!=NULL)
	{
		for(int i=0;i<1;i++)
		{
			projectioncol[i].free();
		}
		delete [] projectioncol;
		projectioncol = NULL;
	}

	projectioncol = new svVector4Array[1];
	
	for(int i=0;i<projectionpos[0].size();i++)
	{
		if(col[i][0]>1.)
		{
			col[i][0]=col[i][0]/255.;
			col[i][1]=col[i][1]/255.;
			col[i][2]=col[i][2]/255.;
		}
		projectioncol[0].add(col[i]);
	}
}


void svCut::SetCutRegion(svVector3 l, svVector3 r, int lable)
{
	int samplesize = wholepos[0].size();
	projectionnum=0;
	int j = 0;
	for(int i=0;i<samplesize;i++)
	{
		if(wholepos[0][i][0]>=l[0]&&wholepos[0][i][0]<=r[0]&&wholepos[0][i][1]>=l[1]&&wholepos[0][i][1]<=r[1]&&wholepos[0][i][2]>=l[2]&&wholepos[0][i][2]<=r[2])
		{
			projectionpos[0][j] = wholepos[0][i];
			projectionpos[1][j] = wholepos[1][i];
			projectioncol[0][j] = wholecol[0][i];
			if(i==lable)
			{
				if(projectionlable[j]==0)
					projectionlable[j] = 1;
				else
					projectionlable[j] = 0;
			}
			j++;
			projectionnum++;
		}
	}
planexy = l[2];planexy = planexy - 3;
planexz = l[1];planexz = planexz - 3;
planeyz = l[0];planeyz = planeyz - 3;

	lbbox = l;
	rbbox = r;
	//cerr<<"l: "<<l[0]<<" "<<l[1]<<" "<<l[2]<<endl;
	//cerr<<"r: "<<r[0]<<" "<<r[1]<<" "<<r[2]<<endl;
}

void svCut::SetNBlockView(GLfloat *m, int num, svVector3 l, svVector3 r)
{
	int i,j;

	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			tb_transform[i][j] = m[j+i*4];

	svVector3 p1[4];
	svVector3 p2[4];

	for(i=0;i<4;i++)
	{p1[i][0]=0;p1[i][1]=0;p1[i][2]=0;}

	p2[0][0]=l[0];p2[0][1]=r[1];p2[0][2]=l[2];
	p2[1][0]=r[0];p2[1][1]=l[1];p2[1][2]=l[2];
	p2[2][0]=l[0];p2[2][1]=l[1];p2[2][2]=r[2];
	p2[3][0]=l[0];p2[3][1]=l[1];p2[3][2]=l[2];

	for(i=0;i<4;i++)
	{
		p1[i][0] = p2[i][0] * tb_transform[0][0] +
			p2[i][1] * tb_transform[1][0] +
			p2[i][2] * tb_transform[2][0] +
			tb_transform[3][0] + p1[i][0];
		p1[i][1] = p2[i][0] * tb_transform[0][1] +
			p2[i][1] * tb_transform[1][1] +
			p2[i][2] * tb_transform[2][1] +
			tb_transform[3][1] + p1[i][1];
		p1[i][2] = p2[i][0] * tb_transform[0][2] +
			p2[i][1] * tb_transform[1][2] +
			p2[i][2] * tb_transform[2][2] +
			tb_transform[3][2] + p1[i][2];
	}
	
	svVector3 A[3];
	svVector3 B[3];
	A[0] = p1[0] - p1[3];
	B[0] = p1[1] - p1[3];
	A[1] = p1[1] - p1[3];
	B[1] = p1[2] - p1[3];
	A[2] = p1[2] - p1[3];
	B[2] = p1[0] - p1[3];

	float z;

	z = A[0][0]*B[0][1] - A[0][1]*B[0][0]; 
	if(z>0) planexy = r[2];
	else planexy = l[2];

	z = A[1][0]*B[1][1] - A[1][1]*B[1][0]; 
	if(z>0) planexz = r[1];
	else planexz = l[1];

	z = A[2][0]*B[2][1] - A[2][1]*B[2][0]; 
	if(z>0) planeyz = r[0];
	else planeyz = l[0];	

	lbbox = l;
	rbbox = r;
}

void svCut::RenderCutting(float z, bool *inbox)
{
	glEnable(GL_BLEND);
	int samplesize = projectionnum;
	svVector3 arrow1, arrow2;
	float x3,y3,z3;
	float x1,y1,z1;
	float adj;
	
	int tmp;
	bool flagone;
	bool flagother;

/*	if(planexy>rb[2])
		planexy = planexy + 3.;
	else
		planexy = planexy - 3.;
	if(planexz>rb[1])
		planexz = planexz + 3.;
	else
		planexz = planexz - 3.;
	if(planeyz>rb[0])
		planeyz = planeyz + 3.;
	else
		planeyz = planeyz - 3.;*/
	//cerr<<samplesize<<endl;
	glPointSize(2.5);

	float alpha;
	//cerr<<samplesize<<endl;

	for(int i =0;i<samplesize;)
	{
		if(projectioncol[0][i][0]>=0)
		{
		
			//if(projectionlable[i])glColor4f(1,1,0,1);
			//else		
			if(inbox[i])	
				alpha = 1;
			else
				alpha = 0.1;		

			glColor4f(projectioncol[0][i][0],projectioncol[0][i][1],projectioncol[0][i][2],alpha);
			glBegin(GL_LINES);
		        glColor4f(0.5,0.5,0.5, alpha);
			glVertex3f(projectionpos[0][i][0],projectionpos[0][i][1],planexy);
			glColor4f(projectioncol[0][i][0],projectioncol[0][i][1],projectioncol[0][i][2],alpha);
			glVertex3f(projectionpos[1][i][0],projectionpos[1][i][1],planexy);
			glEnd();

			glBegin(GL_LINES);
		        glColor4f(0.5,0.5,0.5, alpha);
			glVertex3f(projectionpos[0][i][0],planexz,projectionpos[0][i][2]);
		        glColor4f(projectioncol[0][i][0],projectioncol[0][i][1],projectioncol[0][i][2],alpha);
			glVertex3f(projectionpos[1][i][0],planexz,projectionpos[1][i][2]);
			glEnd();

			glBegin(GL_LINES);
		        glColor4f(0.5,0.5,0.5, alpha);
			glVertex3f(planeyz,projectionpos[0][i][1],projectionpos[0][i][2]);
		        glColor4f(projectioncol[0][i][0],projectioncol[0][i][1],projectioncol[0][i][2],alpha);
			glVertex3f(planeyz,projectionpos[1][i][1],projectionpos[1][i][2]);
			glEnd();

			glBegin(GL_POINTS);
			glVertex3f(projectionpos[1][i][0],projectionpos[1][i][1],planexy);
			glEnd();
			glBegin(GL_POINTS);
			glVertex3f(projectionpos[1][i][0],planexz,projectionpos[1][i][2]);
			glEnd();
			glBegin(GL_POINTS);
			glVertex3f(planeyz,projectionpos[1][i][1],projectionpos[1][i][2]);
			glEnd();
		}

		i = i + Frequency;
	}
	glPointSize(1);
	/*
	glPointSize(2.5);
	for(int i =0;i<samplesize;)
	{

		if(projectionlable[i])glColor4f(1,1,0,1);
		else		glColor4f(projectioncol[0][i][0],projectioncol[0][i][1],projectioncol[0][i][2],projectioncol[0][i][3]);

			glBegin(GL_POINTS);
			glVertex3f(projectionpos[1][i][0],projectionpos[1][i][1],planexy);
			glEnd();			
		i = i + Frequency;
	}
	glPointSize(1);

	for(int i =0;i<samplesize;)
	{
		if(projectionlable[i])glColor4f(1,1,0,1);
		else		glColor4f(projectioncol[0][i][0],projectioncol[0][i][1],projectioncol[0][i][2],projectioncol[0][i][3]);
		glBegin(GL_LINES);
                glColor3f((float)58/255, (float)58/255, (float)58/255);
		glVertex3f(projectionpos[0][i][0],planexz,projectionpos[0][i][2]);
                glColor4f(projectioncol[0][i][0],projectioncol[0][i][1],projectioncol[0][i][2],projectioncol[0][i][3]);
		glVertex3f(projectionpos[1][i][0],planexz,projectionpos[1][i][2]);
		glEnd();
		i = i + Frequency;
	}
	glPointSize(2.5);
	for(int i =0;i<samplesize;)
	{
		if(projectionlable[i])glColor4f(1,1,0,1);
		else		glColor4f(projectioncol[0][i][0],projectioncol[0][i][1],projectioncol[0][i][2],projectioncol[0][i][3]);

			glBegin(GL_POINTS);
			glVertex3f(projectionpos[1][i][0],planexz,projectionpos[1][i][2]);
			glEnd();
		i = i + Frequency;
	}
	glPointSize(1);

	for(int i =0;i<samplesize;)
	{
		if(projectionlable[i])glColor4f(1,1,0,1);
		else		glColor4f(projectioncol[0][i][0],projectioncol[0][i][1],projectioncol[0][i][2],projectioncol[0][i][3]);
		glBegin(GL_LINES);
                glColor3f((float)58/255, (float)58/255, (float)58/255);
		glVertex3f(planeyz,projectionpos[0][i][1],projectionpos[0][i][2]);
                glColor4f(projectioncol[0][i][0],projectioncol[0][i][1],projectioncol[0][i][2],projectioncol[0][i][3]);
		glVertex3f(planeyz,projectionpos[1][i][1],projectionpos[1][i][2]);
		glEnd();
		i = i + Frequency;
	}
	glPointSize(2.5);
	for(int i =0;i<samplesize;)
	{
		if(projectionlable[i])glColor4f(1,1,0,1);
		else		glColor4f(projectioncol[0][i][0],projectioncol[0][i][1],projectioncol[0][i][2],projectioncol[0][i][3]);


			glBegin(GL_POINTS);
			glVertex3f(planeyz,projectionpos[1][i][1],projectionpos[1][i][2]);
			glEnd();
		i = i + Frequency;
	}
	glPointSize(1);
*/

	glColor4f(1,1,1,0.1);
	glBegin(GL_LINE_LOOP);
	glVertex3f(lbbox[0],lbbox[1],planexy);
        glVertex3f(rbbox[0],lbbox[1],planexy);
        glVertex3f(rbbox[0],rbbox[1],planexy);
        glVertex3f(lbbox[0],rbbox[1],planexy);
	glEnd();
	/*glColor4f((float)58.4711/255,(float)58.4711/255,(float)58.4711/255,0.5);
	glBegin(GL_QUADS);
	glVertex3f(lbbox[0],lbbox[1],planexy);
        glVertex3f(rbbox[0],lbbox[1],planexy);
        glVertex3f(rbbox[0],rbbox[1],planexy);
        glVertex3f(lbbox[0],rbbox[1],planexy);
	glEnd();*/

	glColor4f(1,1,1,0.1);
	glBegin(GL_LINE_LOOP);
	glVertex3f(lbbox[0],planexz,lbbox[2]);
        glVertex3f(rbbox[0],planexz,lbbox[2]);
        glVertex3f(rbbox[0],planexz,rbbox[2]);
        glVertex3f(lbbox[0],planexz,rbbox[2]);
	glEnd();
	/*glColor4f((float)58.4711/255,(float)58.4711/255,(float)58.4711/255,0.5);
	glBegin(GL_QUADS);
	glVertex3f(lbbox[0],planexz,lbbox[2]);
        glVertex3f(rbbox[0],planexz,lbbox[2]);
        glVertex3f(rbbox[0],planexz,rbbox[2]);
        glVertex3f(lbbox[0],planexz,rbbox[2]);
	glEnd();*/

	glColor4f(1,1,1,0.1);
	glBegin(GL_LINE_LOOP);
	glVertex3f(planeyz,lbbox[1],lbbox[2]);
        glVertex3f(planeyz,rbbox[1],lbbox[2]);
        glVertex3f(planeyz,rbbox[1],rbbox[2]);
        glVertex3f(planeyz,lbbox[1],rbbox[2]);
	glEnd();

	glColor3f(254./255.,178./255.,76./255.);
	glBegin(GL_LINES);
	glVertex3f(lbbox[0],planexz,z);
        glVertex3f(rbbox[0],planexz,z);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(planeyz,lbbox[1],z);
        glVertex3f(planeyz,rbbox[1],z);
	glEnd();
	/*glColor4f((float)58.4711/255,(float)58.4711/255,(float)58.4711/255,0.5);
	glBegin(GL_QUADS);
	glVertex3f(planeyz,lbbox[1],lbbox[2]);
        glVertex3f(planeyz,rbbox[1],lbbox[2]);
        glVertex3f(planeyz,rbbox[1],rbbox[2]);
        glVertex3f(planeyz,lbbox[1],rbbox[2]);
	glEnd();*/
	//glDisable(GL_BLEND);
}

void svCut::cleanup()
{
	projectionnum = 0;

	if(projectionpos!=NULL)
	{
		for(int i=0;i<2;i++)
		{
			projectionpos[i].free();
		}
		delete [] projectionpos;
		projectionpos = NULL;
	}
	if(projectioncol!=NULL)
	{
		for(int i=0;i<1;i++)
		{
			projectioncol[i].free();
		}
		delete [] projectioncol;
		projectioncol = NULL;
	}

	int wholesize = wholepos[0].size();
	if(wholepos!=NULL)
	{
		for(int i=0;i<2;i++)
		{
			wholepos[i].free();
		}
		delete [] wholepos;
		wholepos = NULL;
	}
	if(wholecol!=NULL)
	{
		for(int i=0;i<1;i++)
		{
			wholecol[i].free();
		}
		delete [] wholecol;
		wholecol = NULL;
	}
}


}
