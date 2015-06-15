#include "svSlice.h"
#include "svUtil.h"
#include <cmath>
#include <algorithm>


using namespace std;

namespace __svl_lib{

svSlice::svSlice(svVector3 l, svVector3 r,int dataSize,
          svVector3 *posIn)
{
	glyphPos = NULL;
	cutPos = NULL;
	cutSamplePos = NULL;
	planex = NULL;
	planey = NULL;

	SetData(l,r,dataSize,posIn);
}

void svSlice::SetData(svVector3 l, svVector3 r,int dataSize,
          svVector3 *posIn)
{

	if(glyphPos!=NULL)
	{
		glyphPos[0].free();
		delete [] glyphPos;
		glyphPos = NULL;
		delete [] cutPos;
		delete [] cutSamplePos;
		delete [] planex;
		delete [] planey;
	}

	lb = l;
	rb = r;
	
	glyphPos = new svVector3Array[1];
	glyphIndex = NULL;

	int i;

	for(i=0;i<dataSize;i++)
	{
		glyphPos[0].add(posIn[i]);
	}

	widgetl = (r[0]-l[0])/2.;
	widgetp[0] = (l + r)/2.;
	widgetp[1] = widgetp[0];
	widgetp[1][1] = widgetp[1][1] + widgetl;
	widgetp[2] = widgetp[0];
	widgetp[3] = widgetp[0];
	widgetp[3][0] = widgetp[3][0] + widgetl;

	side = sqrt((r[0]-l[0])*(r[0]-l[0])
			+(r[1]-l[1])*(r[1]-l[1])
			+(r[2]-l[2])*(r[2]-l[2]))/sqrt(2.);
	SELECTED = GL_FALSE;
	SELECTED_WHITE = GL_FALSE;
	SELECTED_BLUE = GL_FALSE;

	dir = widgetp[1]-widgetp[0];
	dir2 = widgetp[3] - widgetp[2];

	cutPos = new svVector3[1];
	cutSamplePos = new svVector3[1];
	planex = new float[dataSize];
	planey = new float[dataSize];

	intersectionnum = 0;
	intersectionnum2 = 0;
/*
	chooseboxside = rb[0]-lb[0];
	increasesize = 1;
	choosebox[0] = widgetp[0];
	choosebox[0][0] = choosebox[0][0] + chooseboxside/2.;
	choosebox[0][2] = choosebox[0][2] + chooseboxside/2.;
	choosebox[1] = widgetp[0];
	choosebox[1][0] = choosebox[1][0] + chooseboxside/2.;
	choosebox[1][2] = choosebox[1][2] - chooseboxside/2.;	
	choosebox[2] = widgetp[0];
	choosebox[2][0] = choosebox[2][0] - chooseboxside/2.;
	choosebox[2][2] = choosebox[2][2] - chooseboxside/2.;
	choosebox[3] = widgetp[0];
	choosebox[3][0] = choosebox[3][0] - chooseboxside/2.;
	choosebox[3][2] = choosebox[3][2] + chooseboxside/2.;
*/
	Intersection();
	SetPlaneXYZ();

}

void svSlice::RenderSlice(bool stop1, bool stop2)
{
	glColor3f(1.,1.,1.);
	glBegin(GL_LINES);
	glVertex3f(widgetp[0][0],widgetp[0][1],widgetp[0][2]);
	glVertex3f(widgetp[1][0],widgetp[1][1],widgetp[1][2]);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(widgetp[2][0],widgetp[2][1],widgetp[2][2]);
	glVertex3f(widgetp[3][0],widgetp[3][1],widgetp[3][2]);
	glEnd();
	glLineWidth(3.);
	
	glColor3f(1.,1.,1.);
	glPushMatrix();
	glTranslatef(widgetp[1][0],widgetp[1][1],widgetp[1][2]);
	glutSolidSphere(1.,10.,10.);
	glPopMatrix();
	glColor3f(217./255.,240./255.,163./255.);
	glPushMatrix();
	glTranslatef(widgetp[3][0],widgetp[3][1],widgetp[3][2]);
	glutSolidSphere(1.,10.,10.);
	glPopMatrix();

	svVector3 n = widgetp[1] - widgetp[0];
	svVector3 p1=svGetPerpendicularVector(n);
	p1 = normalize(p1);
	p1 = p1*side + widgetp[0];
	p1=svGetRotatePoint(p1, widgetp[0], n,-45);
	
/*	glEnable(GL_BLEND);
	glColor4f(0.,0.,0.,0.5);
	glBegin(GL_QUADS);
	glVertex3f(p1[0],p1[1],p1[2]);

	svVector3 p2=svGetRotatePoint(p1, widgetp[0], n,90);
	glVertex3f(p2[0],p2[1],p2[2]);

	svVector3 p3=svGetRotatePoint(p1, widgetp[0], n,180);
	glVertex3f(p3[0],p3[1],p3[2]);

	svVector3 p4=svGetRotatePoint(p1, widgetp[0], n,270);
	glVertex3f(p4[0],p4[1],p4[2]);
	glEnd();

	glColor4f(1.,1.,1.,0.5);
	glBegin(GL_LINE_LOOP);
	glVertex3f(p1[0],p1[1],p1[2]);
	glVertex3f(p2[0],p2[1],p2[2]);
	glVertex3f(p3[0],p3[1],p3[2]);
	glVertex3f(p4[0],p4[1],p4[2]);
	glEnd();
*/
	if(stop1)
		glColor3f(215./255.,48./255.,31./255.);
	else
		glColor3f(1.,1.,1.);
	glBegin(GL_LINE_LOOP);
	for(int i=0;i<intersectionnum;i++)
		glVertex3f(intersectionp[i][0],intersectionp[i][1],intersectionp[i][2]);
	glEnd();
	if(stop2)
		glColor3f(215./255.,48./255.,31./255.);
	else
		glColor3f(217./255.,240./255.,163./255.);
	glBegin(GL_LINE_LOOP);
	for(int i=6;i<intersectionnum2;i++)
		glVertex3f(intersectionp[i][0],intersectionp[i][1],intersectionp[i][2]);
	glEnd();

/*	glColor3f(253./255.,212./255.,158./255.);
	float scale = 30.;
	glBegin(GL_LINES);
	glVertex3f(planexyz[0][0]*scale+planecenter[0],planexyz[0][1]*scale+planecenter[1],planexyz[0][2]*scale+planecenter[2]);
	glVertex3f(planecenter[0],planecenter[1],planecenter[2]);
	glEnd();
	glPointSize(2.5);
	glBegin(GL_POINTS);
	glVertex3f(planexyz[0][0]*scale+planecenter[0],planexyz[0][1]*scale+planecenter[1],planexyz[0][2]*scale+planecenter[2]);
	glEnd();
	glColor3f(215./255.,48./255.,31./255.);
	glBegin(GL_LINES);
	glVertex3f(planexyz[1][0]*scale+planecenter[0],planexyz[1][1]*scale+planecenter[1],planexyz[1][2]*scale+planecenter[2]);
	glVertex3f(planecenter[0],planecenter[1],planecenter[2]);
	glEnd();
	glPointSize(2.5);
	glBegin(GL_POINTS);
	glVertex3f(planexyz[1][0]*scale+planecenter[0],planexyz[1][1]*scale+planecenter[1],planexyz[1][2]*scale+planecenter[2]);
	glEnd();
	glPointSize(1.);*/

/*	glLineWidth(1.);
	glColor3f(1,1,1);
	glBegin(GL_LINES);
	glVertex3f(lb[0],lb[1],lb[2]);
	glVertex3f(rb[0],lb[1],lb[2]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(lb[0],lb[1],lb[2]);
	glVertex3f(lb[0],rb[1],lb[2]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(lb[0],lb[1],lb[2]);
	glVertex3f(lb[0],lb[1],rb[2]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(lb[0],rb[1],lb[2]);
	glVertex3f(rb[0],rb[1],lb[2]);
	glEnd();
        glBegin(GL_LINES);
        glVertex3f(rb[0],rb[1],lb[2]);
	glVertex3f(rb[0],lb[1],lb[2]);
	glEnd();
        glBegin(GL_LINES);
        glVertex3f(lb[0],lb[1],rb[2]);
        glVertex3f(lb[0],rb[1],rb[2]);
	glEnd();
        glBegin(GL_LINES);
        glVertex3f(lb[0],rb[1],rb[2]);
        glVertex3f(rb[0],rb[1],rb[2]);
	glEnd();
        glBegin(GL_LINES);
        glVertex3f(rb[0],rb[1],rb[2]);
        glVertex3f(rb[0],lb[1],rb[2]);
        glEnd();
        glBegin(GL_LINES);
        glVertex3f(lb[0],lb[1],rb[2]);
        glVertex3f(rb[0],lb[1],rb[2]);
        glEnd();
        glBegin(GL_LINES);
        glVertex3f(lb[0],rb[1],lb[2]);
        glVertex3f(lb[0],rb[1],rb[2]);
        glEnd();
        glBegin(GL_LINES);
        glVertex3f(rb[0],lb[1],lb[2]);
        glVertex3f(rb[0],lb[1],rb[2]);
        glEnd();
        glBegin(GL_LINES);
        glVertex3f(rb[0],rb[1],lb[2]);
        glVertex3f(rb[0],rb[1],rb[2]);
        glEnd();
*/
	/*glColor3f(1.,1.,0.);
	glBegin(GL_LINE_LOOP);
	glVertex3f(choosebox[0][0],choosebox[0][1],choosebox[0][2]);
	glVertex3f(choosebox[1][0],choosebox[1][1],choosebox[1][2]);
	glVertex3f(choosebox[2][0],choosebox[2][1],choosebox[2][2]);
	glVertex3f(choosebox[3][0],choosebox[3][1],choosebox[3][2]);
	glEnd();*/

	//for(int i=0;i<4;i++)
	//{
		//cerr<<choosebox[i][0]<<" "<<choosebox[i][1]<<" "<<choosebox[i][2]<<endl;
	//}
	//cerr<<"---------------"<<endl;
}

svVector3  svSlice::CrossProduct(svVector3 u, svVector3 v)
{
	svVector3 c;

	c[0]=u[1]*v[2] - u[2]*v[1];
	c[1]=u[2]*v[0] - u[0]*v[2];
	c[2]=u[0]*v[1] - u[1]*v[0];

	c = normalize(c);

	return c;
}
svVector3 incenter;
svVector3 indir;
bool compare(svVector3 lhs, svVector3 rhs)
{
	svVector3 u = lhs - incenter;
	svVector3 v = rhs - incenter;
        svVector3 c;

	c[0]=u[1]*v[2] - u[2]*v[1];
	c[1]=u[2]*v[0] - u[0]*v[2];
	c[2]=u[0]*v[1] - u[1]*v[0];

	c = normalize(c);

        float det;
	det = indir[0]*c[0]+indir[1]*c[1]+indir[2]*c[2];
	
	return det<0;
}
void svSlice::Intersection()
{
	svVector3 l0,l;
	svVector3 n,p0;
	float d;
	svVector3 p[12];
	svVector3 tmpp[6];
	int j=0;
	svVector3 out;
	out[0] = rb[0] + 10.;
	out[1] = rb[1] + 10.;
	out[2] = rb[2] + 10.;
	for(int t=0;t<2;t++)
	{
		if(t==0)
		{
			n = normalize(dir);
			p0 = widgetp[0];
		}
		else
		{
			n = normalize(dir2);
			p0 = widgetp[2];
		}

		l[0]=0.;l[1]=0.;l[2]=1.;
		l0 = lb;		
		if(GetDot(l,n)!=0)
		{
			d = GetDot((p0 - l0),n)/GetDot(l,n);
			p[0] = d *l +l0;
		}
		else
			p[0] = out;

		l0[0] = lb[0];l0[1]=rb[1];l0[2]=lb[2];
		if(GetDot(l,n)!=0)
		{
		d = GetDot((p0 - l0),n)/GetDot(l,n);
		p[1] = d *l +l0;
		}
		else
			p[1] = out;

		l0[0] = rb[0];l0[1]=rb[1];l0[2]=lb[2];
		if(GetDot(l,n)!=0)
		{
		d = GetDot((p0 - l0),n)/GetDot(l,n);
		p[2] = d *l +l0;
		}
		else
			p[2] = out;

		l0[0] = rb[0];l0[1]=lb[1];l0[2]=lb[2];
		if(GetDot(l,n)!=0)
		{
		d = GetDot((p0 - l0),n)/GetDot(l,n);
		p[3] = d *l +l0;
		}
		else
			p[3] = out;

		l[0]=0.;l[1]=1.;l[2]=0.;
		l0[0] = lb[0];l0[1]=lb[1];l0[2]=lb[2];
		if(GetDot(l,n)!=0)
		{
		d = GetDot((p0 - l0),n)/GetDot(l,n);
		p[4] = d *l +l0;
		}
		else
			p[4] = out;

		l0[0] = rb[0];l0[1]=lb[1];l0[2]=lb[2];
		if(GetDot(l,n)!=0)
		{
		d = GetDot((p0 - l0),n)/GetDot(l,n);
		p[5] = d *l +l0;
		}
		else
			p[5] = out;

		l0[0] = rb[0];l0[1]=lb[1];l0[2]=rb[2];
		if(GetDot(l,n)!=0)
		{
		d = GetDot((p0 - l0),n)/GetDot(l,n);
		p[6] = d *l +l0;
		}
		else
			p[6] = out;

		l0[0] = lb[0];l0[1]=lb[1];l0[2]=rb[2];
		if(GetDot(l,n)!=0)
		{
		d = GetDot((p0 - l0),n)/GetDot(l,n);
		p[7] = d *l +l0;
		}
		else
			p[7] = out;

		l[0]=1.;l[1]=0.;l[2]=0.;
		l0[0] = lb[0];l0[1]=lb[1];l0[2]=lb[2];
		if(GetDot(l,n)!=0)
		{
		d = GetDot((p0 - l0),n)/GetDot(l,n);
		p[8] = d *l +l0;
		}
		else
			p[8] = out;

		l0[0] = lb[0];l0[1]=rb[1];l0[2]=lb[2];
		if(GetDot(l,n)!=0)
		{
		d = GetDot((p0 - l0),n)/GetDot(l,n);
		p[9] = d *l +l0;
		}
		else
			p[9] = out;

		l0[0] = lb[0];l0[1]=rb[1];l0[2]=rb[2];
		if(GetDot(l,n)!=0)
		{
		d = GetDot((p0 - l0),n)/GetDot(l,n);
		p[10] = d *l +l0;
		}
		else
			p[10] = out;

		l0[0] = lb[0];l0[1]=lb[1];l0[2]=rb[2];
		if(GetDot(l,n)!=0)
		{
		d = GetDot((p0 - l0),n)/GetDot(l,n);
		p[11] = d *l +l0;
		}
		else
			p[11] = out;

		if(t==0)
		{
			j = 0;
			for(int i=0;i<12;i++)
			{
				if(p[i][0]>=lb[0]&&p[i][0]<=rb[0]
				&&p[i][1]>=lb[1]&&p[i][1]<=rb[1]
				&&p[i][2]>=lb[2]&&p[i][2]<=rb[2])
				{
					intersectionp[j]=p[i];
					j++;
				}
			}
			intersectionnum = j;

			incenter = intersectionp[0];
			indir = n;
			std::sort(intersectionp, intersectionp+intersectionnum, compare);
		}
		else
		{
			j = 0;
			for(int i=0;i<12;i++)
			{
				if(p[i][0]>=lb[0]&&p[i][0]<=rb[0]
				&&p[i][1]>=lb[1]&&p[i][1]<=rb[1]
				&&p[i][2]>=lb[2]&&p[i][2]<=rb[2])
				{
					tmpp[j]=p[i];
					j++;
				}
			}
			intersectionnum2 = j+6;

			incenter = tmpp[0];
			indir = n;
			std::sort(tmpp, tmpp+j, compare);
			j = 0;
			for(int i=6;i<intersectionnum2;i++)
			{
				intersectionp[i] = tmpp[j];
				j ++;
			}
		}
	}
	
}

void svSlice::SetPlaneXYZ()
{
	svVector3 px,py,pz,center;
	vector<float> z;
	int index[intersectionnum];
	int indexnum = 0;
	center[0]=0.;
	center[1]=0.;
	center[2]=0.;
	py[0]=0.;py[1]=0.;py[2]=0.;
	for(int i=0;i<intersectionnum;i++)
	{
		center = center + intersectionp[i];
		z.push_back(intersectionp[i][2]);
	}
	center[0] = center[0]/intersectionnum;
	center[1] = center[1]/intersectionnum;
	center[2] = center[2]/intersectionnum;//cerr<<center[0]<<" "<<center[1]<<" "<<center[2]<<endl;
	sort(z.begin(),z.end());
	for(int i=0;i<intersectionnum;i++)
	{
		if(intersectionp[i][2]==z[intersectionnum-1])
			{index[indexnum]=i;indexnum++;}
	}	//cerr<<indexnum<<endl;
	if(indexnum==4)
	{
		indexnum = 1;
		for(int i=0;i<intersectionnum;i++)
		{
			if(((intersectionp[i][1]==intersectionp[indexnum-1][1])
				|| (intersectionp[i][0]==intersectionp[indexnum-1][0]))
				&& !((intersectionp[i][1]==intersectionp[indexnum-1][1])
					&&(intersectionp[i][0]==intersectionp[indexnum-1][0])))
				{index[indexnum]=i;indexnum++;break;}
		}
	}
	for(int i=0;i<indexnum;i++)	
	{
		py = py + intersectionp[index[i]];
	}
	py[0] = py[0]/indexnum;
	py[1] = py[1]/indexnum;
	py[2] = py[2]/indexnum;
	py = py - center;
	py = normalize(py);//cerr<<py[0]<<" "<<py[1]<<" "<<py[2]<<endl;
	pz = dir;//cerr<<pz[0]<<" "<<pz[1]<<" "<<pz[2]<<endl;
	px = CrossProduct(py,pz);//cerr<<px[0]<<" "<<px[1]<<" "<<px[2]<<endl;

	planecenter = center;
	planexyz[0] = px;//cerr<<planexyz[0][0]<<" "<<planexyz[0][1]<<" "<<planexyz[0][2]<<endl;
	planexyz[1] = py;
	planexyz[2] = pz;

	float x[intersectionnum],y[intersectionnum];
	lb_plane[0] = 9e+9;
	lb_plane[1] = 9e+9;
	lb_plane[2] = 9e+9;
	rb_plane[0] = -9e+9;
	rb_plane[1] = -9e+9;
	rb_plane[2] = -9e+9;
	for(int j=0;j<intersectionnum;j++)
	{
		y[j] = PointtoLine2(planecenter, planexyz[0], intersectionp[j]);
		x[j] = PointtoLine2(planecenter, planexyz[1], intersectionp[j]);
		if(GetDot(intersectionp[j]-planecenter,planexyz[0])<0)
			x[j] = -x[j];
		if(GetDot(intersectionp[j]-planecenter,planexyz[1])<0)
			y[j] = -y[j];
		intersectionplanep[j][0] = x[j];
		intersectionplanep[j][1] = y[j];
		if(intersectionplanep[j][0]>rb_plane[0])rb_plane[0]=intersectionplanep[j][0];
		if(intersectionplanep[j][1]>rb_plane[1])rb_plane[1]=intersectionplanep[j][1];
		if(intersectionplanep[j][2]>rb_plane[2])rb_plane[2]=intersectionplanep[j][2];
		if(intersectionplanep[j][0]<lb_plane[0])lb_plane[0]=intersectionplanep[j][0];
		if(intersectionplanep[j][1]<lb_plane[1])lb_plane[1]=intersectionplanep[j][1];
		if(intersectionplanep[j][2]<lb_plane[2])lb_plane[2]=intersectionplanep[j][2];
		//cerr<<x[j]<<" "<<y[j]<<endl;
	}
}

void svSlice::InitChooseBox(float w, float h)
{
	int i;
	float dd1=-9e+9;
	float dd2=-9e+9;
	/*for(i=0;i<intersectionnum;i++)
	{
		if(dd1<=fabs(intersectionplanep[i][0]))
			dd1 = fabs(intersectionplanep[i][0]);
		if(dd2<=fabs(intersectionplanep[i][1]))
			dd2 = fabs(intersectionplanep[i][1]);
	}*/
	dd1 = w/2.;
	dd2 = h/2.;

	chooseboxside[0] = dd1 * 2;
	chooseboxside[1] = dd2 * 2;
	initialside[0] = dd1 * 2;	
	initialside[1] = dd2 * 2;
	choosebox[0][0]=-dd1;choosebox[0][1]=dd2;
	choosebox[1][0]=-dd1;choosebox[1][1]=-dd2;
	choosebox[2][0]=dd1;choosebox[2][1]=-dd2;
	choosebox[3][0]=dd1;choosebox[3][1]=dd2;

	choosebox[0]=planecenter - dd1 * planexyz[0];
	choosebox[0]=choosebox[0] + dd2 * planexyz[1];
	choosebox[1]=planecenter - dd1 * planexyz[0];
	choosebox[1]=choosebox[1] - dd2 * planexyz[1];
	choosebox[2]=planecenter + dd1 * planexyz[0];
	choosebox[2]=choosebox[2] - dd2 * planexyz[1];
	choosebox[3]=planecenter + dd1 * planexyz[0];
	choosebox[3]=choosebox[3] + dd2 * planexyz[1];

	for(i=0;i<4;i++)
	{	initchoosebox[i] = choosebox[i];
		//cerr<<initchoosebox[i][0]<<" "<<initchoosebox[i][1]<<" "<<initchoosebox[i][2]<<endl;
	}

	chooseboxcenter = planecenter;	
	initboxcenter = planecenter;
}

void svSlice::SetChooseBox(float scale)
{
	chooseboxside[0] = initialside[0]/scale;
	chooseboxside[1] = initialside[1]/scale;

	float dd1 = chooseboxside[0]/2.;
	float dd2 = chooseboxside[1]/2.;

	choosebox[0]=initboxcenter - dd1 * planexyz[0];
	choosebox[0]=choosebox[0] + dd2 * planexyz[1];
	choosebox[1]=initboxcenter - dd1 * planexyz[0];
	choosebox[1]=choosebox[1] - dd2 * planexyz[1];
	choosebox[2]=initboxcenter + dd1 * planexyz[0];
	choosebox[2]=choosebox[2] - dd2 * planexyz[1];
	choosebox[3]=initboxcenter + dd1 * planexyz[0];
	choosebox[3]=choosebox[3] + dd2 * planexyz[1];

	for(int i=0;i<4;i++)
		initchoosebox[i] = choosebox[i];	
}

svVector3 * svSlice::GetBoxPoints()
{
	svVector3 *r = new svVector3[4];

	for(int j=0;j<4;j++)
	{
		r[j][1] = PointtoLine2(planecenter, planexyz[0], choosebox[j]);
		r[j][0] = PointtoLine2(planecenter, planexyz[1], choosebox[j]);
		if(GetDot(choosebox[j]-planecenter,planexyz[0])<0)
			r[j][0] = -r[j][0];
		if(GetDot(choosebox[j]-planecenter,planexyz[1])<0)
			r[j][1] = -r[j][1];
	}	

	return r;
}

svIntArray * svSlice::GetSelectedIndex()
{
	if(glyphIndex!=NULL)
	glyphIndex[0].free();
	delete [] glyphIndex;
	glyphIndex = NULL;

	glyphIndex = new svIntArray[1];
	selectedNum = 0;

	svVector3 n = widgetp[1] - widgetp[0];
	svVector3 p[4];
	p[0]=svGetPerpendicularVector(n);
	p[0]=normalize(p[0]);
	p[0]= p[0]*side + widgetp[0];	
	p[1]=svGetRotatePoint(p[0], widgetp[0], n,90);
	p[2]=svGetRotatePoint(p[0], widgetp[0], n,180);
	p[3]=svGetRotatePoint(p[0], widgetp[0], n,270);
	
	svVector3 tmpl,tmpr;
	int i;
	svVector3 deltax;
	float s;

	tmpl = p[0];
	tmpr = p[0];

	for(i=1;i<4;i++)
	{
		if(p[i][0]>=tmpr[0])tmpr[0]=p[i][0];
		if(p[i][0]<=tmpl[0])tmpl[0]=p[i][0];
	}
	
	int sample = glyphPos[0].size();


//http://www.cs.princeton.edu/courses/archive/fall00/cs426/lectures/raycast/sld017.htm
//ray-plane intersection

	for(i=0;i<sample;i++)
	{
		if(glyphPos[0][i][0]>=tmpl[0]&&glyphPos[0][i][0]<=tmpr[0]
			&&glyphPos[0][i][1]>=tmpl[1]&&glyphPos[0][i][1]<=tmpr[1]
			&&glyphPos[0][i][2]>=tmpl[2]&&glyphPos[0][i][2]<=tmpr[2])
		{
			deltax = glyphPos[0][i] - widgetp[0];
			s = deltax[0]*n[0] + deltax[1]*n[1] + deltax[2]*n[2];
			if(s==0)
			{
				glyphIndex[0].add(i);
				selectedNum++;
			}
		}
		
		
	}
	
	return glyphIndex;
}

float svSlice::GetDot(svVector3 a, svVector3 b)
{
	float c;
	c = a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
	return c;
}
float svSlice::PointtoLine(svVector3 p1, svVector3 p2, svVector3 p)
{
	//distance(x=a+tn, p) = ||(a-p)-((a-p)*n)n||
	//wiki http://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line

	svVector3 n;
	n = p1-p2;
	n = normalize(n);
	svVector3 a=p2;
	svVector3 ap=a - p;
	float apdot=GetDot(ap,n);
	svVector3 apn = apdot*n;
	svVector3 d = ap - apn;

	float distance = sqrt(d[0]*d[0]+d[1]*d[1]
				+d[2]*d[2]);	

	return distance;
}
float svSlice::PointtoLine2(svVector3 a, svVector3 n, svVector3 p)
{
	//distance(x=a+tn, p) = ||(a-p)-((a-p)*n)n||
	//wiki http://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line

	svVector3 ap=a - p;
	float apdot=GetDot(ap,n);
	svVector3 apn = apdot*n;
	svVector3 d = ap - apn;

	float distance = sqrt(d[0]*d[0]+d[1]*d[1]
				+d[2]*d[2]);	

	return distance;
}
bool svSlice::pointinpolygon(float x, float y)
{
  int   i, j=intersectionnum-1 ;
  bool  oddNodes=false;

  for (i=0; i<intersectionnum; i++) {
    if (intersectionplanep[i][1]<y && intersectionplanep[j][1]>=y
    ||  intersectionplanep[j][1]<y && intersectionplanep[i][1]>=y) {
      if (intersectionplanep[i][0]+(y-intersectionplanep[i][1])
		/(intersectionplanep[j][1]-intersectionplanep[i][1])
		*(intersectionplanep[j][0]-intersectionplanep[i][0])<x) {
        oddNodes=!oddNodes; }}
    j=i; }

  return oddNodes;
}


float svSlice::GetSliceXY(float *x, float *y, int &num)
{
	int sample;
	//http://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection
	svVector3 p0 = widgetp[0];
	svVector3 n = normalize(dir);
	svVector3 l; 
	svVector3 l0;
	svVector3 ip;
	l[0]=0.;l[1]=0.;l[2]=0.;
	float d;

	if(fabs(dir[0])>=fabs(dir[1])&&fabs(dir[0])>=fabs(dir[2]))
	{
		l[0]=1.;
		l0[0]=lb[0];
		l0[1]=lb[1];
		l0[2]=lb[2];
	}
	else if(fabs(dir[1])>=fabs(dir[2])&&fabs(dir[1])>=fabs(dir[0]))
	{
		l[1] =1.;
		l0[0]=lb[0];
		l0[1]=lb[1];
		l0[2]=lb[2];
	}
	else if(fabs(dir[2])>=fabs(dir[0])&&fabs(dir[2])>=fabs(dir[1]))
	{
		l[2] =1.;
		l0[0]=lb[0];
		l0[1]=lb[1];
		l0[2]=lb[2];	
	}


	selectedNum = 0;

	vector<svVector3> c;

	int ii=0;
	bool flag = true;
	/*while(flag)
	{
		d = GetDot((p0 - l0),n)/GetDot(l,n);
		ip = d *l +l0;
		c.push_back(ip);

		if(l[0]==1.)
		{
			l0[1] = l0[1] + mindistance[1];
			if(l0[1]>rb[1])
			{
				l0[2] = l0[2] +mindistance[2];
				l0[1] = lb[1];
				ii ++;
			}
			if(l0[2]>rb[2]) flag = false;

		}
		else if(l[1]==1.)
		{
			l0[0] = l0[0] + mindistance[0];
			if(l0[0]>rb[0])
			{
				l0[2] = l0[2] + mindistance[2];
				l0[0] = lb[0];
			}
			if(l0[2]>rb[2]) flag = false;
		}
		else if(l[2]==1.)
		{
			l0[0] = l0[0] + mindistance[0];
			if(l0[0]>rb[0])
			{
				l0[1] = l0[1] + mindistance[1];
				l0[0] = lb[0];
			}
			if(l0[1]>rb[1]) flag = false;
		}	
		selectedNum++;
	}*/

	delete [] cutPos;

	cutPos = new svVector3[selectedNum];
	for(int i=0;i<selectedNum;i++)
		cutPos[i] = c[i];		

	num = selectedNum;

	svVector3 px,py,pz,center;
	vector<float> z;
	int index[intersectionnum];
	int indexnum = 0;
	center[0]=0.;
	center[1]=0.;
	center[2]=0.;
	py[0]=0.;py[1]=0.;py[2]=0.;
	for(int i=0;i<intersectionnum;i++)
	{
		center = center + intersectionp[i];
		z.push_back(intersectionp[i][2]);
	}
	center[0] = center[0]/intersectionnum;
	center[1] = center[1]/intersectionnum;
	center[2] = center[2]/intersectionnum;
	sort(z.begin(),z.end());
	for(int i=0;i<intersectionnum;i++)
	{
		if(intersectionp[i][2]==z[intersectionnum-1])
			{index[indexnum]=i;indexnum++;}
	}	
	if(indexnum==4)
	{
		indexnum = 1;
		for(int i=0;i<intersectionnum;i++)
		{
			if(((intersectionp[i][1]==intersectionp[indexnum-1][1])
				|| (intersectionp[i][0]==intersectionp[indexnum-1][0]))
				&& !((intersectionp[i][1]==intersectionp[indexnum-1][1])
					&&(intersectionp[i][0]==intersectionp[indexnum-1][0])))
				{index[indexnum]=i;indexnum++;break;}
		}
	}
	for(int i=0;i<indexnum;i++)	
	{
		py = py + intersectionp[index[i]];
	}
	py[0] = py[0]/indexnum;
	py[1] = py[1]/indexnum;
	py[2] = py[2]/indexnum;
	py = py - center;
	py = normalize(py);
	pz = dir;
	px = CrossProduct(py,pz);
	double dd = 9e+9;
	double td;

	planecenter = center;
	planexyz[0] = px;
	planexyz[1] = py;
	planexyz[2] = pz;

	for(int j=0;j<num;j++)
	{
		y[j] = PointtoLine2(center, px, cutPos[j]);
		x[j] = PointtoLine2(center, py, cutPos[j]);	
		if(GetDot(cutPos[j]-center,px)<0)
			x[j] = -x[j];
		if(GetDot(cutPos[j]-center,py)<0)
			y[j] = -y[j];
		planex[j] = x[j];
		planey[j] = y[j];
		if(j>0)
		{
			td = (x[j]-x[j-1])*(x[j]-x[j-1]) + (y[j]-y[j-1])*(y[j]-y[j-1]);
			if(td<dd)
				dd  = td;
		}
	}



	mindistance = sqrt(dd);
	return mindistance;
}
float svSlice::GetSliceXY(svVector3 *xy,svVector3 *xz,svVector3 *yz,
		int xyn, int xzn, int yzn)
{
	int sample;
	//http://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection
	svVector3 p0 = widgetp[0];
	svVector3 n = normalize(dir);
	svVector3 l; 
	svVector3 l0;
	svVector3 ip;
	l[0]=0.;l[1]=0.;l[2]=0.;
	float d;
	int samplesize;

	if(fabs(dir[0])>=fabs(dir[1])&&fabs(dir[0])>=fabs(dir[2]))
	{
		l[0]=1.;
		samplesize = yzn;
	}
	else if(fabs(dir[1])>=fabs(dir[2])&&fabs(dir[1])>=fabs(dir[0]))
	{
		l[1] =1.;
		samplesize = xzn;
	}
	else if(fabs(dir[2])>=fabs(dir[0])&&fabs(dir[2])>=fabs(dir[1]))
	{
		l[2] =1.;
		samplesize = xyn;	
	}

	selectedNum = 0;

	vector<svVector3> c;

	int ii=0;

	bool flag = true;

	l0 = lb;
	for(int i=0;i<samplesize;i++)
	{
		
		if(l[0]==1.)
		{l0[0]=lb[0];l0[1]=yz[i][1];l0[2]=yz[i][2];}
		else if(l[1]==1.)
		{l0[0]=xz[i][0];l0[1]=lb[1];l0[2]=xz[i][2];}
		else if(l[2]==1.)
		{l0[0]=xy[i][0];l0[1]=xy[i][1];l0[2]=lb[2];}

		d = GetDot((p0 - l0),n)/GetDot(l,n);
		ip = d *l +l0;
		if(ip[0]>=lb[0]&&ip[0]<=rb[0]
		&&ip[1]>=lb[1]&&ip[1]<=rb[1]
		&&ip[2]>=lb[2]&&ip[2]<=rb[2])
		{
			c.push_back(ip);
			selectedNum++;
		}
	//	cerr<<l0[0]<<" "<<l0[1]<<" "<<l0[2]<<endl;
	}


	float *x = new float[selectedNum];
	float *y = new float[selectedNum];
	int num=selectedNum;
	delete [] cutPos;

	cutPos = new svVector3[selectedNum];
	for(int i=0;i<selectedNum;i++)
	{	cutPos[i] = c[i];}		

	num = selectedNum;

	svVector3 px,py,pz,center;
	vector<float> z;
	int index[intersectionnum];
	int indexnum = 0;
	center[0]=0.;
	center[1]=0.;
	center[2]=0.;
	py[0]=0.;py[1]=0.;py[2]=0.;
	for(int i=0;i<intersectionnum;i++)
	{
		center = center + intersectionp[i];
		z.push_back(intersectionp[i][2]);
	}
	center[0] = center[0]/intersectionnum;
	center[1] = center[1]/intersectionnum;
	center[2] = center[2]/intersectionnum;
	sort(z.begin(),z.end());
	for(int i=0;i<intersectionnum;i++)
	{
		if(intersectionp[i][2]==z[intersectionnum-1])
			{index[indexnum]=i;indexnum++;}
	}	
	if(indexnum==4)
	{
		indexnum = 1;
		for(int i=0;i<intersectionnum;i++)
		{
			if(((intersectionp[i][1]==intersectionp[indexnum-1][1])
				|| (intersectionp[i][0]==intersectionp[indexnum-1][0]))
				&& !((intersectionp[i][1]==intersectionp[indexnum-1][1])
					&&(intersectionp[i][0]==intersectionp[indexnum-1][0])))
				{index[indexnum]=i;indexnum++;break;}
		}
	}
	for(int i=0;i<indexnum;i++)	
	{
		py = py + intersectionp[index[i]];
	}

	py[0] = py[0]/indexnum;
	py[1] = py[1]/indexnum;
	py[2] = py[2]/indexnum;
	py = py - center;
	py = normalize(py);
	pz = dir;
	px = CrossProduct(py,pz);
	double dd = 9e+9;
	double td;

	planecenter = center;
	planexyz[0] = px;
	planexyz[1] = py;
	planexyz[2] = pz;

	for(int j=0;j<num;j++)
	{
		y[j] = PointtoLine2(center, px, cutPos[j]);
		x[j] = PointtoLine2(center, py, cutPos[j]);	
		if(GetDot(cutPos[j]-center,px)<0)
			x[j] = -x[j];
		if(GetDot(cutPos[j]-center,py)<0)
			y[j] = -y[j];
		planex[j] = x[j];
		planey[j] = y[j];
		if(j>0)
		{
			td = (x[j]-x[j-1])*(x[j]-x[j-1]) + (y[j]-y[j-1])*(y[j]-y[j-1]);
			if(td<dd)
				dd  = td;
		}
	}


	delete [] x;
	delete [] y;
	c.clear();
	mindistance = sqrt(dd);
	return mindistance;
}
float svSlice::GetSampleSlice(svVector3 *xy,svVector3 *xz,svVector3 *yz,
		int xyn, int xzn, int yzn)
{
	int sample;
	//http://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection
	svVector3 p0 = widgetp[0];
	svVector3 n = normalize(dir);
	svVector3 l; 
	svVector3 l0;
	svVector3 ip;
	l[0]=0.;l[1]=0.;l[2]=0.;
	float d;
	int samplesize;

	if(fabs(dir[0])>=fabs(dir[1])&&fabs(dir[0])>=fabs(dir[2]))
	{
		l[0]=1.;
		samplesize = yzn;
	}
	else if(fabs(dir[1])>=fabs(dir[2])&&fabs(dir[1])>=fabs(dir[0]))
	{
		l[1] =1.;
		samplesize = xzn;
	}
	else if(fabs(dir[2])>=fabs(dir[0])&&fabs(dir[2])>=fabs(dir[1]))
	{
		l[2] =1.;
		samplesize = xyn;	
	}

	selectedNum = 0;

	vector<svVector3> c;

	int ii=0;

	bool flag = true;
	l0 = lb;
	
	for(int i=0;i<samplesize;)
	{
		if(l[0]==1.)
		{l0[0]=lb[0];l0[1]=yz[i][1];l0[2]=yz[i][2];}
		else if(l[1]==1.)
		{l0[0]=xz[i][0];l0[1]=lb[1];l0[2]=xz[i][2];}
		else if(l[2]==1.)
		{l0[0]=xy[i][0];l0[1]=xy[i][1];l0[2]=lb[2];
		//cerr<<l0[0]<<" "<<l0[1]<<" "<<l0[2]<<endl;
		}

		d = GetDot((p0 - l0),n)/GetDot(l,n);
		ip = d *l +l0;

		if(ip[0]>=lb[0]&&ip[0]<=rb[0]
		&&ip[1]>=lb[1]&&ip[1]<=rb[1]
		&&ip[2]>=lb[2]&&ip[2]<=rb[2])
		{
			c.push_back(ip);
			selectedNum++;
		}

		if(samplesize>200)
			i = i + samplesize/200;//cerr<<c[i-1][0]<<" "<<c[i-1][1]<<" "<<c[i-1][2]<<endl;
		else
			i++;
		//cerr<<l0[0]<<" "<<l0[1]<<" "<<l0[2]<<endl;
	}
	//cerr<<"----"<<endl;
	//cerr<<selectedNum<<endl;	


	delete [] cutPos;
	delete [] cutSamplePos;
	float *x = new float[selectedNum];
	float *y = new float[selectedNum];
	int num = selectedNum;
	cutPos = new svVector3[selectedNum];
	cutSamplePos = new svVector3[selectedNum];
	sampleNum = selectedNum;
	for(int i=0;i<selectedNum;i++)
	{
		cutPos[i] = c[i];
		cutSamplePos[i] = c[i];
	}
		

	svVector3 px,py,pz,center;
	vector<float> z;
	int index[intersectionnum];
	int indexnum = 0;
	center[0]=0.;
	center[1]=0.;
	center[2]=0.;
	py[0]=0.;py[1]=0.;py[2]=0.;
	for(int i=0;i<intersectionnum;i++)
	{
		center = center + intersectionp[i];
		z.push_back(intersectionp[i][2]);
	}
	center[0] = center[0]/intersectionnum;
	center[1] = center[1]/intersectionnum;
	center[2] = center[2]/intersectionnum;
	sort(z.begin(),z.end());
	for(int i=0;i<intersectionnum;i++)
	{
		if(intersectionp[i][2]==z[intersectionnum-1])
			{index[indexnum]=i;indexnum++;}
	}	
	if(indexnum==4)
	{
		indexnum = 1;
		for(int i=0;i<intersectionnum;i++)
		{
			if(((intersectionp[i][1]==intersectionp[indexnum-1][1])
				|| (intersectionp[i][0]==intersectionp[indexnum-1][0]))
				&& !((intersectionp[i][1]==intersectionp[indexnum-1][1])
					&&(intersectionp[i][0]==intersectionp[indexnum-1][0])))
				{index[indexnum]=i;indexnum++;break;}
		}
	}
	for(int i=0;i<indexnum;i++)	
	{
		py = py + intersectionp[index[i]];
	}
	py[0] = py[0]/indexnum;
	py[1] = py[1]/indexnum;
	py[2] = py[2]/indexnum;
	py = py - center;
	py = normalize(py);
	pz = dir;
	px = CrossProduct(py,pz);
	double dd = 9e+9;
	double td;

	planecenter = center;
	planexyz[0] = px;
	planexyz[1] = py;
	planexyz[2] = pz;

	for(int j=0;j<num;j++)
	{
		y[j] = PointtoLine2(center, px, cutPos[j]);
		x[j] = PointtoLine2(center, py, cutPos[j]);	
		if(GetDot(cutPos[j]-center,px)<0)
			x[j] = -x[j];
		if(GetDot(cutPos[j]-center,py)<0)
			y[j] = -y[j];
		planex[j] = x[j];
		planey[j] = y[j];
		if(j>0)
		{
			td = (x[j]-x[j-1])*(x[j]-x[j-1]) + (y[j]-y[j-1])*(y[j]-y[j-1]);
			if(td<dd)
				dd  = td;
		}
	}
	delete [] x;
	delete [] y;
	c.clear();

	mindistance = sqrt(dd);
	return mindistance;
}
float svSlice::GetSampleSlice()
{
	int sample;
	//http://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection
	svVector3 p0 = widgetp[0];
	svVector3 n = normalize(dir);
	svVector3 l; 
	svVector3 l0;
	svVector3 ip;
	l[0]=0.;l[1]=0.;l[2]=0.;
	float d;

	if(fabs(dir[0])>=fabs(dir[1])&&fabs(dir[0])>=fabs(dir[2]))
	{
		l[0]=1.;
		l0[0]=lb[0];
		l0[1]=lb[1];
		l0[2]=lb[2];
	}
	else if(fabs(dir[1])>=fabs(dir[2])&&fabs(dir[1])>=fabs(dir[0]))
	{
		l[1] =1.;
		l0[0]=lb[0];
		l0[1]=lb[1];
		l0[2]=lb[2];
	}
	else if(fabs(dir[2])>=fabs(dir[0])&&fabs(dir[2])>=fabs(dir[1]))
	{
		l[2] =1.;
		l0[0]=lb[0];
		l0[1]=lb[1];
		l0[2]=lb[2];	
	}


	selectedNum = 0;

	vector<svVector3> c;

	int ii=0;
	bool flag = true;
	while(flag)
	{
		d = GetDot((p0 - l0),n)/GetDot(l,n);
		ip = d *l +l0;
		c.push_back(ip);

		if(l[0]==1.)
		{
			l0[1] = l0[1] + 3.5;
			if(l0[1]>rb[1])
			{
				l0[2] = l0[2] +3.25;
				if(ii%2!=0)
					l0[1] = lb[1];
				else l0[1] = lb[1] + 0.25;
				ii ++;
			}
			if(l0[2]>rb[2]) flag = false;

		}
		else if(l[1]==1.)
		{
			l0[0] = l0[0] + 3.5;
			if(l0[0]>rb[0])
			{
				l0[2] = l0[2] + 3.25;
				if(ii%2!=0)
					l0[0] = lb[0];
				else l0[0] = lb[0] + 0.25;ii++;
			}
			if(l0[2]>rb[2]) flag = false;
		}
		else if(l[2]==1.)
		{
			l0[0] = l0[0] + 3.5;
			if(l0[0]>rb[0])
			{
				l0[1] = l0[1] + 3.25;
				if(ii%2!=0)
					l0[0] = lb[0];
				else 
					l0[0] = lb[0] + 0.25;ii++;
			}
			if(l0[1]>rb[1]) flag = false;
		}	
		selectedNum++;
	}


	delete [] cutPos;

	cutPos = new svVector3[selectedNum];
	for(int i=0;i<selectedNum;i++)
		cutPos[i] = c[i];		

	int num = selectedNum;

	svVector3 px,py,pz,center;
	vector<float> z;
	int index[intersectionnum];
	int indexnum = 0;
	center[0]=0.;
	center[1]=0.;
	center[2]=0.;
	py[0]=0.;py[1]=0.;py[2]=0.;
	for(int i=0;i<intersectionnum;i++)
	{
		center = center + intersectionp[i];
		z.push_back(intersectionp[i][2]);
	}
	center[0] = center[0]/intersectionnum;
	center[1] = center[1]/intersectionnum;
	center[2] = center[2]/intersectionnum;
	sort(z.begin(),z.end());
	for(int i=0;i<intersectionnum;i++)
	{
		if(intersectionp[i][2]==z[intersectionnum-1])
			{index[indexnum]=i;indexnum++;}
	}	
	if(indexnum==4)
	{
		indexnum = 1;
		for(int i=0;i<intersectionnum;i++)
		{
			if(((intersectionp[i][1]==intersectionp[indexnum-1][1])
				|| (intersectionp[i][0]==intersectionp[indexnum-1][0]))
				&& !((intersectionp[i][1]==intersectionp[indexnum-1][1])
					&&(intersectionp[i][0]==intersectionp[indexnum-1][0])))
				{index[indexnum]=i;indexnum++;break;}
		}
	}
	for(int i=0;i<indexnum;i++)	
	{
		py = py + intersectionp[index[i]];
	}
	py[0] = py[0]/indexnum;
	py[1] = py[1]/indexnum;
	py[2] = py[2]/indexnum;
	py = py - center;
	py = normalize(py);
	pz = dir;
	px = CrossProduct(py,pz);
	double dd = 9e+9;
	double td;

	planecenter = center;
	planexyz[0] = px;
	planexyz[1] = py;
	planexyz[2] = pz;

	float *x = new float[num];
	float *y = new float[num];

	for(int j=0;j<num;j++)
	{
		y[j] = PointtoLine2(center, px, cutPos[j]);
		x[j] = PointtoLine2(center, py, cutPos[j]);	
		if(GetDot(cutPos[j]-center,px)<0)
			x[j] = -x[j];
		if(GetDot(cutPos[j]-center,py)<0)
			y[j] = -y[j];
		planex[j] = x[j];
		planey[j] = y[j];
		if(j>0)
		{
			td = (x[j]-x[j-1])*(x[j]-x[j-1]) + (y[j]-y[j-1])*(y[j]-y[j-1]);
			if(td<dd)
				dd  = td;
		}
	}



	mindistance = sqrt(dd);
	return mindistance;
}
/*float svSlice::GetSliceXY(float *x, float *y, int &num)
{
	int sample;
	//http://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection
	svVector3 p0 = widgetp[0];
	svVector3 n = normalize(dir);
	svVector3 l; 
	svVector3 l0;
	svVector3 ip;
	l[0]=0.;l[1]=0.;l[2]=0.;
	float d;

	if(fabs(dir[0])>=fabs(dir[1])&&fabs(dir[0])>=fabs(dir[2]))
	{
		sample = ((rb[2] - lb[2])/0.25 + 1.) * ((rb[1]-lb[1])/0.25+1.);
		l[0]=1.;
		l0[0]=lb[0];
		l0[1]=lb[1];
		l0[2]=lb[2];
	}
	else if(fabs(dir[1])>=fabs(dir[2])&&fabs(dir[1])>=fabs(dir[0]))
	{
		sample = ((rb[2] - lb[2])/0.25 + 1.) * ((rb[0]-lb[0])/0.25+1.);
		l[1] =1.;
		l0[0]=lb[0];
		l0[1]=lb[1];
		l0[2]=lb[2];
	}
	else if(fabs(dir[2])>=fabs(dir[0])&&fabs(dir[2])>=fabs(dir[1]))
	{
		sample = ((rb[1] - lb[1])/0.25 + 1.) * ((rb[0]-lb[0])/0.25+1.);
		l[2] =1.;
		l0[0]=lb[0];
		l0[1]=lb[1];
		l0[2]=lb[2];	
	}


	delete [] cutPos;

	cutPos = new svVector3[sample];
	selectedNum = 0;

	for(int i=0;i<sample;i++)
	{
		d = GetDot((p0 - l0),n)/GetDot(l,n);
		ip = d *l +l0;
		cutPos[i] = ip;

		if(l[0]==1.)
		{
			l0[1] = l0[1] + 0.25;
			if(l0[1]>rb[1])
			{
				l0[2] = l0[2] +0.25;
				l0[1] = lb[1];
			}
		}
		else if(l[1]==1.)
		{
			l0[0] = l0[0] + 0.25;
			if(l0[0]>rb[0])
			{
				l0[2] = l0[2] + 0.25;
				l0[0] = lb[0];
			}
		}
		else if(l[2]==1.)
		{
			l0[0] = l0[0] + 0.25;
			if(l0[0]>rb[0])
			{
				l0[1] = l0[1] + 0.25;
				l0[0] = lb[0];
			}
		}	
		selectedNum++;
	}

	num = selectedNum;
	svVector3 p[4];
	p[0]=svGetPerpendicularVector(n);
	p[0]=normalize(p[0]);
	p[0]= p[0]*side*2 + widgetp[0];
	p[0]=svGetRotatePoint(p[0], widgetp[0], n,-45.);	
	p[1]=svGetRotatePoint(p[0], widgetp[0], n,90.);
	p[2]=svGetRotatePoint(p[0], widgetp[0], n,180.);
	p[3]=svGetRotatePoint(p[0], widgetp[0], n,270.);

	double dd = 9e+9;
	double td;
	double x0,y0;

	x0 = PointtoLine(p[1], p[0], widgetp[0]);
	y0 = PointtoLine(p[3], p[0], widgetp[0]);

	for(int j=0;j<num;j++)
	{
		x[j] = PointtoLine(p[1], p[0], cutPos[j])-x0;
		y[j] = PointtoLine(p[3], p[0], cutPos[j])-y0;
 		planex[j] = PointtoLine(p[1], p[0], cutPos[j])-x0;
		planey[j] = PointtoLine(p[3], p[0], cutPos[j])-y0;
		if(j>0)
		{
			td = pow(x[j]-x[j-1],2.) + pow(y[j]-y[j-1],2.);
			if(td<dd)
				dd  = td;
		}
	}
	mindistance = sqrt(dd);


	return mindistance;
}*/

float svSlice::GetScale(int w, int h)
{
	float max[intersectionnum*2];
	float scale = 9e+9;
	int j=0;
	for(int i=0;i<intersectionnum;i++)
	{
		max[j] = fabs((w/2.)/intersectionplanep[i][0]);
		//max[j+1] = fabs((h/2.)/intersectionplanep[i][1]);
		max[j+1] = fabs((w/2.)/intersectionplanep[i][0]);
		j = j + 2;
	}
	for(int i=0;i<intersectionnum*2;i++)
	{
		if(max[i]<scale)
			scale = max[i];
	}

	return scale;
}

float svSlice::GetAngle1()
{
	svVector3 z;
	z[0]=0.;z[1]=0.;z[2]=1.;
	float a = GetDot(z,dir);
	a = acos(fabs(a));

	return a;
}
float svSlice::GetAngle2()
{
	svVector3 z;
	z[0]=0.;z[1]=0.;z[2]=1.;
	float a = GetDot(z,dir2);
	a = acos(fabs(a));

	return a;
}
void svSlice::cleanup()
{
	if(glyphPos!=NULL)
	{
		glyphPos[0].free();
		delete [] glyphPos;
		glyphPos = NULL;
	}
	if(glyphIndex!=NULL)
	{
		glyphIndex[0].free();
		delete [] glyphIndex;
		glyphIndex = NULL;
	}
	delete [] cutPos;
	delete [] cutSamplePos;
	delete [] planex;
	delete [] planey;
}
void svSlice::XY_vertical()
{
	dir[0] = 0.;
	dir[1] = 0.;
	dir[2] = 1.;

	widgetp[1][0] = widgetp[0][0]+dir[0]*widgetl;
	widgetp[1][1] = widgetp[0][1]+dir[1]*widgetl;
	widgetp[1][2] = widgetp[0][2]+dir[2]*widgetl;
  //  Intersection();
	dir2[0] = 1;
	dir2[1] = 0;
	dir2[2] = 0;

	widgetp[3][0] = widgetp[2][0]+dir2[0]*widgetl;
	widgetp[3][1] = widgetp[2][1]+dir2[1]*widgetl;
	widgetp[3][2] = widgetp[2][2]+dir2[2]*widgetl;

    Intersection();	
}
void svSlice::YZ_vertical()
{
	dir[0] = 1.;
	dir[1] = 0.;
	dir[2] = 0.;

	widgetp[1][0] = widgetp[0][0]+dir[0]*widgetl;
	widgetp[1][1] = widgetp[0][1]+dir[1]*widgetl;
	widgetp[1][2] = widgetp[0][2]+dir[2]*widgetl;
    Intersection();
	dir2[0] = 0;
	dir2[1] = 1;
	dir2[2] = 0;

	widgetp[3][0] = widgetp[2][0]+dir2[0]*widgetl;
	widgetp[3][1] = widgetp[2][1]+dir2[1]*widgetl;
	widgetp[3][2] = widgetp[2][2]+dir2[2]*widgetl;

    Intersection();	
}
void svSlice::XZ_vertical()
{
	dir[0] = 0.;
	dir[1] = 1.;
	dir[2] = 0.;

	widgetp[1][0] = widgetp[0][0]+dir[0]*widgetl;
	widgetp[1][1] = widgetp[0][1]+dir[1]*widgetl;
	widgetp[1][2] = widgetp[0][2]+dir[2]*widgetl;
    Intersection();
	dir2[0] = 1;
	dir2[1] = 0;
	dir2[2] = 0;

	widgetp[3][0] = widgetp[2][0]+dir2[0]*widgetl;
	widgetp[3][1] = widgetp[2][1]+dir2[1]*widgetl;
	widgetp[3][2] = widgetp[2][2]+dir2[2]*widgetl;

    Intersection();	
}
void svSlice::Reset()
{
	int i;

	widgetl = (rb[0]-lb[0])/2.;
	widgetp[0] = (lb + rb)/2.;
	widgetp[1] = widgetp[0];
	widgetp[1][1] = widgetp[1][1] + widgetl;
	widgetp[2] = widgetp[0];
	widgetp[3] = widgetp[0];
	widgetp[3][0] = widgetp[3][0] + widgetl;

	side = sqrt((rb[0]-lb[0])*(rb[0]-lb[0])
			+(rb[1]-lb[1])*(rb[1]-lb[1])
			+(rb[2]-lb[2])*(rb[2]-lb[2]))/sqrt(2.);
	SELECTED = GL_FALSE;
	SELECTED_WHITE = GL_FALSE;
	SELECTED_BLUE = GL_FALSE;

	dir = widgetp[1]-widgetp[0];
	dir2 = widgetp[3] - widgetp[2];


	intersectionnum = 0;
	intersectionnum2 = 0;

	Intersection();
	SetPlaneXYZ();
}
void svSlice::SetPoints(svVector3 *w, svVector3 d1, svVector3 d2)
{
	for(int i=0;i<5;i++)
		widgetp[i] = w[i];
	dir = d1;
	dir2 = d2;
    Intersection();
}

void svSlice::move_ctrlpoint(int x, int y)
{
    GLdouble xx, yy, zz;
    GLdouble wld_x, wld_y, wld_z;       /* world coordinate */
    GLdouble tx, ty, tz;
    GLfloat invert_tb[16], tm[16];

    int i, j, k;
	float deltal;
	float tmpl;

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

    if(ni==1)
    {
	dir[0] = wld_x - widgetp[0][0];
	dir[1] = wld_y - widgetp[0][1];
	dir[2] = wld_z - widgetp[0][2];
	dir = normalize(dir);
	widgetp[1][0] = widgetp[0][0]+dir[0]*widgetl;
	widgetp[1][1] = widgetp[0][1]+dir[1]*widgetl;
	widgetp[1][2] = widgetp[0][2]+dir[2]*widgetl;
    Intersection();
    }
    else if(ni==0)
    {
	deltal = (wld_x*dir[0]+wld_y*dir[1]+wld_z*dir[2])/
		sqrt(dir[0]*dir[0]+dir[1]*dir[1]+dir[2]*dir[2]);
	tmpl = (widgetp[0][0]*dir[0]+widgetp[0][1]*dir[1]+widgetp[0][2]*dir[2])/
		sqrt(dir[0]*dir[0]+dir[1]*dir[1]+dir[2]*dir[2]);
	dir = normalize(dir);
	widgetp[0][0]=widgetp[0][0]+(deltal-tmpl)*dir[0];
	widgetp[0][1]=widgetp[0][1]+(deltal-tmpl)*dir[1];
	widgetp[0][2]=widgetp[0][2]+(deltal-tmpl)*dir[2];
	widgetp[1][0] = widgetp[0][0]+dir[0]*widgetl;
	widgetp[1][1] = widgetp[0][1]+dir[1]*widgetl;
	widgetp[1][2] = widgetp[0][2]+dir[2]*widgetl;
    Intersection();
    }
    else if(ni==2)
    {
	deltal = (wld_x*dir2[0]+wld_y*dir2[1]+wld_z*dir2[2])/
		sqrt(dir2[0]*dir2[0]+dir2[1]*dir2[1]+dir2[2]*dir2[2]);
	tmpl = (widgetp[2][0]*dir2[0]+widgetp[2][1]*dir2[1]+widgetp[2][2]*dir2[2])/
		sqrt(dir2[0]*dir2[0]+dir2[1]*dir2[1]+dir2[2]*dir2[2]);
	dir2 = normalize(dir2);
	widgetp[2][0]=widgetp[2][0]+(deltal-tmpl)*dir2[0];
	widgetp[2][1]=widgetp[2][1]+(deltal-tmpl)*dir2[1];
	widgetp[2][2]=widgetp[2][2]+(deltal-tmpl)*dir2[2];
	widgetp[3][0] = widgetp[2][0]+dir2[0]*widgetl;
	widgetp[3][1] = widgetp[2][1]+dir2[1]*widgetl;
	widgetp[3][2] = widgetp[2][2]+dir2[2]*widgetl;
    Intersection();
    }
    else if(ni==3)
    {
	dir2[0] = wld_x - widgetp[2][0];
	dir2[1] = wld_y - widgetp[2][1];
	dir2[2] = wld_z - widgetp[2][2];
	dir2 = normalize(dir2);
	widgetp[3][0] = widgetp[2][0]+dir2[0]*widgetl;
	widgetp[3][1] = widgetp[2][1]+dir2[1]*widgetl;
	widgetp[3][2] = widgetp[2][2]+dir2[2]*widgetl;
    Intersection();
    }
    else if(ni==4)
	{moveChooseBox(wld_x,wld_y, wld_z);}
}

void svSlice::moveChooseBox(float wld_x,float wld_y,float wld_z)
{
	//http://stackoverflow.com/questions/8942950/how-do-i-find-the-orthogonal-projection-of-a-point-onto-a-plane
	svVector3 q, p, n;
	q[0] = wld_x;
	q[1] = wld_y;
	q[2] = wld_z;	
	n = dir;
	p = widgetp[0];
	svVector3 q_proj = q - GetDot(q-p,n)*n;
	svVector3 qplane;
	qplane[0] = PointtoLine2(planecenter,planexyz[1],q_proj);
	qplane[1] = PointtoLine2(planecenter,planexyz[0],q_proj);
	if(GetDot(q_proj-planecenter,planexyz[0])<0)
		qplane[0] = -qplane[0];
	if(GetDot(q_proj-planecenter,planexyz[1])<0)
		qplane[1] = -qplane[1];

	if(pointinpolygon(qplane[0],qplane[1]))
	{
		svVector3 delta;
		delta = q_proj - chooseboxcenter;
		svVector3 p[4];
		for(int i=0;i<4;i++)
			p[i] = choosebox[i] + delta;
		svVector3 pp[4];
		for(int i=0;i<4;i++)
		{
			pp[i][0] = PointtoLine2(planecenter,planexyz[1],p[i]);
			pp[i][1] = PointtoLine2(planecenter,planexyz[0],p[i]);
			if(GetDot(pp[i]-planecenter,planexyz[0])<0)
				pp[i][0] = -pp[i][0];
			if(GetDot(pp[i]-planecenter,planexyz[1])<0)
				pp[i][1] = -pp[i][1];
		}
		if(pointinpolygon(pp[0][0],pp[0][1])
		&&pointinpolygon(pp[1][0],pp[1][1])
		&&pointinpolygon(pp[2][0],pp[2][1])
		&&pointinpolygon(pp[3][0],pp[3][1]))
		{
			chooseboxcenter = q_proj;
			for(int i=0;i<4;i++)
				choosebox[i] = choosebox[i] + delta;		
		}
	}
}

void svSlice::SetBoxCenter(float x,float y)
{
	chooseboxcenter = initboxcenter + x * planexyz[0];
	chooseboxcenter = chooseboxcenter + y * planexyz[1];

	for(int i=0;i<4;i++)
	{
		choosebox[i] = initchoosebox[i] + x * planexyz[0];
		choosebox[i] = choosebox[i] + y * planexyz[1];
	}
}

void svSlice::SetBoxShape(float ratio)
{
	/*float dd1 = sqrt(pow(choosebox[3][0] - choosebox[0][0],2.)
			+pow(choosebox[3][1] - choosebox[0][1],2.)
			+pow(choosebox[3][2] - choosebox[0][2],2.));
	float dd2 = sqrt(pow(choosebox[3][0] - choosebox[2][0],2.)
			+pow(choosebox[3][1] - choosebox[2][1],2.)
			+pow(choosebox[3][2] - choosebox[2][2],2.));

	dd1 = dd2 * ratio;
	initialside[0] = dd1;
	initialside[1] = dd2;*/
	initialside[0] = ratio * initialside[1];
	float dd1 = initialside[0];
	float dd2 = initialside[1];
	dd1 = dd1/2.;
	dd2 = dd2/2.;

	//cerr<<initboxcenter[0]<<" "<<initboxcenter[1]<<" "<<initboxcenter[2]<<" "<<dd1<<endl;
	//cerr<<planexyz[0][0]<<" "<<planexyz[0][1]<<" "<<planexyz[0][2]<<endl;
	choosebox[0]=initboxcenter - dd1 * planexyz[0];
	//cerr<<choosebox[0][0]<<" "<<choosebox[0][1]<<" "<<choosebox[0][2]<<endl;
	choosebox[0]=choosebox[0] + dd2 * planexyz[1];
	//cerr<<choosebox[0][0]<<" "<<choosebox[0][1]<<" "<<choosebox[0][2]<<endl;
	choosebox[1]=initboxcenter - dd1 * planexyz[0];
	choosebox[1]=choosebox[1] - dd2 * planexyz[1];
	choosebox[2]=initboxcenter + dd1 * planexyz[0];
	choosebox[2]=choosebox[2] - dd2 * planexyz[1];
	choosebox[3]=initboxcenter + dd1 * planexyz[0];
	choosebox[3]=choosebox[3] + dd2 * planexyz[1];	
	//for(int i=0;i<4;i++)
	//{	
	//	cerr<<choosebox[i][0]<<" "<<choosebox[i][1]<<" "<<choosebox[i][2]<<endl;
	//}cerr<<"------"<<endl;
}

svVector3 svSlice::GetBoxCenter()
{
	svVector3 center;
	float x,y;

	y = PointtoLine2(planecenter, planexyz[0], chooseboxcenter);
	x = PointtoLine2(planecenter, planexyz[1], chooseboxcenter);

	if(GetDot(chooseboxcenter-planecenter,planexyz[0])<0)
		x = -x;
	if(GetDot(chooseboxcenter-planecenter,planexyz[1])<0)
		y = -y;

	center[0] = x;
	center[1] = y;
	center[2] = 0.;

	return center;
}

void svSlice::Find_Nearest(int x, int y)
{
    int i, j;
    GLdouble td, dd;
    svVector3 p[5];

    //glGetIntegerv (GL_VIEWPORT, viewport);
    //glGetDoublev (GL_MODELVIEW_MATRIX, mvmatrix);
    //glGetDoublev (GL_PROJECTION_MATRIX, projmatrix);

	for(i=0;i<5;i++)
	{
		p[i][0]=0;p[i][1]=0;p[i][2]=0;	
	}

	for(i=0;i<5;i++)
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
	
	for(i=0;i<5;i++)
	{
		gluProject((GLdouble)p[i][0],
                        (GLdouble)p[i][1],
                        (GLdouble)p[i][2],
                        mvmatrix, projmatrix, viewport,
                        &(wx[i]), &(wy[i]), &(wz[i]));
		wy[i] = (GLdouble)width-wy[i];
		//cerr<<wx[i]<<" "<<wy[i]<<endl;
	}


    dd=9e+9;

    	ni = 0;
	for(i=0;i<5;i++)
	{
		td = ((GLdouble)x-wx[i]) *  ((GLdouble)x-wx[i]) +
                 ((GLdouble)y-wy[i]) * ((GLdouble)y-wy[i]);
		if(td<dd)
		{
			dd = td;
			ni = i;
		}
	}
	if(ni==0&&dd<50.){SELECTED=GL_TRUE;SELECTED_WHITE=GL_TRUE;}
	else if(ni==1&&dd<100.){SELECTED=GL_TRUE;SELECTED_WHITE=GL_TRUE;}
	else if(ni==2&&dd<50.){SELECTED=GL_TRUE;SELECTED_BLUE=GL_TRUE;}
	else if(ni==3&&dd<100.){SELECTED=GL_TRUE;SELECTED_BLUE=GL_TRUE;}
	else if(ni==4&&dd<100.)SELECTED=GL_TRUE;
}

GLboolean svSlice::invert_matrix( const GLfloat *m, GLfloat *out )
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
