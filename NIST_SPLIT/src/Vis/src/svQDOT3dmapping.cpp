
/** 
 * Author: Jian Chen
 * Date: Nov 30 2013
 * Description: glyphs
 *   calculate the angle of those vectors and map them
*/


#ifdef __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>     
#include <stdlib.h>    
#include <time.h>       
#include <cmath>

#include "svQDOT3dmapping.h"

#define SWAP(T, a, b)   { T t; t=a; a=b; b=t;  }

#define SV_PI	  (3.14159265358979323846)
#define SV_2_PI   (3.14159265358979323846 * 2.0)
#define SV_SMALL  (1e-6)

#define svToDegree(x)             ((x)*(180.)/SV_PI)
#define svToRadian(x)	           ((x)*SV_PI/(180.))
#define svMax(a,b)                (((a) > (b)) ? (a) : (b))
#define svMin(a,b)                (((a) < (b)) ? (a) : (b))
#define svClamp(v, min_v, max_v)  { v = svMax(svMin(v, max_v), min_v); }

	
template <class T> inline
  void svSwap(T & a, T & b)
{  T t = a; a = b; b = t;  }

svVector3 svGetPerpendicularVector(const svVector3 & v)
{
	svVector3 r;
	if (fabs(v[2]) < 0.1){
		r[0] = v[1];
		r[1] = -v[0];
	}
	else if (fabs(v[1]) < 0.1){
		r[0] = v[2];
		r[2] = -v[0];
	}
	else{
		r[1] = v[2];
		r[2] = -v[1];
	}

	return normalize(cross(v, r));
}

svVector3 svGetRotatePoint(const svVector3& start, const svVector3& org, 
			const svVector3& axis, svScalar rotate_degree)
{
   svVector3 p, q, r;
   svScalar theta = svToRadian(rotate_degree);
   svScalar costheta,sintheta;

   // calculate the roatation axis
   r = normalize(axis);
   p = start - org;

   //cerr << "r: "; r.print();
   //cerr << "p: "; p.print();

   costheta = svScalar(cos(theta));
   sintheta = svScalar(sin(theta));

   q[0] += (costheta + (1 - costheta) * r[0] * r[0]) * p[0];
   q[0] += ((1 - costheta) * r[0] * r[1] - r[2] * sintheta) * p[1];
   q[0] += ((1 - costheta) * r[0] * r[2] + r[1] * sintheta) * p[2];

   q[1] += ((1 - costheta) * r[0] * r[1] + r[2] * sintheta) * p[0];
   q[1] += (costheta + (1 - costheta) * r[1] * r[1]) * p[1];
   q[1] += ((1 - costheta) * r[1] * r[2] - r[0] * sintheta) * p[2];

   q[2] += ((1 - costheta) * r[0] * r[2] - r[1] * sintheta) * p[0];
   q[2] += ((1 - costheta) * r[1] * r[2] + r[0] * sintheta) * p[1];
   q[2] += (costheta + (1 - costheta) * r[2] * r[2]) * p[2];
   //cerr << "q: "; q.print();

   q += org;
   //cerr << "org: "; org.print();
   //cerr << "q: "; q.print();
   return q;
}

// get the normal vector
//      v1-------------v0
//                    /
//                   /
//                  / v2
//
svVector3 svGetNorm(const svVector3& v1, const svVector3& v0,
                    const svVector3& v2)
{
  return normalize(cross((v1-v0), (v2-v0)));
}

svVector3 svAverage(const svVector3& v1, const svVector3& v2)
{
  return normalize(v1+v2);
}


