#include "Sliders.h"

float GetAngle(float p1, float p2)
{
	svVector3 x;
	x[0]=1;
	x[1]=0;
	
	svVector3 x2;
	x2[0] = p1 - Widget_xy_center[0];
	x2[1] = p2 - Widget_xy_center[1];

	double sum = x2[0] * x2[0] + x2[1] * x2[1];
	sum = sqrt(sum);
	x2[0] = x2[0]/sum;
	x2[1] = x2[1]/sum;

	float cos = x2[0] * x[0] + x2[1] * x[1];
	float angle;	

	if(x2[0]>=0&&x2[1]>=0)
	{
		angle = acos(cos);
	}		
	else if(x2[0]<0&&x2[1]>=0)
	{
		angle = acos(cos);
	}
	else if(x2[0]<0&&x2[1]<0)
	{
		angle = M_PI * 2 - acos(cos);
	}
	else
	{
		angle = M_PI * 2. - acos(cos);
	}

	return angle;
}

void Reshape_Widget(int w, int h)
{
	Widget_bound_zangle[0][0] = w/2. - 50.;
	Widget_bound_zangle[0][1] = 50.;
	Widget_bound_zangle[1][0] = Widget_bound_zangle[0][0];
	Widget_bound_zangle[1][1] = h/4.;

	Widget_z_angle[0][0] = Widget_bound_zangle[0][0]-10.;
	Widget_z_angle[0][1] = Widget_bound_zangle[0][1]-5.;
	Widget_z_angle[1][0] = Widget_bound_zangle[0][0]+10.;
	Widget_z_angle[1][1] = Widget_bound_zangle[0][1]+5.;


	Widget_z_angle[2][0] = Widget_bound_zangle[1][0]-10.;
	Widget_z_angle[2][1] = Widget_bound_zangle[1][1]-5.;
	Widget_z_angle[3][0] = Widget_bound_zangle[1][0]+10.;
	Widget_z_angle[3][1] = Widget_bound_zangle[1][1]+5.;

	Widget_bound_mag[0][0] = w/2.-250.;
	Widget_bound_mag[0][1] = 50.;
	Widget_bound_mag[1][0] = Widget_bound_mag[0][0];
	Widget_bound_mag[1][1] = h/4.;
	Widget_z_mag[0][0] = Widget_bound_mag[0][0]-10.;
	Widget_z_mag[0][1] = Widget_bound_mag[0][1]-5.;
	Widget_z_mag[1][0] = Widget_bound_mag[0][0]+10.;
	Widget_z_mag[1][1] = Widget_bound_mag[0][1]+5.;
	Widget_z_mag[2][0] = Widget_bound_mag[1][0]-10.;
	Widget_z_mag[2][1] = Widget_bound_mag[1][1]-5.;
	Widget_z_mag[3][0] = Widget_bound_mag[1][0]+10.;
	Widget_z_mag[3][1] = Widget_bound_mag[1][1]+5.;

	Widget_xy_h = 60.;
	Widget_xy_w = w/2. -150.;
}

