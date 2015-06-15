#ifndef __SV_CONTOUR_H
#define __SV_CONTOUR_H

#ifdef _VTK

#include "vtkContourFilter.h"
#include "vtkUnstructuredGridReader.h"
#include "vtkDelaunay2D.h"
#include "vtkDataSetSurfaceFilter.h"
//#include "vtkDataArray.h"
#include "vtkPointData.h"

#endif

#include "svType.h"
#include "svVectorField.h"
#include "svVectorMatrix.h"
#include "svArray.h"

#define POINTS 10

namespace __svl_lib {

class svContour
{
public:
        svContour(){datanum=0;isoPointPos=NULL;
			isoPointDir=NULL;isoPointDen=NULL;EnableAlpha = false;
			Frequency = 1;isoLayerNum=NULL;isoRatio=NULL;isoSample=NULL;isoSampleNum=10;
			ONE=false;OTHER=false;	isoZnum = NULL;
isoPointCoe=NULL;isoPointExp=NULL;}
        ~ svContour(){cleanup();}

	#ifdef _VTK
	void Get2DContour(char *infile,svScalar density);
	#endif

	void GetLineContour(char *infile, svScalar density);

        void Read2DColorFile(char *Dir, char *file1, char *file2);//read contour files using boy's surface

        void SavetoFile();
        void SavetoFile(char *file);
        void SavetosvlFile(char *file);

	void SetZlayer(int n){zlayer = n;}//NEED TO BE USED FIRST!!!!
	int GetZlayer(){return zLayer;}
	
	void SetRatio(svScalar r){isoRatio[0].add(r);}
	void SetZnum(int z){isoZnum[z].add(isoLayerNum[0].size()-1);}
	bool Addnewcontour(){if(isoLayerNum[0][isoLayerNum[0].size()-1]!=0)return true;else return false;}
	
	int getNumOfIntegerDigits(float num);

	void FindNeighbors(int selecti,int *index,int &num);
	void Find2Nei(int selecti, int &index1, int &index2,int e,int s);

        svScalar* GetDensity(){svScalar *den = new svScalar[pointnum];for(int i=0;i<pointnum;i++)den[i] = isoPointDen[0][i];return den;}
		svScalar* GetExponent(){svScalar *den = new svScalar[pointnum];for(int i=0;i<pointnum;i++)den[i] = isoPointExp[0][i];return den;}
		svScalar* GetCoefficient(){svScalar *den = new svScalar[pointnum];for(int i=0;i<pointnum;i++)den[i] = isoPointCoe[0][i];return den;}

        svVector3Array *GetPositon(){return isoPointPos;}

        svVector3Array * GetVector(){return isoPointDir;}
        svInt GetPointnum(){return pointnum;}

	int * GetSample(){int *sample = new int[isoSample[0].size()];for(int i=0;i<isoSample[0].size();i++)sample[i]=isoSample[0][i];return sample;}
	int GetSampleNum(){return isoSample[0].size();}

        svVector3 * GetVecPos(){svVector3 *pos = new svVector3[pointnum];for(int i=0;i<pointnum;i++)pos[i]=isoPointPos[0][i];return pos;}
        svVector3 * GetVecDir(){svVector3 *dir = new svVector3[pointnum];for(int i=0;i<pointnum;i++)dir[i]=isoPointDir[0][i];return dir;}

	void SetScalar(int Den){DenScalar=Den;}
	bool * GetInsidePos_Mag(svScalar minmag, svScalar maxmag);
	bool * GetInsidePos_Vel(vector<double> minangle, vector<double> maxangle, svScalar zangle1, svScalar zangle2);
	svScalar GetAngle(svVector3 p);

        void SavePointtoFile(char *file);
	void NewContours();
        int ChooseArrow(svVector3 a);//choose an arrow, and highlight it.

	void PrintData(char *file);

        void cleanup();
	int Frequency;

	bool ONE;
	bool OTHER;

	int NUM_SEG;
	float ROI_ARROW_BASE;
	float ROI_ALPHA;
	float NROI_ARROW_BASE;
	float NROI_ALPHA;	

   vector<vector<double> >* GetSortVectorPos(){return sortP;}
   vector<vector<double> >* GetSortVectorVel(){return sortV;}
   vector<vector<double> > GetSortVectorDen(){return sortD;}
   double *GetDoubleDensity(){double *d = new double[pointnum];for(int i=0;i<pointnum;i++)d[i]=isoPointDen[0][i];return d;}
protected:

private:

        svScalar datanum;

        svBool *if_triangle;//not used so far

        svVector3Array *isoPointPos;
        svVector3Array *isoPointDir;
        svScalarArray  *isoPointDen;
	svScalarArray *isoPointExp;
	svScalarArray *isoPointCoe;

   vector<vector<double> > sortP[3];
   vector<vector<double> > sortV[3];
   vector<vector<double> > sortD;

        svInt *isoPointLable;
	svIntArray *isoLayerNum;
	svIntArray *isoZnum;
	svScalarArray *isoRatio;
	svIntArray *isoSample;

        svInt pointnum;
        svBool EnableAlpha;
	svInt isoSampleNum;


	int zlayer;
	int zLayer;//actually existing z layers
	
	int encode_mode;
	int DenScalar;
	
	Scalar maxden;
};

}
#endif
