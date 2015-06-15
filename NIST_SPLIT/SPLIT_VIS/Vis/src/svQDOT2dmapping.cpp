#include "svQDOT2dmapping.h"

#ifndef PI 
#define PI 3.1415926
#endif

void svQDOT2dmapping::SetColor(svVector4Array *col, svVector4Array *col1, svVector4Array *col2,
			int zlayer)
{
	dataCol = new svVector4Array[zlayer];
	dataExpCol =  new svVector4Array[zlayer];
	dataSigCol = new svVector4Array[zlayer];
	for(int i=0;i<zlayer;i++)
	{
		for(int j=0;j<col[i].size();j++)
		{
			dataSigCol[i].add(col2[i][j]);
			dataExpCol[i].add(col1[i][j]);
			dataCol[i].add(col[i][j]);
		}
	}
}
void svQDOT2dmapping::SetColor()
{
	initcolor = new svVector3Array[1];
	initlightness = new svVector3Array[1];	

	initcolor[0].add(svVector3(6,30,67));
	initcolor[0].add(svVector3(8,48,107));
	initcolor[0].add(svVector3(8,81,156));

	initcolor[0].add(svVector3(33,113,181));
	initcolor[0].add(svVector3(66,146,198));
	initcolor[0].add(svVector3(107,174,214));

	initcolor[0].add(svVector3(158,202,225));
	initcolor[0].add(svVector3(198,219,239));
	initcolor[0].add(svVector3(222,235,247));

	initcolor[0].add(svVector3(247,251,255));
	initcolor[0].add(svVector3(254,224,210));
	initcolor[0].add(svVector3(252,187,161));

	initcolor[0].add(svVector3(252,146,114));
	initcolor[0].add(svVector3(251,106,74));
	initcolor[0].add(svVector3(239,59,44));

	initcolor[0].add(svVector3(203,24,29));
	initcolor[0].add(svVector3(165,15,21));
	initcolor[0].add(svVector3(103,0,13));

	initlightness[0].add(svVector3(14,14,14));
	initlightness[0].add(svVector3(28,28,28));
	initlightness[0].add(svVector3(42,42,42));

	initlightness[0].add(svVector3(56,56,56));
	initlightness[0].add(svVector3(70,70,70));
	initlightness[0].add(svVector3(84,84,84));

	initlightness[0].add(svVector3(98,98,98));
	initlightness[0].add(svVector3(112,112,112));
	initlightness[0].add(svVector3(126,126,126));

	initlightness[0].add(svVector3(140,140,140));
	initlightness[0].add(svVector3(154,154,154));
	initlightness[0].add(svVector3(168,168,168));

	initlightness[0].add(svVector3(182,182,182));
	initlightness[0].add(svVector3(196,196,196));
	initlightness[0].add(svVector3(210,210,210));

	initlightness[0].add(svVector3(224,224,224));
	initlightness[0].add(svVector3(238,238,238));
	initlightness[0].add(svVector3(252,252,252));

	for(int i=0;i<18;i++)
	{
		initcolor[0][i][0] = initcolor[0][i][0] /255.;
		initcolor[0][i][1] = initcolor[0][i][1] /255.;
		initcolor[0][i][2] = initcolor[0][i][2] /255.;
		initlightness[0][i][0] = initlightness[0][i][0]/255.;
		initlightness[0][i][1] = initlightness[0][i][1]/255.;
		initlightness[0][i][2] = initlightness[0][i][2]/255.;
	}
}
void svQDOT2dmapping::cleanup()
{
	delete [] dataPos;
	delete [] dataDir;
	delete [] dataExp;
	delete [] dataSig;
	//delete [] dataCol;
	//delete [] dataSigCol;
	//delete [] dataExpCol;
	dataSize = 0;
}

void svQDOT2dmapping::initcolors()
{

}

void svQDOT2dmapping::RenderBars(svVector3Array *pos,svScalarArray *vel,
		svScalarArray *exp, svScalarArray *coe,
		int zlayer, 
		float bw, float bh)
{
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHT1);

	block_height = bh;
	block_width = bw;

	float significant_width;
	float pow_width;
	float orientation_width;
	float significant_height;
	float pow_height;
	float orientation_height;
	float x,y;

    int dir;
    float dir2;

    pow_height = block_height/2.;
	significant_height = pow_height/1.7;
	orientation_height = pow_height;

    glPushMatrix();
    //glScalef(scale_value,scale_value,1);
	glLineWidth(0.3);

    for(int j=0;j<zlayer;j++)
   {
	 for(int i=0;i<pos[j].size();i++)
	{
		float tmpcoe = coe[j][i];
		float tmpexp = exp[j][i];
		tmpcoe = tmpcoe/11.;
		tmpexp = tmpexp/11.;
		if(tmpcoe<0)tmpcoe=0;
		if(tmpexp<0)tmpexp=0;

		significant_width = tmpcoe * block_width;
		pow_width = tmpexp * block_width;

	if(vel[j][i]>=0)
        dir = (vel[j][i]/PI * 180./10.);
	else
		dir = 0;
        dir2 = (float)dir/18.;

        orientation_width = dir2*block_width;

		x = pos[j][i][0];
		y = pos[j][i][1];

		glColor4f(0.3,0.3,0.3,1.);

			glBegin(GL_QUADS);
			glVertex2f(x-block_width/2.,y);
			glVertex2f(x-block_width/2.+pow_width,y);
			glVertex2f(x-block_width/2.+pow_width,y+pow_height);
			glVertex2f(x-block_width/2.,y+pow_height);
			glEnd();
		
		
		//bars of orientation
		glBegin(GL_QUADS);
		glVertex2f(x-block_width/2.,y-block_height/2.);
		glVertex2f(x-block_width/2.+orientation_width,y-block_height/2.);
		glVertex2f(x-block_width/2.+orientation_width,y);
		glVertex2f(x-block_width/2.,y);
		glEnd();

		glColor4f(0.,0.,0., 0.75);

		glBegin(GL_LINE_LOOP);
		glVertex2f(x-block_width/2.,y+(pow_height-significant_height)/2.);
		glVertex2f(x-block_width/2.+significant_width,y+(pow_height-significant_height)/2.);
		glVertex2f(x-block_width/2.+significant_width,y+(pow_height-significant_height)/2.+significant_height);
		glVertex2f(x-block_width/2.,y+(pow_height-significant_height)/2.+significant_height);
		glEnd();

		glBegin(GL_LINE_LOOP);
		glVertex2f(x+block_width/2.,y-block_height/2.);
		glVertex2f(x+block_width/2.,y+block_height/2.);
		glVertex2f(x-block_width/2.,y+block_height/2.);
		glVertex2f(x-block_width/2.,y-block_height/2.);
        	glEnd();
   		 }
	}
    glPopMatrix();
	glLineWidth(1.);

}