void Widget_Mag()
{
	glColor3f(0.,0.,0.);
	glLineWidth(3.);
	glBegin(GL_LINES);
	glVertex2f(Widget_bound_mag[0][0],Widget_bound_mag[0][1]);
	glVertex2f(Widget_bound_mag[1][0],Widget_bound_mag[1][1]);
	glEnd();

	glColor3f(1.,1.,1.);
	glLineWidth(2.);
	glBegin(GL_LINES);
	glVertex2f(Widget_bound_mag[0][0],Widget_bound_mag[0][1]);
	glVertex2f(Widget_bound_mag[1][0],Widget_bound_mag[1][1]);
	glEnd();


	char str[100];

	sprintf(str,"%6.2e",min_density);
	glRasterPos2f(Widget_z_mag[1][0]+2.,Widget_z_mag[0][1]);
        for(int j=0;j<strlen(str);j++)
             glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

	sprintf(str,"%6.2e",max_density);
	glRasterPos2f(Widget_z_mag[3][0]+2.,Widget_z_mag[2][1]);
        for(int j=0;j<strlen(str);j++)
             glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

	sprintf(str,"original data");
	glRasterPos2f(Widget_bound_mag[1][0]-10.,Widget_bound_mag[1][1]+30.);
        for(int j=0;j<strlen(str);j++)
             glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

	glPushMatrix();
	glTranslatef(0.,Widget_t_mag[0],0.);

	glColor3f(1.,1.,1.);
	sprintf(str,"%6.2e",range_mag[0]);
	glRasterPos2f(Widget_z_mag[1][0]+2.,Widget_z_mag[0][1]);
        for(int j=0;j<strlen(str);j++)
             glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

	glBegin(GL_QUADS);
	if(Widget_select_mag[0])
		glColor3f(254./255.,178./255.,76./255.);
	else
		glColor3f(1.,1.,1.);

	glVertex2f(Widget_z_mag[0][0],Widget_z_mag[0][1]);
	glVertex2f(Widget_z_mag[1][0],Widget_z_mag[0][1]);
	glVertex2f(Widget_z_mag[1][0],Widget_z_mag[1][1]);
	glVertex2f(Widget_z_mag[0][0],Widget_z_mag[1][1]);
	glEnd();

	glLineWidth(2.);
	glColor3f(0.,0.,0.);
	glBegin(GL_LINE_LOOP);
	glVertex2f(Widget_z_mag[0][0],Widget_z_mag[0][1]);
	glVertex2f(Widget_z_mag[1][0],Widget_z_mag[0][1]);
	glVertex2f(Widget_z_mag[1][0],Widget_z_mag[1][1]);
	glVertex2f(Widget_z_mag[0][0],Widget_z_mag[1][1]);
	glEnd();
	glLineWidth(1.);

	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.,Widget_t_mag[1],0.);

	if(Widget_select_mag[1])
		glColor3f(254./255.,178./255.,76./255.);
	else
		glColor3f(1.,1.,1.);
	glBegin(GL_QUADS);
	glVertex2f(Widget_z_mag[2][0],Widget_z_mag[2][1]);
	glVertex2f(Widget_z_mag[3][0],Widget_z_mag[2][1]);
	glVertex2f(Widget_z_mag[3][0],Widget_z_mag[3][1]);
	glVertex2f(Widget_z_mag[2][0],Widget_z_mag[3][1]);
	glEnd();

	glColor3f(1.,1.,1.);
	sprintf(str,"%6.2e",range_mag[1]);
	glRasterPos2f(Widget_z_mag[3][0]+2.,Widget_z_mag[2][1]);
        for(int j=0;j<strlen(str);j++)
             glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

	glLineWidth(2.);
	glColor3f(0.,0.,0.);
	glBegin(GL_LINE_LOOP);
	glVertex2f(Widget_z_mag[2][0],Widget_z_mag[2][1]);
	glVertex2f(Widget_z_mag[3][0],Widget_z_mag[2][1]);
	glVertex2f(Widget_z_mag[3][0],Widget_z_mag[3][1]);
	glVertex2f(Widget_z_mag[2][0],Widget_z_mag[3][1]);
	glEnd();
	glLineWidth(1.);


	glPopMatrix();
	glColor3f(227./255.,74./255.,51./255.);
	glBegin(GL_LINES);
	glVertex2f((Widget_z_mag[2][0]+Widget_z_mag[3][0])/2., Widget_z_mag[2][1]+Widget_t_mag[1]);
	glVertex2f((Widget_z_mag[1][0]+Widget_z_mag[0][0])/2., Widget_z_mag[1][1]+Widget_t_mag[0]);
	glEnd();

	//glEnable(GL_DEPTH_TEST);
}

