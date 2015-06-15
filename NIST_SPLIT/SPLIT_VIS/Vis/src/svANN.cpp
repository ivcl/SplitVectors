#include <math.h>
#include "svANN.h"
#include "MVectorMatrix.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <vector>


namespace __svl_lib {

svANN::svANN()
{
	NEIGHBOR_NUM = 4;
	ctrpos=NULL;
	dataSize = 0;

}

void svANN::SetData(svInt num, svVector3Array *pos,
		   int znum)
{
	dataSize = num;
	ctrpos = new double[dataSize*3];
	mapindex = new svVector3[dataSize];
	
	int k=0;
	for(int i=0;i<znum;i++)
	{
		for(int j=0;j<pos[i].size();j++)
		{
			ctrpos[k] = pos[i][j][0];
			ctrpos[k+dataSize] = pos[i][j][1];
			ctrpos[k+dataSize*2] = pos[i][j][2];
			mapindex[k][0] = i;
			mapindex[k][1] = j;
			k ++;
		}
		zValue.push_back(k);
	}
}

void svANN::SetData(svInt num, svVector3 *pos)
{
	if(dataSize!=0)
	{
		delete [] ctrpos;
	}

	dataSize = num;
	ctrpos = new double[dataSize*3];
	
	int k=0;
	for(int i=0;i<num;i++)
	{

			ctrpos[k] = pos[i][0];
			ctrpos[k+dataSize] = pos[i][1];
			ctrpos[k+dataSize*2] = pos[i][2];
			k++;
	}
}

void svANN::ANN_montecarlo(svInt num, svVector3Array *pos,int znum, float zvalue,
			int *n1, int *n2, svVector3 v)
{
	//cerr<<v[0]<<" "<<v[1]<<" "<<v[2]<<endl;
	int	nPts;					// actual number of data points
	int	nPts_in;					// actual number of data points
	ANNpointArray	dataPts;				// data points
	ANNpoint	queryPt;				// query point
	ANNidxArray	nnIdx;					// near neighbor indices
	ANNdistArray    dists;					// near neighbor distances
	ANNkd_tree*	kdTree;					// search structure
	nnIdx = new ANNidx[5];						// allocate near neigh indices
	dists = new ANNdist[5];						// allocate near neighbor dists
	int *p[5];

	int z;
	for(int i=0;i<znum;i++)
	{
		if(fabs(zvalue - pos[i][0][2]) < 0.00001)
		{
			z = i;
			break;
		}
	}

	nPts = pos[z].size();
	int dim = 3;
	int maxPts = pos[z].size()*dim;
	p[0] = new int[pos[z].size()];
	p[1] = new int[pos[z].size()];
	p[2] = new int[pos[z].size()];
	p[3] = new int[pos[z].size()];
	p[4] = new int[pos[z].size()];
	queryPt = annAllocPt(dim);					// allocate query point
	dataPts = annAllocPts(maxPts, dim);			// allocate data points

	for(int i=0;i<pos[z].size();i++)
	{
		dataPts[i][0] = pos[z][i][0];
		dataPts[i][1] = pos[z][i][1];
		dataPts[i][2] = pos[z][i][2];
	}

	kdTree = new ANNkd_tree(	dataPts,					// the data points
					nPts,						// number of points
					dim);						// dimension of space	
	for(int i=0;i<pos[z].size();i++)
	{
		queryPt[0] = pos[z][i][0];
		queryPt[1] = pos[z][i][1];
		queryPt[2] = pos[z][i][2];

		kdTree->annkSearch(									// search
				queryPt,						// query point
				5,								// number of near neighbors
				nnIdx,							// nearest neighbors (returned)
				dists,							// distance (returned)
				0.0);		// error bound

		p[0][i] = nnIdx[0];
		p[1][i] = nnIdx[1];
		p[2][i] = nnIdx[2];
		p[3][i] = nnIdx[3];
		p[4][i] = nnIdx[4];
	}

	delete [] nnIdx;
	delete [] dists;
	nnIdx = new ANNidx[1];
	dists = new ANNdist[1];

	int t = 0;
	int tt = 0;
	for(int i=0;i<znum;i++)
	{
		if(i!=z)
		{
			//cerr<<pos[i].size()<<" "<<endl;
			for(int j=0;j<pos[i].size();j++)
			{
				queryPt[0] = pos[i][j][0];
				queryPt[1] = pos[i][j][1];
				queryPt[2] = pos[i][j][2];

				kdTree->annkSearch(									
						queryPt,						
						1,								
						nnIdx,							
						dists,							
						0.0);		// error bound
				int ii = z;
				int jj = nnIdx[0];
				double dd[5];
				double td[5];
				double dd_min;
				double td_min;
				int index_min;
				double sum;
				svVector3 nv;
				int index_m;
				td[0] = sqrt((pos[ii][jj][0] - pos[i][j][0]) * 
						(pos[ii][jj][0] - pos[i][j][0]) +
						(pos[ii][jj][1] - pos[i][j][1]) * 
						(pos[ii][jj][1] - pos[i][j][1]));
				nv[0] = (pos[ii][jj][0] - pos[i][j][0]) / td[0];
				nv[1] = (pos[ii][jj][1] - pos[i][j][1]) / td[0];
				dd[0] = nv[0] * v[0] + nv[1] * v[1];	
				td_min = td[0];
				dd_min = dd[0];
				index_min = jj;
				index_m = 0;
				if(td[0] == 0.)
				{
					n1[t] = z;
					n2[t] = jj;
				}
				else
				{
					while(1)
					{		
						for(int m=0;m<4;m++)
						{//cerr<<"m: "<<m<<" "<<p[m+1][jj]<< endl;
							td[m+1] = sqrt((pos[ii][p[m+1][jj]][0] - pos[i][j][0]) * 
								(pos[ii][p[m+1][jj]][0] - pos[i][j][0]) +
								(pos[ii][p[m+1][jj]][1] - pos[i][j][1]) * 
								(pos[ii][p[m+1][jj]][1] - pos[i][j][1]));
							nv[0] = (pos[ii][p[m+1][jj]][0] - pos[i][j][0]) / td[m+1];
							nv[1] = (pos[ii][p[m+1][jj]][1] - pos[i][j][1]) / td[m+1];
							dd[m+1] = nv[0] * v[0] + nv[1] * v[1];
							if(dd[m+1] > dd_min && td[m+1]<=td_min)
							{
								index_min = p[m+1][jj];
								index_m = m+1;
								dd_min = dd[m+1];
								td_min = td[m+1];
							}
						}
						//cerr<<index_min<<" "<<index_m<<endl;
						if(fabs(dd_min - 1.) < 0.0001 && index_min == jj)
						{
							break;
						}
						else if(index_min == jj)
						{
							jj = -1;
							break;
						}
						else
						{
							jj = index_min;
							td[0] = td[index_m];
							dd[0] = dd[index_m];
						}
					}
					tt++;

					n1[t] = z;
					n2[t] = jj;
					//if(pos[i][j][0]==15.75&&pos[i][j][1]==16.75){
					//cerr<<jj<<" "<<dd_min<<" "<<td_min<<" "<<pos[i][j][0]<<" "<<pos[i][j][1]<<" "<<pos[z][index_min][0]<<" "<<pos[z][index_min][1]<<endl;}

				}
				t++;
			}
		}
		else
		{
			for(int j=0;j<pos[i].size();j++)
			{
				n1[t] = -1;
				n2[t] = -1;
				t++;
			}
		}
	}	
}

svVector4 * svANN::ANN_individual(svVector3 *pos, int num, svVector4 *IDX, bool *in, bool *all_in)
{

		int	nPts;					// actual number of data points
		int	nPts_in;					// actual number of data points
		ANNpointArray	dataPts;				// data points
		ANNpointArray	dataPts_in;				// data points
		ANNpoint	queryPt;				// query point
		ANNidxArray	nnIdx;					// near neighbor indices
		ANNdistArray    dists;					// near neighbor distances
		ANNkd_tree*	kdTree;					// search structure
		ANNkd_tree*	kdTree_in;					// search structure
		int dim = 3;
		int maxPts = dataSize*dim;
		int k = 5;

		queryPt = annAllocPt(dim);					// allocate query point
		dataPts = annAllocPts(maxPts, dim);			// allocate data points
		nnIdx = new ANNidx[k];						// allocate near neigh indices
		dists = new ANNdist[k];						// allocate near neighbor dists

		nPts = dataSize;									// read data points

		double  *querypos = new double[num*3];
		int kk=0;	
		svVector4 *idx_q = new svVector4[num];

		for(int i=0;i<num;i++)
		{
			querypos[kk] = pos[i][0];
			querypos[kk+num] = pos[i][1];
			querypos[kk+num*2] = pos[i][2];
			kk++;
		}

		int in_count = 0;
		for(int i=0;i<dataSize;i++)
		{
			dataPts[i][0] = ctrpos[i];
			dataPts[i][1] = ctrpos[i+dataSize];
			dataPts[i][2] = ctrpos[i+dataSize*2];
			if(all_in[i]) in_count ++;
		}

		nPts_in = in_count;
		dataPts_in = annAllocPts(in_count*dim, dim);
		for(int i=0;i<in_count;i++)
		{
			if(all_in[i]){
			dataPts_in[i][0] = ctrpos[i];
			dataPts_in[i][1] = ctrpos[i+dataSize];
			dataPts_in[i][2] = ctrpos[i+dataSize*2];
			}			
		}

		

		kdTree = new ANNkd_tree(					// build search structure
						dataPts,					// the data points
						nPts,						// number of points
						dim);						// dimension of space

		kdTree_in = new ANNkd_tree(					// build search structure
						dataPts_in,					// the data points
						nPts_in,						// number of points
						dim);	
		for(int i=0;i<num;i++)
		{
			queryPt[0] = querypos[i];
			queryPt[1] = querypos[i+num];
			queryPt[2] = querypos[i+num*2];
			
			if(!in[i] || nPts_in<1)
			kdTree->annkSearch(									// search
					queryPt,						// query point
					1,								// number of near neighbors
					nnIdx,							// nearest neighbors (returned)
					dists,							// distance (returned)
					0.0);		
					// error bound

			else
			kdTree_in->annkSearch(									// search
					queryPt,						// query point
					1,								// number of near neighbors
					nnIdx,							// nearest neighbors (returned)
					dists,							// distance (returned)
					0.0);		
					// error bound

			/*svVector4 index;
			int count[k];
			for(int j=0;j<k;j++)
			{
				count[j] = 0;
				if(i==0)
					cerr<<IDX[nnIdx[j]][0] <<" "<<IDX[nnIdx[j]][1] <<" "<<IDX[nnIdx[j]][2] <<endl;
			}

			for(int j=0;j<k;j++)
			{
				for(int m=j+1;m<k;m++)
				{
					if(fabs(IDX[nnIdx[j]][0] - IDX[nnIdx[m]][0])<=0.0001
					&& fabs(IDX[nnIdx[j]][1] - IDX[nnIdx[m]][1])<=0.0001
					&& fabs(IDX[nnIdx[j]][2] - IDX[nnIdx[m]][2])<=0.0001)
					{
						count[j]++;
					}
				}
			}
			index = IDX[nnIdx[0]];
			for(int j=1;j<k;j++)
			{
				if(count[j]>count[j-1])
				{	
					index = IDX[nnIdx[j]];
				}
			}
			idx_q[i] = index;
			if(i==0)
				cerr<<idx_q[i][0]<<" "<<idx_q[i][1]<<" "<<idx_q[i][2]<<endl;*/
			if(i==0)
			{
				cerr<<pos[i][0]<<" "<<pos[i][1]<<" "<<pos[i][2]<<endl;
				cerr<<dataPts[nnIdx[0]][0]<<" "<<dataPts[nnIdx[0]][1]<<" "<<dataPts[nnIdx[0]][2]<<endl;
			}
			idx_q[i] = IDX[nnIdx[0]];

		}
	    delete [] nnIdx;							// clean things up
	    delete [] dists;
	    delete kdTree;
		
	annClose();			

						// done with ANN

	return idx_q;
}


void svANN::ANN_space(char *file)
{
	char filename[100];
	sprintf(filename,"%s_inspace",file);
	
	ifstream infile(filename);
	if(infile.is_open())
	{
		infile.close();
	}
	else
	{
		cerr<<"Create file"<<" "<<file<<endl;
		int					nPts;					// actual number of data points
		ANNpointArray		dataPts;				// data points
		ANNpoint			queryPt;				// query point
		ANNidxArray			nnIdx;					// near neighbor indices
		ANNdistArray		dists;					// near neighbor distances
		ANNkd_tree*			kdTree;					// search structure
		int dim = 3;
		int maxPts = dataSize*dim;
		int k = NEIGHBOR_NUM;

		queryPt = annAllocPt(dim);					// allocate query point
		dataPts = annAllocPts(maxPts, dim);			// allocate data points
		nnIdx = new ANNidx[k];						// allocate near neigh indices
		dists = new ANNdist[k];						// allocate near neighbor dists

		nPts = dataSize;									// read data points

		for(int i=0;i<dataSize;i++)
		{
			dataPts[i][0] = ctrpos[i];
			dataPts[i][1] = ctrpos[i+dataSize];
			dataPts[i][2] = ctrpos[i+dataSize*2];
		}

		kdTree = new ANNkd_tree(					// build search structure
						dataPts,					// the data points
						nPts,						// number of points
						dim);						// dimension of space
		
		ofstream outfile(filename);
		for(int i=0;i<dataSize;i++)
		{
			queryPt[0] = ctrpos[i];
			queryPt[1] = ctrpos[i+dataSize];
			queryPt[2] = ctrpos[i+dataSize*2];
			
			kdTree->annkSearch(									// search
								queryPt,						// query point
								k,								// number of near neighbors
								nnIdx,							// nearest neighbors (returned)
								dists,							// distance (returned)
								0.0);							// error bound
			for(int j=0;j<k;j++)
			{
				outfile<<mapindex[nnIdx[j]][0]<<" "<<mapindex[nnIdx[j]][1]<<" ";
			}
			outfile<<endl;
		}
	    delete [] nnIdx;							// clean things up
	    delete [] dists;
	    delete kdTree;
		annClose();									// done with ANN
		
		outfile.close();

	}
}

void svANN::ANN_layer(char *file)
{
	char filename[100];
	sprintf(filename,"%s_onelayer",file);
	
	ifstream infile(filename);
	if(infile.is_open())
	{
		infile.close();
	}
	else
	{	cerr<<"Create file"<<" "<<file<<endl;
		int					nPts;					// actual number of data points
		ANNpointArray		dataPts;				// data points
		ANNpoint			queryPt;				// query point
		ANNidxArray			nnIdx;					// near neighbor indices
		ANNdistArray		dists;					// near neighbor distances
		ANNkd_tree*			kdTree;					// search structure
		int dim = 3;
		int maxPts = dataSize;
		int k = NEIGHBOR_NUM;

		queryPt = annAllocPt(dim);					// allocate query point
		dataPts = annAllocPts(maxPts, dim);			// allocate data points
		nnIdx = new ANNidx[k];						// allocate near neigh indices
		dists = new ANNdist[k];						// allocate near neighbor dists

		ofstream outfile(filename);


		for(int ii=0;ii<zValue.size();ii++)
		{
			int s;
			
			if(ii==0)
			{
				nPts = zValue[0];
				s = 0;
			}
			else
			{
				nPts = zValue[ii] - zValue[ii-1];// read data points
				s = zValue[ii-1];
			}
	
			for(int i=s;i<zValue[ii];i++)
			{
				dataPts[i-s][0] = ctrpos[i];
				dataPts[i-s][1] = ctrpos[i+dataSize];
				dataPts[i-s][2] = ctrpos[i+dataSize*2];
			}
	
			kdTree = new ANNkd_tree(					// build search structure
							dataPts,					// the data points
							nPts,						// number of points
							dim);						// dimension of space
			
			
			for(int i=s;i<zValue[ii];i++)
			{
				queryPt[0] = ctrpos[i];
				queryPt[1] = ctrpos[i+dataSize];
				queryPt[2] = ctrpos[i+dataSize*2];
				
				kdTree->annkSearch(									// search
									queryPt,						// query point
									k,								// number of near neighbors
									nnIdx,							// nearest neighbors (returned)
									dists,							// distance (returned)
									0.0);							// error bound
				for(int j=0;j<k;j++)
				{
					//outfile<<nnIdx[j]<<" ";
					outfile<<mapindex[nnIdx[j]+s][0]<<" "<<mapindex[nnIdx[j]+s][1]<<" ";
				}
				outfile<<endl;
			}

			delete kdTree;	
		}
		
		outfile.close();
		delete [] nnIdx;							// clean things up
		delete [] dists;
		annClose();									// done with ANN
	}
}

void svANN::ANN_layer_multiple(char *file)
{
	char filename[100];
	sprintf(filename,"%s_multiplelayer",file);
	
	ifstream infile(filename);
	if(infile.is_open())
	{
		infile.close();
	}
	else
	{	cerr<<"Create file"<<" "<<file<<endl;
		int s;
		int					nPts;					// actual number of data points
		ANNpointArray		dataPts;				// data points
		ANNpoint			queryPt;				// query point
		ANNidxArray			nnIdx;					// near neighbor indices
		ANNdistArray		dists;					// near neighbor distances
		ANNkd_tree*			kdTree;					// search structure
		int dim = 3;
		int maxPts = dataSize;
		int k = NEIGHBOR_NUM;

		queryPt = annAllocPt(dim);					// allocate query point
		dataPts = annAllocPts(maxPts, dim);			// allocate data points
		nnIdx = new ANNidx[k];						// allocate near neigh indices
		dists = new ANNdist[k];						// allocate near neighbor dists

		ofstream outfile(filename);

		for(int kk=0;kk<zValue.size();kk++)//zValue.size()
		{	
			cerr<<kk<<endl;

			if(kk==0)
			{
				nPts = zValue[0];
				s = 0;
			}
			else
			{
				nPts = zValue[kk] - zValue[kk-1];// read data points
				s = zValue[kk-1];
			}
	
			for(int i=s;i<zValue[kk];i++)
			{
				dataPts[i-s][0] = ctrpos[i];
				dataPts[i-s][1] = ctrpos[i+dataSize];
				dataPts[i-s][2] = ctrpos[i+dataSize*2];
			}
	
			kdTree = new ANNkd_tree(					// build search structure
						dataPts,					// the data points
						nPts,						// number of points
						dim);						// dimension of space


			for(int ii=0;ii<zValue.size();ii++)
			{
				int s1;
				if(ii==0)
				{
					//nPts = zValue[0];
					s1 = 0;
				}
				else
				{
					//nPts = zValue[ii] - zValue[ii-1];// read data points
					s1 = zValue[ii-1];
				}
				//if(ii!=kk)
				//{

					for(int i=s1;i<zValue[ii];i++)
					{
						queryPt[0] = ctrpos[i];
						queryPt[1] = ctrpos[i+dataSize];
						queryPt[2] = ctrpos[i+dataSize*2];
				
						kdTree->annkSearch(								// search
											queryPt,						// query point
											k,								// number of near neighbors
											nnIdx,							// nearest neighbors (returned)
											dists,							// distance (returned)
											0.0);							// error bound
						for(int j=0;j<k;j++)
						{
							//outfile<<nnIdx[j]<<" ";
							outfile<<mapindex[nnIdx[j]+s][0]<<" "<<mapindex[nnIdx[j]+s][1]<<" ";
						}
						
					}
					outfile<<endl;

					
				//}
			}
			//outfile<<endl;
			delete kdTree;	
		}
		outfile.close();
		delete [] nnIdx;							// clean things up
		delete [] dists;
		annClose();									// done with ANN
	}
	
}
void svANN::ANN_layer_single(int z, char *file)
{
	char filename[100];
	sprintf(filename,"%stmpplanecomparison",file);
	
	ifstream infile(filename);
	if(infile.is_open())
	{
		infile.close();
	}
	else
	{	cerr<<"Create file"<<" "<<filename<<endl;
		int s;
		int					nPts;					// actual number of data points
		ANNpointArray		dataPts;				// data points
		ANNpoint			queryPt;				// query point
		ANNidxArray			nnIdx;					// near neighbor indices
		ANNdistArray		dists;					// near neighbor distances
		ANNkd_tree*			kdTree;					// search structure
		int dim = 3;
		int maxPts = dataSize;
		int k = NEIGHBOR_NUM;

		queryPt = annAllocPt(dim);					// allocate query point
		dataPts = annAllocPts(maxPts, dim);			// allocate data points
		nnIdx = new ANNidx[k];						// allocate near neigh indices
		dists = new ANNdist[k];						// allocate near neighbor dists

		ofstream outfile(filename);

		int kk = z;	


		if(kk==0)
		{
			nPts = zValue[0];
			s = 0;
		}
		else
		{
			nPts = zValue[kk] - zValue[kk-1];// read data points
			s = zValue[kk-1];
		}
	
		for(int i=s;i<zValue[kk];i++)
		{
			dataPts[i-s][0] = ctrpos[i];
			dataPts[i-s][1] = ctrpos[i+dataSize];
			dataPts[i-s][2] = ctrpos[i+dataSize*2];
		}
	
		kdTree = new ANNkd_tree(					// build search structure
					dataPts,					// the data points
					nPts,						// number of points
					dim);						// dimension of space

		for(int ii=0;ii<zValue.size();ii++)
		{
			int s1;
			if(ii==0)
			{
				//nPts = zValue[0];
				s1 = 0;
			}
			else
			{
				//nPts = zValue[ii] - zValue[ii-1];// read data points
				s1 = zValue[ii-1];
			}

			for(int i=s1;i<zValue[ii];i++)
			{
				queryPt[0] = ctrpos[i];
				queryPt[1] = ctrpos[i+dataSize];
				queryPt[2] = ctrpos[i+dataSize*2];
				
				kdTree->annkSearch(								// search
											queryPt,				// query point
											k,								// number of near neighbors
											nnIdx,							// nearest neighbors (returned)
											dists,							// distance (returned)
											0.0);							// error bound
				for(int j=0;j<k;j++)
				{
					//outfile<<nnIdx[j]<<" ";
					outfile<<mapindex[nnIdx[j]+s][0]<<" "<<mapindex[nnIdx[j]+s][1]<<" ";
				}
						
			}
			outfile<<endl;

		}

		delete kdTree;	
		
		outfile.close();
		delete [] nnIdx;							// clean things up
		delete [] dists;
		annClose();									// done with ANN
	}
	
}
/*
void svANN::ANN_layer_multiple(char *file)
{
	char filename[100];
	sprintf(filename,"%s_multiplelayer",file);
	
	ifstream infile(filename);
	if(infile.is_open())
	{
		infile.close();
	}
	else
	{	cerr<<"Create file"<<" "<<file<<endl;
		int			nPts;					// actual number of data points
		ANNpointArray		dataPts;				// data points
		ANNpoint			queryPt;				// query point
		ANNidxArray			nnIdx;					// near neighbor indices
		ANNdistArray		dists;					// near neighbor distances
		ANNkd_tree*			kdTree;					// search structure
		int dim = 3;
		int maxPts = dataSize;
		int k = NEIGHBOR_NUM;

		queryPt = annAllocPt(dim);					// allocate query point
		dataPts = annAllocPts(maxPts, dim);			// allocate data points
		nnIdx = new ANNidx[k];						// allocate near neigh indices
		dists = new ANNdist[k];						// allocate near neighbor dists

		ofstream outfile(filename);


		for(int ii=0;ii<zValue.size();ii++)
		{
			int s;cerr<<ii<<endl;
			
			if(ii==0)
			{
				s = 0;
			}
			else
			{
				s = zValue[ii-1];
			}

			for(int i=s;i<zValue[ii];i++)
			{
				queryPt[0] = ctrpos[i];
				queryPt[1] = ctrpos[i+dataSize];
				queryPt[2] = ctrpos[i+dataSize*2];
			
				for(int kk=0;kk<zValue.size();kk++)
				{
					int s1;

					if(kk==0)
					{
						nPts = zValue[0];
						s1 = 0;
					}
					else
					{
						nPts = zValue[kk] - zValue[kk-1];// read data points
						s1 = zValue[kk-1];
					}
					nPts = 0;
					vector<int> index;
					for(int i=s1;i<zValue[kk];i++)
					{
						if(ctrpos[i]>=queryPt[0]-1.
						&& ctrpos[i]<=queryPt[0]+1.
						&&ctrpos[i+dataSize]>=queryPt[1]-1.
						&&ctrpos[i+dataSize]<=queryPt[1]+1.)				
						{						
							dataPts[i-s1][0] = ctrpos[i];
							dataPts[i-s1][1] = ctrpos[i+dataSize];
							dataPts[i-s1][2] = ctrpos[i+dataSize*2];
							index.push_back(i);
							nPts++;
						}
					}
					//cerr<<nPts<<endl;
	
					kdTree = new ANNkd_tree(					// build search structure
									dataPts,					// the data points
									nPts,						// number of points
									dim);						// dimension of space
			
				
					kdTree->annkSearch(									// search
											queryPt,					// query point
											k,							// number of near neighbors
											nnIdx,						// nearest neighbors (returned)
											dists,						// distance (returned)
											0.0);						// error bound
					for(int j=0;j<k;j++)
					{
						//outfile<<nnIdx[j]<<" ";
						outfile<<mapindex[index[nnIdx[j]]][0]<<" "<<mapindex[index[nnIdx[j]]][1]<<" ";
					}
						

					delete kdTree;	
				}
				outfile<<endl;	
			}
		}
		
		outfile.close();
		delete [] nnIdx;							// clean things up
		delete [] dists;
		annClose();									// done with ANN
	}
	
}*/
}
