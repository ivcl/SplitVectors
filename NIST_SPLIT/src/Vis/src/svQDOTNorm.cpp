
#include <math.h>
#include "svQDOTNorm.h"
#include "QDOTdataNorm.h"

#include "ludcmp.h"
#include "lubksb.h"
#include "MVectorMatrix.h"
#include <iostream>
#include <fstream>
#include <algorithm>

#include <vector>

#ifndef max
#define max(a,b)  ((a)>(b)?(a):(b))
#endif

#define NEIGHBOR_NUM 50

#ifndef M_PI
#define M_PI 3.1415926
#endif 

namespace __svl_lib {

svQDOTNorm::svQDOTNorm()
{
  ctrpos=NULL;
  ctrvel=NULL;
  ctrden=NULL;
  ctrexp=NULL;
  ctrcoe=NULL;
  xyplane=NULL;
  yzplane=NULL;
  xzplane=NULL;
  denMax = NULL;
  denMin = NULL;
  zsplitpos = NULL;
  zsplitvel = NULL;
  zsplitden = NULL;
  zsplitexp = NULL;
  zsplitcoe = NULL;
  zsplit_neighbor1 = NULL;
  zsplit_neighbor2 = NULL;
  sortPos = NULL;
  sortDen = NULL;
  sortVel = NULL;
  sortVel2 = NULL;
  histoden=NULL;
  histoROIden=NULL;
}

svQDOTNorm::~svQDOTNorm()
{
  cleanUp();
}

// read qdot data 
void svQDOTNorm::New(char *indir, char *infname, int seedNum)
{
  dimension = 3;
  max_varing_time = 0;
  flow_type = SV_STEADY;

  setProfileFromData(indir, infname, seedNum);
}

void svQDOTNorm::New(char *indir, char *infname, float contourvalue)
{
  dimension = 3;
  max_varing_time = 0;
  flow_type = SV_STEADY;

  setProfileFromData(indir, infname, contourvalue);
}

void svQDOTNorm::New(char *indir, char *infname)
{
  dimension = 3;
  max_varing_time = 0;
  flow_type = SV_STEADY;

  setProfileFromData(indir, infname);
}

vector<float> xvector;
vector<float> yvector;
vector<float> zvector;
vector<int> index;

bool comparez (int a, int b) { return zvector[a] < zvector[b]; }
bool comparey (int a, int b) { return yvector[a] < yvector[b]; }
bool comparex (int a, int b) { return xvector[a] < xvector[b]; }

bool compare(int a, int b)
{
	if(zvector[a]!=zvector[b])
		return zvector[a]<zvector[b];
	else if(yvector[a]!=yvector[b])
		return yvector[a]<yvector[b];
	else if(xvector[a]!=xvector[b])
		return xvector[a]<xvector[b];
}

bool comparexy(int a, int b)
{
	if(yvector[a]!=yvector[b])
		return yvector[a]<yvector[b];
	else if(xvector[a]!=xvector[b])
		return xvector[a]<xvector[b];	
}
bool comparexz(int a, int b)
{
	if(zvector[a]!=zvector[b])
		return zvector[a]<zvector[b];
	else if(xvector[a]!=xvector[b])
		return xvector[a]<xvector[b];	
}
bool compareyz(int a, int b)
{
	if(zvector[a]!=zvector[b])
		return zvector[a]<zvector[b];
	else if(yvector[a]!=yvector[b])
		return yvector[a]<yvector[b];	
}
void svQDOTNorm::SortData()
{	
	cerr<<"Start sorting data. Please wait...."<<endl;
	int i;

	zvector.clear();
	xvector.clear();
	yvector.clear();
	index.clear();

	for(i=0;i<dataSize;i++)
	{
		zvector.push_back(ctrpos[i][2]);
		xvector.push_back(ctrpos[i][0]);
		yvector.push_back(ctrpos[i][1]);
		index.push_back(i);
//cerr<<index[i]<<" "<<ctrpos[index[i]][0]<<" "<<ctrpos[index[i]][1]<<" "<<ctrpos[index[i]][2]<<" "<<endl;
	}

	sort(index.begin(),index.end(), compare);
	sort(index.begin(),index.end(), compare);
	sort(index.begin(),index.end(), compare);


	xindex.clear();
	yindex.clear();
	zindex.clear();
	yinz.clear();
	xinz.clear();
	xiny.clear();
	zformat.clear();
	num_z[0].clear();
	num_z[1].clear();
	sortpos.clear();
	sortvel.clear();
	sortindex.clear();
	ydistance.clear();
	xdistance.clear();
	delete [] xyplane;
	delete [] xzplane;
	delete [] yzplane;
	sortP[0].clear();
	sortP[1].clear();
	sortP[2].clear();
	sortV[0].clear();
	sortV[1].clear();
	sortV[2].clear();
	sortD.clear();

	svVector3 tmp;
	int countx = 0;
	int county = 0;
	int countxy = 0;
	xindex.push_back(0);
	yindex.push_back(0);
	zindex.push_back(0);
	xinz.push_back(0);
	yinz.push_back(0);
	xiny.push_back(0);
	//xdistance.push_back(0);
	//ydistance.push_back(0);

	float deltax;
	deltax = fabs(ctrpos[index[0]][0]-(int)ctrpos[index[0]][0]);
	float deltay;
	deltay = fabs(ctrpos[index[0]][1]-(int)ctrpos[index[0]][1]);
	if(deltax==deltay) zformat.push_back(true);
	else zformat.push_back(false);


	//ofstream infile("/home/butterfly/Documents/test.txt");
	//infile<<dataSize<<endl;
	zlayernum = 1;
	float yd,xd;
	yd = 0;xd = 0;

	int count_eachz = 0;
	num_z[1].push_back(0);
	//ofstream outfiles("test");
	for(i=0;i<dataSize;i++)
	{
		tmp = ctrpos[index[i]];

		sortpos.push_back(tmp);
		sortvel.push_back(ctrvel[index[i]]);
		//infile<<tmp[0]<<" "<<tmp[1]<<" "<<tmp[2]<<endl;
		sortindex.push_back(index[i]);
		if(i!=0)
		{
			if(sortpos[i][2]!=sortpos[i-1][2])
			{
				count_eachz = 0;
				zindex.push_back(i);
				yinz.push_back(county);
				xinz.push_back(countx);
				deltax = fabs(ctrpos[index[i]][0]-(int)ctrpos[index[i]][0]);
				deltay = fabs(ctrpos[index[i]][1]-(int)ctrpos[index[i]][1]);
				if(deltax==deltay) zformat.push_back(true);
				else zformat.push_back(false);
				zlayernum++;
				ydistance.push_back(yd);
				yd = 0;
				zdistance = sortpos[i][2] -sortpos[i-1][2];
			}
			if(sortpos[i][1]!=sortpos[i-1][1])
			{
				yindex.push_back(i);
				xiny.push_back(countx+1);
				if(sortpos[i][2]==sortpos[i-1][2])
					yd = sortpos[i][1]-sortpos[i-1][1];
				county++;
				xdistance.push_back(xd);
				xd = 0;
			}
			if(sortpos[i][0]!=sortpos[i-1][0])
			{
				xindex.push_back(i);
				if(sortpos[i][2]==sortpos[i-1][2]
					&&sortpos[i][1]==sortpos[i-1][1])
					xd = sortpos[i][0]-sortpos[i-1][0];
				countx++;
			}
			if(sortpos[i][2]==sortpos[i-1][2])
				count_eachz++;
			//cerr<<(float)count_eachz<<" ";
			
			num_z[1].push_back(count_eachz);
		}
		num_z[0].push_back(zlayernum-1);
	}
	//outfiles.close();

	ydistance.push_back(yd);
	xdistance.push_back(xd);
	//infile.close();
	
	float tmpzmin = ctrpos[index[0]][2];
	for(int ii=0;ii<dataSize;ii++)
	{
		int jj = (int)((ctrpos[index[ii]][2] - tmpzmin)/zdistance);
		
		zsplitpos[jj].add(ctrpos[index[ii]]);
		zsplitvel[jj].add(ctrvel[index[ii]]);
		zsplitden[jj].add(ctrden[index[ii]]);
		zsplitexp[jj].add(ctrexp[index[ii]]);
		zsplitcoe[jj].add(ctrcoe[index[ii]]);
		
	}
	/*ofstream outfile("z_10z_50_out");
	for(int ii=45;ii<=45;ii++)
	{
		for(int jj=0;jj<zsplitpos[ii].size();jj++)
		{
			outfile<<zsplitpos[ii][jj][0]<<" "<<zsplitpos[ii][jj][1]<<" "<<zsplitpos[ii][jj][2]<<" "
				<<zsplitvel[ii][jj][0]<<" "<<zsplitvel[ii][jj][1]<<" "<<zsplitvel[ii][jj][2]<<endl;
		}
	}
	outfile.close();*/

	for(i=0;i<dataSize;i++)
	{
		index[i] = i;
	}
	xyplane = new svVector3[dataSize];
	xysample = 1;
	sort(index.begin(),index.end(), comparexy);
	sort(index.begin(),index.end(), comparexy);
	xyplane[0] = ctrpos[index[0]];
	int j=1;
	for(i=1;i<dataSize;i++)
	{
		if(ctrpos[index[i]][0]!=ctrpos[index[i-1]][0]
		||ctrpos[index[i]][1]!=ctrpos[index[i-1]][1])
		{xyplane[j]=ctrpos[index[i]];j++;xysample++;}
		
	}
	//cerr<<xysample<<endl;
	for(i=0;i<dataSize;i++)
	{
		index[i] = i;
	}
	xzplane = new svVector3[dataSize];
	xzsample = 1;
	sort(index.begin(),index.end(), comparexz);
	sort(index.begin(),index.end(), comparexz);
	xzplane[0] = ctrpos[index[0]];
	j=1;
	for(i=1;i<dataSize;i++)
	{
		if(ctrpos[index[i]][0]!=ctrpos[index[i-1]][0]
		||ctrpos[index[i]][2]!=ctrpos[index[i-1]][2])
		{xzplane[j]=ctrpos[index[i]];j++;xzsample++;}
	}
	//cerr<<xzsample<<endl;
	for(i=0;i<dataSize;i++)
	{
		index[i] = i;
	}
	yzplane = new svVector3[dataSize];
	yzsample = 1;
	sort(index.begin(),index.end(), compareyz);
	sort(index.begin(),index.end(), compareyz);
	yzplane[0] = ctrpos[index[0]];
	j=1;
	for(i=1;i<dataSize;i++)
	{
		if(ctrpos[index[i]][2]!=ctrpos[index[i-1]][2]
		||ctrpos[index[i]][1]!=ctrpos[index[i-1]][1])
		{yzplane[j]=ctrpos[index[i]];j++;yzsample++;}
	}

	int ii=0;
	for(int i=0;i<zValue;i++)
	{
		for(int j=0;j<zsplitpos[i].size();j++)
		{
			sortPos[ii] = zsplitpos[i][j];
			sortDen[ii] = zsplitden[i][j];
			sortVel[ii] = zsplitvel[i][j];
			ii ++;
		}
	}
	//cerr<<yzsample<<endl;
/*	
	mindistance[0] = 9e+9;
	mindistance[1] = 9e+9;
	mindistance[2] = 9e+9;
	for(int ii=0;ii<dataSize;ii++)
	{
		for(int jj=ii+1;jj<dataSize;jj++)
		{
			if(ctrpos[jj][0]>ctrpos[ii][0]-xdistance[0]
				 &&ctrpos[jj][0]<ctrpos[ii][0]+xdistance[0]
				 &&ctrpos[jj][1]>ctrpos[ii][1]-ydistance[0]
				&&ctrpos[jj][1]<ctrpos[ii][1]+ydistance[0])									

			{
				float tmpdis = fabs(ctrpos[ii][0]-ctrpos[jj][0]);			
				if(tmpdis<mindistance[0])
					mindistance[0] = tmpdis;
				
				tmpdis = fabs(ctrpos[ii][1]-ctrpos[jj][1]);
				if(tmpdis<mindistance[1])
					mindistance[1] = tmpdis;	
			}
		}
	}
	mindistance[2] = zdistance;
*/	
	ii=0;
	vector<double> p[3];
	vector<double> v[3];
	vector<double> d;

	for(int i=0;i<zValue;i++)
	{
		for(int j=0;j<zsplitpos[i].size();j++)
		{
			p[0].push_back(zsplitpos[i][j][0]);
			p[1].push_back(zsplitpos[i][j][1]);
			p[2].push_back(zsplitpos[i][j][2]);
			v[0].push_back(zsplitvel[i][j][0]);
			v[1].push_back(zsplitvel[i][j][1]);
			v[2].push_back(zsplitvel[i][j][2]);
			d.push_back(zsplitden[i][j]);
		}
		sortP[0].push_back(p[0]);
		sortP[1].push_back(p[1]);
		sortP[2].push_back(p[2]);
		sortV[0].push_back(v[0]);
		sortV[1].push_back(v[1]);
		sortV[2].push_back(v[2]);
		sortD.push_back(d);

		p[0].clear();
		p[1].clear();
		p[2].clear();
		v[0].clear();
		v[1].clear();
		v[2].clear();
		d.clear();
	}
	cerr<<"Sorting is done"<<endl;
/*svVector3 sss;
sss[0]=-17;sss[1]=-17;sss[2]=-10;
cerr<<GetInterDen(sss)<<endl;
int aaa = Interpolation_neighbor(-9, 1, sss, sss);
if(aaa!=-1)
cerr<<sortpos[aaa][0]<<" "<<sortpos[aaa][1]<<" "<<sortpos[aaa][2]<<" "<<endl;
cerr<<"1"<<endl;

aaa = Interpolation_neighbor(-9, 2, sss, sss);
if(aaa!=-1)
cerr<<sortpos[aaa][0]<<" "<<sortpos[aaa][1]<<" "<<sortpos[aaa][2]<<" "<<endl;
cerr<<"1"<<endl;

aaa = Interpolation_neighbor(-9, 3, sss, sss);
cerr<<aaa<<endl;
if(aaa!=-1)
cerr<<sortpos[aaa][0]<<" "<<sortpos[aaa][1]<<" "<<sortpos[aaa][2]<<" "<<endl;
cerr<<"1"<<endl;

aaa = Interpolation_neighbor(-9, 4, sss, sss);
if(aaa!=-1)
cerr<<sortpos[aaa][0]<<" "<<sortpos[aaa][1]<<" "<<sortpos[aaa][2]<<" "<<endl;
cerr<<"1"<<endl;

aaa = Interpolation_neighbor(-9, 5, sss, sss);
if(aaa!=-1)
cerr<<sortpos[aaa][0]<<" "<<sortpos[aaa][1]<<" "<<sortpos[aaa][2]<<" "<<endl;
cerr<<"1"<<endl;*/
}

void svQDOTNorm::SetSymmetry()
{
	cerr<<"Start detecting the symmetric mode .... Several minutes are needed...."<<endl;

	svIntArray *tmpzi = new svIntArray[zValue];
	svVector3Array *tmpvel = new svVector3Array[zValue];
	svVector3Array *tmppos = new svVector3Array[zValue];

	int count = 0;
	bool flag = false;

	for(int i=0;i<zValue;i++)
	{
		for(int j=0;j<zsplitpos[i].size();j++)
		{
			tmpzi[i].add(-1);
		}
	}

	for(int i=0;i<zValue;i++)
	{
		//cerr<<i<<endl;	
		count = 0;
		for(int j=0;j<zsplitpos[i].size();j++)
		{
			for(int k=j;k<zsplitpos[i].size();k++)
			{
				if(fabs(zsplitpos[i][j][0]+zsplitpos[i][k][0])<0.0001
				&& fabs(zsplitpos[i][j][1]-zsplitpos[i][k][1])<0.0001)//y
				{
					if(fabs(zsplitvel[i][j][0]+zsplitvel[i][k][0])<0.0001
					&& fabs(zsplitvel[i][j][1]-zsplitvel[i][k][1])<0.0001)
					{
						flag = true;
					}
				}
				else if(fabs(zsplitpos[i][j][1]+zsplitpos[i][k][1])<0.0001
				&& fabs(zsplitpos[i][j][0]-zsplitpos[i][k][0])<0.0001)//x
				{
					if(fabs(zsplitvel[i][j][1]+zsplitvel[i][k][1])<0.0001
					&& fabs(zsplitvel[i][j][0]-zsplitvel[i][k][0])<0.0001)
					{
						flag = true;
					}
				}
				else if(fabs(zsplitpos[i][j][1]+zsplitpos[i][k][1])<0.0001
				&& fabs(zsplitpos[i][j][0]+zsplitpos[i][k][0])<0.0001)//origin
				{
					if(fabs(zsplitvel[i][j][1]+zsplitvel[i][k][1])<0.0001
					&& fabs(zsplitvel[i][j][0]+zsplitvel[i][k][0])<0.0001)
					{
						flag = true;
					}
				}
				else if(fabs(zsplitpos[i][j][1]-zsplitpos[i][k][0])<0.0001
				&& fabs(zsplitpos[i][j][0]-zsplitpos[i][k][1])<0.0001)//y=x
				{
					if(fabs(zsplitvel[i][j][1]-zsplitvel[i][k][0])<0.0001
					&& fabs(zsplitvel[i][j][0]-zsplitvel[i][k][1])<0.0001)
					{
						flag = true;
					}
				}
				else if(fabs(zsplitpos[i][j][1]+zsplitpos[i][k][0])<0.0001
				&& fabs(zsplitpos[i][j][0]+zsplitpos[i][k][1])<0.0001)//y=-x
				{
					if(fabs(zsplitvel[i][j][1]+zsplitvel[i][k][0])<0.0001
					&& fabs(zsplitvel[i][j][0]+zsplitvel[i][k][1])<0.0001)
					{
						flag = true;
					}
				}

				if(flag==true)
				{
					if(tmpzi[i][j]==-1&&tmpzi[i][k]==-1)
					{
						count ++;
						tmpzi[i][j] = count;
						tmpzi[i][k] = count;
						tmpvel[i].add(zsplitvel[i][j]);
						tmppos[i].add(zsplitpos[i][j]);
					}
					else if(tmpzi[i][j]!=-1)
					{
						tmpzi[i][k] = tmpzi[i][j];
					}
					else if(tmpzi[i][k]!=-1)
					{
						tmpzi[i][j] = tmpzi[i][k];
					}
				}
				flag = false;
			}
		}
	}

	//update 
	for(int i=0;i<zValue;i++)
	{
		for(int j=0;j<zsplitpos[i].size();j++)
		{
			if(tmpzi[i][j]!=-1)
			{
				//cerr<<tmpzi[i][j]<<endl;
				/*if(zsplitpos[i][j][0]>tmppos[i][tmpzi[i][j]-1][0])
				{
					tmppos[i][tmpzi[i][j]-1] = zsplitpos[i][j];
					tmpvel[i][tmpzi[i][j]-1] = zsplitvel[i][j];
				}	
				else if(zsplitpos[i][j][0]==tmppos[i][tmpzi[i][j]-1][0]
					&& zsplitpos[i][j][1]>tmppos[i][tmpzi[i][j]-1][1])
				{
					tmppos[i][tmpzi[i][j]-1] = zsplitpos[i][j];
					tmpvel[i][tmpzi[i][j]-1] = zsplitvel[i][j];
				}*/	
				if(zsplitvel[i][j][0]>tmpvel[i][tmpzi[i][j]-1][0])
				{
					tmppos[i][tmpzi[i][j]-1] = zsplitpos[i][j];
					tmpvel[i][tmpzi[i][j]-1] = zsplitvel[i][j];
				}	
				else if(zsplitvel[i][j][0]==tmpvel[i][tmpzi[i][j]-1][0]
					&& zsplitvel[i][j][1]>tmpvel[i][tmpzi[i][j]-1][1])
				{
					tmppos[i][tmpzi[i][j]-1] = zsplitpos[i][j];
					tmpvel[i][tmpzi[i][j]-1] = zsplitvel[i][j];
				}
			}
		}
	}

	//restore
	int ii=0;
	for(int i=0;i<zValue;i++)
	{
		for(int j=0;j<zsplitpos[i].size();j++)
		{
			sortPos[ii] = zsplitpos[i][j];
			sortDen[ii] = zsplitden[i][j];
			sortVel2[ii] = zsplitvel[i][j];
			if(tmpzi[i][j]!=-1)
				sortVel[ii] = tmpvel[i][tmpzi[i][j]-1];
			else
				sortVel[ii] = zsplitvel[i][j];
			ii ++;
		}
	}

	//release
	for(int i=0;i<zValue;i++)
	{
		tmpvel[i].free();
		tmppos[i].free();
		tmpzi[i].free();
	}
	delete [] tmpvel;
	delete [] tmppos;
	delete [] tmpzi;

}

void svQDOTNorm::StoreNeighbours_space(char *infile)
{
	int z = zValue/2;
	float lx = rbbox[0]-lbbox[0];
	float ly = rbbox[1]-lbbox[1];
	
	for(int j=0;j<zsplitpos[z].size();j++)
	{
		if(zsplitpos[z][j][1]>lbbox[1]+ly/3.
				&&zsplitpos[z][j][1]<lbbox[1]+ly*2./3.
				&&zsplitpos[z][j][0]>lbbox[0]+lx/3.
				&&zsplitpos[z][j][0]<lbbox[0]+lx*2./3.)
		{
			sample_index[1] = j; 
			break;
		}
	}
	sample_index[0] = z;
	
	int count = 0;
	vector<float> dis;
	int neighbor[NEIGHBOR_NUM*2];
	int num = NEIGHBOR_NUM;

	
	while(count>=NEIGHBOR_NUM)
	{
		for(int i=0;i<zValue;i++)
		{
			if(zsplitpos[i][0][2]>zsplitpos[sample_index[0]][sample_index[1]][2]-num*zdistance
					&&zsplitpos[i][0][2]<zsplitpos[sample_index[0]][sample_index[1]][2]+num*zdistance)
					{
						for(int j=0;j<zsplitpos[i].size();j++)
						{
							if(zsplitpos[i][j][0]>zsplitpos[sample_index[0]][sample_index[1]][0]-num*zdistance
									&&zsplitpos[i][j][0]<zsplitpos[sample_index[0]][sample_index[1]][0]+num*zdistance
									&&zsplitpos[i][j][1]>zsplitpos[sample_index[0]][sample_index[1]][1]-num*zdistance
									&&zsplitpos[i][j][1]<zsplitpos[sample_index[0]][sample_index[1]][1]+num*zdistance)
							{
								float td = (zsplitpos[i][j][0]-zsplitpos[sample_index[0]][sample_index[1]][0])*(zsplitpos[i][j][0]-zsplitpos[sample_index[0]][sample_index[1]][0])
										+ (zsplitpos[i][j][1]-zsplitpos[sample_index[0]][sample_index[1]][1])*(zsplitpos[i][j][1]-zsplitpos[sample_index[0]][sample_index[1]][1])
										+ (zsplitpos[i][j][2]-zsplitpos[sample_index[0]][sample_index[1]][2])*(zsplitpos[i][j][2]-zsplitpos[sample_index[0]][sample_index[1]][2]);
								dis.push_back(td);
								count++;
							}
						}
					}
		}
		
		if(count<NEIGHBOR_NUM)
		{
			num = num + 5;
			dis.clear();
		}
	}
	sort(dis.begin(),dis.end());
	
	char *infilename;
	sprintf(infilename,"%s_inspcae",infile);
	
	ofstream readfile(infilename);
	if(readfile)
	{
		readfile.close();
	}
	else
	{
		for(int ii=0;ii<zValue;ii++)
		{
				for(int jj=0;jj<zsplitpos[ii].size();jj++)
				{
					
					for(int i=0;i<NEIGHBOR_NUM*2;i++)
						neighbor[i] = -1;
					
					for(int i=0;i<zValue;i++)
					{
						if(zsplitpos[i][0][2]>zsplitpos[ii][jj][2]-num*zdistance
								&&zsplitpos[i][0][2]<zsplitpos[ii][jj][2]+num*zdistance)
								{
									for(int j=0;j<zsplitpos[i].size();j++)
									{
										if(zsplitpos[i][j][0]>zsplitpos[ii][jj][0]-num*zdistance
												&&zsplitpos[i][j][0]<zsplitpos[ii][jj][0]+num*zdistance
												&&zsplitpos[i][j][1]>zsplitpos[ii][jj][1]-num*zdistance
												&&zsplitpos[i][j][1]<zsplitpos[ii][jj][1]+num*zdistance)
										{
											float td = (zsplitpos[i][j][0]-zsplitpos[ii][jj][0])*(zsplitpos[i][j][0]-zsplitpos[ii][jj][0])
													+ (zsplitpos[i][j][1]-zsplitpos[ii][jj][1])*(zsplitpos[i][j][1]-zsplitpos[ii][jj][1])
													+ (zsplitpos[i][j][2]-zsplitpos[ii][jj][2])*(zsplitpos[i][j][2]-zsplitpos[ii][jj][2]);
											for(int k=0;k<NEIGHBOR_NUM;k++)
											{
												if(td==dis[k]&&neighbor[k*2]==-1)
												{
													neighbor[k*2] = i; 
													neighbor[k*2+1] = j;
													break;
												}
											}
										}
									}
								}
					}
					
					for(int i=0;i<NEIGHBOR_NUM;i++)
					{
						readfile<<neighbor[i*2]<<" "<<neighbor[i*2+1]<<" ";
					}readfile<<endl;
					
				}
		}
		readfile.close();
	}
}
void svQDOTNorm::StoreNeighbours_layer(char *infile)
{
	int z = zValue/2;
	float lx = rbbox[0]-lbbox[0];
	float ly = rbbox[1]-lbbox[1];
	
	for(int j=0;j<zsplitpos[z].size();j++)
	{
		if(zsplitpos[z][j][1]>lbbox[1]+ly/3.
				&&zsplitpos[z][j][1]<lbbox[1]+ly*2./3.
				&&zsplitpos[z][j][0]>lbbox[0]+lx/3.
				&&zsplitpos[z][j][0]<lbbox[0]+lx*2./3.)
		{
			sample_index[1] = j; 
			break;
		}
	}
	sample_index[0] = z;
	
	int count = 0;
	vector<float> dis;
	int neighbor[NEIGHBOR_NUM*2];
	int num = NEIGHBOR_NUM;

	
	while(count>=NEIGHBOR_NUM)
	{
			for(int j=0;j<zsplitpos[sample_index[0]].size();j++)
			{
						if(zsplitpos[sample_index[0]][j][0]>zsplitpos[sample_index[0]][sample_index[1]][0]-num*zdistance
								&&zsplitpos[sample_index[0]][j][0]<zsplitpos[sample_index[0]][sample_index[1]][0]+num*zdistance
								&&zsplitpos[sample_index[0]][j][1]>zsplitpos[sample_index[0]][sample_index[1]][1]-num*zdistance
								&&zsplitpos[sample_index[0]][j][1]<zsplitpos[sample_index[0]][sample_index[1]][1]+num*zdistance)
						{
							float td = (zsplitpos[sample_index[0]][j][0]-zsplitpos[sample_index[0]][sample_index[1]][0])*(zsplitpos[sample_index[0]][j][0]-zsplitpos[sample_index[0]][sample_index[1]][0])
									+ (zsplitpos[sample_index[0]][j][1]-zsplitpos[sample_index[0]][sample_index[1]][1])*(zsplitpos[sample_index[0]][j][1]-zsplitpos[sample_index[0]][sample_index[1]][1]);
							dis.push_back(td);
							count++;
						}
			}
		
		if(count<NEIGHBOR_NUM)
		{
			num = num + 5;
			dis.clear();
		}
	}
	sort(dis.begin(),dis.end());
	
	char *infilename;
	sprintf(infilename,"%s_onespcae",infile);
	
	ofstream readfile(infilename);
	if(readfile)
	{
		readfile.close();
	}
	else
	{
		for(int ii=0;ii<zValue;ii++)
		{
				for(int jj=0;jj<zsplitpos[ii].size();jj++)
				{
					
					for(int i=0;i<NEIGHBOR_NUM*2;i++)
						neighbor[i] = -1;
					
					for(int j=0;j<zsplitpos[ii].size();j++)
					{
							if(zsplitpos[ii][j][0]>zsplitpos[ii][jj][0]-num*zdistance
									&&zsplitpos[ii][j][0]<zsplitpos[ii][jj][0]+num*zdistance
									&&zsplitpos[ii][j][1]>zsplitpos[ii][jj][1]-num*zdistance
									&&zsplitpos[ii][j][1]<zsplitpos[ii][jj][1]+num*zdistance)
							{
								float td = (zsplitpos[ii][j][0]-zsplitpos[ii][jj][0])*(zsplitpos[ii][j][0]-zsplitpos[ii][jj][0])
										+ (zsplitpos[ii][j][1]-zsplitpos[ii][jj][1])*(zsplitpos[ii][j][1]-zsplitpos[ii][jj][1]);
								for(int k=0;k<NEIGHBOR_NUM;k++)
								{
									if(td==dis[k]&&neighbor[k*2]==-1)
									{
										neighbor[k*2] = ii; 
										neighbor[k*2+1] = j;
										break;
									}
								}
							}
					}

					
					for(int i=0;i<NEIGHBOR_NUM;i++)
					{
						readfile<<neighbor[i*2]<<" "<<neighbor[i*2+1]<<" ";
					}readfile<<endl;
					
				}
		}
		readfile.close();
	}
}

void svQDOTNorm::PlaneNeighbors(float z, int x)
{
	float zmin = sortpos[0][2];
	int zindex_standard = (z-zmin)/zdistance;

	if(zsplit_neighbor1!=NULL)
	{
		for(int i=0;i<zValue;i++)
		{
			zsplit_neighbor1[i].free();
			zsplit_neighbor2[i].free();
		}
		delete [] zsplit_neighbor1;
		delete [] zsplit_neighbor2;
		zsplit_neighbor1 = NULL;
		zsplit_neighbor2 = NULL;
	}

	zsplit_neighbor1 = new svIntArray[zValue];
	zsplit_neighbor2 = new svIntArray[zValue];

	for(int i=0;i<zValue;i++)
	{
		
		if(i!=zindex_standard)
		{
			for(int j=0;j<zsplitpos[i].size();j++)
			{
				int tmp = Interpolation_neighbor(z,x,zsplitpos[i][j],zsplitvel[i][j]);	
				if(tmp!=-1)
				{
					zsplit_neighbor1[i].add(num_z[0][tmp]);
					zsplit_neighbor2[i].add(num_z[1][tmp]);
				}
				else
				{
					zsplit_neighbor1[i].add(-1);
					zsplit_neighbor2[i].add(-1);					
				}
			}	
		}
		else
		{
			for(int j=0;j<zsplitpos[i].size();j++)
			{
				zsplit_neighbor1[i].add(i);
				zsplit_neighbor2[i].add(j);
			}
		}
	}	
}

int svQDOTNorm::Interpolation_neighbor(float z, int x, svVector3 p, svVector3 v)
{
	float zmin = sortpos[0][2];
	int zindex_standard = (z-zmin)/zdistance;

	int y1,y2,x1,x2,x3,x4,y3,y4;
	float tmpx,tmpy;
	int pz,px,py;
	float deltaz,deltax,deltay;
	float d;
	float ymin,xmin;
	
	int layer;
	int xyzn[8];


	if(zindex_standard>=zindex.size()||zindex_standard<0)return -1;
	else if(zindex[zindex_standard]>=sortpos.size())return -1;
	ymin = sortpos[zindex[zindex_standard]][1];
	if(zindex_standard>=ydistance.size())return -1;
	else if(zindex_standard>=yinz.size())return -1;
	int yindex1 = (int)((p[1]-ymin)/ydistance[zindex_standard])+yinz[zindex_standard];
	int yindex2 = yindex1 + 1;
	int xindex1, xindex2, xindex3, xindex4;


		//x1,x2 (y1)(z1)
		if(yindex1>=yindex.size()||yindex1<0){xindex1 = -1;xindex2 = -1;}
		else if(yindex[yindex1]>=sortpos.size()){xindex1 = -1;xindex2 = -1;}
		else{
			if(sortpos[yindex[yindex1]][2]!=sortpos[zindex[zindex_standard]][2]){xindex1 = -1;xindex2 = -1;}
			else {
				xmin = sortpos[yindex[yindex1]][0];
				if(yindex1>=xdistance.size()){xindex1 = -1;xindex2 = -1;}
				else if(yindex1>=xiny.size()){xindex1 = -1;xindex2 = -1;}
				else{
					if(p[0]<xmin) {xindex1 = -1;xindex2 = (int)((p[0]-xmin)/xdistance[yindex1])+xiny[yindex1];}else{
					xindex1 = (int)((p[0]-xmin)/xdistance[yindex1])+xiny[yindex1];
					xindex2 = xindex1 + 1;}
					if(xindex1>=xindex.size()||xindex1<0) xindex1 = -1;
					else if(sortpos[xindex[xindex1]][2]!=sortpos[zindex[zindex_standard]][2]
						||sortpos[xindex[xindex1]][1]!=sortpos[yindex[yindex1]][1])
						xindex1 = -1;
					if(xindex2>=xindex.size()||xindex2<0) xindex2 = -1;
					else if(sortpos[xindex[xindex2]][2]!=sortpos[zindex[zindex_standard]][2]
						||sortpos[xindex[xindex2]][1]!=sortpos[yindex[yindex1]][1])
						xindex2 = -1;
				}
			}
		}
		//else if(sortpos[xindex[xindex2]][2]!=sortpos[zindex[zindex_standard]][2]
		//	||sortpos[xindex[xindex2]][1]!=sortpos[yindex[yindex1]][1])
		//	return -1;

		//x3,x4 (y2)(z1)
		if(yindex2>=yindex.size()||yindex2<0){xindex3 = -1;xindex4 = -1;}
		else if(yindex[yindex2]>=sortpos.size()){xindex3 = -1;xindex4 = -1;}
		else{
			if(sortpos[yindex[yindex2]][2]!=sortpos[zindex[zindex_standard]][2]){xindex3 = -1;xindex4 = -1;}
			else {
				xmin = sortpos[yindex[yindex2]][0];
				if(yindex2>=xdistance.size()){xindex3 = -1;xindex4 = -1;}
				else if(yindex2>=xiny.size()){xindex3 = -1;xindex4 = -1;}
				else {
					if(p[0]<xmin){xindex3=-1;xindex4 = (int)((p[0]-xmin)/xdistance[yindex2])+xiny[yindex2];}else {
					xindex3 = (int)((p[0]-xmin)/xdistance[yindex2])+xiny[yindex2];
					xindex4 = xindex3 + 1;}
					if(xindex3>=xindex.size()||xindex3<0)xindex3 =  -1;
					else if(sortpos[xindex[xindex3]][2]!=sortpos[zindex[zindex_standard]][2]
						||sortpos[xindex[xindex3]][1]!=sortpos[yindex[yindex2]][1])
						xindex3 =  -1;
					if(xindex4>=xindex.size()||xindex4<0)xindex4 = -1;
					else if(sortpos[xindex[xindex4]][2]!=sortpos[zindex[zindex_standard]][2]
						||sortpos[xindex[xindex4]][1]!=sortpos[yindex[yindex2]][1])
						xindex4 = -1;
				}
			}
		}
		//else if(sortpos[xindex[xindex4]][2]!=sortpos[zindex[zindex_standard]][2]
		//	||sortpos[xindex[xindex4]][1]!=sortpos[yindex[yindex2]][1])
		//	return -1;

	if(x==5)
	{//cerr<<x<<" "<<xindex3<<endl;
		double max = 2;
		double c[4];
		for(int i=0;i<4;i++)c[i] = 2;
		if(xindex1!=-1) c[0] = v[0] * sortvel[xindex1][0] +  v[1] * sortvel[xindex1][1] +  v[2] * sortvel[xindex1][2];
		if(xindex2!=-1) c[1] = v[0] * sortvel[xindex2][0] +  v[1] * sortvel[xindex2][1] +  v[2] * sortvel[xindex2][2];
		if(xindex3!=-1) c[2] = v[0] * sortvel[xindex3][0] +  v[1] * sortvel[xindex3][1] +  v[2] * sortvel[xindex3][2];
		if(xindex4!=-1) c[3] = v[0] * sortvel[xindex4][0] +  v[1] * sortvel[xindex4][1] +  v[2] * sortvel[xindex4][2];
		int j;
		for(int i=0;i<4;i++)
		{
			if(c[i] < max)
			{max = c[i];j = i;}
		}
		if(j==0)return xindex1;
		if(j==1)return xindex2;
		if(j==2)return xindex3;
		if(j==3)return xindex4;
		else return -1;		
	}
	else if(xindex1!=-1)
	{//cerr<<xindex1<<endl;
		if(sortpos[xindex1][0] == p[0]
	&& sortpos[xindex1][1] == p[1])
		return xindex1;
	}
	else if(xindex2!=-1)
	{//cerr<<xindex1<<endl;
		if(sortpos[xindex2][0] == p[0]
	&& sortpos[xindex2][1] == p[1])
		return xindex2;
	}
	else if(xindex3!=-1)
	{//cerr<<xindex1<<endl;
		if(sortpos[xindex3][0] == p[0]
	&& sortpos[xindex3][1] == p[1])
		return xindex3;
	}
	else if(xindex4!=-1)
	{//cerr<<xindex1<<endl;
		if(sortpos[xindex4][0] == p[0]
	&& sortpos[xindex4][1] == p[1])
		return xindex4;
	}

	if(x==1)
		return xindex1;
	else if(x==2)
		return xindex2;
	else if(x==3)
	{
		return xindex3;
	}
	else if(x==4)
		return xindex4;

}

//Interpolation
//Find 8 points making up a "box" to calculate the density of a given point
svScalar svQDOTNorm::GetInterDen(svVector3 p)
{
	interpolationv[0]=0.;
	interpolationv[1]=0.;
	interpolationv[2]=0.;

	int z1,z2,y1,y2,x1,x2,x3,x4,y3,y4,y5,y6,y7,y8;
	float tmpx,tmpy;
	int pz,px,py;
	float deltaz,deltax,deltay;
	float d;
	float zmin,ymin,xmin;
	
	int layer;
	int xyzn[8];

	zmin = sortpos[0][2];
	
	int zindex1 = (int)((p[2] - zmin)/zdistance); 
	int zindex2 = zindex1 + 1;

	if(zindex1>=zindex.size()||zindex1<0)return 0;
	else if(zindex[zindex1]>=sortpos.size())return 0;
	ymin = sortpos[zindex[zindex1]][1];
	if(zindex1>=ydistance.size())return 0;
	else if(zindex1>=yinz.size())return 0;
	int yindex1 = (int)((p[1]-ymin)/ydistance[zindex1])+yinz[zindex1];
	int yindex2 = yindex1 + 1;

	//x1,x2 (y1)(z1)
	if(yindex1>=yindex.size()||yindex1<0)return 0;
	else if(yindex[yindex1]>=sortpos.size())return 0;
	else if(sortpos[yindex[yindex1]][2]!=sortpos[zindex[zindex1]][2])return 0;
	xmin = sortpos[yindex[yindex1]][0];
	if(yindex1>=xdistance.size())return 0;
	else if(yindex1>=xiny.size())return 0;
	int xindex1 = (int)((p[0]-xmin)/xdistance[yindex1])+xiny[yindex1];
	if(!(xindex1>=xindex.size()||xindex1<0))
	{
		if(fabs(sortpos[xindex1][2]-p[2])<0.00001
		&& fabs(sortpos[xindex1][1]-p[1])<0.00001
		&& fabs(sortpos[xindex1][0]-p[0])<0.00001)
		{	
			interpolationv[0] = ctrvel[sortindex[xindex1]][0];
			interpolationv[1] = ctrvel[sortindex[xindex1]][1];
			interpolationv[2] = ctrvel[sortindex[xindex1]][2];
			return ctrden[sortindex[xindex1]];
		}
	}
	int xindex2 = xindex1 + 1;
	if(xindex1>=xindex.size()||xindex1<0)return 0;
	else if(sortpos[xindex[xindex1]][2]!=sortpos[zindex[zindex1]][2]
		||sortpos[xindex[xindex1]][1]!=sortpos[yindex[yindex1]][1])
		return 0;
	if(xindex2>=xindex.size()||xindex2<0)return 0;
	else if(sortpos[xindex[xindex2]][2]!=sortpos[zindex[zindex1]][2]
		||sortpos[xindex[xindex2]][1]!=sortpos[yindex[yindex1]][1])
		return 0;	

	//x3,x4 (y2)(z1)
	if(yindex2>=yindex.size()||yindex2<0)return 0;
	else if(yindex[yindex2]>=sortpos.size())return 0;
	else if(sortpos[yindex[yindex2]][2]!=sortpos[zindex[zindex1]][2])return 0;
	xmin = sortpos[yindex[yindex2]][0];
	if(yindex2>=xdistance.size())return 0;
	else if(yindex2>=xiny.size())return 0;
	int xindex3 = (int)((p[0]-xmin)/xdistance[yindex2])+xiny[yindex2];
	int xindex4 = xindex3 + 1;
	if(xindex3>=xindex.size()||xindex3<0)return 0;
	else if(sortpos[xindex[xindex3]][2]!=sortpos[zindex[zindex1]][2]
		||sortpos[xindex[xindex3]][1]!=sortpos[yindex[yindex2]][1])
		return 0;
	if(xindex4>=xindex.size()||xindex4<0)return 0;
	else if(sortpos[xindex[xindex4]][2]!=sortpos[zindex[zindex1]][2]
		||sortpos[xindex[xindex4]][1]!=sortpos[yindex[yindex2]][1])
		return 0;	

	if(zindex2>=zindex.size()||zindex2<0)return 0;
	else if(zindex[zindex2]>=sortpos.size())return 0;
	ymin = sortpos[zindex[zindex2]][1];
	if(zindex2>=ydistance.size())return 0;
	else if(zindex2>=yinz.size())return 0;
	int yindex3 = (int)((p[1]-ymin)/ydistance[zindex2])+yinz[zindex2];
	int yindex4 = yindex3 + 1;

	//x5,x6 (y3,z2)
	if(yindex3>=yindex.size()||yindex3<0)return 0;
	else if(yindex[yindex3]>=sortpos.size())return 0;
	else if(sortpos[yindex[yindex3]][2]!=sortpos[zindex[zindex2]][2])return 0;
	xmin = sortpos[yindex[yindex3]][0];
	if(yindex3>=xdistance.size())return 0;
	else if(yindex3>=xiny.size())return 0;
	int xindex5 = (int)((p[0]-xmin)/xdistance[yindex3])+xiny[yindex3];
	int xindex6 = xindex5 + 1;
	if(xindex5>=xindex.size()||xindex5<0)return 0;
	else if(sortpos[xindex[xindex5]][2]!=sortpos[zindex[zindex2]][2]
		||sortpos[xindex[xindex5]][1]!=sortpos[yindex[yindex3]][1])
		return 0;
	if(xindex6>=xindex.size()||xindex6<0)return 0;
	else if(sortpos[xindex[xindex6]][2]!=sortpos[zindex[zindex2]][2]
		||sortpos[xindex[xindex6]][1]!=sortpos[yindex[yindex3]][1])
		return 0;

	//x7,x8 (y4,z2)
	if(yindex4>=yindex.size()||yindex4<0)return 0;
	else if(yindex[yindex4]>=sortpos.size())return 0;
	else if(sortpos[yindex[yindex4]][2]!=sortpos[zindex[zindex2]][2])return 0;
	xmin = sortpos[yindex[yindex4]][0];
	if(yindex4>=xdistance.size())return 0;
	else if(yindex4>=xiny.size())return 0;
	int xindex7 = (int)((p[0]-xmin)/xdistance[yindex4])+xiny[yindex4];
	int xindex8 = xindex7 + 1;
	if(xindex7>=xindex.size()||xindex7<0)return 0;
	else if(sortpos[xindex[xindex7]][2]!=sortpos[zindex[zindex2]][2]
		||sortpos[xindex[xindex7]][1]!=sortpos[yindex[yindex4]][1])
		return 0;
	if(xindex8>=xindex.size()||xindex8<0)return 0;
	else if(sortpos[xindex[xindex8]][2]!=sortpos[zindex[zindex2]][2]
		||sortpos[xindex[xindex8]][1]!=sortpos[yindex[yindex4]][1])
		return 0;

	int xn;
	long double distancesum=0.;

	long double singledistance[8];

	interpolationv[0]=0.;
	interpolationv[1]=0.;
	interpolationv[2]=0.;

	int tmpindex[8];
	tmpindex[0]=xindex1;
	tmpindex[1]=xindex2;
	tmpindex[2]=xindex3;
	tmpindex[3]=xindex4;
	tmpindex[4]=xindex5;
	tmpindex[5]=xindex6;
	tmpindex[6]=xindex7;
	tmpindex[7]=xindex8;

	for(int i=0;i<8;i++)
	{
		xn = tmpindex[i];
		//cerr<<sortpos[xn][0]<<" "<<sortpos[xn][1]<<" "<<sortpos[xn][2]<<" "<<endl;
		singledistance[i] = sqrt((sortpos[xn][0]-p[0])*(sortpos[xn][0]-p[0])
				+(sortpos[xn][1]-p[1])*(sortpos[xn][1]-p[1])
				+(sortpos[xn][2]-p[2])*(sortpos[xn][2]-p[2]));
		if(singledistance[i]<=1e-79||singledistance[i]==0.)

		{
			interpolationv[0] = ctrvel[sortindex[xn]][0];
			interpolationv[1] = ctrvel[sortindex[xn]][1];
			interpolationv[2] = ctrvel[sortindex[xn]][2];

			return ctrden[sortindex[xn]];
		}
		//cerr<<singledistance[i]<<endl;
		distancesum = distancesum +1./singledistance[i];
	}
	//cerr<<distancesum<<endl;
	
	float r[8];
	svScalar den=0;

	for(int i=0;i<8;i++)
	{
		r[i] = (1/singledistance[i])/distancesum;
		den = den + r[i] * ctrden[sortindex[tmpindex[i]]];
		interpolationv[0] = interpolationv[0] + r[i] * ctrvel[sortindex[tmpindex[i]]][0];
		interpolationv[1] = interpolationv[1] + r[i] * ctrvel[sortindex[tmpindex[i]]][1];
		interpolationv[2] = interpolationv[2] + r[i] * ctrvel[sortindex[tmpindex[i]]][2];
	}

	interpolationv = normalize(interpolationv);

	return den;
}

void svQDOTNorm::setProfileFromData(char *indir, char *infname, int seedNum)
{
  cleanUp();

  QDOTdataNorm * myData = new QDOTdataNorm(indir, infname);
  myData->SetSeedNumber(seedNum);
  //setBBox(lbbox, rbbox);
  myData->GetBoundingBox(&lbbox, &rbbox);

  dataSize = myData->GetTotalROIPoints();
  cerr << "... total ROI Points: " << dataSize << endl;

  ctrpos = new svVector3[dataSize];
  ctrvel = new svVector3[dataSize];
  ctrden = new svScalar[dataSize];

  svIntArray roiPoints = myData->GetROINumOfPoints();
  int sss = 0;
  for(int zi=0; zi<roiPoints.size(); zi++)
  {
	   for(int roii=0; roii<roiPoints[zi]; roii++)
	   {
	     ctrpos[sss] = myData->GetROIPosition(zi, roii);
		 ctrvel[sss] = myData->GetROIVelocity(zi, roii);
         ctrden[sss] = myData->GetROIDensity(zi, roii);
		 /*
		 cerr << "assigned .. " << sss << ": (" 
		      << ctrpos[sss][0] << ", "
		      << ctrpos[sss][1] << ", "
		      << ctrpos[sss][2] << ", "
			  << "), velocity: ("
		      << ctrvel[sss][0] << ", "
		      << ctrvel[sss][1] << ", "
		      << ctrvel[sss][2] << ", "
			  << "), density: " 
			  << ctrden[sss]
			  << endl;
			  */
		 sss++;
	   }; // end for(roii)
  }; // end for(zi)

  cerr << "... Done svQDOTNorm: setting sampling profile ... " << endl;

  svInt alldatasize;
  alldatasize = myData->GetDatasize();

  svScalarArray *Den = new svScalarArray[1];
  Den = myData->GetDensity();

  histoden = new svFloat[alldatasize];
  
  for(int zi = 0; zi < alldatasize; zi ++)
  {
	if(Den[0][zi]>=0&&Den[0][zi]<1e+0) histoden[zi] = -1;
	if(Den[0][zi]>=1e+0&&Den[0][zi]<1e+1) histoden[zi] = 0;
	if(Den[0][zi]>=1e+1&&Den[0][zi]<1e+2) histoden[zi] = 1;
	if(Den[0][zi]>=1e+2&&Den[0][zi]<1e+3) histoden[zi] = 2;
	if(Den[0][zi]>=1e+3&&Den[0][zi]<1e+4) histoden[zi] = 3;	
	if(Den[0][zi]>=1e+4&&Den[0][zi]<1e+5) histoden[zi] = 4;
	if(Den[0][zi]>=1e+5&&Den[0][zi]<1e+6) histoden[zi] = 5;
	if(Den[0][zi]>=1e+6&&Den[0][zi]<1e+7) histoden[zi] = 6;
	if(Den[0][zi]>=1e+7&&Den[0][zi]<1e+8) histoden[zi] = 7;
	if(Den[0][zi]>=1e+8&&Den[0][zi]<1e+9) histoden[zi] = 8;	
  }

  //delete [] Den;
  histodatasize = myData->GetDatasize();
  delete myData;
}
int svQDOTNorm::getNumOfIntegerDigits(svScalar num)
{
	  int multiplier = 0;

	  svScalar numCopy = num;

	  if(numCopy<1&&numCopy!=0) 
	  {
	    while ((fabs(numCopy-1.)>0.000001)&&(numCopy-1.)<0.000001)
		{

		  multiplier--;//cerr<<num<<" "<<multiplier<<endl;
		  numCopy *= 10;
		}; // end while(1)
	  }
	  else if(numCopy==0)
		multiplier=0;
	  else if(numCopy>=1)
	  {
	    while(1)
		{
		  if(numCopy>=0 && numCopy <10)
		  {
		    break;
		  };
		  multiplier++;
		  numCopy /= 10.;
		}; // end while (1)
	  }; // end if(numCopy)

	  return multiplier;	
}
void svQDOTNorm::setProfileFromData(char *indir, char *infname, float contourvalue)
{
// cleanUp();
	  
  if(ctrpos != NULL)
    delete [] ctrpos;
  if(ctrvel != NULL)
    delete [] ctrvel;
  if(ctrden != NULL)
    delete [] ctrden;
  if(ctrexp !=NULL)
	delete [] ctrexp;
  if(ctrcoe !=NULL)
	delete [] ctrcoe;

  if(zsplitpos!=NULL)
  {
	  for(int i=0;i<zValue;i++)	  
	  {
		  zsplitpos[i].free();
		  zsplitvel[i].free();
		  zsplitden[i].free();
		  zsplitexp[i].free();
		  zsplitcoe[i].free();	
	  }
	  
	  delete [] zsplitpos;
	  delete [] zsplitvel;
	  delete [] zsplitden;
	  delete [] zsplitexp;
	  delete [] zsplitcoe;
	
	  zsplitpos=NULL;
	  zsplitvel=NULL;
	  zsplitden=NULL;
	  zsplitexp=NULL;
	  zsplitcoe=NULL;

	delete [] sortPos;
	delete [] sortDen;
	delete [] sortVel;
	delete [] sortVel2;
	delete [] denMin;
	delete [] denMax;
  }

  QDOTdataNorm * myData = new QDOTdataNorm(indir, infname);
  //myData->SetIsoRegion(contourvalue); 
  //setBBox(lbbox, rbbox);
  myData->GetBoundingBox(&lbbox, &rbbox); //cerr<<"2"<<endl;

  dataSize = myData->GetDatasize();
  cerr << "... total ROI Points: " << dataSize << endl;

  ctrpos = new svVector3[dataSize];
  ctrvel = new svVector3[dataSize];
  ctrden = new svScalar[dataSize];
  ctrexp = new svScalar[dataSize];
  ctrcoe = new svScalar[dataSize];
  
  zValue = myData->GetZvalue();
  zsplitpos = new svVector3Array[zValue];
  zsplitvel = new svVector3Array[zValue];
  zsplitden = new svScalarArray[zValue];
  zsplitexp = new svScalarArray[zValue];
  zsplitcoe = new svScalarArray[zValue];

  sortPos = new svVector3[dataSize];
  sortDen = new svScalar[dataSize];
  sortVel = new svVector3[dataSize];
   sortVel2 = new svVector3[dataSize]; 

  svIntArray *roiPoints = myData-> GetPointsNum() ;
  int sss = 0;
  svScalar minexp = 9e+9;
  svScalar maxexp = -9e+9;
  for(int zi=0; zi< myData->GetZvalue(); zi++)
  {
	   for(int roii=0; roii<roiPoints[zi].size(); roii++)
	   {
	     ctrpos[sss] = myData->GetPosition(zi, roii);
		 ctrvel[sss] = myData->GetVelocity(zi, roii);
         ctrden[sss] = myData->GetDensity(zi, roii);
         
         ctrexp[sss] = (svScalar)getNumOfIntegerDigits(ctrden[sss]);

	float tmppow = 1;
	for(int iii=0;iii<ctrexp[sss];iii++)
		tmppow = tmppow * 10.;
         ctrcoe[sss] = ctrden[sss]/tmppow;///pow(10.,ctrexp[sss]);
         
         if(ctrexp[sss]>maxexp&&ctrcoe[sss]!=0)
        	 maxexp = ctrexp[sss];
         else if(ctrexp[sss]<minexp&&ctrcoe[sss]!=0)
         {
        	 minexp = ctrexp[sss];
		//if(minexp==-16)
		//	cerr<<ctrden[sss]<<endl;
         }
		 sss++;
	   }; // end for(roii)
  }; // end for(zi)  


  if(minexp<0)
 	DEN_SCALAR = (int)fabs(minexp);
  else
	DEN_SCALAR = 0;

  expMax = DEN_SCALAR;

	//cerr<<getNumOfIntegerDigits(1.6e-14)<<" "<<getNumOfIntegerDigits(0.00000000000001)<<endl;
	//cerr<<minexp<<" "<<maxexp<<" "<<DEN_SCALAR<<endl;

	float tmppow_scalar = 1;
        for(int iii=0;iii<DEN_SCALAR;iii++)
                tmppow_scalar = tmppow_scalar *10.;


  for(int i=0;i<dataSize;i++)
  {
	  ctrexp[i] = ctrexp[i] + DEN_SCALAR;
	  ctrden[i] = ctrden[i] * tmppow_scalar;//pow(10.,(float)DEN_SCALAR);
  }
  
  cerr << "... Done svQDOTNorm: setting sampling profile ... " << endl;

  svScalarArray den1, den2;
  den1 = myData->GetDenMin();
  den2 = myData->GetDenMax();
  
  denMin = new svScalar[den1.size()];
  denMax = new svScalar[den2.size()];

	min_density = 9e+9;
	min_density_nonzero = 9e+9;
	max_density = -9e+9;
  for(int i=0;i<den1.size();i++)
  {

	denMin[i] = den1[i] * tmppow_scalar;//pow(10.,(float)DEN_SCALAR);
  	if(min_density>denMin[i])min_density=denMin[i];
  	if(min_density_nonzero>denMin[i]&&denMin[i]!=0)min_density_nonzero=denMin[i];
}
	
  for(int i=0;i<den2.size();i++)
  {
	denMax[i] = den2[i] * tmppow_scalar;//pow(10.,(float)DEN_SCALAR);
	if(max_density<denMax[i])max_density=denMax[i];
  }

  //delete [] Den;
  delete myData;
}
void svQDOTNorm::setProfileFromData(char *indir, char *infname)
{
  cleanUp();

  readdata * myData = new readdata(indir, infname);
  //myData->SetIsoRegion(contourvalue); 
  //setBBox(lbbox, rbbox);
  myData->GetBoundingBox(&lbbox, &rbbox); //cerr<<"2"<<endl;

  dataSize = myData->GetDatasize();
  cerr << "... total ROI Points: " << dataSize << endl;

  ctrpos = new svVector3[dataSize];
  ctrvel = new svVector3[dataSize];
  ctrden = new svScalar[dataSize];
  ctrexp = new svScalar[dataSize];
  ctrcoe = new svScalar[dataSize];
  
  zValue = myData->GetZvalue();
  zsplitpos = new svVector3Array[zValue];
  zsplitvel = new svVector3Array[zValue];
  zsplitden = new svScalarArray[zValue];
  zsplitexp = new svScalarArray[zValue];
  zsplitcoe = new svScalarArray[zValue];

  sortPos = new svVector3[dataSize];
  sortDen = new svScalar[dataSize];
  sortVel = new svVector3[dataSize];
    sortVel2 = new svVector3[dataSize];
  //svIntArray *roiPoints = myData-> GetPointsNum() ;
  int sss = 0;
  svScalar minexp = 9e+9;
  svScalar maxexp = -9e+9;
  for(int i=0;i<dataSize;i++)
  {
	     ctrpos[sss] = myData->GetPosition(i);
		 ctrvel[sss] = myData->GetVelocity(i);
         ctrden[sss] = myData->GetDensity(i);
                  ctrexp[sss] = (svScalar)getNumOfIntegerDigits(ctrden[sss]);
        float tmppow=1.;
        for(int iii=0;iii<ctrexp[sss];iii++)
                tmppow = tmppow *10.;

         ctrexp[sss] = (svScalar)getNumOfIntegerDigits(ctrden[sss]);
         ctrcoe[sss] = ctrden[sss]/tmppow;//pow(10.,ctrexp[sss]);
         
         if(ctrexp[sss]>maxexp&&ctrcoe[sss]!=0)
        	 maxexp = ctrexp[sss];
         else if(ctrexp[sss]<minexp&&ctrcoe[sss]!=0)
         {
        	 minexp = ctrexp[sss];
		//if(minexp==-16)
		//	cerr<<ctrden[sss]<<endl;
         }
		 sss++;
  }; // end for(zi)  

  if(minexp<0)
 	DEN_SCALAR = (int)fabs(minexp);
  else
	DEN_SCALAR = 0;
	//cerr<<getNumOfIntegerDigits(1.6e-14)<<" "<<getNumOfIntegerDigits(0.00000000000001)<<endl;
	//cerr<<minexp<<" "<<maxexp<<" "<<DEN_SCALAR<<endl;

	float tmppow_scalar = 1;

        for(int iii=0;iii<DEN_SCALAR;iii++)
                tmppow_scalar = tmppow_scalar *10.;

  for(int i=0;i<dataSize;i++)
  {
	  ctrexp[i] = ctrexp[i] + DEN_SCALAR;
	  ctrden[i] = ctrden[i] * tmppow_scalar;//pow(10.,(float)DEN_SCALAR);
  }
  
  cerr << "... Done svQDOTNorm: setting sampling profile ... " << endl;

  svScalarArray den1, den2;
  den1 = myData->GetDenMin();
  den2 = myData->GetDenMax();
  
  denMin = new svScalar[den1.size()];
  denMax = new svScalar[den2.size()];

  for(int i=0;i<den1.size();i++)
  {
	denMin[i] = den1[i] *tmppow_scalar;// pow(10.,(float)DEN_SCALAR);
  }
  for(int i=0;i<den2.size();i++)
  {
	denMax[i] = den2[i] *tmppow_scalar;// pow(10.,(float)DEN_SCALAR);
  }

  //delete [] Den;
  delete myData;
}
void svQDOTNorm::cleanUp()
{
 
	  
  if(ctrpos != NULL)
    delete [] ctrpos;
  if(ctrvel != NULL)
    delete [] ctrvel;
  if(ctrden != NULL)
    delete [] ctrden;
  if(ctrexp !=NULL)
	delete [] ctrexp;
  if(ctrcoe !=NULL)
	delete [] ctrcoe;

  if(zsplitpos!=NULL)
  {
	  for(int i=0;i<zValue;i++)	  
	  {
		  zsplitpos[i].free();
		  zsplitvel[i].free();
		  zsplitden[i].free();
		  zsplitexp[i].free();
		  zsplitcoe[i].free();
			zsplit_neighbor1[i].free();
			zsplit_neighbor2[i].free();
		  
		  sortP[0][i].clear();
		  sortP[1][i].clear();
		  sortP[2][i].clear();
		  sortV[0][i].clear();
		  sortV[1][i].clear();
		  sortV[2][i].clear();
		  sortD[i].clear();		
	  }
	  
	  delete [] zsplitpos;
	  delete [] zsplitvel;
	  delete [] zsplitden;
	  delete [] zsplitexp;
	  delete [] zsplitcoe;
	delete [] zsplit_neighbor1;
	delete [] zsplit_neighbor2;	
	
	  zsplitpos=NULL;
	  zsplitvel=NULL;
	  zsplitden=NULL;
	  zsplitexp=NULL;
	  zsplitcoe=NULL;
	zsplit_neighbor1=NULL;
	zsplit_neighbor2=NULL;
  }

if(xyplane!=NULL)
delete [] xyplane;
if(xzplane!=NULL)
delete [] xzplane;
if(yzplane!=NULL)
delete [] yzplane;
if(index.size()!=0)
index.clear();
if(denMin!=NULL)
delete [] denMin;
if(denMax!=NULL)
delete [] denMax;

if(sortindex.size()!=0){
sortindex.clear();
zindex.clear();
zformat.clear();
   yindex.clear();
 xindex.clear();
num_z[0].clear();
num_z[1].clear();

ydistance.clear();
  xdistance.clear();

yinz.clear();
xinz.clear();
xiny.clear();}
  


	sortP[0].clear();sortP[1].clear();sortP[2].clear();
	sortV[0].clear();sortV[1].clear();sortV[2].clear();

  delete [] sortPos;
  delete [] sortDen;
  delete [] sortVel;
  delete [] sortVel2;
}

svVector3 svQDOTNorm::GetVector(const svParticle & p)
{
  return GetVector(p.position, p.time);
}

svScalar svQDOTNorm::GetBoxDensity(svVector3 l,svVector3 r)
{
	svScalar den=0.;
	for(int i=0;i<dataSize;i++)
	{
		if(ctrpos[i][0]>=l[0]&&ctrpos[i][0]<=r[0]
			&& ctrpos[i][1]>=l[1]&&ctrpos[i][1]<=r[1]
			&& ctrpos[i][2]>=l[2]&&ctrpos[i][2]<=r[2])
		{
			den = den + ctrden[i];
		}
	}
	return den;
}

// TODO: not done yet- haven't calcuated velocity; has to do a search
//       in a large array; need an efficient method
svVector3 svQDOTNorm::GetVector(const svVector3& pos, svScalar t)
{
   double vx=0.,vy=0,vz=0;
   float nnx[2], nny[2], nnz[2];
   float wx, wy, wz; // weight to the left most point - the other one should be 1-w;

   //do the interpolation around the nearest neighbors to get the velocity
   // get the nearest neighbor
//   findNN (pos[0], nnx, &wx);
//   findNN (pos[1], nny, &wy);
//   findNN (pos[2], nnz, &wz);

   //gen_rbf(pos[0],pos[1],pos[2],&vx,&vy,&vz);
   //cerr << "----> v: (" << vx <<", " << vy<<", " << vz<<endl;
   //double s = sqrt(vx*vx+vy*vy+vz*vz);
   //cerr << "----> s: " << s <<endl;

   svVector3 rv(vx,vy,vz);
   return rv;
}
/*
void svQDOTNorm::findNN(float pos, float nn[2], float *weightToTheFirst)
{
   int posInt = (int)(pos-0.5); // get the integer bit

   if(pos-posInt  >=0 && pos-posInt <=0.25)
   {
     nn[0]=posInt;
     nn[1]=posInt+0.25;
   }
   else if(pos-posInt  >0.25 && pos-posInt <=0.5)
   {
     nn[0]=posInt+0.25;
     nn[1]=posInt+0.5;
   }
   else if(pos-posInt  >0.5 && pos-posInt <=0.75)
   {
     nn[0]=posInt+0.5;
     nn[1]=posInt+0.75;
   }
   else
   {
     nn[0]=posInt+0.55;
     nn[1]=posInt+1;
   }; // end if (pos-posInt)

   *weightToTheFirst = (nn[1]-pos) / 0.25;
}*/

svFloat svQDOTNorm::GetDenRatio(svScalar den, svFloat z)
{
	int count = 0;
	int flag = 0;
	svFloat *zz = new svFloat[dataSize];
	int index;
	float dd1,dd2;
	float r;

	for(int i=0;i<dataSize;i++)
	{
		if(i==0)
		{
			zz[count] = ctrpos[i][2];count++;
		}
		else
		{
			for(int j=0;j<count;j++)
			{
				if(zz[j]==ctrpos[i][2])flag = 1;
			}
			if(flag == 0)
			{	
				zz[count] = ctrpos[i][2];
				count++;
			}
		}
		flag = 0; 
	}
	sort(zz,zz+count);
	for(int i=0;i<count;i++)
	{
		if(z<=zz[i])
		{
			index = i;break;
		}
	}
	
	if(index == count-1)
	{
		index = count - 1;
	}
	else
	{
		dd1 = z - zz[index];
		dd2 = zz[index+1] - z;
		if(dd1>dd2) index = index + 1;
	}

	r = den/GetDenMax(index);

	return r;
}
svVector3 svQDOTNorm::Interpolation(svScalar &den, svScalar &exp, svScalar &coe, svVector3 xx)
{
	den = GetInterDen(xx);
	exp = getNumOfIntegerDigits(den);
	float tmppow = 1;
        for(int iii=0;iii<exp;iii++)
                tmppow = tmppow *10.;	
	coe = den/tmppow;//pow(10.,exp);
	return interpolationv;
}

void svQDOTNorm::Savetofile(char *file, svVector3 *p, int num)
{
	svVector3 *v = new svVector3[num];
	svScalar *d = new svScalar[num];
	svIntArray *in_cutting = new svIntArray[zlayernum];
	double degree = (rbbox[2] - lbbox[2])/((double)zlayernum-1.);

	for(int i=0;i<num;i++)
	{
		svVector3 pos;
		pos = p[i];
		d[i] = GetInterDen(pos);
		v[i] = interpolationv;
		int j = (p[i][2] - lbbox[2])/degree;
		in_cutting[j].add(i);
		
	}
/*	for(int i=0;i<zlayernum;i++)
	{
		cerr<<i<<" "<<in_cutting[i].size()<<endl;
	}cerr<<endl;*/
	ofstream outfile;

	for(int i=0;i<zlayernum;i++)
	{
		char str[20];
		sprintf(str, "%sprojection%d", file, i);
		outfile.open(str);	
		//cerr<<str<<endl;	
		outfile<<in_cutting[i].size()<<endl;
		for(int j=0;j<in_cutting[i].size();j++)
		{
			outfile<<p[in_cutting[i][j]][0]<<" "<<p[in_cutting[i][j]][1]<<" "<<p[in_cutting[i][j]][2]<<" "
				<<v[in_cutting[i][j]][0]<<" "<<v[in_cutting[i][j]][1]<<" "<<v[in_cutting[i][j]][2]<<" "
				<<d[in_cutting[i][j]]<<endl;
		}
		outfile<<endl;

		outfile.close();
	}

	delete [] v;
	delete [] d;
	for(int i=0;i<zlayernum;i++)
		in_cutting[i].free();
	delete [] in_cutting;
	in_cutting = NULL;

}

void svQDOTNorm::Savetovtk(char *file, svVector3 *p, int num)
{
	svVector3 *v = new svVector3[num];
	svScalar *d = new svScalar[num];
	svIntArray *in_cutting = new svIntArray[zlayernum];
	double degree = (rbbox[2] - lbbox[2])/((double)zlayernum-1.);

	for(int i=0;i<num;i++)
	{
		svVector3 pos;
		pos = p[i];
		d[i] = GetInterDen(pos);
		v[i] = interpolationv;
		int j = (p[i][2] - lbbox[2])/degree;
		in_cutting[j].add(i);
		
	}
	for(int i=0;i<zlayernum;i++)
	{
		cerr<<i<<" "<<in_cutting[i].size()<<endl;
	}cerr<<endl;
	ofstream outfile;

	for(int i=0;i<zlayernum;i++)
	{
		char str[20];
		sprintf(str, "%sprojection%d.vtk", file, i);
		outfile.open(str);	
		cerr<<str<<endl;	

		outfile<<"# vtk DataFile Version 2.0"<<endl;
		outfile<<str<<endl;
		outfile<<"ASCII"<<endl;
		outfile<<"DATASET UNSTRUCTURED_GRID"<<endl;
		//cerr<<in_cutting[i].size()<<endl;

		outfile<<"POINTS "<<in_cutting[i].size()<<" float"<<endl;

		for(int j=0;j<in_cutting[i].size();j++)
		{
			outfile<<p[in_cutting[i][j]][0]<<" "<<p[in_cutting[i][j]][1]<<" "<<p[in_cutting[i][j]][2]<<endl;
		}
		outfile<<endl;

		outfile<<"POINT_DATA "<<in_cutting[i].size()<<endl;
		outfile<<"VECTORS velocity float"<<endl;
		for(int j=0;j<in_cutting[i].size();j++)
		{
			outfile<<v[in_cutting[i][j]][0]<<" "<<v[in_cutting[i][j]][1]<<" "<<v[in_cutting[i][j]][2]<<endl;
		}
		outfile<<endl;

		outfile<<"SCALARS density float 1"<<endl;
		outfile<<"LOOKUP_TABLE default"<<endl;
		for(int j=0;j<in_cutting[i].size();j++)
		{
			outfile<<d[in_cutting[i][j]]<<endl;
		}
		outfile<<endl;

		outfile.close();
	}

	delete [] v;
	delete [] d;
	for(int i=0;i<zlayernum;i++)
		in_cutting[i].free();
	delete [] in_cutting;
	in_cutting = NULL;
}	

bool *svQDOTNorm::GetInsidePos_Mag(svScalar minmag, svScalar maxmag)
{
	bool *in = new bool[dataSize];
	int t=0;
	for(int i=0;i<zValue;i++)
	{
		for(int j=0;j<zsplitpos[i].size();j++)
		{
			if(zsplitden[i][j]>=minmag*pow(10.,(double)DEN_SCALAR)&&zsplitden[i][j]<=maxmag*pow(10.,(double)DEN_SCALAR))
			{
				in[t]=true;
			}
			else
			{
				in[t] = false;
			}
			t++;
		}
	}
	return in;
}

svScalar svQDOTNorm::GetAngle(svVector3 p)
{
	float sum = p[0] * p[0] + p[1] * p[1];
	sum = sqrt(sum);
	svVector3 p2;
	svScalar a;
	p2[0] = p[0]/sum;
	p2[1] = p[1]/sum;
	if(p2[1]>=0)
	{
		a = acos(p2[0]);
	}
	else
	{
		a = M_PI * 2. - acos(p2[0]);
	}
	return a;
}

bool *svQDOTNorm::GetInsidePos_Vel(vector<double> minangle, vector<double> maxangle, svScalar zangle1, svScalar zangle2)//zangle1 and zangle2 : [0, 180]
{
	bool *in = new bool[dataSize];
	for(int i=0;i<dataSize;i++)
		in[i] = false;

	zangle1 = zangle1/180. * M_PI;
	zangle2 = zangle2/180. * M_PI;

	for(int k=0;k<minangle.size();k++)
	{
		if(minangle[k]>maxangle[k])
		{
			svScalar z;
			int t=0;
			for(int i=0;i<zValue;i++)
			{
				for(int j=0;j<zsplitvel[i].size();j++)
				{
					svScalar angle = GetAngle(zsplitvel[i][j]);
					z = acos(zsplitvel[i][j][2]);
					if(((angle>=minangle[k]&&angle<=M_PI*2.)||(angle>=0.&&angle<=maxangle[k]))
						&&z>=zangle1&&z<=zangle2)
					{
						in[t]=true;
					}
					t++;
				}
			}	
		}
		else
		{
			int t=0;
			for(int i=0;i<zValue;i++)
			{
				for(int j=0;j<zsplitvel[i].size();j++)
				{
					svScalar angle = GetAngle(zsplitvel[i][j]);
					svScalar z = acos(zsplitvel[i][j][2]);
					if(angle>=minangle[k]&&angle<=maxangle[k]&&z>=zangle1&&z<=zangle2)
					{
						in[t]=true;
					}
					t++;
				}
			}	
		}
	}
	return in;
}
}