void svQDOT2dmapping::RenderSizeColor(svVector3Array *pos,svScalarArray *vel,
		svScalarArray *exp, svScalarArray *coe,
		int zlayer,
		float bw, float bh)
{
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHT1);

	block_width = bw;
	block_height = bh;
	//cerr<<bw<<" "<<bh<<endl;

	float significant_width;
	float pow_width;
	float significant_height;
	float pow_height;

	float x,y;

    glPushMatrix();
  //  glScalef(scale_value,scale_value,1);

    glLineWidth(0.2);

	for(int j=0;j<zlayer;j++)
	{
	for(int i=0;i<pos[j].size();i++)
	{

		float tmpcoe = coe[j][i];
		float tmpexp = exp[j][i];
		tmpcoe = tmpcoe/11.;
		tmpexp = tmpexp/11.;
		if(tmpcoe<0)tmpcoe=0;
		if(tmpexp<0)tmpexp=0;
		

        significant_height = sqrt(tmpcoe) * block_height;
        pow_height = sqrt(tmpexp) * block_height;
        significant_width = sqrt(tmpcoe) * block_width;
        pow_width = sqrt(tmpexp) *block_width;

	//cerr<< significant_height<<" "<<pow_height<<" "<<significant_width<<" "<<pow_width<<endl;

		x = pos[j][i][0];
		y = pos[j][i][1];
	
		int color_index;
		if(vel[j][i]>=0)
			color_index = (vel[j][i]/PI * 180.)/10;//cerr<<vel[j][i]<<" "<<color_index<<endl;
		else
			color_index = 0;

		//bars of significant
		if(exp[j][i]>coe[j][i])
		{


			//glColor4f(dataCol[j][i][0],dataCol[j][i][1],dataCol[j][i][2],dataCol[j][i][3]);
			glColor4f(initcolor[0][color_index][0],initcolor[0][color_index][1],
				initcolor[0][color_index][2],1.);
			//pow square
			glBegin(GL_QUADS);
			glVertex2f(x+pow_width/2.,y-pow_height/2.);
			glVertex2f(x+pow_width/2.,y+pow_height/2.);
			glVertex2f(x-pow_width/2.,y+pow_height/2.);
			glVertex2f(x-pow_width/2.,y-pow_height/2.);
			glEnd();

           		 glColor4f(0.3,0.3,0.3,1);
			//significant square
			glBegin(GL_LINE_LOOP);
			glVertex2f(x+significant_width/2.,y-significant_height/2.);
			glVertex2f(x+significant_width/2.,y+significant_height/2.);
			glVertex2f(x-significant_width/2.,y+significant_height/2.);
			glVertex2f(x-significant_width/2.,y-significant_height/2.);
			glEnd();
		}
		else
		{
           	 glColor4f(0.3,0.3,0.3,1.);
			//significant square
			glBegin(GL_LINE_LOOP);
			glVertex2f(x+significant_width/2.,y-significant_height/2.);
			glVertex2f(x+significant_width/2.,y+significant_height/2.);
			glVertex2f(x-significant_width/2.,y+significant_height/2.);
			glVertex2f(x-significant_width/2.,y-significant_height/2.);
			glEnd();

	

			//glColor4f(dataCol[j][i][0],dataCol[j][i][1],dataCol[j][i][2],dataCol[j][i][3]);
			glColor4f(initcolor[0][color_index][0],initcolor[0][color_index][1],
				initcolor[0][color_index][2],1.);
			//pow square
			glBegin(GL_QUADS);
			glVertex2f(x+pow_width/2.,y-pow_height/2.);
			glVertex2f(x+pow_width/2.,y+pow_height/2.);
			glVertex2f(x-pow_width/2.,y+pow_height/2.);
			glVertex2f(x-pow_width/2.,y-pow_height/2.);
			glEnd();
		}
		

        glColor4f(0.5,0.5,0.5,0.5);
		//halo
		glBegin(GL_LINE_LOOP);
		glVertex2f(x+pow_width/2.,y-pow_height/2.);
		glVertex2f(x+pow_width/2.,y+pow_height/2.);
		glVertex2f(x-pow_width/2.,y+pow_height/2.);
		glVertex2f(x-pow_width/2.,y-pow_height/2.);
		glEnd();

	}
	}
    glPopMatrix();
    glLineWidth(1.);


}
void svQDOT2dmapping::RenderLengthColor(svVector3Array *pos,svScalarArray *vel,
		svScalarArray *exp, svScalarArray *coe,
		int zlayer,
		float bw, float bh)
{
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHT1);

	block_width = bw;
	block_height = bh;

	float significant_width;
	float pow_width;
	float significant_height;
	float pow_height;

	float x,y;

    glPushMatrix();
  //  glScalef(scale_value,scale_value,1);

    glLineWidth(0.2);

	for(int j=0;j<zlayer;j++)
	{
	for(int i=0;i<pos[j].size();i++)
	{
		float tmpcoe = coe[j][i];
		float tmpexp = exp[j][i];
		tmpcoe = tmpcoe/11.;
		tmpexp = tmpexp/11.;
		if(tmpcoe<0)tmpcoe=0;
		if(tmpexp<0)tmpexp=0;
	
        significant_height = block_height/1.7;
        pow_height = block_height;
        significant_width = tmpcoe * block_width;
        pow_width = tmpexp *block_width;

		x = pos[j][i][0]-block_width/2.;
		y = pos[j][i][1];

		int color_index;
		if(vel[j][i]>=0)
			color_index = (vel[j][i]/PI * 180.)/10;//cerr<<vel[j][i]<<" "<<color_index<<endl;
		else
			color_index = 0;

			//glColor4f(dataCol[j][i][0],dataCol[j][i][1],dataCol[j][i][2],dataCol[j][i][3]);
			glColor4f(initcolor[0][color_index][0],initcolor[0][color_index][1],
				initcolor[0][color_index][2],1.);
			//pow square
			glBegin(GL_QUADS);
			glVertex2f(x+pow_width,y-pow_height/2.);
			glVertex2f(x+pow_width,y+pow_height/2.);
			glVertex2f(x,y+pow_height/2.);
			glVertex2f(x,y-pow_height/2.);
			glEnd();

           		 glColor4f(0.5,0.5,0.5,0.5);
			//significant square
			glBegin(GL_LINE_LOOP);
			glVertex2f(x+significant_width,y-significant_height/2.);
			glVertex2f(x+significant_width,y+significant_height/2.);
			glVertex2f(x,y+significant_height/2.);
			glVertex2f(x,y-significant_height/2.);
			glEnd();
glEnd();
		
		

        glColor4f(0.5,0.5,0.5,0.5);
		//halo
		glBegin(GL_LINE_LOOP);
		glVertex2f(x+pow_width,y-pow_height/2.);
		glVertex2f(x+pow_width,y+pow_height/2.);
		glVertex2f(x,y+pow_height/2.);
		glVertex2f(x,y-pow_height/2.);
		glEnd();

	}
	}

    glLineWidth(1.);

    glPopMatrix();
}
void svQDOT2dmapping::RenderSizeLightness(svVector3Array *pos,svScalarArray *vel,
		svScalarArray *exp, svScalarArray *coe,
		int zlayer,
		float bw, float bh)
{
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHT1);

	block_width = bw;
	block_height = bh;

	float significant_width;
	float pow_width;
	float significant_height;
	float pow_height;

	float x,y;

    glPushMatrix();
  //  glScalef(scale_value,scale_value,1);

    glLineWidth(0.2);
	
	for(int j=0;j<zlayer;j++)
	{
	for(int i=0;i<pos[j].size();i++)
	{
		float tmpcoe = coe[j][i];
		float tmpexp = exp[j][i];
		tmpcoe = tmpcoe/11.;
		tmpexp = tmpexp/11.;
		if(tmpcoe<0)tmpcoe=0;
		if(tmpexp<0)tmpexp=0;
		

        significant_height = sqrt(tmpcoe) * block_height;
        pow_height = sqrt(tmpexp) * block_height;
        significant_width = sqrt(tmpcoe) * block_width;
        pow_width = sqrt(tmpexp) *block_width;

		x = pos[j][i][0];
		y = pos[j][i][1];

	//	int dir = (int)(vel[j][i]/PI * 180./10.);
	//	float lightness = 1-(float)dir/18.;

		int color_index;
		if(vel[j][i]>=0)
			color_index = (vel[j][i]/PI * 180.)/10;//cerr<<vel[j][i]<<" "<<color_index<<endl;
		else
			color_index = 0;

		//bars of significant
		if(exp[j][i]>coe[j][i])
		{

			//glColor4f(lightness,lightness,lightness,1);
			glColor4f(initlightness[0][color_index][0],initlightness[0][color_index][1],
				initlightness[0][color_index][2],1.);
			//pow square
			glBegin(GL_QUADS);
			glVertex2f(x+pow_width/2.,y-pow_height/2.);
			glVertex2f(x+pow_width/2.,y+pow_height/2.);
			glVertex2f(x-pow_width/2.,y+pow_height/2.);
			glVertex2f(x-pow_width/2.,y-pow_height/2.);
			glEnd();

            glColor4f(0.5,0.5,0.5,0.5);
			//significant square
			glBegin(GL_LINE_LOOP);
			glVertex2f(x+significant_width/2.,y-significant_height/2.);
			glVertex2f(x+significant_width/2.,y+significant_height/2.);
			glVertex2f(x-significant_width/2.,y+significant_height/2.);
			glVertex2f(x-significant_width/2.,y-significant_height/2.);
			glEnd();
		}
		else
		{
            glColor4f(0.5,0.5,0.5,0.5);
			//significant square
			glBegin(GL_LINE_LOOP);
			glVertex2f(x+significant_width/2.,y-significant_height/2.);
			glVertex2f(x+significant_width/2.,y+significant_height/2.);
			glVertex2f(x-significant_width/2.,y+significant_height/2.);
			glVertex2f(x-significant_width/2.,y-significant_height/2.);
			glEnd();

			glColor4f(initlightness[0][color_index][0],initlightness[0][color_index][1],
				initlightness[0][color_index][2],1.);
			//glColor4f(lightness,lightness,lightness,1);
			//pow square
			glBegin(GL_QUADS);
			glVertex2f(x+pow_width/2.,y-pow_height/2.);
			glVertex2f(x+pow_width/2.,y+pow_height/2.);
			glVertex2f(x-pow_width/2.,y+pow_height/2.);
			glVertex2f(x-pow_width/2.,y-pow_height/2.);
			glEnd();
		}
		

        glColor4f(0.5,0.5,0.5,0.5);
		//halo
		glBegin(GL_LINE_LOOP);
		glVertex2f(x+pow_width/2.,y-pow_height/2.);
		glVertex2f(x+pow_width/2.,y+pow_height/2.);
		glVertex2f(x-pow_width/2.,y+pow_height/2.);
		glVertex2f(x-pow_width/2.,y-pow_height/2.);
		glEnd();

	}
	}

    glLineWidth(1.);

    glPopMatrix();
}
void svQDOT2dmapping::RenderLengthLightness(svVector3Array *pos,svScalarArray *vel,
		svScalarArray *exp, svScalarArray *coe,
		int zlayer,
		float bw, float bh)
{
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHT1);

	block_width = bw;
	block_height = bh;

	float significant_width;
	float pow_width;
	float significant_height;
	float pow_height;

	float x,y;

    glPushMatrix();
  //  glScalef(scale_value,scale_value,1);

    glLineWidth(0.2);
	
	for(int j=0;j<zlayer;j++)
	{
	for(int i=0;i<pos[j].size();i++)
	{
		float tmpcoe = coe[j][i];
		float tmpexp = exp[j][i];
		tmpcoe = tmpcoe/11.;
		tmpexp = tmpexp/11.;
		if(tmpcoe<0)tmpcoe=0;
		if(tmpexp<0)tmpexp=0;

        significant_height = block_height/1.7;
        pow_height = block_height;
        significant_width = tmpcoe * block_width;
        pow_width = tmpexp *block_width;

		x = pos[j][i][0]-block_width/2.;
		y = pos[j][i][1];

		//int dir = (int)(vel[j][i] * 180./10.);
		//float lightness = 1-(float)dir/18.;
		int color_index;
		if(vel[j][i]>=0)
			color_index = (vel[j][i]/PI * 180.)/10;//cerr<<vel[j][i]<<" "<<color_index<<endl;
		else
			color_index = 0;

			//glColor4f(lightness,lightness,lightness,1);
			glColor4f(initlightness[0][color_index][0],initlightness[0][color_index][1],
				initlightness[0][color_index][2],1.);
			//pow square
			glBegin(GL_QUADS);
			glVertex2f(x+pow_width,y-pow_height/2.);
			glVertex2f(x+pow_width,y+pow_height/2.);
			glVertex2f(x,y+pow_height/2.);
			glVertex2f(x,y-pow_height/2.);
			glEnd();

           		 glColor4f(0.5,0.5,0.5,0.5);
			//significant square
			glBegin(GL_LINE_LOOP);
			glVertex2f(x+significant_width,y-significant_height/2.);
			glVertex2f(x+significant_width,y+significant_height/2.);
			glVertex2f(x,y+significant_height/2.);
			glVertex2f(x,y-significant_height/2.);
			glEnd();

		

        	glColor4f(0.5,0.5,0.5,0.5);
		//halo
		glBegin(GL_LINE_LOOP);
		glVertex2f(x+pow_width,y-pow_height/2.);
		glVertex2f(x+pow_width,y+pow_height/2.);
		glVertex2f(x,y+pow_height/2.);
		glVertex2f(x,y-pow_height/2.);
		glEnd();

	}
	}

    glLineWidth(1.);

    glPopMatrix();
}
void svQDOT2dmapping::RenderSizeOrientation(svVector3Array *pos,svScalarArray *vel,
		svScalarArray *exp, svScalarArray *coe,
		int zlayer,
		float bw, float bh)
{
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHT1);

	block_width = bw;
	block_height = bh;

    float significant_width;
    float pow_width;
    float significant_height;
    float pow_height;

    float x,y;

    glPushMatrix();
   // glScalef(scale_value,scale_value,1);

    glLineWidth(0.2);

	for(int j=0;j<zlayer;j++)
	{
    for(int i=0;i<pos[j].size();i++)
    {
		float tmpcoe = coe[j][i];
		float tmpexp = exp[j][i];
		tmpcoe = tmpcoe/11.;
		tmpexp = tmpexp/11.;
		if(tmpcoe<0)tmpcoe=0;
		if(tmpexp<0)tmpexp=0;
		

        significant_height = sqrt(tmpcoe) * block_height;
        pow_height = sqrt(tmpexp) * block_height;
        significant_width = sqrt(tmpcoe) * block_width;
        pow_width = sqrt(tmpexp) *block_width;

        int dir;	
	if(vel[j][i]>=0)
        dir = (int)(vel[j][i]/PI * 180./10.);
	else
	dir= 0;

        x = pos[j][i][0];
        y = pos[j][i][1];


        //bars of significant
        glColor4f(0.3,0.3,0.3,0.5);
        svVector3 side;

        if(dir<9)
        {
	float tmptan = pow_height/pow_width;
		tmptan = tmptan/2.;
	float tana = tan((float)dir * 10./180 * PI);
		if(tmptan>=tana)
            {
            side[1] = pow_width/2. * tan((float)dir * 10./180 * PI)
                    + y - pow_height/2.;
            side[0] = x + pow_width/2.;

            glBegin(GL_POLYGON);
            glVertex2f(x,y-pow_height/2.);
            glVertex2f(x+pow_width/2.,y-pow_height/2.);
            glVertex2f(side[0],side[1]);
            glEnd();
		}
		else
		{

            side[1] = y + pow_height/2.;
            side[0] = x + pow_height/tan((float)dir * 10./180 * PI);

            glBegin(GL_POLYGON);
            glVertex2f(x,y-pow_height/2.);
            glVertex2f(x+pow_width/2.,y-pow_height/2.);
             glVertex2f(x+pow_width/2.,y+pow_height/2.);
            glVertex2f(side[0],side[1]);
            glEnd();}
        }
        else if(dir ==9)
        {
            side[0] = x;
            side[1] = y + pow_height/2.;
            glBegin(GL_POLYGON);
            glVertex2f(x,y-pow_height/2.);
            glVertex2f(x+pow_width/2.,y-pow_height/2.);
            glVertex2f(side[0],side[1]);
            glVertex2f(x,y+pow_height/2.);
            glEnd();
        }
        else if(dir<=18)
        {

		float tmptan = pow_height/pow_width;
		tmptan = tmptan/2;
		float tana = tan(PI - (float)dir * 10./180 * PI);
		if(tmptan>tana)
		{
		    side[1] = y + pow_height;
		    side[0] = x - pow_height * tana;
		    glBegin(GL_POLYGON);
		    glVertex2f(x,y-pow_height/2.);
		    glVertex2f(x+pow_width/2.,y-pow_height/2.);
		    glVertex2f(x+pow_width/2.,y+pow_height/2.);
		    glVertex2f(side[0],side[1]);
		    glEnd();
		}
		else
		{
		    side[1] = pow_width/2. / tana
		            + y - pow_height/2.;
		    side[0] = x - pow_width/2.;
		    glBegin(GL_POLYGON);
		    glVertex2f(x,y-pow_height/2.);
		    glVertex2f(x+pow_width/2.,y-pow_height/2.);
		    glVertex2f(x+pow_width/2.,y+pow_height/2.);
		    glVertex2f(x-pow_width/2.,y+pow_height/2.);
		    glVertex2f(side[0],side[1]);
		    glEnd();
		}
        }



            glColor4f(0.3,0.3,0.3,1.);
            //significant square
            glBegin(GL_LINE_LOOP);
            glVertex2f(x+significant_width/2.,y-significant_height/2.);
            glVertex2f(x+significant_width/2.,y+significant_height/2.);
            glVertex2f(x-significant_width/2.,y+significant_height/2.);
            glVertex2f(x-significant_width/2.,y-significant_height/2.);
            glEnd();


        glColor4f(0.3,0.3,0.3,1.);
        //halo
        glBegin(GL_LINE_LOOP);
        glVertex2f(x+pow_width/2.,y-pow_height/2.);
        glVertex2f(x+pow_width/2.,y+pow_height/2.);
        glVertex2f(x-pow_width/2.,y+pow_height/2.);
        glVertex2f(x-pow_width/2.,y-pow_height/2.);
        glEnd();
    }
	}
    glLineWidth(1.);

    glPopMatrix();
}
void svQDOT2dmapping::RenderLengthOrientation(svVector3Array *pos,svScalarArray *vel,
		svScalarArray *exp, svScalarArray *coe,
		int zlayer,
		float bw, float bh)
{
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHT1);

	block_width = bw;
	block_height = bh;

    float significant_width;
    float pow_width;
    float significant_height;
    float pow_height;

    float x,y;

    glPushMatrix();
   // glScalef(scale_value,scale_value,1);

    glLineWidth(0.2);

	for(int j=0;j<zlayer;j++)
	{
    for(int i=0;i<pos[j].size();i++)
    {
		float tmpcoe = coe[j][i];
		float tmpexp = exp[j][i];
		tmpcoe = tmpcoe/11.;
		tmpexp = tmpexp/11.;
		if(tmpcoe<0)tmpcoe=0;
		if(tmpexp<0)tmpexp=0;

        significant_height = block_height/1.7;
        pow_height = block_height;
        significant_width = tmpcoe * block_width;
        pow_width = tmpexp *block_width;

        int dir;
	if(vel[j][i]>=0)
		dir = (int)(vel[j][i]/PI * 180./10.);
	else
		dir = 0;

        x = pos[j][i][0]-block_width/2.;
        y = pos[j][i][1];

	float tana = tan((float)dir * 10./180 * PI);


        //bars of significant
        glColor4f(0.3,0.3,0.3,0.5);
        svVector3 side;

        if(dir<9)
        {
		float tmptan = pow_height/pow_width;
		tmptan = tmptan/2.;
		if(tmptan>=tana)
            {side[1] = y- pow_height/2.;
            side[0] = pow_height/2. * tana
			+ x;

            glBegin(GL_TRIANGLES);
            glVertex2f(x,y);
            glVertex2f(x,y-pow_height/2.);
            glVertex2f(side[0],side[1]);
            glEnd();
		}
		else
		{

            side[1] = y - pow_width/tana;
            side[0] = x + pow_width;

            glBegin(GL_QUADS);
            glVertex2f(x,y);
            glVertex2f(x,y-pow_height/2.);
             glVertex2f(x+pow_width,y-pow_height/2.);
            glVertex2f(side[0],side[1]);
            glEnd();
		}
        }
        else if(dir ==9)
        {
            side[0] = x+pow_width;
            side[1] = y;

            glBegin(GL_QUADS);
            glVertex2f(x,y);
            glVertex2f(x,y-pow_height/2.);
            glVertex2f(x+pow_width,y-pow_height/2.);
            glVertex2f(side[0],side[1]);
            glEnd();
        }
        else if(dir<=18)
        {
		float tmptan = pow_height/pow_width;
		tmptan = tmptan/2.;
		tana = tan(PI - (float)dir * 10./180 * PI);
		if(tmptan>tana)
		{
	            side[1] = pow_width * tana + y;
            		side[0] = x + pow_width;
			
			glBegin(GL_QUADS);
			glVertex2f(x,y);
			glVertex2f(x,y-pow_height/2.);
			glVertex2f(x + pow_width, y-pow_height/2.);
			glVertex2f(side[0],side[1]);
			glEnd();
		}
		else
		if(tmptan>tana)
		{
	            side[1] = y + pow_height/2.;
            		side[0] = x + (pow_height/2.)/tana;
			
			glBegin(GL_POLYGON);
			glVertex2f(x,y);
			glVertex2f(x,y-pow_height/2.);
			glVertex2f(x + pow_width, y-pow_height/2.);
			glVertex2f(x + pow_width, y+pow_height/2.);
			glVertex2f(side[0],side[1]);
			glEnd();
		}
        }



            glColor4f(0.3,0.3,0.3,1.);
            //significant square
            glBegin(GL_LINE_LOOP);
            glVertex2f(x+significant_width,y-significant_height/2.);
            glVertex2f(x+significant_width,y+significant_height/2.);
            glVertex2f(x,y+significant_height/2.);
            glVertex2f(x,y-significant_height/2.);
            glEnd();


        glColor4f(0.3,0.3,0.3,1.);
        //halo
        glBegin(GL_LINE_LOOP);
        glVertex2f(x+pow_width,y-pow_height/2.);
        glVertex2f(x+pow_width,y+pow_height/2.);
        glVertex2f(x,y+pow_height/2.);
        glVertex2f(x,y-pow_height/2.);
        glEnd();
    }
	}
    glLineWidth(1.);

    glPopMatrix();
}
void svQDOT2dmapping::RenderColorColor(svVector3Array *pos,svScalarArray *vel,
		svScalarArray *exp, svScalarArray *coe,
		int zlayer,
		float bw, float bh)
{
   glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHT1);

	block_width = bw;
	block_height = bh;

    float significant_width;
    float pow_width;
    float significant_height;
    float pow_height;
	float orientation_height;
	float orientation_width;

    float x,y;

    glPushMatrix();
   // glScalef(scale_value,scale_value,1);

    glLineWidth(0.2);

        significant_height = block_height;
        pow_height = block_height*5./6.;
        significant_width =block_width;
        pow_width = block_width*5./6.;
	orientation_height = block_height * 2.5/6.;
	orientation_width = block_width * 2.5/6.;

	for(int j=0;j<zlayer;j++)
	{
    for(int i=0;i<pos[j].size();i++)
    {

        x = pos[j][i][0];
        y = pos[j][i][1];
		
		int color_index_1;
		if(vel[j][i]>=0)
			color_index_1 = vel[j][i]/PI * 180/10;
		else
			color_index_1 = 0;
		
		int color_index_2;
		if(exp[j][i]>=0)
			color_index_2 = exp[j][i];
		else
			color_index_2 = 0;

		int color_index_3;
		if(coe[j][i]>=0)
			color_index_3 = coe[j][i];
		else
			color_index_3 = 0;

	    //glColor4f(dataCol[j][i][0],dataCol[j][i][1],dataCol[j][i][2],dataCol[j][i][3]);
	glColor4f(initcolor[0][color_index_1][0],initcolor[0][color_index_1][1],
				initcolor[0][color_index_1][2],1.);
            glBegin(GL_QUADS);
            glVertex2f(x+orientation_width/2.,y-orientation_height/2.);
            glVertex2f(x+orientation_width/2.,y+orientation_height/2.);
            glVertex2f(x-orientation_width/2.,y+orientation_height/2.);
            glVertex2f(x-orientation_width/2.,y-orientation_height/2.);
            glEnd();

	    //glColor4f(dataExpCol[j][i][0],dataExpCol[j][i][1],dataExpCol[j][i][2],dataExpCol[j][i][3]);
	glColor4f(initcolor[0][color_index_2][0],initcolor[0][color_index_2][1],
				initcolor[0][color_index_2][2],1.);
            glBegin(GL_QUADS);
	    glVertex2f(x-pow_width/2.,y+orientation_height/2.);
            glVertex2f(x+pow_width/2.,y+orientation_height/2.);
            glVertex2f(x+pow_width/2.,y+pow_height/2.);
            glVertex2f(x-pow_width/2.,y+pow_height/2.);
            glEnd();

            glBegin(GL_QUADS);
            glVertex2f(x-pow_width/2.,y-pow_height/2.);
            glVertex2f(x+pow_width/2.,y-pow_height/2.);
            glVertex2f(x+pow_width/2.,y-orientation_height/2.);
	    glVertex2f(x-pow_width/2.,y-orientation_height/2.);
            glEnd();

            glBegin(GL_QUADS);
            glVertex2f(x-pow_width/2.,y-orientation_height/2.);
            glVertex2f(x-orientation_width/2.,y-orientation_height/2.);
            glVertex2f(x-orientation_width/2.,y+orientation_height/2.);
	    glVertex2f(x-pow_width/2.,y+orientation_height/2.);
            glEnd();

            glBegin(GL_QUADS);
            glVertex2f(x+pow_width/2.,y-orientation_height/2.);
	    glVertex2f(x+pow_width/2.,y+orientation_height/2.);
            glVertex2f(x+orientation_width/2.,y+orientation_height/2.);
            glVertex2f(x+orientation_width/2.,y-orientation_height/2.);
            glEnd();

	    //glColor4f(dataSigCol[j][i][0],dataSigCol[j][i][1],dataSigCol[j][i][2],dataSigCol[j][i][3]);
	glColor4f(initcolor[0][color_index_3][0],initcolor[0][color_index_3][1],
				initcolor[0][color_index_3][2],1.);
            glBegin(GL_QUADS);
	    glVertex2f(x-significant_width/2.,y+pow_height/2.);
            glVertex2f(x+significant_width/2.,y+pow_height/2.);
            glVertex2f(x+significant_width/2.,y+significant_height/2.);
            glVertex2f(x-significant_width/2.,y+significant_height/2.);
            glEnd();

            glBegin(GL_QUADS);
            glVertex2f(x-significant_width/2.,y-significant_height/2.);
            glVertex2f(x+significant_width/2.,y-significant_height/2.);
            glVertex2f(x+significant_width/2.,y-pow_height/2.);
	    glVertex2f(x-significant_width/2.,y-pow_height/2.);
            glEnd();

            glBegin(GL_QUADS);
            glVertex2f(x-significant_width/2.,y-pow_height/2.);
            glVertex2f(x-pow_width/2.,y-pow_height/2.);
            glVertex2f(x-pow_width/2.,y+pow_height/2.);
	    glVertex2f(x-significant_width/2.,y+pow_height/2.);
            glEnd();

            glBegin(GL_QUADS);
            glVertex2f(x+significant_width/2.,y-pow_height/2.);
	    glVertex2f(x+significant_width/2.,y+pow_height/2.);
            glVertex2f(x+pow_width/2.,y+pow_height/2.);
            glVertex2f(x+pow_width/2.,y-pow_height/2.);
            glEnd();
    }
	}

	glColor4f(0.5,0.5,0.5,1.);
        glBegin(GL_LINE_LOOP);
        glVertex2f(x+significant_width/2.,y-significant_height/2.);
        glVertex2f(x+significant_width/2.,y+significant_height/2.);
        glVertex2f(x-significant_width/2.,y+significant_height/2.);
        glVertex2f(x-significant_width/2.,y-significant_height/2.);
        glEnd();

    glLineWidth(1.);

    glPopMatrix();
}

