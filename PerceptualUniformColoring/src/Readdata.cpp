#include "Readdata.h"
#define PI 3.1415926

namespace __svl_lib {

void Readdata::ProcessData(char *infname, char *outfname)
{
	ifstream infile;
	infile.open(infname);

	char str[100];
	sprintf(str, "tmp.data");

	ofstream outfile;
	outfile.open(str);

	svScalar num;
	infile>>num;
	outfile<<num<<endl;

	int data = 0;
	for(int i=0;i<num;i++)
	{
		svVector3 p;
		svVector3 c;
		vector<svVector3> pos;
		vector<svVector3> col;
		svScalar n;
		svScalar fa;
		int count = 0;

		infile>>n;//cerr<<n<<endl;

		for(int j=0;j<(int)n;j++)
		{
			//infile>>fa;
			infile>>p[0]>>p[1]>>p[2]>>c[0]>>c[1]>>c[2];
			fa = 1-c[2];

			if(fa >= 0.)
			{
				count++;
				pos.push_back(p);
				col.push_back(c);
			}
			
			if((fa<0. && count>1) || (j == (int)n-1 && count >1))
			{
				outfile<<count<<endl;
				for(int m=0;m<pos.size();m++)
				{
					outfile<<pos[m][0]<<" "<<pos[m][1]<<" "<<pos[m][2]<<" "<<col[m][0]<<" "<<col[m][1]<<" "<<col[m][2]<<endl;
				}
				pos.clear();
				col.clear();
				count = 0;
				data ++;
			}
		}

	}

	infile.close();
	outfile.close();

	infile.open(str);
	outfile.open(outfname);

	infile>>num;
	outfile<<data<<endl;

	for(int i=0;i<data;i++)
	{
		svVector3 p;
		svVector3 c;
		svScalar n;

		infile>>n;//cerr<<n<<endl;
		outfile<<n<<endl;
		for(int j=0;j<(int)n;j++)
		{
			//infile>>fa;
			infile>>p[0]>>p[1]>>p[2]>>c[0]>>c[1]>>c[2];
			outfile<<p[0]<<" "<<p[1]<<" "<<p[2]<<" "<<c[0]<<" "<<c[1]<<" "<<c[2]<<endl;
		}

	}

	infile.close();
	outfile.close();

}

void Readdata::New(char *infname)
{
	ifstream infile;
	infile.open(infname);

	svScalar num;
	infile>>num;

	linenum = (int)num;
	//vector<double> fasum;
	//vector<double> fasum2;

	datapos = new svVector3Array[linenum];
	//data2dpos = new svVector3Array[linenum];
	datacol = new svVector3Array[linenum];
	datafa = new svScalarArray[linenum];
	//dataorder = new svInt[linenum];

	dataSize = 0;
	maxfa = -9e+9;
	minfa = 9e+9;

	svScalar x = 0;
	svScalar y = 0;
	svScalar size = 1.;

	/*for(int i=0;i<linenum;i++)
	{
		fasum.push_back(0);
		fasum2.push_back(0);
	}
*/

	lb[0]=9e+9;lb[1]=9e+9;lb[2]=9e+9;
	rb[0]=-9e+9;rb[1]=-9e+9;rb[2]=-9e+9;
	lb2[0]=9e+9;lb2[1]=9e+9;lb2[2]=9e+9;
	rb2[0]=-9e+9;rb2[1]=-9e+9;rb2[2]=-9e+9;
	bool inrange[2];
	inrange[0] = false;
	inrange[1] = false;
	for(int i=0;i<linenum;i++)
	{
		svVector3 p;
		svVector3 c;
		svScalar n;
		svScalar fa;

		inrange[0] = true;
		inrange[1] = true;

		infile>>n;//cerr<<n<<endl;
		for(int j=0;j<(int)n;j++)
		{
			//infile>>fa;
			infile>>p[0]>>p[1]>>p[2]>>c[0]>>c[1]>>c[2];
			fa = 1-c[2];
			datapos[i].add(p);
			datacol[i].add(c);
			datafa[i].add(fa);

			if(c[2]==0)cerr<<i<<" ";

				if(p[0]>rb[0])rb[0]=p[0];
				if(p[1]>rb[1])rb[1]=p[1];
				if(p[2]>rb[2])rb[2]=p[2];
				if(p[0]<lb[0])lb[0]=p[0];
				if(p[1]<lb[1])lb[1]=p[1];
				if(p[2]<lb[2])lb[2]=p[2];
						
				if(maxfa<fa)maxfa=fa;
				if(minfa>fa)minfa=fa;

				p[0] = x;
				p[1] = y;
				//data2dpos[i].add(p);	
			
				if(p[0]>rb2[0])rb2[0]=p[0];
				if(p[1]>rb2[1])rb2[1]=p[1];
				if(p[0]<lb2[0])lb2[0]=p[0];
				if(p[1]<lb2[1])lb2[1]=p[1];

				y = y + size;

				dataSize ++;

				//fasum[i] = fasum[i] + fa;
			
		}
		//fasum[i] = fasum[i]/datapos[i].size();
		
		//fasum2[i] = fasum[i];

		y = 0;
		x = x + size;
	}

	infile.close();

	/*sort(fasum.begin(),fasum.end());
	for(int i=0;i<linenum;i++)
	{
		int tt;
		for(int j=0;j<linenum;j++)
		{
			if(fasum[i] == fasum2[j])
			{
				tt = j;
				break;
			}
		}
		dataorder[i] = tt;
		//cerr<<dataorder[i]<<" ";
	}//cerr<<endl;
	
*/
	//cerr<<minfa<<" "<<maxfa<<endl;
	//databound = new svIntArray[linenum];
	minfa = 0.;
	maxfa = 1;
	databound = NULL;
	data2dpos = NULL;
	dataorder_y = NULL;

	row_num = linenum/iwidth;
}

void Readdata::Removedata(double bound)
{
	if(databound !=NULL)
	{
		for(int i=0;i<linenum;i++)	
		{
			databound[i].free();
		}
		delete [] databound;
		databound = NULL;
		delete [] datafiberin;
	}

	databound = new svIntArray[linenum];
	datafiberin = new svInt[linenum];


	for(int i=0;i<linenum;i++)
	{
		for(int j=0;j<datapos[i].size();j++)
		{
			databound[i].add(1);
		}
	}
	

	for(int i=0;i<linenum;i++)
	{
		for(int j=0;j<datapos[i].size();j++)
		{
			if(datafa[i][j]<bound)
			{
				databound[i][j] = 0;//cerr<<datafa[i][j]<<endl;
			}
			else
			{
				break;
			}
		}
	}
	for(int i=0;i<linenum;i++)
	{//cerr<<i<<" ";
		for(int j=datapos[i].size()-1;j>=0;j--)
		{
			if(datafa[i][j]<bound)
			{
				databound[i][j] = 0;
			}
			else
			{
				break;
			}
		}
	}

	int count = 0;
	
	for(int i=0;i<linenum;i++)
	{
		count = 0;
		for(int j=0;j<datapos[i].size();j++)
		{
			if(databound[i][j])
			{
				count ++;
			}
			else
			{
				//cerr<<datafa[i][j]<<endl;
			}
		}
		if(count > 0)
			datafiberin[i] = 1;
		else
		{	datafiberin[i] = 0;}
	}
}

void Readdata::ReOrder()
{
	if(data2dpos != NULL)
	{
		for(int i=0;i<linenum;i++)
			data2dpos[i].free();
		delete [] data2dpos;
		data2dpos = NULL;
	}
	if(dataorder_y!=NULL)
	{
		for(int i=0;i<linenum;i++)
		{
			dataorder_y[i].free();
		}
		delete [] dataorder_y;
		dataorder_y =NULL;
		delete [] dataorder;
	}

	data2dpos = new svVector3Array[linenum];
	dataorder = new svInt[linenum];
	dataorder_y = new svIntArray[linenum];

	vector<double> fasum;
	vector<double> fasum2;
	vector<double> fasum_y;
	vector<double> fasum_y2;

	svScalar x = 0;
	svScalar y = 0;
	svScalar size = 1.;
	svVector3 p;

	for(int i=0;i<linenum;i++)
	{
		fasum.push_back(0);
		fasum2.push_back(0);
	}
	//int count = 0;

	maxfa = -9e+9;
	minfa = 9e+9;

	for(int i=0;i<linenum;i++)
	{//count = 0;
		if(datafiberin[i])
		{	//count++;	
			for(int j=0;j<datapos[i].size();j++)
			{
				if(databound[i][j])
				{
					//if(datafa[i][j]<0.2)
					//cerr<<datafa[i][j]<<" "<<databound[i][j]<<endl;
					if(maxfa<datafa[i][j])maxfa=datafa[i][j];
					if(minfa>datafa[i][j])minfa=datafa[i][j];

					p[0] = x;
					p[1] = y;
					data2dpos[i].add(p);
					y = y + size;
					
					fasum[i] = fasum[i] + datafa[i][j];

					fasum_y.push_back(datafa[i][j]);
					fasum_y2.push_back(datafa[i][j]);	
				}
			}
			fasum[i] = fasum[i]/datapos[i].size();
			fasum2[i] = fasum[i];

			y = 0;
			x = x + size;

			sort(fasum_y.begin(), fasum_y.end());
			for(int ii=0;ii<datapos[i].size();ii++)
			{
				int tt;
				if(databound[i][ii])
				{
					for(int jj=0;jj<datapos[i].size();jj++)
					{
						if(databound[i][jj])
						{
							if(fasum_y[ii] == fasum_y2[jj])
							{
								tt = jj;break;
							}
						}
					}
					dataorder_y[i].add(tt);
				}
			}		

			fasum_y.clear();
			fasum_y2.clear();	
		

		}//cerr<<data2dpos[i].size()<<" ";
	}

	sort(fasum.begin(),fasum.end());cerr<<"b"<<endl;
	for(int i=0;i<linenum;i++)
	{//cerr<<i<<endl;
		int tt;
		for(int j=0;j<linenum;j++)
		{
			if(fasum[i] == fasum2[j])
			{
				tt = j;
				break;
			}
		}
		dataorder[i] = tt;
		//if(tt>count)cerr<<dataorder[i]<<" ";
	}//cerr<<endl;
	maxfa = 1;
	minfa = 0.;
	cerr<<minfa<<" "<<maxfa<<endl;
}


void Readdata::NewMultipleColors()
{
	if(multiplecol!=NULL)
	{
		delete [] multiplecol;
	}

	multiplecol = new svVector3[numofcolor];

	CIEColor *cc = new CIEColor();

	double t = 0;
	double s=1.;
	double b=0.1;
	double c=0.8;
	double h1;
	double h2= 0;
	double w=0.15;
	double degree = 1/((double)numofcolor-1);
	double hdegree = 360/((double)numofcolor-1);
	vector<double> rgb;

	for(int i=0;i<numofcolor;i++)
	{
		t = i * degree;
		rgb = cc->MultiplehueSequential(t,s,b,c,h1,h2,w);
		h2 = i * hdegree;
		multiplecol[i][0] = rgb[0];
		multiplecol[i][1] = rgb[1];
		multiplecol[i][2] = rgb[2];

	}

	delete [] cc;
}

void Readdata::NewSingleColors()
{
	if(singlecol!=NULL)
	{
		delete [] singlecol;
	}

	singlecol = new svVector3[numofcolor];

	CIEColor *cc = new CIEColor();

	double t = 0.;
	double s= 0.6;
	double b= 0.75;
	double c= 0.88;
	double h1 = 180;
	double h2= 0;
	double w=0.15;
	double degree = 1/((double)numofcolor-1);
	double hdegree = 360/((double)numofcolor-1);
	vector<double> rgb;

	for(int i=0;i<numofcolor;i++)
	{
		t = i * degree;
		rgb = cc->SinglehueSequential(t,s,b,c,h1);
		h2 = i * hdegree;
		singlecol[i][0] = rgb[0];
		singlecol[i][1] = rgb[1];
		singlecol[i][2] = rgb[2];

	}

	delete [] cc;
}

void Readdata::NewDivergingColors()
{
	if(divergecol!=NULL)
	{
		delete [] divergecol;
	}

	divergecol = new svVector3[numofcolor];

	CIEColor *cc = new CIEColor();

	double t = 0.;
	double s= 0.6;
	double b= 0.75;
	double c= 0.88;
	double h1 = 40;
	double h2= 120;
	double w=0.;
	double m =0.5;
	double degree = 1/((double)numofcolor-1.);
	double hdegree = 360/((double)numofcolor-1.);
	vector<double> rgb;

	for(int i=0;i<numofcolor;i++)
	{
		t = i * degree;
		rgb = cc->DiverginghueSequential(t,s,b,c,h1,h2,w,m); //not implemented 
		divergecol[i][0] = rgb[0];
		divergecol[i][1] = rgb[1];
		divergecol[i][2] = rgb[2];
		//cerr<<rgb[0]<<" "<<rgb[1]<<" "<<rgb[2]<<endl;
	}

	delete [] cc;
}


void InitLight()
{
  //setting of lighting
  GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8, .4};
  GLfloat mat_specular[] = { 1, 1, 1, 1 };
  GLfloat mat_shininess[] = { 20.0 };
 // GLfloat light_position[] = { 0, 0, 60, 0.0 };
  GLfloat light_position[] = { 0.,0.,100, 0.0 };
  GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };
  glClearColor(0.0, 0.0, 0.0, 0.0);

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);

}

