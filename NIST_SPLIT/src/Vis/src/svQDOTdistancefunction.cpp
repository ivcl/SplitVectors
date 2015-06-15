/**------------------------------------------------------------------
QDOTdistancefunction
09022014
Distance Function
---------------------------------------------------------------------
*/

#include "svQDOTdistancefunction.h"
#include <vector>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>

using namespace std;

namespace __svl_lib {
void QDOTdistancefunction::SetData(svVector3Array *pos,svVector3Array *vel,svInt num,int znum,
	svVector3 l, svVector3 r)
{	
	dataPos = new svVector3Array[znum];
	dataVel = new svVector3Array[znum];
	datalayerpos = new svVector3Array[znum];
	dataNei_planecomparsion1 = new int[num];
	dataNei_planecomparsion2 = new int[num];
	//datalayerdis = new svScalarArray[znum];

	//datacorner = new svIntArray[znum];

	dataSize = num;
	zlayer = znum;
	dataNei_multiplelayer_1 = new svIntArray[dataSize];
	dataNei_multiplelayer_2 = new svIntArray[dataSize];

	xd = pos[0][1][0] - pos[0][0][0];
	int count;
	for(int i=0;i<znum;i++)
	{
		for(int j=0;j<pos[i].size();j++)
		{
			vel[i][j].normalize();
			dataPos[i].add(pos[i][j]);
			dataVel[i].add(vel[i][j]);
			if(i==0&&j>0)
				if(dataPos[i][j][1]!=dataPos[i][j-1][1])
					yd = dataPos[i][j][1]-dataPos[i][j-1][1];
			//datacorner[i].add(0);
		}
	}

	float tmpn = sqrt(znum);
	y_num = (int)tmpn;
	if(fabs(tmpn - (int)tmpn) > 0)
		x_num = tmpn+1;
	else
		x_num = tmpn;

	lbbox = l;
	rbbox = r;

	xl = rbbox[0] - lbbox[0];
	yl = rbbox[1] - lbbox[1];

	svVector3 p;
	svVector3 tmpr;
	svVector3 tmpl;
	tmpr[0]=-9e+9;tmpr[1]=-9e+9;tmpr[2]=-9e+9;
	tmpl[0]=9e+9;tmpl[1]=9e+9;tmpl[2]=9e+9;
	for(int i=0;i<znum;i++)
	{
		
		for(int j=0;j<pos[i].size();j++)
		{
			p[0] = dataPos[i][j][0] + (xl + xd*2) * (float)(i%x_num);
			p[1] = dataPos[i][j][1] - (yl + yd*2) * (float)(i/x_num);
			p[2] = dataPos[i][j][2];
			datalayerpos[i].add(p);
			if(p[0]>tmpr[0])tmpr[0]=p[0];
			if(p[1]>tmpr[1])tmpr[1]=p[1];
			if(p[2]>tmpr[2])tmpr[2]=p[2];
			if(p[0]<tmpl[0])tmpl[0]=p[0];
			if(p[1]<tmpl[1])tmpl[1]=p[1];
			if(p[2]<tmpl[2])tmpl[2]=p[2];
		}
		//cerr<<datalayerpos[i][0][0]<<" "<<datalayerpos[i][0][1]<<endl;
	}
	
	/*svVector3 tmpr = datalayerpos[0][0];
	svVector3 tmpl;
	tmpl[0] = datalayerpos[0][0][0] + x_num * (xl + xd*2) ;
	tmpl[1] = datalayerpos[0][0][1] - y_num * (yl + yd*2) ;
	tmpl[2] = 0;*/
	
	center = (tmpl + tmpr)/2;
	for(int i=0;i<znum;i++)//move center to 0 point
	{
		for(int j=0;j<pos[i].size();j++)
		{
			float tmpz = datalayerpos[i][j][2];
			datalayerpos[i][j] = datalayerpos[i][j] - center;
			datalayerpos[i][j][2] = tmpz;
		}
	}
}

void QDOTdistancefunction::ReadNei(char *file)
{
	/*dataNei_inspace_1 = new svIntArray[dataSize];
	dataNei_inspace_2 = new svIntArray[dataSize];
	dataNei_onelayer_1 = new svIntArray[dataSize];
	dataNei_onelayer_2 = new svIntArray[dataSize];*/

	dataNei_multiplelayer_1 = new svIntArray[dataSize];
	dataNei_multiplelayer_2 = new svIntArray[dataSize];

	/*char file1[100];
	sprintf(file1, "%s_inspace",file);
	char file2[100];
	sprintf(file2, "%s_onelayer", file);*/
	char file3[100];
	sprintf(file3, "%s_multiplelayer", file);

	ifstream infile;
	int p;
/*	infile.open(file1);cerr<<file1<<endl;

	int p;

	for(int i=0;i<dataSize;i++)
	{

			for(int j=0;j<50;j++)
			{

				infile>>p;
				dataNei_inspace_1[i].add(p);
				infile>>p;
				dataNei_inspace_2[i].add(p);
			}
		
	}
	infile.close();

	infile.open(file2);cerr<<file2<<endl;
	for(int i=0;i<dataSize;i++)
	{		

			for(int j=0;j<50;j++)
			{
				infile>>p;
				dataNei_onelayer_1[i].add(p);
				infile>>p;
				dataNei_onelayer_2[i].add(p);
			}
		
	}
	infile.close();*/

	infile.open(file3);cerr<<file3<<endl;
	for(int j=0;j<zlayer;j++) //hard coding!!!!!!!!!!!!!!!!!!!!!!!!!
	{
		for(int i=0;i<dataSize;i++)
		{		
			for(int k=0;k<4;k++)
			{
				infile>>p;
				dataNei_multiplelayer_1[i].add(p);
				infile>>p;
				dataNei_multiplelayer_2[i].add(p);
			}
		}
	}
	infile.close();

	cerr<<file3<<endl;
}
void QDOTdistancefunction::SetNeighbours_Multiplelayer()
{
/*	if(datalayerdis!=NULL)
	{
		for(int i=0;i<zlayer;i++)
			datalayerdis[i].free();
		delete [] datalayerdis;
		datalayerdis = NULL;
	}

	datalayerdis = new svScalarArray[zlayer];

	int index;
	int count = 0;
	int index_z = 0;
	double maxcos;
	for(int i=0;i<zlayer;i++)
	{	
		for(int j=0;j<dataPos[i].size();j++)
		{//cerr<<i<<" "<<j<<endl;
			maxcos = 2;
			double near = 9e+9;

			for(int k=4*z;k<4*(z+1);k++)//get rid of the points itself
			{//cerr<<dataNei_multiplelayer_1[j+index_z][k]<<" "<<dataNei_multiplelayer_2[j+index_z][k]<<endl;
			double dd = (dataPos[dataNei_multiplelayer_1[j+index_z][k]][dataNei_multiplelayer_2[j+index_z][k]][0] - dataPos[i][j][0] - p[0])*
			(dataPos[dataNei_multiplelayer_1[j+index_z][k]][dataNei_multiplelayer_2[j+index_z][k]][0] - dataPos[i][j][0] - p[0])
			+ (dataPos[dataNei_multiplelayer_1[j+index_z][k]][dataNei_multiplelayer_2[j+index_z][k]][1] - dataPos[i][j][1] - p[1])*
		(dataPos[dataNei_multiplelayer_1[j+index_z][k]][dataNei_multiplelayer_2[j+index_z][k]][1] - dataPos[i][j][1] - p[1])
			+ (dataPos[dataNei_multiplelayer_1[j+index_z][k]][dataNei_multiplelayer_2[j+index_z][k]][2] - dataPos[i][j][2] - p[2])*
		(dataPos[dataNei_multiplelayer_1[j+index_z][k]][dataNei_multiplelayer_2[j+index_z][k]][2] - dataPos[i][j][2] - p[2]);
				if(dd<near)
				{
					near = dd;
					index = k;
				}
			}

			double cos = dataVel[i][j][0]*
				dataVel[dataNei_multiplelayer_1[j+index_z][index]][dataNei_multiplelayer_2[j+index_z][index]][0]
			+ dataVel[i][j][1]*
				dataVel[dataNei_multiplelayer_1[j+index_z][index]][dataNei_multiplelayer_2[j+index_z][index]][1]
			+ dataVel[i][j][2]*
				dataVel[dataNei_multiplelayer_1[j+index_z][index]][dataNei_multiplelayer_2[j+index_z][index]][2];

			if(cos<maxcos)
			{
				maxcos = cos;
			}
			
			if(maxcos>1||maxcos<-1)
			{
				//cerr<<maxcos<<endl;


				datalayerdis[i].add(0);
			}
			else
				datalayerdis[i].add(acos(maxcos));
			count ++;

		}
		//cerr<<i<<" "<<count<<" "<<index_z<<endl;
		index_z = count;

	}*/
	if(datalayerdis!=NULL)
	{
		for(int i=0;i<zlayer;i++)
			datalayerdis[i].free();
		delete [] datalayerdis;
		datalayerdis = NULL;
	}

	datalayerdis = new svScalarArray[zlayer];

	int index;
	int count = 0;
	int index_z = 0;
	double maxcos;
	for(int i=0;i<zlayer;i++)
	{	
		for(int j=0;j<dataPos[i].size();j++)
		{//cerr<<i<<" "<<j<<endl;
			maxcos = 2;
			
			if(dataNei_planecomparsion1[count]>=0
				&& dataNei_planecomparsion2[count]>=0)
			{
			
				double cos = dataVel[i][j][0]*
					dataVel[dataNei_planecomparsion1[count]][dataNei_planecomparsion2[count]][0]
				+ dataVel[i][j][1]*
					dataVel[dataNei_planecomparsion1[count]][dataNei_planecomparsion2[count]][1]
				+ dataVel[i][j][2]*
					dataVel[dataNei_planecomparsion1[count]][dataNei_planecomparsion2[count]][2];
				//cerr<<dataNei_planecomparsion1[count]<<" ";

				if(cos<maxcos)
				{
					maxcos = cos;
				}
			
				if((maxcos>1||maxcos<-1)&&fabs(fabs(maxcos)-1)>0.0001)
				{
				//if(dataPos[i][j][2] == -7)
				//cerr<<maxcos<<" "<<dataPos[i][j][0]<<" "<<dataPos[i][j][1]<<endl;
					//cerr<<maxcos<<endl;


					datalayerdis[i].add(5);
				}
				else
				{
					if(fabs(fabs(maxcos)-1)<=0.0001)
						maxcos = 1.;
					datalayerdis[i].add(acos(maxcos));
				}
			}
			else
			{
				//if(dataPos[i][j][2] == -7)
				//cerr<<dataPos[i][j][0]<<" "<<dataPos[i][j][1]<<endl;
				datalayerdis[i].add(5);
			}			

			count ++;
		}
	}
}


void QDOTdistancefunction::SetNeighbours_Onelayer(int n)
{
	if(datalayerdis!=NULL)
	{
		for(int i=0;i<zlayer;i++)
			datalayerdis[i].free();
		delete [] datalayerdis;
		datalayerdis = NULL;
	}

	datalayerdis = new svScalarArray[zlayer];

	int nei_num;
	int nei_index[2];
	float cos;
	float maxcos;
	double dd;
	double td;
	double maxl;
	int index_z;
	int index;

	int sample[2];
	sample[0] = zlayer/2;

	svVector3 c = (lbbox+rbbox)/2;

	
	td = 9e+9;
	for(int i=0;i<dataPos[sample[0]].size();i++)
	{
		dd = (dataPos[sample[0]][i][0] - c[0])*(dataPos[sample[0]][i][0] - c[0])
			+ (dataPos[sample[0]][i][1] - c[1])*(dataPos[sample[0]][i][1] - c[1]);
		if(dd<td)
		{
			td = dd;
			sample[1] = i;
		}
	}
	td = -9e+9;
	index_z = 0;
	for(int i=0;i<sample[0];i++)
	{
		index_z  = index_z + dataPos[i].size();
	}

	for(int k=n;k<n+1;k++)
	{
		index = index_z + sample[1];
		dd = (dataPos[sample[0]][sample[1]][0] 
			- dataPos[dataNei_onelayer_1[index][k]][dataNei_onelayer_2[index][k]][0])*(dataPos[sample[0]][sample[1]][0]   
                        - dataPos[dataNei_onelayer_1[index][k]][dataNei_onelayer_2[index][k]][0])
			+ (dataPos[sample[0]][sample[1]][1] 
			- dataPos[dataNei_onelayer_1[index][k]][dataNei_onelayer_2[index][k]][1])*(dataPos[sample[0]][sample[1]][1]   
                        - dataPos[dataNei_onelayer_1[index][k]][dataNei_onelayer_2[index][k]][1]);
		if(dd>td)
		{
			td = dd;
		}
	}
	maxl = td;	
	//cerr<<maxl<<endl;

	index_z = 0;
	for(int i=0;i<zlayer;i++)
	{	
		for(int j=0;j<dataPos[i].size();j++)
		{//cerr<<i<<" "<<j<<endl;
			maxcos = 2;
			index = index_z + j;
			for(int k=1;k<n+1;k++)//get rid of the points itself
			{
				dd = (dataPos[i][j][0]
				 - dataPos[dataNei_onelayer_1[index][k]][dataNei_onelayer_2[index][k]][0])*(dataPos[i][j][0]
                                 - dataPos[dataNei_onelayer_1[index][k]][dataNei_onelayer_2[index][k]][0])
				+ (dataPos[i][j][1] 
				- dataPos[dataNei_onelayer_1[index][k]][dataNei_onelayer_2[index][k]][1])*(dataPos[i][j][1]   
                                - dataPos[dataNei_onelayer_1[index][k]][dataNei_onelayer_2[index][k]][1]);
				if(dd>maxl)
				{
					//cerr<<dataPos[i][j][0]<<" "<<dataPos[i][j][1]<<
					//" "<<dataPos[dataNei_onelayer_1[index][k]][dataNei_onelayer_2[index][k]][0]<<" "<< dataPos[dataNei_onelayer_1[index][k]][dataNei_onelayer_2[index][k]][1]<<" "<<i<<" "<<j<<" "<<dataNei_onelayer_1[index][k]<<" "<<dataNei_onelayer_2[index][k]<<endl  ;
					//break;
				}

				cos = dataVel[i][j][0]*
					dataVel[dataNei_onelayer_1[index][k]][dataNei_onelayer_2[index][k]][0]
				+ dataVel[i][j][1]*
					dataVel[dataNei_onelayer_1[index][k]][dataNei_onelayer_2[index][k]][1]
				+ dataVel[i][j][2]*
					dataVel[dataNei_onelayer_1[index][k]][dataNei_onelayer_2[index][k]][2];

				if(cos<maxcos)
				{
					maxcos = cos;
					//nei_index[0] = dataNei_onelayer_1[k];
					//nei_index[1] = dataNei_onelayer_2[k];
				}
			}
			if(maxcos>1||maxcos<-1)
			{
				//cerr<<maxcos<<endl;
				datalayerdis[i].add(0);
			}
			else
				datalayerdis[i].add(acos(maxcos));
			//cerr<<acos(maxcos)<<" "<<maxcos<<endl;
			//if(i==zlayer-2)
				//cerr<<maxcos<<" ";
		}
		index_z = index_z + dataPos[i].size();
	}//cerr<<endl;
}

void QDOTdistancefunction::SetNeighbours_Ofspace(int n)
{
	if(datalayerdis!=NULL)
	{
		for(int i=0;i<zlayer;i++)
			datalayerdis[i].free();
		delete [] datalayerdis;
		datalayerdis = NULL;
	}
	datalayerdis = new svScalarArray[zlayer];

	int nei_num;
	int nei_index[2];
	float cos;
	float maxcos;
	double dd;
	double td;
	double maxl;
	int index_z;
	int index;

	int sample[2];
	sample[0] = zlayer/2;
	
	td = 9e+9;
	for(int i=0;i<dataPos[sample[0]].size();i++)
	{
		dd = (dataPos[sample[0]][i][0] - center[0])*(dataPos[sample[0]][i][0] - center[0])
			+ (dataPos[sample[0]][i][1] - center[1])*(dataPos[sample[0]][i][1] - center[1]);
		if(dd<td)
		{
			td = dd;
			sample[1] = i;
		}
	}
	td = -9e+9;
	index_z = 0;
	for(int i=0;i<sample[0];i++)
	{
		index_z  = index_z + dataPos[i].size();
	}

	for(int k=1;k<n+1;k++)
	{
		index = index_z + sample[1];
		dd = (dataPos[sample[0]][sample[1]][0] 
			- dataPos[dataNei_inspace_1[index][k]][dataNei_inspace_2[index][k]][0])*(dataPos[sample[0]][sample[1]][0]   
                        - dataPos[dataNei_inspace_1[index][k]][dataNei_inspace_2[index][k]][0])
			+ (dataPos[sample[0]][sample[1]][1] 
			- dataPos[dataNei_inspace_1[index][k]][dataNei_inspace_2[index][k]][1])*(dataPos[sample[0]][sample[1]][1]   
                        - dataPos[dataNei_inspace_1[index][k]][dataNei_inspace_2[index][k]][1])
			+ (dataPos[sample[0]][sample[1]][2] 
			- dataPos[dataNei_inspace_1[index][k]][dataNei_inspace_2[index][k]][2])*(dataPos[sample[0]][sample[1]][2]   
                        - dataPos[dataNei_inspace_1[index][k]][dataNei_inspace_2[index][k]][2]);
		if(dd>td)
		{
			td = dd;
		}
	}
	maxl = td;

	index_z = 0;
	int counttmp = 0;
	for(int i=0;i<zlayer;i++)
	{//cerr<<i<<endl;
		for(int j=0;j<dataPos[i].size();j++)
		{
			maxcos = 2;
			//nei_num = 0;
			index = index_z + j;
			//cerr<<i<<" "<<j<<" "<<dataPos[i].size()<<" "<<index_z<<endl;
			for(int k=1;k<n+1;k++)//get rid of the points itself
			{
				//if(i==78&&j==3926)
					//cerr<<dataNei_inspace_1[index][k]<<endl;
				dd = (dataPos[i][j][0]
				 - dataPos[dataNei_inspace_1[index][k]][dataNei_inspace_2[index][k]][0])*(dataPos[i][j][0]
                                 - dataPos[dataNei_inspace_1[index][k]][dataNei_inspace_2[index][k]][0])
				+ (dataPos[i][j][1] 
				- dataPos[dataNei_inspace_1[index][k]][dataNei_inspace_2[index][k]][1])*(dataPos[i][j][1]   
                                - dataPos[dataNei_inspace_1[index][k]][dataNei_inspace_2[index][k]][1])
				+ (dataPos[i][j][2] 
				- dataPos[dataNei_inspace_1[index][k]][dataNei_inspace_2[index][k]][2])*(dataPos[i][j][2]   
                                - dataPos[dataNei_inspace_1[index][k]][dataNei_inspace_2[index][k]][2]);
				//if(dd>maxl)
				//	break;

				cos = dataVel[i][j][0]*
					dataVel[dataNei_inspace_1[index][k]][dataNei_inspace_2[index][k]][0]
				+ dataVel[i][j][1]*
					dataVel[dataNei_inspace_1[index][k]][dataNei_inspace_2[index][k]][1]
				+ dataVel[i][j][2]*
					dataVel[dataNei_inspace_1[index][k]][dataNei_inspace_2[index][k]][2];

				if(cos<maxcos)
				{
					maxcos = cos;
					//nei_index[0] = dataNei_inspace_1[k];
					//nei_index[1] = dataNei_inspace_2[k];
				}
				//nei_num ++;
			}
			//if(maxcos>0)
			if(maxcos>1||maxcos<-1)
			{
				//cerr<<maxcos<<endl;
				datalayerdis[i].add(0);
				counttmp++;
			}
			else
				datalayerdis[i].add(acos(maxcos));
			//else
			//	datalayerdis[i].add(acos(maxcos)+PI);
		}
		index_z = index_z + dataPos[i].size();
	}//cerr<<counttmp<<endl;
}

void QDOTdistancefunction::SetGlobalFunction(int x, int y)
{
	if(datalayerdis!=NULL)
	{
		for(int i=0;i<zlayer;i++)
			datalayerdis[i].free();
		delete [] datalayerdis;
		datalayerdis = NULL;
	}
	datalayerdis = new svScalarArray[zlayer];

	float cos;
	for(int i=0;i<zlayer;i++)
	{
		for(int j=0;j<dataPos[i].size();j++)
		{

			cos = dataVel[i][j][0]*
				dataVel[x][y][0]
				+ dataVel[i][j][1]*
				dataVel[x][y][1]
				+ dataVel[i][j][2]*
				dataVel[x][y][2];

			datalayerdis[i].add(acos(cos));
		}
	}

}

void QDOTdistancefunction::SetPlaneCosFunction(svIntArray *n1, svIntArray *n2)
{
	if(datalayerdis!=NULL)
	{
		for(int i=0;i<zlayer;i++)
			datalayerdis[i].free();
		delete [] datalayerdis;
		datalayerdis = NULL;
	}
	datalayerdis = new svScalarArray[zlayer];

	double maxcos,cos;

	for(int i=0;i<zlayer;i++)
	{
		for(int j=0;j<dataPos[i].size();j++)
		{
			//maxcos = 2;
			if(n1[i][j]!=-1)
			{
				cos = dataVel[i][j][0]*
					dataVel[n1[i][j]][n2[i][j]][0]
				+ dataVel[i][j][1]*
					dataVel[n1[i][j]][n2[i][j]][1]
				+ dataVel[i][j][2]*
					dataVel[n1[i][j]][n2[i][j]][2];
			}
			else
			{
				cos = 2;
			}

			/*if(cos<maxcos)
			{
				maxcos = cos;
			}*/
			//cerr<<cos<<" ";

			if(cos>1||cos<-1)
			{
				datalayerdis[i].add(4);//?????????????????????
			}
			else
				datalayerdis[i].add(acos(cos));
		}
	}
}

void QDOTdistancefunction::SetPlaneClusterFunction(int *idx, svIntArray *n1, svIntArray *n2)
{
	if(datalayerdis!=NULL)
	{
		for(int i=0;i<zlayer;i++)
			datalayerdis[i].free();
		delete [] datalayerdis;
		datalayerdis = NULL;
	}
	datalayerdis = new svScalarArray[zlayer];

	int index[zlayer];
	for(int i=0;i<zlayer;i++)
	{
		index[i] = 0;
	}
	for(int i=0;i<zlayer;i++)
	{
		for(int j=0;j<i;j++)
		{
			index[i] = index[i] + dataPos[j].size();
		}
	}

	double dis;

	for(int i=0;i<zlayer;i++)
	{
		for(int j=0;j<dataPos[i].size();j++)
		{
			if(idx[index[n1[i][j]] + n2[i][j]]!=-1&&idx[index[i]+j]!=-1)
			{
				dis = idx[index[i] + j] - idx[index[n1[i][j]] + n2[i][j]];
				//if(dis>0)cerr<<dis<<" ";
			}
			else if(idx[index[i]+j]!=-1)
				dis = idx[index[i] + j];
			else
				dis = -9e+9;

			datalayerdis[i].add(dis);
		}
	}//cerr<<"---"<<endl;
}
/*
void QDOTdistancefunction::SetPlaneCosClusterFunction(int *idx, svIntArray *n1, svIntArray *n2, int n)
{
	if(datalayerdis!=NULL)
	{
		for(int i=0;i<zlayer;i++)
		
			datalayerdis[i].free();
		delete [] datalayerdis;
		datalayerdis = NULL;
	}
	if(datalayerdis2!=NULL)
	{
		for(int i=0;i<zlayer;i++)
		
			datalayerdis2[i].free();
		delete [] datalayerdis2;
		datalayerdis2 = NULL;
	}
	if(datalayercol!=NULL)
	{
		for(int i=0;i<zlayer;i++)
			datalayercol[i].free();
		delete [] datalayercol;
		datalayercol = NULL;
	}
	datalayerdis = new svScalarArray[zlayer];
	datalayerdis2 = new svScalarArray[zlayer];
	datalayercol = new svVector3Array[zlayer];

	int index[zlayer];
	for(int i=0;i<zlayer;i++)
	{
		index[i] = 0;
	}
	for(int i=0;i<zlayer;i++)
	{
		for(int j=0;j<i;j++)
		{
			index[i] = index[i] + dataPos[j].size();
		}
	}

	double dis;
	double cos;

	CIEColor *c = new CIEColor();

	for(int i=0;i<zlayer;i++)
	{
		for(int j=0;j<dataPos[i].size();j++)
		{
			
			if(n1[i][j]!=-1&&idx[index[i]+j]!=-1)
			{
				if(idx[index[n1[i][j]] + n2[i][j]]!=-1) //????????????
					dis = idx[index[i] + j] - idx[index[n1[i][j]] + n2[i][j]];
				else
					dis = idx[index[i] + j];
			}
			else
				dis = -9e+9;

			datalayerdis[i].add(dis);

			if(n1[i][j]!=-1&&idx[index[i]+j]!=-1)
			{
				cos = dataVel[i][j][0]*
					dataVel[n1[i][j]][n2[i][j]][0]
				+ dataVel[i][j][1]*
					dataVel[n1[i][j]][n2[i][j]][1]
				+ dataVel[i][j][2]*
					dataVel[n1[i][j]][n2[i][j]][2];
			}
			else
			{
				cos = 2;
			}

			if(cos>1||cos<-1)
			{
				datalayerdis2[i].add(-9e+9);//?????????????????????
			}
			else
				datalayerdis2[i].add(acos(cos));

			if(fabs(datalayerdis[i][j])<=100.&&fabs(datalayerdis2[i][j])<=100.)
			{
				int cc = (int)((n+1)/2) +datalayerdis[i][j];
				vector<double> RGB = c->SinglehueSequential(datalayerdis2[i][j]/PI,0.5,0.,1.,
				 (double)cc * (double)(270./(n+1)));
				svVector3 r;
				r[0] =RGB[0];r[1]=RGB[1];r[2]=RGB[2];
				datalayercol[i].add(r);
			}			
		}
	}
	delete [] c;
}*/
void QDOTdistancefunction::SetPlaneCosClusterFunction(int *idx, svIntArray *n1, svIntArray *n2)
{
	if(datalayerdis!=NULL)
	{
		for(int i=0;i<zlayer;i++)
		
			datalayerdis[i].free();
		delete [] datalayerdis;
		datalayerdis = NULL;
	}
	if(datalayerdis2!=NULL)
	{
		for(int i=0;i<zlayer;i++)
		
			datalayerdis2[i].free();
		delete [] datalayerdis2;
		datalayerdis2 = NULL;
	}

	datalayerdis = new svScalarArray[zlayer];
	datalayerdis2 = new svScalarArray[zlayer];

	int index[zlayer];
	for(int i=0;i<zlayer;i++)
	{
		index[i] = 0;
	}
	for(int i=0;i<zlayer;i++)
	{
		for(int j=0;j<i;j++)
		{
			index[i] = index[i] + dataPos[j].size();
		}
	}

	double dis;
	double cos;


	for(int i=0;i<zlayer;i++)
	{
		for(int j=0;j<dataPos[i].size();j++)
		{
			
			if(n1[i][j]!=-1&&idx[index[i]+j]!=-1)
			{
				if(idx[index[n1[i][j]] + n2[i][j]]!=-1) //????????????
					dis = idx[index[i] + j] - idx[index[n1[i][j]] + n2[i][j]];
				else
					dis = idx[index[i] + j];
			}
			else
				dis = -9e+9;

			datalayerdis[i].add(dis);

			if(n1[i][j]!=-1&&idx[index[i]+j]!=-1)
			{
				cos = dataVel[i][j][0]*
					dataVel[n1[i][j]][n2[i][j]][0]
				+ dataVel[i][j][1]*
					dataVel[n1[i][j]][n2[i][j]][1]
				+ dataVel[i][j][2]*
					dataVel[n1[i][j]][n2[i][j]][2];
			}
			else
			{
				cos = 2;
			}

			if(cos>1||cos<-1)
			{
				datalayerdis2[i].add(-9e+9);//?????????????????????
			}
			else
				datalayerdis2[i].add(acos(cos));
			
		}
	}

}
void QDOTdistancefunction::cleanData()
{
	if(dataSize!=0)
	{
		for(int i=0;i<zlayer;i++)
		{
			dataPos[i].free();
			dataVel[i].free();
			datalayerpos[i].free();
			datalayerdis[i].free();
			datalayerdis2[i].free();	
			datalayercol[i].free();			
		}
		delete [] dataPos;
		delete [] dataVel;
		delete [] datalayerpos;
		delete [] datalayerdis;
		delete [] datalayerdis2;
		delete [] datalayercol;
		dataPos = NULL;
		dataVel = NULL;
		datalayerpos = NULL;	
		datalayerdis = NULL;
		datalayerdis2 = NULL;
		datalayercol = NULL;
		for(int i=0;i<dataSize;i++)
		{
			dataNei_inspace_1[i].free();
			dataNei_inspace_2[i].free();
			dataNei_onelayer_1[i].free();
			dataNei_onelayer_2[i].free();
			dataNei_multiplelayer_1[i].free();
			dataNei_multiplelayer_2[i].free();
		}
		delete [] dataNei_inspace_1;
		delete [] dataNei_inspace_2;
		delete [] dataNei_onelayer_1;
		delete [] dataNei_onelayer_2;
		delete [] dataNei_multiplelayer_1;
		delete [] dataNei_multiplelayer_2;
		dataNei_inspace_1= NULL;
		dataNei_inspace_2 = NULL;
		dataNei_onelayer_1 = NULL;	
		dataNei_onelayer_2 = NULL;
		dataNei_multiplelayer_1 = NULL;
		dataNei_multiplelayer_2 = NULL;

		delete [] dataNei_planecomparsion1;
		delete [] dataNei_planecomparsion2;

		dataSize = 0;
	}
}

}