void svQDOT2dmapping::RenderMarker()
{
    float x, y;

    float degree=2;

    glPushMatrix();
   // glScalef(scale_value,scale_value,1);

    if(selected_index1!=-1)
    {
        x = dataPos[selected_index1][0];
        y = dataPos[selected_index1][1]+block_height/degree;

        glColor4f(1.,0.,0.,0.5);
        glBegin(GL_TRIANGLES);
        glVertex2f(x,y-block_height/degree);
        glVertex2f(x-block_width/(degree*2),y-block_width/(degree*2) * 1.7-block_height/degree);
        glVertex2f(x+block_width/(degree*2),y-block_width/(degree*2) * 1.7-block_height/degree);
        glEnd();
        glBegin(GL_LINES);
        glVertex2f(x,y-block_width/(degree*2) * 1.7-block_height/degree);
        glVertex2f(x,y-block_width/(degree*2) * 3.4-block_height/degree);
        glEnd();

       //cerr<<y<<" "<<y-block_height/2.<<" "<<y-block_width/6. * 1.7-block_height/2.<<endl;
        //cerr<<block_width/6. * 1.7<<" "<<block_height/2.<<endl;
    }

    if(selected_index2!=-1)
    {
        x = dataPos[selected_index2][0];
        y = dataPos[selected_index2][1];

        glColor4f(1.,0.,0.,0.5);
        glBegin(GL_TRIANGLES);
        glVertex2f(x,y-block_height/degree);
        glVertex2f(x-block_width/(degree*2),y-block_width/(degree*2) * 1.7-block_height/degree);
        glVertex2f(x+block_width/(degree*2),y-block_width/(degree*2) * 1.7-block_height/degree);
        glEnd();
    }
    glPopMatrix();
}