void  Readdata::RenderMultipleLegend()
{
/*
    GLdouble vmat[16], pmat[16], sx, sy, sz;
    GLint    viewport[4];

    glGetDoublev(GL_MODELVIEW_MATRIX, vmat);
    glGetDoublev(GL_PROJECTION_MATRIX, pmat);
    glGetIntegerv(GL_VIEWPORT, viewport);


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,(GLfloat) viewport[2], 0.0, (GLfloat) viewport[3]);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_LIGHTING);*/


	//glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	float x = 0;
	float y = 0;
	float size = 1;

	glPushMatrix();
	glTranslatef(iwidth+50.,200.,0);
	glScalef(20,4,1);

	int n = (maxfa-minfa)/0.1;cerr<<n<<endl;
	double degree = (maxfa-minfa)/(float)n;cerr<<degree<<endl;
	double fa;


	for(int i=0;i<numofcolor;i++)
	{
		glColor3f(multiplecol[i][0],multiplecol[i][1],multiplecol[i][2]);
		glBegin(GL_QUADS);
		glVertex2f(x,y);
		glVertex2f(x+size,y);
		glVertex2f(x+size,y+size);
		glVertex2f(x,y+size);
		glEnd();
		y = y + size;
	}

	glColor3f(0.1,0.1,0.1);

	glBegin(GL_LINES);
	glVertex2f(size *1.5,y);
	glVertex2f(size *1.5,0);
	glEnd();

	x = size * 1.5; y =0;

	for(int i=0;i<n+1;i++)
	{
		glBegin(GL_LINES);
		
		glVertex2f(x,y);
		glVertex2f(x*1.5,y);

		glEnd();

		char str[20];
		sprintf(str, "%0.2f", minfa + i * degree);
		glRasterPos2f(x*1.5,y);
		for(int j=0;j<strlen(str);j++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);		

		y = y + (double)numofcolor/(float)n;
	}

	glPopMatrix();

/*    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(pmat);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(vmat);*/
}

