#include <math.h>
#include "svKNN.h"
#include "MVectorMatrix.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <vector>

#define NEIGHBOR_NUM 50

namespace __svl_lib {

svKNN::svKNN()
{
	if(!(ep = engOpen(""))){
		fprintf(stderr, "\nCan't start MATLAB engine\n");
		exit(0);
	}
	else
	{
		cerr<<"Open Matlab successfully"<<endl;
	}
	ctrpos=NULL;

}

void svKNN::SetData(svInt num, svVector3Array *pos,
		   int znum)
{
	dataSize = num;
	ctrpos = new double[dataSize*3];
	
	int k=0;
	for(int i=0;i<znum;i++)
	{
		for(int j=0;j<pos[i].size();j++)
		{
			ctrpos[k] = pos[i][j][0];
			ctrpos[k+dataSize] = pos[i][j][1];
			ctrpos[k+dataSize*2] = pos[i][j][2];
			k++;
		}
		zValue.push_back(k);
	}
}

void svKNN:KNN_space(char *file)
{
	char *filename;
	sprintf(filename,"%s_inspace",file);
	
	ifstream infile;
	if(infile(filename))
	{
		infile.close();
	}
	else
	{
		int kn[1];
		kn[0] = NEIGHBOR_NUM;
		int datanum[1];
		datanum[0] = dataSize;
		
		mxArray *knum = NULL;
		mxArray *num = NULL;
		mxArray *DATA1 = NULL;
		mxArray *DATA2 = NULL;
		double *a;
		
		knum = mxCreateNumericMatrix(1, 1,mxINT16_CLASS, mxREAL);
		num = mxCreateNumericMatrix(1, 1,mxINT16_CLASS, mxREAL);
		
		DATA1 = mxCreateDoubleMatrix(dataSize,3,mxREAL);
		DATA2 = mxCreateDoubleMatrix(dataSize,3,mxREAL);
		
		memcpy(mxGetData(knum),kn,sizeof(int));
		memcpy(mxGetData(num),dataSize,sizeof(int));
		memcpy((void *)mxGetPr(DATA1), (void *)ctrpos, dataSize*3*sizeof(double));
		memcpy((void *)mxGetPr(DATA2), (void *)ctrpos, dataSize*3*sizeof(double));
		
		engPutVariable(ep,"knum",knum);
		engPutVariable(ep,"num",num);
		engPutVariable(ep,"DATA1",DATA1);
		engPutVariable(ep,"DATA1",DATA1);
		
		engEvalString(ep,"IDX=knnsearch(DATA1, DATA2, 'k', knum);");
		
		//save to file
		mxArray *outfilename = NULL;
		
		outfilename = mxCreateString(filename);
		  
		engPutVariable(ep,"outfilename",outfilename);
		
		engEvalString(ep,"outfile = fopen(outfilename,'w')");
		engEvalString(ep,"for t=1:num;fprintf(outfile,'%d ',IDX(t,:));fprintf(outfile,'\n');end");	
		engEvalString(ep,"fclose(outfile);");

		mxDestroyArray(outfilename);
		mxDestroyArray(knum);
		mxDestroyArray(num);
		mxDestroyArray(DATA1);
		mxDestroyArray(DATA2);
	}
}

void svKNN:KNN_layer(char *file)
{
	char *filename;
	sprintf(filename,"%s_onelayer",file);
	
	ifstream infile;
	if(infile(filename))
	{
		infile.close();
	}
	else
	{	
		int kn[1];
		kn[0] = NEIGHBOR_NUM;
		int datanum[1];
		datanum[0] = dataSize;	
		double *layerpos;

		for(int i=0;i<zValue.size();i++)
		{
			mxArray *knum = NULL;
			mxArray *num = NULL;
			mxArray *DATA1 = NULL;
			mxArray *DATA2 = NULL;
			double *a;
			int s;
			int n[1];

			if(i==0)
			{
				n[0] = zValue[i]*3;
				layerpos = new double[zValue[i]*3];
				s = 0;
			}
			else
			{
				n[0] = 3*(zValue[i]-zValue[i-1]);
				layerpos = new double[3*(zValue[i]-zValue[i-1])];
				s = zValue[i-1];
			}

			for(int j=s;i<zValue[i];j++)
			{
				layerpos[j] = ctrpos[j];
				layerpos[j+n[0]] = ctrpos[j+dataSize];
				layerpos[j+n[0]*2] = ctrpos[j+dataSize*2];
			}
		
			knum = mxCreateNumericMatrix(1, 1,mxINT16_CLASS, mxREAL);
			num = mxCreateNumericMatrix(1, 1,mxINT16_CLASS, mxREAL);

			DATA1 = mxCreateDoubleMatrix(n[0],3,mxREAL);
			DATA2 = mxCreateDoubleMatrix(n[0],3,mxREAL);
		
			memcpy(mxGetData(knum),kn,sizeof(int));
			memcpy(mxGetData(num),n,sizeof(int));
			memcpy((void *)mxGetPr(DATA1), (void *)layerpos, n[0]*3*sizeof(double));
			memcpy((void *)mxGetPr(DATA2), (void *)layerpos, n[0]*3*sizeof(double));
		
			engPutVariable(ep,"knum",knum);
			engPutVariable(ep,"num",num);
			engPutVariable(ep,"DATA1",DATA1);
			engPutVariable(ep,"DATA1",DATA1);
		
			engEvalString(ep,"IDX=knnsearch(DATA1, DATA2, 'k', knum);");
		
			//save to file
			mxArray *outfilename = NULL;
		
			outfilename = mxCreateString(filename);
		  
			engPutVariable(ep,"outfilename",outfilename);
		
			engEvalString(ep,"outfile = fopen(outfilename,'w')");
			engEvalString(ep,"for t=1:num;fprintf(outfile,'%d ',IDX(t,:));fprintf(outfile,'\n');end");	
			engEvalString(ep,"fclose(outfile);");

			mxDestroyArray(outfilename);
			mxDestroyArray(knum);
			mxDestroyArray(num);
			mxDestroyArray(DATA1);
			mxDestroyArray(DATA2);
		}
	}
}

}