void svQDOT2dmapping::RenderOrientationLegend()
{
	float x, y;

	x = 0;y = 0;

	float distance = 1.5;
	char str[50];

	
	glColor4f(0.5,0.5,0.5,0.5);

		glPushMatrix();
		//glScalef(scale_value,scale_value,1);

		glBegin(GL_LINE_LOOP);
		glVertex2f(0.,0.);
		glVertex2f(1.,0.);
		glVertex2f(1.,0.5);
		glVertex2f(0.,0.5);
		glEnd();

		glBegin(GL_LINE_LOOP);
		glVertex2f(0.25,0.125);
		glVertex2f(0.75,0.125);
		glVertex2f(0.75,0.375);
		glVertex2f(0.25,0.375);
		glEnd();

		glBegin(GL_TRIANGLES);
		glVertex2f(.5,0.);
		glVertex2f(1.,0.);
		glVertex2f(1., tan(40./180. * 3.1415926) * 0.5);
		glEnd();		
		
	glColor3f(0.,0.,0.);
		sprintf(str, "angle: orientation");
		glRasterPos2f(1.,0.4);

		for (int j = 0; j < strlen(str); j++) 
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,str[j]);
		}
		glPopMatrix();
		
		sprintf(str, "size of rectangle associated with angle: exponent");
		glRasterPos2f(1,0.);
		for (int j = 0; j < strlen(str); j++) 
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,str[j]);
		}

		sprintf(str, "size of another rectangle: significand");
		glRasterPos2f(0.75,0.125);
		for (int j = 0; j < strlen(str); j++) 
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,str[j]);
		}
		//glPopMatrix();
	
}