void  Readdata::RenderDivergingLegend()
{

 GLdouble vmat[16], pmat[16], sx, sy, sz;
    GLint    viewport[4];

  /*  glGetDoublev(GL_MODELVIEW_MATRIX, vmat);
    glGetDoublev(GL_PROJECTION_MATRIX, pmat);
    glGetIntegerv(GL_VIEWPORT, viewport);


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,(GLfloat) viewport[2], 0.0, (GLfloat) viewport[3]);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();*/

	//glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	int n = (maxfa-minfa)/0.1;
	double degree = (maxfa-minfa)/(float)n;
	float x = 0;
	float y = 0;
	float size = 1;

	glPushMatrix();
	glTranslatef(iwidth+50.,200.,0);
	glScalef(20,4,1);

	for(int i=0;i<numofcolor;i++)
	{
		glColor3f(divergecol[i][0],divergecol[i][1],divergecol[i][2]);
		glBegin(GL_QUADS);
		glVertex2f(x,y);
		glVertex2f(x+size,y);
		glVertex2f(x+size,y+size);
		glVertex2f(x,y+size);
		glEnd();

		y = y + size;
	}
	glColor3f(0.1,0.1,0.1);

	glBegin(GL_LINES);
	glVertex2f(size *1.5,y);
	glVertex2f(size *1.5,0);
	glEnd();

	x = size * 1.5; y =0;

	for(int i=0;i<n+1;i++)
	{
		glBegin(GL_LINES);
		
		glVertex2f(x,y);
		glVertex2f(x*1.5,y);

		glEnd();

		char str[20];
		sprintf(str, "%0.2f", minfa + i * degree);
		glRasterPos2f(x*1.5,y);
		for(int j=0;j<strlen(str);j++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);		

		y = y + (double)numofcolor/(float)n;
	}
	glPopMatrix();
/*
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(pmat);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(vmat);
*/
}
void  Readdata::RenderSingleLegend()
{

 GLdouble vmat[16], pmat[16], sx, sy, sz;
    GLint    viewport[4];

  /*  glGetDoublev(GL_MODELVIEW_MATRIX, vmat);
    glGetDoublev(GL_PROJECTION_MATRIX, pmat);
    glGetIntegerv(GL_VIEWPORT, viewport);


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,(GLfloat) viewport[2], 0.0, (GLfloat) viewport[3]);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();*/

	//glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	int n = (maxfa-minfa)/0.1;
	double degree = (maxfa-minfa)/(float)n;
	float x = 0;
	float y = 0;
	float size = 1;

	glPushMatrix();
	glTranslatef(iwidth+50.,200.,0);
	glScalef(20,4,1);

	for(int i=0;i<numofcolor;i++)
	{
		//glColor3f(1., 1.-i*1./float(numofcolor-1),1.-i*1./float(numofcolor-1));
		glColor3f(singlecol[i][0],singlecol[i][1],singlecol[i][2]);
		glBegin(GL_QUADS);
		glVertex2f(x,y);
		glVertex2f(x+size,y);
		glVertex2f(x+size,y+size);
		glVertex2f(x,y+size);
		glEnd();

		y = y + size;
	}
	glColor3f(0.1,0.1,0.1);

	glBegin(GL_LINES);
	glVertex2f(size *1.5,y);
	glVertex2f(size *1.5,0);
	glEnd();

	x = size * 1.5; y =0;

	for(int i=0;i<n+1;i++)
	{
		glBegin(GL_LINES);
		
		glVertex2f(x,y);
		glVertex2f(x*1.5,y);

		glEnd();

		char str[20];
		sprintf(str, "%0.2f", minfa + i * degree);
		glRasterPos2f(x*1.5,y);
		for(int j=0;j<strlen(str);j++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);		

		y = y + (double)numofcolor/(float)n;
	}
	glPopMatrix();
/*
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(pmat);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(vmat);
*/
}
void Readdata::Generate(int type)
{
	if(glIsList(1))
		glDeleteLists(1, 1);

	glGenLists(1);
	glNewList(1, GL_COMPILE);


	glEnable(GL_DEPTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
    	glEnable(GL_COLOR_MATERIAL);
    	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	
	glEnable( GL_LINE_SMOOTH );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPushMatrix();
	glTranslatef((lb[0]+rb[0])/-2, (lb[1]+rb[1])/-2, (lb[2]+rb[2])/-2);

	int section_num = 15;
	svScalar r = 0.25;
	svVector3 *norm, *prevnorm;
	svVector3 *pos, *prevpos;
	norm = new svVector3[section_num];
	prevnorm = new svVector3[section_num];
	pos = new svVector3[section_num];
	prevpos = new svVector3[section_num];
	svVector3 segment_dir;
	svVector3 prev_segment_dir;
	double angle_degree = 360/((float)section_num-1.);
	svVector3 c, prevc;

	int count = 0;
	for(int i = 0;i<linenum;i++)
	{//cerr<<i<<endl;
		int j =0;
		count = 0;
		if(datafiberin[i])
		{
			for(int jj=0;jj<datapos[i].size();jj++)
			{//cerr<<j<<" ";
				//j = jj;
				if(databound[i][jj])
				{
					///count ++;
					if(j!=datapos[i].size()-1)
						segment_dir = normalize(datapos[i][j+1] - datapos[i][j]);
					if(j!=0)
						prev_segment_dir = datapos[i][j] - datapos[i][j-1];
					if(j==0)
					{
							if(type==1)
							{
								int index = (int)((datafa[i][j] - minfa)/colordegree);
								c = singlecol[index];
							}
							else if(type == 2)
							{
								int index = (int)((datafa[i][j] - minfa)/colordegree);//cerr<<index<<" ";
								prevc = multiplecol[index];
							}
							else if(type == 3)
							{
								int index = (int)((datafa[i][j] - minfa)/colordegree);//cerr<<index<<" ";
								prevc = divergecol[index];
							}
							c = prevc;
					}	
					else
					{		
						prevc = c;
						if(type==1)
						{
						int index = (int)((datafa[i][j] - minfa)/colordegree);
						c = singlecol[index];
						}
						else if(type == 2)
						{
							int index = (int)((datafa[i][j] - minfa)/colordegree);//cerr<<index<<" ";
							c = multiplecol[index];
						}
						else if(type == 3)
						{
							int index = (int)((datafa[i][j] - minfa)/colordegree);
							c = divergecol[index];
						}
					}
					//cerr<<j<<" ";
					glBegin(GL_QUAD_STRIP);
					for(int m =0; m<section_num;m++)
					{
						if(j==0)
						{
							if(m==0)
							{
								prevnorm[m] = svGetPerpendicularVector(segment_dir);
								prevpos[m] = datapos[i][j] + r * prevnorm[m];
							}
							else
							{
								prevpos[m] = svGetRotatePoint(prevpos[0], datapos[i][j], segment_dir,  (float)m * angle_degree);
								prevnorm[m] = normalize(prevpos[m] - datapos[i][j]);
							}
				
							norm[m] = prevnorm[m];
							pos[m] = prevpos[m];
						//	if(j==0 &&i==0)
		//cerr<<pos[m][0]<<" "<<pos[m][1]<<" "<<pos[m][2]<<" "<<endl;

						}	
						else
						{
							prevnorm[m] = norm[m];
							prevpos[m] = pos[m];
							if(m==0)
							{
								pos[m] = prevpos[m] + prev_segment_dir;
								norm[m] = normalize(pos[m] - datapos[i][j]);
							}
							else
							{
								pos[m] = svGetRotatePoint(pos[0], datapos[i][j], segment_dir, (float)m * angle_degree);
								norm[m] = normalize(pos[m] - datapos[i][j]);
							}

							glNormal3f(prevnorm[m][0],prevnorm[m][1],prevnorm[m][2]);
							glColor3f(prevc[0],prevc[1],prevc[2]);
							glVertex3f(prevpos[m][0],prevpos[m][1],prevpos[m][2]);

							glNormal3f(norm[m][0],norm[m][1],norm[m][2]);
							glColor3f(c[0],c[1],c[2]);
							glVertex3f(pos[m][0],pos[m][1],pos[m][2]);

					
						}
					}
					glEnd();
					j++;			
				}
			}//cerr<<j<<" "<<count<<endl;
		}
	//	cerr<<j<<" ";
	}

	glPopMatrix();

	glEndList();

	delete [] norm;
	delete [] prevnorm;
	delete [] pos;
	delete [] prevpos;
}
void Readdata::Generate_Bar(int type)
{
	/*if(glIsList(3))
		glDeleteLists(3, 1);

	glGenLists(1);
	glNewList(3, GL_COMPILE);*/

	glDisable(GL_DEPTH);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	glPushMatrix();
	glTranslatef(1.,2.*(iheight*(row_num-1)/row_num + 10.),0.);
	glScalef(1.,1.5,1.);

	int section_num = 6;
	svScalar r = 1;
	svVector3 *norm, *prevnorm;
	svVector3 *pos, *prevpos;
	norm = new svVector3[section_num];
	prevnorm = new svVector3[section_num];
	pos = new svVector3[section_num];
	prevpos = new svVector3[section_num];
	svVector3 segment_dir;
	double angle_degree = 2*PI/(section_num-1);
	svVector3 c, prevc;

	float x,y,size;
	float oy = 0;
	size = 1.;
	x = 0;
	y = 0;

	double maxy = -9e+9;

	for(int ii = 0;ii<linenum;ii++)
	{
		int i = ii;
		if(datafiberin[i] && datapos[i].size()>1)
		{
			glBegin(GL_QUAD_STRIP);
			for(int jj=datapos[i].size()-1;jj>=0;jj--)
			{
				if(databound[i][jj])
				{
					int j = jj;//dataorder_y[i][jj];
					//segment_dir = normalize(datapos[i][1] - datapos[i][0]);
					if(type==1)
					{
						int index = (int)((datafa[i][j] - minfa)/colordegree);
						c = singlecol[index];
						//prevc = datacol[i][j+1];
					}
					else if(type == 2)
					{
						int index = (int)((datafa[i][j] - minfa)/colordegree);
						c = multiplecol[index];
						//index = (int)((datafa[i][j+1] - minfa)/colordegree);
						//prevc = multiplecol[index];
					}
					else if(type == 3)
					{
						int index = (int)((datafa[i][j] - minfa)/colordegree);
						c = divergecol[index];
						//index = (int)((datafa[i][j+1] - minfa)/colordegree);
						//prevc = divergecol[index];
					}
			
					glColor3f(c[0],c[1],c[2]);
//cerr<<x<<" "<<y<<" "<<x+size<<" "<<c[0]<<" "<<c[1]<<" "<<c[2]<<endl;
					glVertex2f(x,y);
					glVertex2f(x+size,y);
					//glVertex2f(data2dpos[i][j][0],data2dpos[i][j][1]);
					//glVertex2f(data2dpos[i][j][0]+1,data2dpos[i][j][1]);
					//glColor3f(prevc[0],prevc[1],prevc[2]);
					//glVertex2f(data2dpos[i][j+1][0],data2dpos[i][j+1][1]);
					//glVertex2f(data2dpos[i][j+1][0]+1,data2dpos[i][j+1][1]);

					y = y+size;
				}
			
			}
			glEnd();
			
			//if(y==126)cerr<<x<<" "<<y<<" "<<i<<endl;
			x = x + x_d;
			//if(ii<20)
			//cerr<<x<<" "<<y<<" "<<(int)iheight<<" "<<(int)iwidth<<" "<<row_num<<" "<<((int)x/(int)iwidth)<<endl;
			oy = oy - iheight/row_num * (((int)x)/((int)iwidth));
			y = oy;
			x = x - iwidth * ((int)x/(int)iwidth);
			//if(ii<20)
			//cerr<<x<<" "<<y<<" "<<(int)iheight<<" "<<(int)iwidth<<" "<<row_num<<" "<<((int)x/(int)iwidth)<<endl;
		}
	}
	//cerr<<maxy<<endl;
	glPopMatrix();
}
void Readdata::Render_OrderBar(int type, int order_type)
{

	/*if(glIsList(3))
		glDeleteLists(3, 1);

	glGenLists(1);
	glNewList(3, GL_COMPILE);*/

	glDisable(GL_DEPTH);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	glPushMatrix();
	glTranslatef(1.,2.*(iheight*(row_num-1)/row_num + 10.),0.);
	glScalef(1.,1.5,1.);

	int section_num = 6;
	svScalar r = 1;
	svVector3 *norm, *prevnorm;
	svVector3 *pos, *prevpos;
	norm = new svVector3[section_num];
	prevnorm = new svVector3[section_num];
	pos = new svVector3[section_num];
	prevpos = new svVector3[section_num];
	svVector3 segment_dir;
	double angle_degree = 2*PI/(section_num-1);
	svVector3 c, prevc;

	float x,y,size;
	size = 1.;
	x = 0;
	y = 0;
	float oy = 0;

	double maxy = -9e+9;
	bool flag =false;

	for(int ii = 0;ii<linenum;ii++)
	{
		int i;
		if(order_type==0 || order_type == 2) i= dataorder[ii];
		else i = ii;
		if(datafiberin[i] && datapos[i].size()>1)
		{
			glBegin(GL_QUAD_STRIP);
			for(int jj=datapos[i].size()-1;jj>=0;jj--)
			{
				if(databound[i][jj])
				{
					int j;
					if(order_type==1 || order_type == 2) j= dataorder_y[i][jj];
					else j = jj;
					//segment_dir = normalize(datapos[i][1] - datapos[i][0]);
					if(type==1)
					{
						int index = (int)((datafa[i][j] - minfa)/colordegree);
						c = singlecol[index];
						//prevc = datacol[i][j+1];
					}
					else if(type == 2)
					{
						int index = (int)((datafa[i][j] - minfa)/colordegree);
						c = multiplecol[index];
						//index = (int)((datafa[i][j+1] - minfa)/colordegree);
						//prevc = multiplecol[index];
					}
					else if(type == 3)
					{
						int index = (int)((datafa[i][j] - minfa)/colordegree);
						c = divergecol[index];
						//index = (int)((datafa[i][j+1] - minfa)/colordegree);
						//prevc = divergecol[index];
					}
			
					glColor3f(c[0],c[1],c[2]);
					glVertex2f(x,y);
					glVertex2f(x+size,y);
					//glVertex2f(data2dpos[i][j][0],data2dpos[i][j][1]);
					//glVertex2f(data2dpos[i][j][0]+1,data2dpos[i][j][1]);
					//glColor3f(prevc[0],prevc[1],prevc[2]);
					//glVertex2f(data2dpos[i][j+1][0],data2dpos[i][j+1][1]);
					//glVertex2f(data2dpos[i][j+1][0]+1,data2dpos[i][j+1][1]);

					y = y+size;
				}
			
			}
			glEnd();
			//if(ii<20)
			//cerr<<x<<" "<<y<<" "<<endl;
			//if(y==126)cerr<<i<<endl;
			//y = 0;
			x = x + x_d;
			//if(ii<20)
			//cerr<<x<<" "<<y<<" "<<(int)iheight<<" "<<(int)iwidth<<" "<<row_num<<" "<<((int)x/(int)iwidth)<<endl;
			oy = oy - iheight/row_num * (((int)x)/((int)iwidth));
			y = oy;
			x = x - iwidth * ((int)x/(int)iwidth);
		}
	}
	//cerr<<maxy<<endl;
	glPopMatrix();

	//glEndList();
}

void Readdata::Render(int n)
{
	glCallList(n);
}

}
