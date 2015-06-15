/*  
 * svIconGlyph
 */



#include <iostream>
#include <fstream>
#include <string.h>
#include <cmath>

#include "svArrowGlyph.h"
#include "svException.h"
#include "svUtil.h"


#ifndef PI
#define PI 3.1415926
#endif

using namespace std;


namespace __svl_lib {

svArrowGlyph::svArrowGlyph( svVector3 l, svVector3 r,int dataSize,
     svVector3 *posIn, svVector3 *velIn,  svScalar *denIn,
	svScalar *exp, svScalar *coe,
	 svVector4 *color4In1,svVector4 *color4In2,
	 int num,float R)
{
  //float tmpden;

  
  glyphColors1 = NULL; // Colors of lines
  glyphColors2 = NULL; //Colors of arrows

  glyphArrowPos = NULL; //position of arrow heads
  glyphArrowNorm = NULL;
  
  glyphPos = NULL;
  glyphDir = NULL;
  glyphLegend = NULL;
  glyphMag = NULL;
  glyphExp = NULL;
  glyphCoe = NULL;
  glyphradius = NULL;


  cerr << "... Setting ArrowGlyph ..." << endl;

  SetData(l,r,dataSize,posIn,velIn,denIn,exp,coe,color4In1,color4In2,num,R);
  
  cerr << "... Done init ArrowGlyph ... " << endl;
}
void svArrowGlyph::SetData(
	svVector3 l, svVector3 r,int dataSize,
                     svVector3 *posIn, svVector3 *velIn,  svScalar *denIn,
                        svScalar *exp, svScalar *coe,
                         svVector4 *color4In1,svVector4 *color4In2,
                         int num,float R)
{
	if(glyphColors2!=NULL)
		cleanup();
	
  float tmpden;
  
  lb = l;
  rb = r;

  NUM_SEG=num;
  
  ARROW_BASE = R;
  
  //cerr<<NUM_SEG<<" "<<R<<endl;

  // data attributes
  glyphPos = new svVector3Array[3]; // starting and ending positions
  glyphDir = new svVector3Array[1];
  glyphLegend = new svVector3Array[1];
  glyphMag = new svScalar[dataSize];
  glyphCoe = new svScalar[dataSize];
  glyphExp = new svScalar[dataSize];
  glyphradius = new svScalar[dataSize];

  //rendering attributes
  glyphArrowPos = new svVector3Array[NUM_SEG+1];
  glyphArrowNorm = new svVector3Array[NUM_SEG+1];
  glyphFrequency = 1;
  glyphColors1 = new svVector4Array[1];
  glyphColors2 = new svVector4Array[1];


    int stacks = 10;
    int slices = 10;
    int size = stacks * (slices+1) * 2 * 3;
    sphereVertexArray = new float[dataSize *size];
    sphereNormalArray = new float[dataSize *size];

  for(int j=0; j<dataSize; j++)
  {
    	// set position
	
   	glyphPos[0].add(posIn[j]);
   	glyphPos[1].add(posIn[j]);
   	glyphPos[2].add(posIn[j]);
	glyphLegend[0].add(posIn[j]);
   	   	//outfile<<glyphPos[0][j][0]<<" "<<glyphPos[0][j][1]<<" "<<glyphPos[0][j][2]<<" "<<endl;
	for(int i=0;i<NUM_SEG+1;i++)	
	{
		glyphArrowPos[i].add(posIn[j]);	
		glyphArrowNorm[i].add(posIn[j]);
	}
		

   	// set vector dir
	svVector3 nvel = normalize(velIn[j]);
	glyphDir[0].add(nvel);

	//set magnitude
	glyphMag[j]=(denIn[j]);

	//set exp and coeffecient
	glyphCoe[j]=coe[j];
	glyphExp[j]=exp[j];

	//set colors	
    if(color4In1!=NULL)  
	{
		if(color4In1[0][0]>1)
	  glyphColors1[0].add(svVector4(color4In1[j][0]/255., color4In1[j][1]/255., color4In1[j][2]/255., color4In1[j][3]/255.));
		else
		 glyphColors1[0].add(svVector4(color4In1[j][0], color4In1[j][1], color4In1[j][2], color4In1[j][3]));
	}
	else  
	{
	  glyphColors1[0].add(svVector4(1,1,1,0)); // default white
	}
    	if(color4In2!=NULL)  
	{
		if(color4In2[0][0]>1)
		glyphColors2[0].add(svVector4(color4In2[j][0]/255., color4In2[j][1]/255., color4In2[j][2]/255., color4In2[j][3]/255.));
	else
	  glyphColors2[0].add(svVector4(color4In2[j][0], color4In2[j][1], color4In2[j][2], color4In2[j][3]));
	}
	else  
	{
	  glyphColors2[0].add(svVector4(1,1,1,0)); // default white
	}
	
  }; // end for(j)

	//outfile.close();

	//cerr<<"Setting arrow data done"<<endl;

}
void svArrowGlyph::SetColors(svVector4 *color4In1,svVector4 *color4In2 )
{
	if(glyphColors2!=NULL)
	{
		glyphColors1[0].free();
		delete [] glyphColors1;
		glyphColors1=NULL;

		glyphColors2[0].free();
		delete [] glyphColors2;
		glyphColors2=NULL;
	}

  glyphColors1 = new svVector4Array[1];
  glyphColors2 = new svVector4Array[1];
  int dataSize = glyphPos[0].size();

  for(int j=0; j<dataSize; j++)
  {


	//set colors	
    if(color4In1!=NULL)  
	{
		if(color4In1[0][0]>1)
	  glyphColors1[0].add(svVector4(color4In1[j][0]/255., color4In1[j][1]/255., color4In1[j][2]/255., color4In1[j][3]/255.));
		else
		 glyphColors1[0].add(svVector4(color4In1[j][0], color4In1[j][1], color4In1[j][2], color4In1[j][3]));
	}
	else  
	{
	  glyphColors1[0].add(svVector4(1,1,1,0)); // default white
	}
    	if(color4In2!=NULL)  
	{
		if(color4In2[0][0]>1)
		glyphColors2[0].add(svVector4(color4In2[j][0]/255., color4In2[j][1]/255., color4In2[j][2]/255., color4In2[j][3]/255.));
	else
	  glyphColors2[0].add(svVector4(color4In2[j][0], color4In2[j][1], color4In2[j][2], color4In2[j][3]));
	}
	else  
	{
	  glyphColors2[0].add(svVector4(1,1,1,0)); // default white
	}
	
  }; // end for(j)

}
void svArrowGlyph::SetSamples(int *sample, int num)
{
	sample_index.clear();
	
	for(int i=0;i<num;i++)
	{
		sample_index.push_back(sample[i]);
	}
}

void svArrowGlyph::SetArrows(bool enablesample, bool enablelines, svScalar *size)
{
        float x3,y3,z3;
        float x1,y1,z1;
        float adj;
        svVector3 midpoint, u, v, tmp1, tmp2, tmp3, tmp4;
        svVector3 head,headn;
	svVector3 norm;
	svVector3 segment_dir;	
	svFloat radius = ARROW_BASE*2./3.;
        svFloat headlength = 1;
	svFloat radius2;
	float length = ARROW_BASE * 4.;
	svVector3 point;

	int s, e;
	int i;
	int sample = glyphPos[0].size();

	int end;
	
	if(!enablesample)
	{
		end = sample;
	}
	else
	{
		end = sample_index.size();
	}
	
	svScalar min_size;
	int min_index;
	
    for(int ii=0;ii<end;ii++)
    {
    	if(!enablesample)
		{
			i = ii;
		}
		else
		{
			i = sample_index[ii];
		}
    	if(size[i]!=0)
    	{
    		min_index = i;
    		break;
    	}
    }
    
    if(encode_mode==1)
    {
    	min_size = size[min_index]/glyphMag[min_index];
    }
    else if(encode_mode==2)
    {
    	min_size = size[min_index]/glyphExp[min_index];
    }
    else if(encode_mode==3)
    {
    	min_size = size[min_index]/log10(glyphMag[min_index]);
    }


        for(int ii=0;ii<end;ii++)
        {
        	if(!enablesample)
			{
				i = ii;
			}
			else
			{
				i = sample_index[ii];
			}

        	//size[i] = size[i]/10.;
		radius = size[i]/20.;
		glyphradius[i] = radius;
		length = radius*4.;

		if(encode_mode!=2)
        	{
			glyphPos[1][i][0] = glyphPos[0][i][0] + size[i] * glyphDir[0][i][0];
			glyphPos[1][i][1] = glyphPos[0][i][1] + size[i] * glyphDir[0][i][1];
			glyphPos[1][i][2] = glyphPos[0][i][2] + size[i] * glyphDir[0][i][2];
			glyphLegend[0][i][0] = glyphPos[0][i][0] + min_size * 5. * glyphDir[0][i][0];
			glyphLegend[0][i][1] = glyphPos[0][i][1] + min_size * 5. * glyphDir[0][i][1];
			glyphLegend[0][i][2] = glyphPos[0][i][2] + min_size * 5. * glyphDir[0][i][2];
		}

        	else
        	{       
        		float scale;
        		if(glyphCoe[i]!=0)
        		{
        			scale = size[i]/glyphExp[i]*glyphCoe[i];
        		}
        		else
        			scale = 0;
		    	glyphPos[1][i][0] = glyphPos[0][i][0] + scale * glyphDir[0][i][0];
		    	glyphPos[1][i][1] = glyphPos[0][i][1] + scale * glyphDir[0][i][1];
		    	glyphPos[1][i][2] = glyphPos[0][i][2] + scale * glyphDir[0][i][2];   
			glyphPos[2][i][0] = glyphPos[0][i][0] + size[i] * glyphDir[0][i][0];
			glyphPos[2][i][1] = glyphPos[0][i][1] + size[i] * glyphDir[0][i][1];
			glyphPos[2][i][2] = glyphPos[0][i][2] + size[i] * glyphDir[0][i][2];   
			glyphLegend[0][i][0] = glyphPos[0][i][0] + min_size * 5. * glyphDir[0][i][0];
			glyphLegend[0][i][1] = glyphPos[0][i][1] + min_size * 5. * glyphDir[0][i][1];
			glyphLegend[0][i][2] = glyphPos[0][i][2] + min_size * 5. * glyphDir[0][i][2];        	
        	}
       	
        	if(enablelines)
			{
        		point[0]=glyphPos[1][i][0]+length*glyphDir[0][i][0];
				point[1]=glyphPos[1][i][1]+length*glyphDir[0][i][1];
				point[2]=glyphPos[1][i][2]+length*glyphDir[0][i][2];
				norm = svGetPerpendicularVector(glyphDir[0][i]);
				head = glyphPos[1][i] + radius * norm;
			}
        	else
        	{
        		point = glyphPos[1][i];
        		norm =svGetPerpendicularVector( glyphDir[0][i]);
        		head = glyphPos[0][i] + radius * norm;
        	}
	
			segment_dir = normalize(glyphDir[0][i]);	
			/*if(i==0)
			{
				cerr<<enablelines<<endl;
				cerr<<segment_dir[0]<<" "<<segment_dir[1]<<" "<<segment_dir[2]<<" "<<endl;
				cerr<<head[0]<<" "<<head[1]<<" "<<head[2]<<endl;
				cerr<<glyphPos[1][i][0]<<" "<<glyphPos[1][i][1]<<" "<<glyphPos[1][i][2]<<" "<<endl;
			}*/
			for(int j=0;j<NUM_SEG;j++)
			{
				if(enablelines)
					glyphArrowPos[j][i] = svGetRotatePoint(head, glyphPos[1][i], segment_dir, (float)j * 360./(float)NUM_SEG);
				else
					glyphArrowPos[j][i] = svGetRotatePoint(head, glyphPos[0][i], segment_dir, (float)j * 360./(float)NUM_SEG);

			}
			for(int j=0;j<NUM_SEG;j++)
			{
				if(j!=NUM_SEG-1)
					glyphArrowNorm[j+1][i] = svGetNorm(glyphArrowPos[j][i],point,glyphArrowPos[j+1][i]);
				else
					glyphArrowNorm[j+1][i] = svGetNorm(glyphArrowPos[j][i],point,glyphArrowPos[0][i]);

			}
			for(int j=1;j<NUM_SEG+1;j++)
			{
				if(j!=1)
					glyphArrowNorm[j][i] = (glyphArrowNorm[j][i] + glyphArrowNorm[j-1][i]-segment_dir);
				else
					glyphArrowNorm[j][i] = (glyphArrowNorm[j][i] + glyphArrowNorm[NUM_SEG][i]-segment_dir);
				glyphArrowNorm[j][i] = normalize(glyphArrowNorm[j][i]);
			}
			glyphArrowNorm[0][i] = segment_dir;
			glyphArrowPos[NUM_SEG][i] = point; 
        }	
        
	if(!enablesample)
		createSphereArraysAndVBOs(enablesample);
	//cerr<<"Setting arrow size done"<<endl;
}

void svArrowGlyph::createSphereArraysAndVBOs(bool enablesample) 
{
    int i,j;
    double radius = 0.4;
    int stacks = 10;
    int slices = 10;
    int size = stacks * (slices+1) * 2 * 3;
    int k = 0;

	int end;

	if(!enablesample)
	{
		end = glyphPos[0].size();
	}
	else
	{
		end = sample_index.size();
	}

	int ii;

        for(int iii=0;iii<end;iii++)
        {
        	if(!enablesample)
		{
			ii = iii;
		}
		else
		{
			ii = sample_index[iii];
		}

		for (j = 0; j < stacks; j++) 
		{
			double latitude1 = (PI/stacks) * j - PI/2;
			double latitude2 = (PI/stacks) * (j+1) - PI/2;
			double sinLat1 = sin(latitude1);
			double cosLat1 = cos(latitude1);
			double sinLat2 = sin(latitude2);
			double cosLat2 = cos(latitude2);
			for (i = 0; i <= slices; i++) 
			{
			    double longitude = (2*PI/slices) * i;
			    double sinLong = sin(longitude);
			    double cosLong = cos(longitude);
			    double x1 = cosLong * cosLat1;
			    double y1 = sinLong * cosLat1;
			    double z1 = sinLat1;
			    double x2 = cosLong * cosLat2;
			    double y2 = sinLong * cosLat2;
			    double z2 = sinLat2;

			    sphereNormalArray[k] =  (float)x2;
			    sphereNormalArray[k+1] =  (float)y2;
			    sphereNormalArray[k+2] =  (float)z2;
			    sphereVertexArray[k] =    (float)((glyphradius[ii]*x2)+glyphPos[2][ii][0]+glyphradius[ii]*glyphDir[0][ii][0]);
			    sphereVertexArray[k+1] =  (float)((glyphradius[ii]*y2)+glyphPos[2][ii][1]+glyphradius[ii]*glyphDir[0][ii][1]);
			    sphereVertexArray[k+2] =  (float)((glyphradius[ii]*z2)+glyphPos[2][ii][2]+glyphradius[ii]*glyphDir[0][ii][2]);

			    k += 3;
			    sphereNormalArray[k] =  (float)x1;
			    sphereNormalArray[k+1] =  (float)y1;
			    sphereNormalArray[k+2] =  (float)z1;
			    sphereVertexArray[k] =    (float)((glyphradius[ii]*x1)+glyphPos[2][ii][0]+glyphradius[ii]*glyphDir[0][ii][0]);
			    sphereVertexArray[k+1] =  (float)((glyphradius[ii]*y1)+glyphPos[2][ii][1]+glyphradius[ii]*glyphDir[0][ii][1]);
			    sphereVertexArray[k+2] =  (float)((glyphradius[ii]*z1)+glyphPos[2][ii][2]+glyphradius[ii]*glyphDir[0][ii][2]);

			    k += 3;
			}
		    }
	}
    /*GLuint bufferIDs[3];
    glGenBuffers(3, bufferIDs);
    vertexVboId = bufferIDs[0];
    normalVboId = bufferIDs[1];
    colorVboId = bufferIDs[2];
    glBindBuffer(GL_ARRAY_BUFFER, vertexVboId);
    glBufferData(GL_ARRAY_BUFFER, size*sizeof(float), sphereVertexArray, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, normalVboId);
    glBufferData(GL_ARRAY_BUFFER, size*sizeof(float), sphereNormalArray, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, colorVboId);
    glBufferData(GL_ARRAY_BUFFER, size*sizeof(float), sphereColorArray, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);*/
}

void svArrowGlyph::drawSphereDirectWithDataFromArrays(bool enablesample)  
{
    int i,j;
    int slices = 10;
    int stacks = 10;
    int vertices;
	int end;
	int ii;
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);     
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);

            glVertexPointer(3,GL_FLOAT,0,sphereVertexArray);
            glNormalPointer(GL_FLOAT,0,sphereNormalArray);

	if(!enablesample)
	{
		end = glyphPos[0].size();
	}
	else
	{
		end = sample_index.size();
	}

	for(int iii=0;iii<end;iii++)
	{
       		if(!enablesample)
		{
			ii = iii;
		}
		else
		{
			ii = sample_index[iii];
		}

	 	vertices = (slices+1)*2;
		glColor3f(glyphColors1[0][ii][0],glyphColors1[0][ii][1],glyphColors1[0][ii][2]);

		    for (i = 0; i < stacks; i++) {
			int pos = ii * stacks*(slices+1)*2 + i*(slices+1)*2;
			glDrawArrays(GL_QUAD_STRIP, pos, vertices);
		    }
	}
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
}