void svQDOT2dmapping::RenderSizeLegend()
{
	float x, y;

	x = 0;y = 0;

	float distance = 1.5;
	char str[50];

	
	glColor4f(0,1.,0.,0.75);

		glPushMatrix();
		//glScalef(scale_value,scale_value,1);

		glBegin(GL_QUADS);
		glVertex2f(0.,0.);
		glVertex2f(1.,0.);
		glVertex2f(1.,0.5);
		glVertex2f(0.,0.5);
		glEnd();

		glColor3f(0.5,0.5,0.5);
		glBegin(GL_LINE_LOOP);
		glVertex2f(0.25,0.125);
		glVertex2f(0.75,0.125);
		glVertex2f(0.75,0.375);
		glVertex2f(0.25,0.375);
		glEnd();		
		
	glColor3f(0.,0.,0.);
		sprintf(str, "color: orientation");
		glRasterPos2f(1.,0.4);

		for (int j = 0; j < strlen(str); j++) 
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,str[j]);
		}
		glPopMatrix();
		
		sprintf(str, "size of rectangle associated with color: exponent");
		glRasterPos2f(1,0.);
		for (int j = 0; j < strlen(str); j++) 
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,str[j]);
		}

		sprintf(str, "size of another rectangle: significand");
		glRasterPos2f(0.75,0.125);
		for (int j = 0; j < strlen(str); j++) 
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,str[j]);
		}
	
}
void svQDOT2dmapping::RenderSizeLegend2()
{
	float x, y;

	x = 0;y = 0;

	float distance = 1.5;
	char str[50];

	
	glColor4f(0.5,0.5,0.5,0.75);

		glPushMatrix();
		//glScalef(scale_value,scale_value,1);

		glBegin(GL_QUADS);
		glVertex2f(0.,0.);
		glVertex2f(1.,0.);
		glVertex2f(1.,0.5);
		glVertex2f(0.,0.5);
		glEnd();

		glColor3f(0.5,0.5,0.5);
		glBegin(GL_LINE_LOOP);
		glVertex2f(0.25,0.125);
		glVertex2f(0.75,0.125);
		glVertex2f(0.75,0.375);
		glVertex2f(0.25,0.375);
		glEnd();		
		
	glColor3f(0.,0.,0.);
		sprintf(str, "brightness: orientation");
		glRasterPos2f(1.,0.4);

		for (int j = 0; j < strlen(str); j++) 
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,str[j]);
		}
		glPopMatrix();
		
		sprintf(str, "size of rectangle associated with color: exponent");
		glRasterPos2f(1,0.);
		for (int j = 0; j < strlen(str); j++) 
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,str[j]);
		}

		sprintf(str, "size of another rectangle: significand");
		glRasterPos2f(0.75,0.125);
		for (int j = 0; j < strlen(str); j++) 
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,str[j]);
		}
	
}
void svQDOT2dmapping::RenderColorLegend()
{
   float significant_width;
    float pow_width;
    float significant_height;
    float pow_height;
	float orientation_height;
	float orientation_width;

    float x,y;

   // glScalef(scale_value,scale_value,1);

    glLineWidth(0.2);

        significant_height = block_height;
        pow_height = block_height*5./6.;
        significant_width =block_width;
        pow_width = block_width*5./6.;
	orientation_height = block_height * 2.5/6.;
	orientation_width = block_width * 2.5/6.;

x = 0.5;
y = 0.25;

//	    glColor4f(dataCol[0][0],dataCol[0][1],dataCol[0][2],dataCol[0][3]);
            glBegin(GL_QUADS);
            glVertex2f(x+orientation_width/2.,y-orientation_height/2.);
            glVertex2f(x+orientation_width/2.,y+orientation_height/2.);
            glVertex2f(x-orientation_width/2.,y+orientation_height/2.);
            glVertex2f(x-orientation_width/2.,y-orientation_height/2.);
            glEnd();

//	    glColor4f(dataExpCol[0][0],dataExpCol[0][1],dataExpCol[0][2],dataExpCol[0][3]);
            glBegin(GL_QUADS);
	    glVertex2f(x-pow_width/2.,y+orientation_height/2.);
            glVertex2f(x+pow_width/2.,y+orientation_height/2.);
            glVertex2f(x+pow_width/2.,y+pow_height/2.);
            glVertex2f(x-pow_width/2.,y+pow_height/2.);
            glEnd();

            glBegin(GL_QUADS);
            glVertex2f(x-pow_width/2.,y-pow_height/2.);
            glVertex2f(x+pow_width/2.,y-pow_height/2.);
            glVertex2f(x+pow_width/2.,y-orientation_height/2.);
	    glVertex2f(x-pow_width/2.,y-orientation_height/2.);
            glEnd();

            glBegin(GL_QUADS);
            glVertex2f(x-pow_width/2.,y-orientation_height/2.);
            glVertex2f(x-orientation_width/2.,y-orientation_height/2.);
            glVertex2f(x-orientation_width/2.,y+orientation_height/2.);
	    glVertex2f(x-pow_width/2.,y+orientation_height/2.);
            glEnd();

            glBegin(GL_QUADS);
            glVertex2f(x+pow_width/2.,y-orientation_height/2.);
	    glVertex2f(x+pow_width/2.,y+orientation_height/2.);
            glVertex2f(x+orientation_width/2.,y+orientation_height/2.);
            glVertex2f(x+orientation_width/2.,y-orientation_height/2.);
            glEnd();

//	    glColor4f(dataSigCol[0][0],dataSigCol[0][1],dataSigCol[0][2],dataSigCol[0][3]);
            glBegin(GL_QUADS);
	    glVertex2f(x-significant_width/2.,y+pow_height/2.);
            glVertex2f(x+significant_width/2.,y+pow_height/2.);
            glVertex2f(x+significant_width/2.,y+significant_height/2.);
            glVertex2f(x-significant_width/2.,y+significant_height/2.);
            glEnd();

            glBegin(GL_QUADS);
            glVertex2f(x-significant_width/2.,y-significant_height/2.);
            glVertex2f(x+significant_width/2.,y-significant_height/2.);
            glVertex2f(x+significant_width/2.,y-pow_height/2.);
	    glVertex2f(x-significant_width/2.,y-pow_height/2.);
            glEnd();

            glBegin(GL_QUADS);
            glVertex2f(x-significant_width/2.,y-pow_height/2.);
            glVertex2f(x-pow_width/2.,y-pow_height/2.);
            glVertex2f(x-pow_width/2.,y+pow_height/2.);
	    glVertex2f(x-significant_width/2.,y+pow_height/2.);
            glEnd();

            glBegin(GL_QUADS);
            glVertex2f(x+significant_width/2.,y-pow_height/2.);
	    glVertex2f(x+significant_width/2.,y+pow_height/2.);
            glVertex2f(x+pow_width/2.,y+pow_height/2.);
            glVertex2f(x+pow_width/2.,y-pow_height/2.);
            glEnd();


	glColor4f(0.5,0.5,0.5,0.75);
        glBegin(GL_LINE_LOOP);
        glVertex2f(x+significant_width/2.,y-significant_height/2.);
        glVertex2f(x+significant_width/2.,y+significant_height/2.);
        glVertex2f(x-significant_width/2.,y+significant_height/2.);
        glVertex2f(x-significant_width/2.,y-significant_height/2.);
        glEnd();

	char str[50];

	glColor3f(0.,0.,0.);
		sprintf(str, "color in the center: orientation");
		glRasterPos2f(0.5,0.4);

		for (int j = 0; j < strlen(str); j++) 
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,str[j]);
		}

		
		sprintf(str, "color in the middle: exponent");
		glRasterPos2f(0.75,0.125);
		for (int j = 0; j < strlen(str); j++) 
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,str[j]);
		}

		sprintf(str, "color outside: significand");
		glRasterPos2f(1,0);
		for (int j = 0; j < strlen(str); j++) 
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,str[j]);
		}

    glLineWidth(1.);

}
void svQDOT2dmapping::RenderBrightLegend()
{
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHT1);
    glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);

    float x,y;

    x =0.;y=0.;

    float degree = 1.;

    char str[20];

    for(int i=0;i<11;i++)
    {
        glBegin(GL_QUADS);
        //glColor3f(spiralcol[(i+5)*4][0],spiralcol[(i+5)*4][1],spiralcol[(i+5)*4][2]);
        glColor3f(1-(float)i/18,1-(float)i/18,1-(float)i/18);
        glVertex2f(x,y);
        glVertex2f(x+degree,y);
        glVertex2f(x+degree,y+degree);
        glVertex2f(x,y+degree);
        glEnd();

        glColor3f(0.,0.,0.);
	if(i==0||i==5||i==10||i==15||i==18)
        {    glRasterPos2f(x, y-degree);
            sprintf(str,"%d",i * (int)180/18);
            for(int j=0;j<strlen(str);j++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, str[j]);}
        x = x + degree;
    }

    glBegin(GL_LINE_LOOP);
    glColor3f(0.5,0.5,0.5);
    glVertex2f(0,0);
    glVertex2f(x,0);
    glVertex2f(x,degree);
    glVertex2f(0,degree);
    glEnd();

}
void svQDOT2dmapping::RenderBarsLegend(float scale)
{
    float x, y;
    float degreey = block_height*2;
    float degreex = block_width*2;
	float significant_width;
	float pow_width;
	float orientation_width;
	float significant_height;
	float pow_height;
	float orientation_height;

char str[50];
    glDisable(GL_DEPTH_TEST);

x = 0.5;y=0.25;
    pow_height = block_height/2.;
	significant_height = pow_height/1.7;
	orientation_height = pow_height;

		significant_width = 0.5 * block_width;
		pow_width = 0.8 * block_width;
        orientation_width = 0.4*block_width;

		glColor4f(0.5,0.5,0.5,1.);
			glBegin(GL_QUADS);
			glVertex2f(x-block_width/2.,y);
			glVertex2f(x-block_width/2.+pow_width,y);
			glVertex2f(x-block_width/2.+pow_width,y+pow_height);
			glVertex2f(x-block_width/2.,y+pow_height);
			glEnd();

		glBegin(GL_QUADS);
		glVertex2f(x-block_width/2.,y-block_height/2.);
		glVertex2f(x-block_width/2.+orientation_width,y-block_height/2.);
		glVertex2f(x-block_width/2.+orientation_width,y);
		glVertex2f(x-block_width/2.,y);
		glEnd();

		glColor4f(0.,0.,0., 0.75);

		glBegin(GL_LINE_LOOP);
		glVertex2f(x-block_width/2.,y+(pow_height-significant_height)/2.);
		glVertex2f(x-block_width/2.+significant_width,y+(pow_height-significant_height)/2.);
		glVertex2f(x-block_width/2.+significant_width,y+(pow_height-significant_height)/2.+significant_height);
		glVertex2f(x-block_width/2.,y+(pow_height-significant_height)/2.+significant_height);
		glEnd();

		glBegin(GL_LINE_LOOP);
		glVertex2f(x+block_width/2.,y-block_height/2.);
		glVertex2f(x+block_width/2.,y+block_height/2.);
		glVertex2f(x-block_width/2.,y+block_height/2.);
		glVertex2f(x-block_width/2.,y-block_height/2.);
        	glEnd();

		sprintf(str, "%d", 0);
		glRasterPos2f(x-0.5,y-0.5);
		for (int j = 0; j < strlen(str); j++) 
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,str[j]);
		}
		sprintf(str, "%d", 180);
		glRasterPos2f(x+block_width-0.5,y-0.5);
		for (int j = 0; j < strlen(str); j++) 
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,str[j]);
		}
		sprintf(str, "length of bar on the bottom: orientation");
		glRasterPos2f(x,y-1.1);
		for (int j = 0; j < strlen(str); j++) 
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,str[j]);
		}

		sprintf(str, "size of solid bar on the top: exponent");
		glRasterPos2f(x+block_width/2.-0.5,y+block_height);
		for (int j = 0; j < strlen(str); j++) 
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,str[j]);
		}

		sprintf(str, "size of wireframe bar on the top: significand");
		glRasterPos2f(x+block_width,y+block_height/2.);
		for (int j = 0; j < strlen(str); j++) 
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,str[j]);
		}
}