void Widget_Z_angle()
{
	glColor3f(0.,0.,0.);
	glLineWidth(3.);
	glBegin(GL_LINES);
	glVertex2f(Widget_bound_zangle[0][0],Widget_bound_zangle[0][1]);
	glVertex2f(Widget_bound_zangle[1][0],Widget_bound_zangle[1][1]);
	glEnd();

	glColor3f(1.,1.,1.);
	glLineWidth(2.);
	glBegin(GL_LINES);
	glVertex2f(Widget_bound_zangle[0][0],Widget_bound_zangle[0][1]);
	glVertex2f(Widget_bound_zangle[1][0],Widget_bound_zangle[1][1]);
	glEnd();


	char str[100];
	sprintf(str,"-90");
	glRasterPos2f(Widget_z_angle[1][0]+2.,Widget_z_angle[0][1]);
        for(int j=0;j<strlen(str);j++)
             glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

	sprintf(str,"90");
	glRasterPos2f(Widget_z_angle[3][0]+2.,Widget_z_angle[2][1]);
        for(int j=0;j<strlen(str);j++)
             glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

	sprintf(str,"Z angle");
	glRasterPos2f(Widget_bound_zangle[1][0]-5.,Widget_bound_zangle[1][1]+30.);
        for(int j=0;j<strlen(str);j++)
             glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

	glPushMatrix();
	glTranslatef(0.,Widget_t_zangle[0],0.);

	if(Widget_select_zangle[0])
		glColor3f(254./255.,178./255.,76./255.);
	else
		glColor3f(1.,1.,1.);
	glBegin(GL_QUADS);
	glVertex2f(Widget_z_angle[0][0],Widget_z_angle[0][1]);
	glVertex2f(Widget_z_angle[1][0],Widget_z_angle[0][1]);
	glVertex2f(Widget_z_angle[1][0],Widget_z_angle[1][1]);
	glVertex2f(Widget_z_angle[0][0],Widget_z_angle[1][1]);
	glEnd();

	glColor3f(1.,1.,1.);
	sprintf(str,"%.2f",(zangle_min-90));
	glRasterPos2f(Widget_z_angle[1][0]+2.,Widget_z_angle[0][1]);
        for(int j=0;j<strlen(str);j++)
             glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

	glLineWidth(2.);
	glColor3f(0.,0.,0.);
	glBegin(GL_LINE_LOOP);
	glVertex2f(Widget_z_angle[0][0],Widget_z_angle[0][1]);
	glVertex2f(Widget_z_angle[1][0],Widget_z_angle[0][1]);
	glVertex2f(Widget_z_angle[1][0],Widget_z_angle[1][1]);
	glVertex2f(Widget_z_angle[0][0],Widget_z_angle[1][1]);
	glEnd();
	glLineWidth(1.);

	glPopMatrix();


	glPushMatrix();
	glTranslatef(0.,Widget_t_zangle[1],0.);

	if(Widget_select_zangle[1])
		glColor3f(254./255.,178./255.,76./255.);
	else
		glColor3f(1.,1.,1.);
	glBegin(GL_QUADS);
	glVertex2f(Widget_z_angle[2][0],Widget_z_angle[2][1]);
	glVertex2f(Widget_z_angle[3][0],Widget_z_angle[2][1]);
	glVertex2f(Widget_z_angle[3][0],Widget_z_angle[3][1]);
	glVertex2f(Widget_z_angle[2][0],Widget_z_angle[3][1]);
	glEnd();

	glColor3f(1.,1.,1.);
	sprintf(str,"%.2f",zangle_max-90);
	glRasterPos2f(Widget_z_angle[3][0]+2.,Widget_z_angle[2][1]);
        for(int j=0;j<strlen(str);j++)
             glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

	glLineWidth(2.);
	glColor3f(0.,0.,0.);
	glBegin(GL_LINE_LOOP);
	glVertex2f(Widget_z_angle[2][0],Widget_z_angle[2][1]);
	glVertex2f(Widget_z_angle[3][0],Widget_z_angle[2][1]);
	glVertex2f(Widget_z_angle[3][0],Widget_z_angle[3][1]);
	glVertex2f(Widget_z_angle[2][0],Widget_z_angle[3][1]);
	glEnd();
	glLineWidth(1.);

	glPopMatrix();

	glColor3f(227./255.,74./255.,51./255.);
	glBegin(GL_LINES);
	glVertex2f((Widget_z_angle[2][0]+Widget_z_angle[3][0])/2., Widget_z_angle[2][1]+Widget_t_zangle[1]);
	glVertex2f((Widget_z_angle[1][0]+Widget_z_angle[0][0])/2., Widget_z_angle[1][1]+Widget_t_zangle[0]);
	glEnd();

}