void svQDOT3dmapping::SetData(svVector3Array *pos, svVector3Array *dir, 
		svScalarArray *den, svScalarArray *vel,
		svScalarArray *exp, svScalarArray *coe,int num,
		svVector3 l, svVector3 r)
{
	if(dataPos!=NULL)
	{	cleanup();}
	
	minExp = 9e+9;
	maxExp = -9e+9;

	NUM_SEG=10;

	dataPos = new svVector3Array[1];
	dataEnd = new svVector3Array[1];
	dataDir = new svVector3[Array[1];
	dataDen = new svScalarArray[1];
	dataEnd2 = new svVector3Array[1];
	dataPoint = new svVector3Array[1];
	dataExp = new svScalarArray[1];
	dataCoe = new svScalarArray[1];
	dataCol = new svVector3Array[1];
	dataVel = new svScalarArray[1];


	dataSize = 0;
	for(int i=0;i<num;i++)
	{
		for(int j=0;j<pos[i].size();j++)
		{
			dataPos[i].add(pos[i][j]);
			dataDir[i].add(dir[i][j]);
			dataDen[i].add(den[i][j]);
			dataVel[i].add(vel[i][j]);
			dataExp[i].add(exp[i][j]);
			dataCoe[i].add(coe[i][j]);
			dataCol[i].add(pos[i][j]);//tmp
			dataEnd[i].add(pos[i][j]);//tmp
			dataEnd2[i].add(pos[i][j]);//tmp
			dataPoint[i].add(pos[i][j]);//tmp

			if(dataExp[i][j]>maxExp)maxExp = dataExp[i][j];
			if(dataExp[i][j]<minExp)minExp = dataExp[i][j];
			
			dataSize ++;
		}
		
	}

	lbbox= l;
	rbbox= r;
	zlayer = num; //zlayer 

	glyphFrequency = 1;
	//cerr<<minExp<<" "<<maxExp<<endl;

	minl_distribute = 0.05/2.;
	minr_splitvector = 0.05/SV_PI;

	arrow_radius = 0.05/2.;
	arrow_length = 0.2;
}

void svQDOT3dmapping::SetEndPos()
{

	svScalar scale;

	for(int i=0;i<zlayer;i++)
	{
		for(int j=0;j<dataPos[i].size();j++)
		{
			scale = dataCoe[i][j]*minl_distribute;

			dataEnd[i][j] = dataPos[i][j] + scale*dataDir[i][j];
			dataEnd2[i][j] = dataPos[i][j] + dataExp[i][j]*minl_distribute*dataDir[i][j];
			dataPoint[i][j] = dataEnd[i][j] + arrow_length*dataDir[i][j];
		}
	}
}


void svQDOT3dmapping::SetColors(svVector3Array *c)
{
	if(c==NULL)
	{
		for(int i=0;i<zlayer;i++)
		{
			for(int j=0;j<dataPos[i].size();j++)
			{
				dataCol[i][j][0] = 0.5;
				dataCol[i][j][1] = 0.5;
				dataCol[i][j][2] = 0.5;
			}
		}
	}
	else
	{	
		for(int i=0;i<zlayer;i++)
		{
			for(int j=0;j<dataPos[i].size();j++)
			{
				dataCol[i][j][0] = c[i][j][0];
				dataCol[i][j][1] = c[i][j][1];
				dataCol[i][j][2] = c[i][j][2];
			}
		}
	}
}

void svQDOT3dmapping::RenderCylinderLengthLength()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	glEnable( GL_LINE_SMOOTH );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glDisable(GL_TEXTURE_2D); 

	float expr;
	float coer;
	float exeh;
	float coeh;

	svVector3 segment_dir,norm,head,o,linestart;
	int NUM = 15;
	svVector3 p1[NUM];
	svVector3 n1[NUM];
	svVector3 p2[NUM];
	svVector3 n2[NUM];

	float length;

        for(int ii =0;ii<zlayer;ii++)
        {
		for(int j=0;j<dataPos[i].size();j++)
		{
			if(dataCoe[ii][j]!=0)
			{
				exeh = dataExp[ii][j] * minl_distribute;
				coer = dataVel[ii][j] * minr_splitvector;
				exer = coer * 1.7;

				//reference lines
					glDisable(GL_LIGHTING);
					glDisable(GL_LIGHT0);
	
					glLineWidth(1.);
					glColor4f(0.3,0.3,0.3,1.);
					glBegin(GL_LINES);
					glVertex3f(dataPos[ii][0],dataPos[ii][1],dataPos[ii][2]);
					glVertex3f(dataPos[ii][0]+5.*minl_distribute*dataDir[ii][0],
						dataPos[ii][1]+5.*minl_distribute*dataDir[ii][1],
						dataPos[ii][2]+5.*minl_distribute*dataDir[ii][2]);
					glEnd();

				//cylinders
				glEnable(GL_LIGHTING);
				glEnable(GL_LIGHT0);

				segment_dir = normalize(dataDir[ii]);
				norm = svGetPerpendicularVector(segment_dir);
				head = dataPos[ii][j] + coer * norm;
				o = dataPos[ii][j];
				for(int i=0;i<NUM;i++)
				{
					p1[i]=svGetRotatePoint(head, o, segment_dir, (float)i * 360./(NUM-1));
					n1[i]=p1[i]-o;
					n1[i]=normalize(n1[i]);
				}			

				o = dataEnd[ii][j];
				head = o + coer * norm;
				for(int i=0;i<NUM;i++)
				{
					p2[i]=svGetRotatePoint(head, o, segment_dir, (float)i * 360./(NUM-1));
					n2[i] = p2[i] - o;
					n2[i]=normalize(n2[i]);
				}
				glColor4f(dataCol[ii][j][0],dataCol[ii][j][1],dataCol[ii][j][2],0.5);
				for(int i=0;i<NUM-1;i++)
				{
					glBegin(GL_QUADS);
					glNormal3f(n1[i][0],n1[i][1],n1[i][2]);
					glVertex3f(p1[i][0],p1[i][1],p1[i][2]);
					glNormal3f(n1[i+1][0],n1[i+1][1],n1[i+1][2]);
					glVertex3f(p1[i+1][0],p1[i+1][1],p1[i+1][2]);
					glNormal3f(n2[i+1][0],n2[i+1][1],n2[i+1][2]);
					glVertex3f(p2[i+1][0],p2[i+1][1],p2[i+1][2]);		
					glNormal3f(n2[i][0],n2[i][1],n2[i][2]);
					glVertex3f(p2[i][0],p2[i][1],p2[i][2]);			
					glEnd();
				}

				for(int i=0;i<NUM-1;i++)
				{
					glNormal3f(segment_dir[0],segment_dir[1],segment_dir[2]);
					glBegin(GL_TRIANGLES);
					glVertex3f(p2[i][0],p2[i][1],p2[i][2]);
					glVertex3f(p2[i+1][0],p2[i+1][1],p2[i+1][2]);		
					glVertex3f(o[0],o[1],o[2]);	
					glEnd();
				}	

				segment_dir = normalize(dataDir[ii][j]);
				norm = svGetPerpendicularVector(segment_dir);
				head = dataPos[ii][j] + expr * norm;
				o = dataPos[ii][j];
				for(int i=0;i<NUM;i++)
				{
					p1[i]=svGetRotatePoint(head, o, segment_dir, (float)i * 360./(NUM-1));
					n1[i]=p1[i]-o;
					n1[i]=normalize(n1[i]);
				}
				for(int i=0;i<NUM-1;i++)
				{
					glNormal3f(-segment_dir[0],-segment_dir[1],-segment_dir[2]);
					glBegin(GL_TRIANGLES);
					glVertex3f(p1[i][0],p1[i][1],p1[i][2]);
					glVertex3f(p1[i+1][0],p1[i+1][1],p1[i+1][2]);		
					glVertex3f(o[0],o[1],o[2]);	
					glEnd();
				}				
				o = dataEnd2[ii][j];
				head = o + expr * norm;
				for(int i=0;i<NUM;i++)
				{
					p2[i]=svGetRotatePoint(head, o, segment_dir, (float)i * 360./(NUM-1));
					n2[i] = p2[i] - o;
					n2[i]=normalize(n2[i]);
				}
				glColor4f(dataCol[ii][j][0],dataCol[ii][j][1],dataCol[ii][j][2],0.5);
				for(int i=0;i<NUM-1;i++)
				{
					glBegin(GL_QUADS);
					glNormal3f(n1[i][0],n1[i][1],n1[i][2]);
					glVertex3f(p1[i][0],p1[i][1],p1[i][2]);
					glNormal3f(n1[i+1][0],n1[i+1][1],n1[i+1][2]);
					glVertex3f(p1[i+1][0],p1[i+1][1],p1[i+1][2]);
					glNormal3f(n2[i+1][0],n2[i+1][1],n2[i+1][2]);
					glVertex3f(p2[i+1][0],p2[i+1][1],p2[i+1][2]);		
					glNormal3f(n2[i][0],n2[i][1],n2[i][2]);
					glVertex3f(p2[i][0],p2[i][1],p2[i][2]);			
					glEnd();
				}
				if(dataExp[ii][j]>dataCoe[ii][j])
				{
					for(int i=0;i<NUM-1;i++)
					{
						glNormal3f(segment_dir[0],segment_dir[1],segment_dir[2]);
						glBegin(GL_TRIANGLES);
						glVertex3f(p2[i][0],p2[i][1],p2[i][2]);
						glVertex3f(p2[i+1][0],p2[i+1][1],p2[i+1][2]);		
						glVertex3f(o[0],o[1],o[2]);	
						glEnd();
					}			
				}
				else
				{
					head = dataEnd2[ii][j] + coer * norm;
					for(int i=0;i<NUM;i++)
					{
						p1[i]=svGetRotatePoint(head, o, segment_dir, (float)i * 360./(NUM-1));
						n1[i]=p1[i]-o;
						n1[i]=normalize(n1[i]);
					}	
					for(int i=0;i<NUM-1;i++)//clockwise????
					{
						glNormal3f(segment_dir[0],segment_dir[1],segment_dir[2]);
						glBegin(GL_QUADS);
						glVertex3f(p1[i][0],p1[i][1],p1[i][2]);
						glVertex3f(p1[i+1][0],p1[i+1][1],p1[i+1][2]);
						glVertex3f(p2[i+1][0],p2[i+1][1],p2[i+1][2]);		
						glVertex3f(p2[i][0],p2[i][1],p2[i][2]);			
						glEnd();
					}
				}
				for(int i=0;i<NUM;i++)
				{
					p1[i] = p2[i];n1[i]=n2[i];
				}
			}
		}
        }
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        glDisable(GL_BLEND);
	glLineWidth(1.);
}
void svQDOT3dmapping::RenderCylinderLengthColor()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	glEnable( GL_LINE_SMOOTH );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glDisable(GL_TEXTURE_2D); 

	float expr;
	float coer;
	float exeh;
	float coeh;

	svVector3 segment_dir,norm,head,o,linestart;
	int NUM = 15;
	svVector3 p1[NUM];
	svVector3 n1[NUM];
	svVector3 p2[NUM];
	svVector3 n2[NUM];

	float length;

        for(int ii =0;ii<zlayer;ii++)
        {
		for(int j=0;j<dataPos[i].size();j++)
		{
			if(dataCoe[ii][j]!=0)
			{
				exeh = dataExp[ii][j] * minl_distribute;
				coer = minl_distribute;
				exer = coer * 1.7;

				//reference lines
					glDisable(GL_LIGHTING);
					glDisable(GL_LIGHT0);
	
					glLineWidth(1.);
					glColor4f(0.3,0.3,0.3,1.);
					glBegin(GL_LINES);
					glVertex3f(dataPos[ii][0],dataPos[ii][1],dataPos[ii][2]);
					glVertex3f(dataPos[ii][0]+5.*minl_distribute*dataDir[ii][0],
						dataPos[ii][1]+5.*minl_distribute*dataDir[ii][1],
						dataPos[ii][2]+5.*minl_distribute*dataDir[ii][2]);
					glEnd();

				//cylinders
				glEnable(GL_LIGHTING);
				glEnable(GL_LIGHT0);

				segment_dir = normalize(dataDir[ii]);
				norm = svGetPerpendicularVector(segment_dir);
				head = dataPos[ii][j] + coer * norm;
				o = dataPos[ii][j];
				for(int i=0;i<NUM;i++)
				{
					p1[i]=svGetRotatePoint(head, o, segment_dir, (float)i * 360./(NUM-1));
					n1[i]=p1[i]-o;
					n1[i]=normalize(n1[i]);
				}			

				o = dataEnd[ii][j];
				head = o + coer * norm;
				for(int i=0;i<NUM;i++)
				{
					p2[i]=svGetRotatePoint(head, o, segment_dir, (float)i * 360./(NUM-1));
					n2[i] = p2[i] - o;
					n2[i]=normalize(n2[i]);
				}
				glColor4f(dataCol[ii][j][0],dataCol[ii][j][1],dataCol[ii][j][2],0.5);
				for(int i=0;i<NUM-1;i++)
				{
					glBegin(GL_QUADS);
					glNormal3f(n1[i][0],n1[i][1],n1[i][2]);
					glVertex3f(p1[i][0],p1[i][1],p1[i][2]);
					glNormal3f(n1[i+1][0],n1[i+1][1],n1[i+1][2]);
					glVertex3f(p1[i+1][0],p1[i+1][1],p1[i+1][2]);
					glNormal3f(n2[i+1][0],n2[i+1][1],n2[i+1][2]);
					glVertex3f(p2[i+1][0],p2[i+1][1],p2[i+1][2]);		
					glNormal3f(n2[i][0],n2[i][1],n2[i][2]);
					glVertex3f(p2[i][0],p2[i][1],p2[i][2]);			
					glEnd();
				}

				for(int i=0;i<NUM-1;i++)
				{
					glNormal3f(segment_dir[0],segment_dir[1],segment_dir[2]);
					glBegin(GL_TRIANGLES);
					glVertex3f(p2[i][0],p2[i][1],p2[i][2]);
					glVertex3f(p2[i+1][0],p2[i+1][1],p2[i+1][2]);		
					glVertex3f(o[0],o[1],o[2]);	
					glEnd();
				}	

				segment_dir = normalize(dataDir[ii][j]);
				norm = svGetPerpendicularVector(segment_dir);
				head = dataPos[ii][j] + expr * norm;
				o = dataPos[ii][j];
				for(int i=0;i<NUM;i++)
				{
					p1[i]=svGetRotatePoint(head, o, segment_dir, (float)i * 360./(NUM-1));
					n1[i]=p1[i]-o;
					n1[i]=normalize(n1[i]);
				}
				for(int i=0;i<NUM-1;i++)
				{
					glNormal3f(-segment_dir[0],-segment_dir[1],-segment_dir[2]);
					glBegin(GL_TRIANGLES);
					glVertex3f(p1[i][0],p1[i][1],p1[i][2]);
					glVertex3f(p1[i+1][0],p1[i+1][1],p1[i+1][2]);		
					glVertex3f(o[0],o[1],o[2]);	
					glEnd();
				}				
				o = dataEnd2[ii][j];
				head = o + expr * norm;
				for(int i=0;i<NUM;i++)
				{
					p2[i]=svGetRotatePoint(head, o, segment_dir, (float)i * 360./(NUM-1));
					n2[i] = p2[i] - o;
					n2[i]=normalize(n2[i]);
				}
				glColor4f(dataCol[ii][j][0],dataCol[ii][j][1],dataCol[ii][j][2],0.5);
				for(int i=0;i<NUM-1;i++)
				{
					glBegin(GL_QUADS);
					glNormal3f(n1[i][0],n1[i][1],n1[i][2]);
					glVertex3f(p1[i][0],p1[i][1],p1[i][2]);
					glNormal3f(n1[i+1][0],n1[i+1][1],n1[i+1][2]);
					glVertex3f(p1[i+1][0],p1[i+1][1],p1[i+1][2]);
					glNormal3f(n2[i+1][0],n2[i+1][1],n2[i+1][2]);
					glVertex3f(p2[i+1][0],p2[i+1][1],p2[i+1][2]);		
					glNormal3f(n2[i][0],n2[i][1],n2[i][2]);
					glVertex3f(p2[i][0],p2[i][1],p2[i][2]);			
					glEnd();
				}
				if(dataExp[ii][j]>dataCoe[ii][j])
				{
					for(int i=0;i<NUM-1;i++)
					{
						glNormal3f(segment_dir[0],segment_dir[1],segment_dir[2]);
						glBegin(GL_TRIANGLES);
						glVertex3f(p2[i][0],p2[i][1],p2[i][2]);
						glVertex3f(p2[i+1][0],p2[i+1][1],p2[i+1][2]);		
						glVertex3f(o[0],o[1],o[2]);	
						glEnd();
					}			
				}
				else
				{
					head = dataEnd2[ii][j] + coer * norm;
					for(int i=0;i<NUM;i++)
					{
						p1[i]=svGetRotatePoint(head, o, segment_dir, (float)i * 360./(NUM-1));
						n1[i]=p1[i]-o;
						n1[i]=normalize(n1[i]);
					}	
					for(int i=0;i<NUM-1;i++)//clockwise????
					{
						glNormal3f(segment_dir[0],segment_dir[1],segment_dir[2]);
						glBegin(GL_QUADS);
						glVertex3f(p1[i][0],p1[i][1],p1[i][2]);
						glVertex3f(p1[i+1][0],p1[i+1][1],p1[i+1][2]);
						glVertex3f(p2[i+1][0],p2[i+1][1],p2[i+1][2]);		
						glVertex3f(p2[i][0],p2[i][1],p2[i][2]);			
						glEnd();
					}
				}
				for(int i=0;i<NUM;i++)
				{
					p1[i] = p2[i];n1[i]=n2[i];
				}
			}
		}
        }
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        glDisable(GL_BLEND);
	glLineWidth(1.);
}
void svQDOT3dmapping::RenderCylinderLengthLightness()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	glEnable( GL_LINE_SMOOTH );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glDisable(GL_TEXTURE_2D); 

	float expr;
	float coer;
	float exeh;
	float coeh;

	svVector3 segment_dir,norm,head,o,linestart;
	int NUM = 15;
	svVector3 p1[NUM];
	svVector3 n1[NUM];
	svVector3 p2[NUM];
	svVector3 n2[NUM];

	float length;

        for(int ii =0;ii<zlayer;ii++)
        {
		for(int j=0;j<dataPos[i].size();j++)
		{
			if(dataCoe[ii][j]!=0)
			{
				exeh = dataExp[ii][j] * minl_distribute;
				coer = minl_distribute;
				exer = coer * 1.7;
				float light = dataVel[ii][j]/18.;

				//reference lines
					glDisable(GL_LIGHTING);
					glDisable(GL_LIGHT0);
	
					glLineWidth(1.);
					glColor4f(0.3,0.3,0.3,1.);
					glBegin(GL_LINES);
					glVertex3f(dataPos[ii][0],dataPos[ii][1],dataPos[ii][2]);
					glVertex3f(dataPos[ii][0]+5.*minl_distribute*dataDir[ii][0],
						dataPos[ii][1]+5.*minl_distribute*dataDir[ii][1],
						dataPos[ii][2]+5.*minl_distribute*dataDir[ii][2]);
					glEnd();

				//cylinders
				glEnable(GL_LIGHTING);
				glEnable(GL_LIGHT0);

				segment_dir = normalize(dataDir[ii]);
				norm = svGetPerpendicularVector(segment_dir);
				head = dataPos[ii][j] + coer * norm;
				o = dataPos[ii][j];
				for(int i=0;i<NUM;i++)
				{
					p1[i]=svGetRotatePoint(head, o, segment_dir, (float)i * 360./(NUM-1));
					n1[i]=p1[i]-o;
					n1[i]=normalize(n1[i]);
				}			

				o = dataEnd[ii][j];
				head = o + coer * norm;
				for(int i=0;i<NUM;i++)
				{
					p2[i]=svGetRotatePoint(head, o, segment_dir, (float)i * 360./(NUM-1));
					n2[i] = p2[i] - o;
					n2[i]=normalize(n2[i]);
				}
				glColor4f(light,light,light,0.5);
				for(int i=0;i<NUM-1;i++)
				{
					glBegin(GL_QUADS);
					glNormal3f(n1[i][0],n1[i][1],n1[i][2]);
					glVertex3f(p1[i][0],p1[i][1],p1[i][2]);
					glNormal3f(n1[i+1][0],n1[i+1][1],n1[i+1][2]);
					glVertex3f(p1[i+1][0],p1[i+1][1],p1[i+1][2]);
					glNormal3f(n2[i+1][0],n2[i+1][1],n2[i+1][2]);
					glVertex3f(p2[i+1][0],p2[i+1][1],p2[i+1][2]);		
					glNormal3f(n2[i][0],n2[i][1],n2[i][2]);
					glVertex3f(p2[i][0],p2[i][1],p2[i][2]);			
					glEnd();
				}

				for(int i=0;i<NUM-1;i++)
				{
					glNormal3f(segment_dir[0],segment_dir[1],segment_dir[2]);
					glBegin(GL_TRIANGLES);
					glVertex3f(p2[i][0],p2[i][1],p2[i][2]);
					glVertex3f(p2[i+1][0],p2[i+1][1],p2[i+1][2]);		
					glVertex3f(o[0],o[1],o[2]);	
					glEnd();
				}	

				segment_dir = normalize(dataDir[ii][j]);
				norm = svGetPerpendicularVector(segment_dir);
				head = dataPos[ii][j] + expr * norm;
				o = dataPos[ii][j];
				for(int i=0;i<NUM;i++)
				{
					p1[i]=svGetRotatePoint(head, o, segment_dir, (float)i * 360./(NUM-1));
					n1[i]=p1[i]-o;
					n1[i]=normalize(n1[i]);
				}
				for(int i=0;i<NUM-1;i++)
				{
					glNormal3f(-segment_dir[0],-segment_dir[1],-segment_dir[2]);
					glBegin(GL_TRIANGLES);
					glVertex3f(p1[i][0],p1[i][1],p1[i][2]);
					glVertex3f(p1[i+1][0],p1[i+1][1],p1[i+1][2]);		
					glVertex3f(o[0],o[1],o[2]);	
					glEnd();
				}				
				o = dataEnd2[ii][j];
				head = o + expr * norm;
				for(int i=0;i<NUM;i++)
				{
					p2[i]=svGetRotatePoint(head, o, segment_dir, (float)i * 360./(NUM-1));
					n2[i] = p2[i] - o;
					n2[i]=normalize(n2[i]);
				}
				glColor4f(light,light,light,0.5);
				for(int i=0;i<NUM-1;i++)
				{
					glBegin(GL_QUADS);
					glNormal3f(n1[i][0],n1[i][1],n1[i][2]);
					glVertex3f(p1[i][0],p1[i][1],p1[i][2]);
					glNormal3f(n1[i+1][0],n1[i+1][1],n1[i+1][2]);
					glVertex3f(p1[i+1][0],p1[i+1][1],p1[i+1][2]);
					glNormal3f(n2[i+1][0],n2[i+1][1],n2[i+1][2]);
					glVertex3f(p2[i+1][0],p2[i+1][1],p2[i+1][2]);		
					glNormal3f(n2[i][0],n2[i][1],n2[i][2]);
					glVertex3f(p2[i][0],p2[i][1],p2[i][2]);			
					glEnd();
				}
				if(dataExp[ii][j]>dataCoe[ii][j])
				{
					for(int i=0;i<NUM-1;i++)
					{
						glNormal3f(segment_dir[0],segment_dir[1],segment_dir[2]);
						glBegin(GL_TRIANGLES);
						glVertex3f(p2[i][0],p2[i][1],p2[i][2]);
						glVertex3f(p2[i+1][0],p2[i+1][1],p2[i+1][2]);		
						glVertex3f(o[0],o[1],o[2]);	
						glEnd();
					}			
				}
				else
				{
					head = dataEnd2[ii][j] + coer * norm;
					for(int i=0;i<NUM;i++)
					{
						p1[i]=svGetRotatePoint(head, o, segment_dir, (float)i * 360./(NUM-1));
						n1[i]=p1[i]-o;
						n1[i]=normalize(n1[i]);
					}	
					for(int i=0;i<NUM-1;i++)//clockwise????
					{
						glNormal3f(segment_dir[0],segment_dir[1],segment_dir[2]);
						glBegin(GL_QUADS);
						glVertex3f(p1[i][0],p1[i][1],p1[i][2]);
						glVertex3f(p1[i+1][0],p1[i+1][1],p1[i+1][2]);
						glVertex3f(p2[i+1][0],p2[i+1][1],p2[i+1][2]);		
						glVertex3f(p2[i][0],p2[i][1],p2[i][2]);			
						glEnd();
					}
				}
				for(int i=0;i<NUM;i++)
				{
					p1[i] = p2[i];n1[i]=n2[i];
				}
			}
		}
        }
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        glDisable(GL_BLEND);
	glLineWidth(1.);
}
/*
void svQDOT3dmapping::RenderColorColor()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	glEnable( GL_LINE_SMOOTH );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glDisable(GL_TEXTURE_2D); 

	float expr;
	float coer;
	float exeh;
	float coeh;

	svVector3 segment_dir,norm,head,o,linestart;
	int NUM = 15;
	svVector3 p1[NUM];
	svVector3 n1[NUM];
	svVector3 p2[NUM];
	svVector3 n2[NUM];

	float length;

        for(int ii =0;ii<zlayer;ii++)
        {
		for(int j=0;j<dataPos[i].size();j++)
		{
			if(dataCoe[ii][j]!=0)
			{
				exeh = dataExp[ii][j] * minl_distribute;
				coer = minl_distribute;
				exer = coer * 1.7;
				float light = dataVel[ii][j]/18.;

				//reference lines
					glDisable(GL_LIGHTING);
					glDisable(GL_LIGHT0);
	
					glLineWidth(1.);
					glColor4f(0.3,0.3,0.3,1.);
					glBegin(GL_LINES);
					glVertex3f(dataPos[ii][0],dataPos[ii][1],dataPos[ii][2]);
					glVertex3f(dataPos[ii][0]+5.*minl_distribute*dataDir[ii][0],
						dataPos[ii][1]+5.*minl_distribute*dataDir[ii][1],
						dataPos[ii][2]+5.*minl_distribute*dataDir[ii][2]);
					glEnd();

				//cylinders
				glEnable(GL_LIGHTING);
				glEnable(GL_LIGHT0);

				segment_dir = normalize(dataDir[ii]);
				norm = svGetPerpendicularVector(segment_dir);
				head = dataPos[ii][j] + coer * norm;
				o = dataPos[ii][j];
				for(int i=0;i<NUM;i++)
				{
					p1[i]=svGetRotatePoint(head, o, segment_dir, (float)i * 360./(NUM-1));
					n1[i]=p1[i]-o;
					n1[i]=normalize(n1[i]);
				}			

				o = dataEnd[ii][j];
				head = o + coer * norm;
				for(int i=0;i<NUM;i++)
				{
					p2[i]=svGetRotatePoint(head, o, segment_dir, (float)i * 360./(NUM-1));
					n2[i] = p2[i] - o;
					n2[i]=normalize(n2[i]);
				}
				glColor4f(light,light,light,0.5);
				for(int i=0;i<NUM-1;i++)
				{
					glBegin(GL_QUADS);
					glNormal3f(n1[i][0],n1[i][1],n1[i][2]);
					glVertex3f(p1[i][0],p1[i][1],p1[i][2]);
					glNormal3f(n1[i+1][0],n1[i+1][1],n1[i+1][2]);
					glVertex3f(p1[i+1][0],p1[i+1][1],p1[i+1][2]);
					glNormal3f(n2[i+1][0],n2[i+1][1],n2[i+1][2]);
					glVertex3f(p2[i+1][0],p2[i+1][1],p2[i+1][2]);		
					glNormal3f(n2[i][0],n2[i][1],n2[i][2]);
					glVertex3f(p2[i][0],p2[i][1],p2[i][2]);			
					glEnd();
				}

				for(int i=0;i<NUM-1;i++)
				{
					glNormal3f(segment_dir[0],segment_dir[1],segment_dir[2]);
					glBegin(GL_TRIANGLES);
					glVertex3f(p2[i][0],p2[i][1],p2[i][2]);
					glVertex3f(p2[i+1][0],p2[i+1][1],p2[i+1][2]);		
					glVertex3f(o[0],o[1],o[2]);	
					glEnd();
				}	

				segment_dir = normalize(dataDir[ii][j]);
				norm = svGetPerpendicularVector(segment_dir);
				head = dataPos[ii][j] + expr * norm;
				o = dataPos[ii][j];
				for(int i=0;i<NUM;i++)
				{
					p1[i]=svGetRotatePoint(head, o, segment_dir, (float)i * 360./(NUM-1));
					n1[i]=p1[i]-o;
					n1[i]=normalize(n1[i]);
				}
				for(int i=0;i<NUM-1;i++)
				{
					glNormal3f(-segment_dir[0],-segment_dir[1],-segment_dir[2]);
					glBegin(GL_TRIANGLES);
					glVertex3f(p1[i][0],p1[i][1],p1[i][2]);
					glVertex3f(p1[i+1][0],p1[i+1][1],p1[i+1][2]);		
					glVertex3f(o[0],o[1],o[2]);	
					glEnd();
				}				
				o = dataEnd2[ii][j];
				head = o + expr * norm;
				for(int i=0;i<NUM;i++)
				{
					p2[i]=svGetRotatePoint(head, o, segment_dir, (float)i * 360./(NUM-1));
					n2[i] = p2[i] - o;
					n2[i]=normalize(n2[i]);
				}
				glColor4f(light,light,light,0.5);
				for(int i=0;i<NUM-1;i++)
				{
					glBegin(GL_QUADS);
					glNormal3f(n1[i][0],n1[i][1],n1[i][2]);
					glVertex3f(p1[i][0],p1[i][1],p1[i][2]);
					glNormal3f(n1[i+1][0],n1[i+1][1],n1[i+1][2]);
					glVertex3f(p1[i+1][0],p1[i+1][1],p1[i+1][2]);
					glNormal3f(n2[i+1][0],n2[i+1][1],n2[i+1][2]);
					glVertex3f(p2[i+1][0],p2[i+1][1],p2[i+1][2]);		
					glNormal3f(n2[i][0],n2[i][1],n2[i][2]);
					glVertex3f(p2[i][0],p2[i][1],p2[i][2]);			
					glEnd();
				}
				if(dataExp[ii][j]>dataCoe[ii][j])
				{
					for(int i=0;i<NUM-1;i++)
					{
						glNormal3f(segment_dir[0],segment_dir[1],segment_dir[2]);
						glBegin(GL_TRIANGLES);
						glVertex3f(p2[i][0],p2[i][1],p2[i][2]);
						glVertex3f(p2[i+1][0],p2[i+1][1],p2[i+1][2]);		
						glVertex3f(o[0],o[1],o[2]);	
						glEnd();
					}			
				}
				else
				{
					head = dataEnd2[ii][j] + coer * norm;
					for(int i=0;i<NUM;i++)
					{
						p1[i]=svGetRotatePoint(head, o, segment_dir, (float)i * 360./(NUM-1));
						n1[i]=p1[i]-o;
						n1[i]=normalize(n1[i]);
					}	
					for(int i=0;i<NUM-1;i++)//clockwise????
					{
						glNormal3f(segment_dir[0],segment_dir[1],segment_dir[2]);
						glBegin(GL_QUADS);
						glVertex3f(p1[i][0],p1[i][1],p1[i][2]);
						glVertex3f(p1[i+1][0],p1[i+1][1],p1[i+1][2]);
						glVertex3f(p2[i+1][0],p2[i+1][1],p2[i+1][2]);		
						glVertex3f(p2[i][0],p2[i][1],p2[i][2]);			
						glEnd();
					}
				}
				for(int i=0;i<NUM;i++)
				{
					p1[i] = p2[i];n1[i]=n2[i];
				}
			}
		}
        }
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        glDisable(GL_BLEND);
	glLineWidth(1.);
}*/
void svQDOT3dmapping::RenderLines()
{
	glCallList(listIndex);
}
void svQDOT3dmapping::GenerateLines()
{
	listIndex = glGenLists(1);
	if (listIndex != 0) {
	   glNewList(listIndex,GL_COMPILE);


	glEnable( GL_LINE_SMOOTH );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);

	float expr = minl_distribute*1.7;//0.06/1.5;
	float coer = minl_distribute;//0.035/1.5;
	float exeh;
	float coeh;

	svVector3 segment_dir,norm,head,o,linestart;
	int NUM = 15;
	svVector3 p1[NUM];
	svVector3 n1[NUM];
	svVector3 p2[NUM];
	svVector3 n2[NUM];
	svVector3 point;

	float length;
	int num;

	glLineWidth(0.5);
        for(int ii =0;ii<dataSize;)
        {
		if(coe[ii]!=0)
		{

			glDisable(GL_LIGHTING);
			glDisable(GL_LIGHT0);
			glDisable(GL_TEXTURE_2D); 
			if(ENCODE_MODE==1)
				num = dataDen[ii];
			else if(ENCODE_MODE==2)
				num = coe[ii];
			else if(ENCODE_MODE==4)
				num = log10(dataDen[ii]);
			else if(ENCODE_MODE==3)
				num = minl_text/minl_direct;

			segment_dir = normalize(dataDir[ii]);
			norm = svGetPerpendicularVector(segment_dir);	

			glColor4f(0.2,0.2,0.2,1);
		
			for(int j=0;j<num;j++)
			{
				point = dataPos[ii] + dataDir[ii] * (j+1) * minl_direct;	
				head =  point + coer * norm;
				o = point;
				glBegin(GL_LINE_STRIP);
				for(int i=0;i<NUM;i++)
				{
					p1[i]=svGetRotatePoint(head, o, segment_dir, (float)i * 360./(NUM-1));
					glVertex3f(p1[i][0],p1[i][1],p1[i][2]);
				}
				glEnd();
			}

			if(ENCODE_MODE==2)
			{
				num = exp[ii];
				segment_dir = normalize(dataDir[ii]);
				norm = svGetPerpendicularVector(segment_dir);	

				glColor4f(0.2,0.2,0.2,1);
		
				for(int j=0;j<num;j++)
				{
					point = dataPos[ii] + dataDir[ii] * (j+1) * minl_direct;;	
					head =  point + expr * norm;
					o = point;
					glBegin(GL_LINE_STRIP);
					for(int i=0;i<NUM;i++)
					{
						p1[i]=svGetRotatePoint(head, o, segment_dir, (float)i * 360./(NUM-1));
						glVertex3f(p1[i][0],p1[i][1],p1[i][2]);
					}
					glEnd();
				}
			}
			
		}

		ii = ii + glyphFrequency;
        }
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        glDisable(GL_BLEND);
	glLineWidth(1.);
	   glEndList();
	}
}


void svQDOT3dmapping::cleanup()
{

	for(int i=0;i<zlayer;i++)
	{
		dataPos[i].free();
		dataEnd[i].free();
		dataDen[i].free();	
		dataDir[i].free();
		dataPoint[i].free();
		dataEnd2[i].free();
		dataVel[i].free();
		dataExp[i].free();
		dataCoe[i].free();
		dataCol[i].free();
	}
	
	delete [] dataPos;
	delete [] dataEnd;
	delete [] dataDen;
	delete [] dataDir;
	delete [] dataPoint;
	delete [] dataEnd2;
	delete [] dataVel;
	delete [] dataExp;	
	delete [] dataCoe;
	delete [] dataCol;

	dataPos = NULL;
	dataEnd = NULL;
	dataDen = NULL;
	dataDir = NULL;
	dataPoint = NULL;
	dataEnd2 = NULL;
	dataVel = NULL;
	dataExp = NULL;
	dataCoe = NULL;
	dataCol = NULL;
}

svQDOT3dmapping::~svQDOT3dmapping()
{
cleanup();
	
}