void svQDOT2dmapping::SetTransformation(GLfloat *m, int n)
{
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
        {
           tb_transform[i][j] =m[j+i*4];
        }
}

void svQDOT2dmapping::SelectPoint(int x, int y)
{

    GLint viewport[4];
    GLdouble mvmatrix[16], projmatrix[16];

    int i, j;

    GLdouble td, dd;
    svVector3 *p = new svVector3[dataSize];
    GLdouble *wz = new GLdouble[dataSize];
    GLdouble *wx = new GLdouble[dataSize];
    GLdouble *wy = new GLdouble[dataSize];

    glGetIntegerv (GL_VIEWPORT, viewport);
    glGetDoublev (GL_MODELVIEW_MATRIX, mvmatrix);
    glGetDoublev (GL_PROJECTION_MATRIX, projmatrix);

    for(i=0;i<dataSize;i++)
    {
        p[i][0]=0;p[i][1]=0;p[i][2]=0;
    }

    for(i=0;i<dataSize;i++)
    {
        p[i][0]  = dataPos[i][0] * tb_transform[0][0]  +
                dataPos[i][1] * tb_transform[1][0] +
                dataPos[i][2] * tb_transform[2][0] +
                tb_transform[3][0] + p[i][0];
       p[i][1]  =dataPos[i][0] * tb_transform[0][1]+
                   dataPos[i][1] * tb_transform[1][1]  +
                    dataPos[i][2] * tb_transform[2][1] +
                            tb_transform[3][1] + p[i][1];
       p[i][2]  = dataPos[i][0] * tb_transform[0][2] +
                    dataPos[i][1] * tb_transform[1][2]  +
                     dataPos[i][2] * tb_transform[2][2] +
                            tb_transform[3][2] + p[i][2];
    }

    for(i=0;i<dataSize;i++)
    {
        gluProject((GLdouble)p[i][0],
                        (GLdouble)p[i][1],
                        (GLdouble)p[i][2],
                        mvmatrix, projmatrix, viewport,
                        &(wx[i]), &(wy[i]), &(wz[i]));
        wy[i] = (GLdouble)image_height-wy[i];
        //printf("%lf, %lf, %lf\n", wx[i], wy[i], wz[i]);
    }
   // printf("\n");
  //  printf("x=%d, y=%d\n", x, y);
    dd=9e+9;

    int in;
    for(i=0;i<dataSize;i++)
    {
        td = ((GLdouble)x-wx[i]) *  ((GLdouble)x-wx[i]) +
                 ((GLdouble)y-wy[i]) * ((GLdouble)y-wy[i]);
        if(td<dd)
        {
            dd = td;
            in = i;
        }
    }//cerr<<dd<<endl;

    if(dd<(block_width*scale_value*2)*(block_width*scale_value*2))
    {
        selected_index1 = in;//cerr<<in<<endl;

    }

    delete [] p;
    delete [] wz;
    delete [] wy;
    delete [] wx;
}