void Widget_XY()
{
	char str[50]; 

	glPushMatrix();

	glTranslatef(Widget_xy_w, Widget_xy_h,0);
	glColor3f(1.,1.,1.);
	glBegin(GL_LINE_LOOP);
	float degree = M_PI/10.;
	for(int i=0;i<20;i++)
	{
		glVertex2f(cos((float)i * degree)* Widget_xy_radius, sin((float)i * degree ) *Widget_xy_radius);	
	}
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(-1.5* Widget_xy_radius,0.);
	glVertex2f(2* Widget_xy_radius,0.);

	glVertex2f(0.,-1.5* Widget_xy_radius);
	glVertex2f(0.,1.5* Widget_xy_radius);

	glVertex2f(0.,1.5* Widget_xy_radius);
	glVertex2f(0.075* Widget_xy_radius,1.35* Widget_xy_radius);

	glVertex2f(0.,1.5* Widget_xy_radius);
	glVertex2f(-0.075* Widget_xy_radius,1.35* Widget_xy_radius);

	glVertex2f(2* Widget_xy_radius,0.);
	glVertex2f(1.85* Widget_xy_radius,0.075* Widget_xy_radius);

	glVertex2f(2* Widget_xy_radius,0.);
	glVertex2f(1.85* Widget_xy_radius,-0.075* Widget_xy_radius);
	glEnd();

	glPopMatrix();


	sprintf(str,"xy angle");
	glRasterPos2f(Widget_xy_w, Widget_xy_radius*2 + Widget_xy_h);
	for(int j=0;j<strlen(str);j++)
		    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

	sprintf(str,"x");
	glRasterPos2f(Widget_xy_radius*2.5 + Widget_xy_w, Widget_xy_h);
	for(int j=0;j<strlen(str);j++)
		    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

	sprintf(str,"y");
	glRasterPos2f(Widget_xy_w, Widget_xy_radius*1.5 + Widget_xy_h);
	for(int j=0;j<strlen(str);j++)
		    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

	glPushMatrix();
	glTranslatef(Widget_xy_w, Widget_xy_h,0);
	glPointSize(8.);
	glBegin(GL_POINTS);
	for(int i=0;i<Widget_xy_min[0].size();i++)
	{
		if(!Widget_select_xy[i])
			glColor3f(1.,1.,1.);
		else
			glColor3f(254./255.,178./255.,76./255.);
		glVertex2f(Widget_xy_min[0][i], Widget_xy_min[1][i]);
	}
	for(int i=0;i<Widget_xy_max[0].size();i++)
	{
		if(!Widget_select_xy[i+Widget_xy_min[0].size()])
			glColor3f(1.,1.,1.);
		else
			glColor3f(254./255.,178./255.,76./255.);
		glVertex2f(Widget_xy_max[0][i], Widget_xy_max[1][i]);
	}
	glEnd();
	glPointSize(1.);

	glColor3f(1.,1.,1.);
	for(int i=0;i<xy_min.size();i++)
	{
		sprintf(str,"%.2f",xy_min[i]/M_PI * 180.);
		glRasterPos2f(Widget_xy_min[0][i]+5., Widget_xy_min[1][i]);
		for(int j=0;j<strlen(str);j++)
		     glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

		sprintf(str,"%.2f",xy_max[i]/M_PI * 180.);
		glRasterPos2f(Widget_xy_max[0][i]+5., Widget_xy_max[1][i]);
		for(int j=0;j<strlen(str);j++)
		     glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
	}

	glColor3f(227./255.,74./255.,51./255.);
	for(int i=0;i<Widget_xy_min[0].size();i++)
	{
		float angle1 =  xy_min[i];//GetAngle(Widget_xy_min[0][i],Widget_xy_min[1][i]);
		float angle2 =  xy_max[i];//GetAngle(Widget_xy_max[0][i],Widget_xy_max[1][i]);
		if(xy_min[i]<xy_max[i])
		{
			degree = (angle2-angle1)/20.;
			glBegin(GL_LINE_STRIP);
			for(int j=0;j<20;j++)
			{
				glVertex2f(Widget_xy_center[0] + Widget_xy_radius * cos(angle1 + degree * (float)j), 
					Widget_xy_center[1] + Widget_xy_radius *sin(angle1 + degree * (float)j));	
			}
			glVertex2f(Widget_xy_max[0][i],Widget_xy_max[1][i]);	
			glEnd();
		}
		else
		{
			degree = (M_PI*2.-angle1)/20.;
			glBegin(GL_LINE_STRIP);
			glVertex2f(Widget_xy_min[0][i],Widget_xy_min[1][i]);	
			for(int j=1;j<21;j++)
			{
				glVertex2f(Widget_xy_center[0] + Widget_xy_radius * cos(angle1 + degree * (float)j), 
					Widget_xy_center[1] + Widget_xy_radius *sin(angle1 + degree * (float)j));	
			}
			glEnd();

			degree = (angle2)/20.;
			glBegin(GL_LINE_STRIP);
			for(int j=0;j<20;j++)
			{
				glVertex2f(Widget_xy_center[0] + Widget_xy_radius * cos(degree * (float)j), 
					Widget_xy_center[1] + Widget_xy_radius *sin(degree * (float)j));	
			}
			glVertex2f(Widget_xy_max[0][i],Widget_xy_max[1][i]);	
			glEnd();
		}
	}
	glPopMatrix();
}