void svArrowGlyph::Render(bool *inbox)
{
	float alpha;
	float s = 0.5;

	int pointnum = glyphPos[0].size();
	//cerr<<pointnum<<endl;
	for(int i=0;i<pointnum;i++)
	{
		if(inbox[i])
		{
			alpha = 1;
		}
		else
		{
			alpha = 0.1;
					
		}

		if(glyphColors1[0][i][0]>=0){

		glColor4f(glyphColors1[0][i][0],glyphColors1[0][i][1],glyphColors1[0][i][2],alpha);

		glBegin(GL_POINTS);
		glVertex3f(glyphPos[0][i][0],glyphPos[0][i][1],glyphPos[0][i][2]);
		glEnd();

		glBegin(GL_LINES);
		glVertex3f(glyphPos[0][i][0],glyphPos[0][i][1],glyphPos[0][i][2]);
		glVertex3f(glyphPos[0][i][0]+s*glyphDir[0][i][0],
				glyphPos[0][i][1]+s*glyphDir[0][i][1],
				glyphPos[0][i][2]+s*glyphDir[0][i][2]);
		glEnd();
		}
	}
}

void svArrowGlyph::RenderArrow(bool *inbox, int *sc, int num, bool enablesample, bool enablelines, int *index)
{
	float alpha = 0.1;
    int slices = 10;
    int stacks = 10;
    int vertices;
	
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);     
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	glEnable( GL_LINE_SMOOTH );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);

            glVertexPointer(3,GL_FLOAT,0,sphereVertexArray);
            glNormalPointer(GL_FLOAT,0,sphereNormalArray);

    GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8, 1};
    GLfloat mat_ambient[] = {0.8, 0.8, 0.8, 1};
    GLfloat mat_specular[] = { 0.5, 0.5, 0.5, 1 };
    GLfloat mat_shininess[] = { 2 };

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
	

        svVector3 arrow1, arrow2;
        float x3,y3,z3;
        float x1,y1,z1;
        float adj;
	bool flag;
	int pointnum;
	
	if(!enablesample)
	pointnum = glyphPos[0].size();
	else 
		pointnum = sample_index.size();
	int j=0;
	float length = 0.2;
	svVector3 point;
	int i;

	if(inbox==NULL)
	{
		inbox = new bool[pointnum];
		 for(int ii =0;ii<pointnum;ii++)
       		 {		
			inbox[ii] = true;

		}
	}
	if(sc == NULL)
	{
		sc = new int [num];
		for(int k=0;k<num;k++)
			sc[k] = -1;
	}



        for(int ii =0;ii<pointnum;)
        {		
        	if(!enablesample)
        	{
        		i = ii;
        	}
        	else
        	{
        		i = sample_index[ii];
        	}
        	
				if(inbox[i])
				{
					alpha = 1;
				}
				else
				{
					alpha = 0.1;
					
				}

			if(glyphColors2[0][i][0]>=0)
			{
	
			point = glyphArrowPos[NUM_SEG][i];
                        glColor4f(glyphColors2[0][i][0],glyphColors2[0][i][1],glyphColors2[0][i][2],alpha);

			glBegin(GL_TRIANGLES);
			for(int j=0;j<NUM_SEG;j++)
			{

				glNormal3f(glyphArrowNorm[0][i][0],glyphArrowNorm[0][i][1],glyphArrowNorm[0][i][2]);
				glVertex3f(point[0],point[1],point[2]);
				glNormal3f(glyphArrowNorm[j+1][i][0],glyphArrowNorm[j+1][i][1],glyphArrowNorm[j+1][i][2]);
				glVertex3f(glyphArrowPos[j][i][0],glyphArrowPos[j][i][1],glyphArrowPos[j][i][2]);

				if(j!=NUM_SEG-1)
		                {
					glNormal3f(glyphArrowNorm[j+2][i][0],glyphArrowNorm[j+2][i][1],glyphArrowNorm[j+2][i][2]);
		                        glVertex3f(glyphArrowPos[j+1][i][0],glyphArrowPos[j+1][i][1],glyphArrowPos[j+1][i][2]);
				}
				else
				{
		                        glNormal3f(glyphArrowNorm[1][i][0],glyphArrowNorm[1][i][1],glyphArrowNorm[1][i][2]);
		                        glVertex3f(glyphArrowPos[0][i][0],glyphArrowPos[0][i][1],glyphArrowPos[0][i][2]);
				}
				
			}
			glEnd();
			
			glBegin(GL_QUADS);
			glNormal3f(-glyphArrowNorm[0][i][0],-glyphArrowNorm[0][i][1],-glyphArrowNorm[0][i][2]);
			for(int j=0;j<NUM_SEG;j++)
			{
				glVertex3f(glyphArrowPos[j][i][0],glyphArrowPos[j][i][1],glyphArrowPos[j][i][2]);
			}			
			
			glEnd();
			
			svVector3 end;
			end = glyphPos[1][i];
			
			if(encode_mode==2)
			{
			/*	glPushMatrix();
				glTranslatef(glyphPos[2][i][0]+glyphradius[i]*glyphDir[0][i][0],
						glyphPos[2][i][1]+glyphradius[i]*glyphDir[0][i][1],
						glyphPos[2][i][2]+glyphradius[i]*glyphDir[0][i][2]);
				glutSolidSphere(glyphradius[i],5,5);
				glPopMatrix();*/
				vertices = (slices+1)*2;
				glColor4f(glyphColors1[0][i][0],glyphColors1[0][i][1],glyphColors1[0][i][2],alpha);

		    		for (int ij = 0; ij < stacks; ij++) 
				{
					int pos = i * stacks*(slices+1)*2 + ij*(slices+1)*2;
					glDrawArrays(GL_QUAD_STRIP, pos, vertices);
		    		}
				if(glyphExp[i]>glyphCoe[i])
				{
					end = glyphPos[2][i];
				}
					
			}
			if(enablelines)
			{
				glDisable(GL_LIGHTING);
				glDisable(GL_LIGHT0);

				glLineWidth(3.);
				//if(flag)
					 glColor4f(glyphColors1[0][i][0],glyphColors1[0][i][1],glyphColors1[0][i][2],alpha);
				//else
				//	glColor4f(1.,1.,0.,alpha);
				glBegin(GL_LINES);
				glVertex3f(glyphPos[0][i][0],glyphPos[0][i][1],glyphPos[0][i][2]);
				glVertex3f(end[0],end[1],end[2]);
				glEnd();
				
				glLineWidth(6.);
				glColor4f(0.,0.,0.,alpha);
				glBegin(GL_LINES);
				glVertex3f(glyphPos[0][i][0],glyphPos[0][i][1],glyphPos[0][i][2]);
				glVertex3f(end[0],end[1],end[2]);
				glEnd();
			}
								
         		glLineWidth(0.75);  
			glDepthRange(0,0.9999999);
			glColor4f(0.,0.,0.,alpha);
			glBegin(GL_LINES);
            		glVertex3f(glyphPos[0][i][0],glyphPos[0][i][1],glyphPos[0][i][2]);
			glVertex3f(glyphLegend[0][i][0],glyphLegend[0][i][1],glyphLegend[0][i][2]);
            		glEnd();
			glDepthRange(0.0f, 1.0f);
			//glEnable(GL_DEPTH_TEST);
			

				glEnable(GL_LIGHTING);
				glEnable(GL_LIGHT0);
			
			}

		ii = ii + glyphFrequency;

        }
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        glDisable(GL_BLEND);
	glLineWidth(1.);
}

void svArrowGlyph::cleanup()
{
	if(glyphPos!=NULL)
	{
		glyphPos[0].free();
		glyphPos[1].free();
		glyphPos[2].free();
		glyphLegend[0].free();
		delete [] glyphPos;
		delete [] glyphLegend;
		glyphPos=NULL;
		glyphLegend = NULL;
	}

	glyphDir[0].free();
	delete [] glyphDir;
	glyphDir=NULL;

	glyphColors1[0].free();
	delete [] glyphColors1;
	glyphColors1=NULL;

	glyphColors2[0].free();
	delete [] glyphColors2;
	glyphColors2=NULL;

	for(int i=0;i<NUM_SEG+1;i++)
	{
		glyphArrowPos[i].free();
	}
	delete [] glyphArrowPos;
	glyphArrowPos=NULL;

	for(int i=0;i<NUM_SEG+1;i++)
	{
		glyphArrowNorm[i].free();
	}
	delete [] glyphArrowNorm;
	glyphArrowNorm=NULL;

	delete [] glyphMag;
	delete [] glyphExp;
	delete [] glyphCoe;

	delete [] glyphradius;

	delete [] sphereVertexArray;
	delete [] sphereNormalArray;
	
	sample_index.clear();
}

}
