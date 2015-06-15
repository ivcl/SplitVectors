
#ifndef __SV_QDOT_NORM_H
#define __SV_QDOT_NORM_H

#include "svAnalyticField.h"
#include "svException.h"
#include "svUtil.h"
#include "svArray.h"

#include "QDOTdataNorm.h"
#include "readdata.h"
#include <vector>
#include <math.h>

#define MAX_NUMBER_OF_CRITICAL_POINTS  10

namespace __svl_lib {

class svQDOTNorm : public svAnalyticField {
 public:
   svQDOTNorm();
   virtual ~svQDOTNorm();

   void New(char *indir, char *infname); // for readdata.cpp
   void New(char *indir, char *infname, int seedNumber);
   void New(char *indir, char *infname, float contourvalue);

   int getNumOfIntegerDigits(svScalar num);
   
   void SortData();
   void SetSymmetry();
   
   //slow version-------------
   void StoreNeighbours_space(char *infile);
   void StoreNeighbours_layer(char *infile);
   //----------------

   virtual svVector3 GetVector(const svParticle & p);
   virtual svVector3 GetVector(const svVector3& pos, svScalar t);
   virtual svVector3 GetCenter()
   {
     return svVector3((lbbox+rbbox)*0.5);
   }
   virtual void GetPhysicalDimension(svScalar *x, 
                        svScalar *y, svScalar *z)
   {
     *x = rbbox[0]-lbbox[0];
     *y = rbbox[1]-lbbox[1];
     *z = rbbox[2]-lbbox[2];
   }

   bool *GetInsidePos_Mag(svScalar minmag, svScalar maxmag);
   svScalar GetAngle(svVector3 p);
   bool *GetInsidePos_Vel(vector<double> minangle, vector<double> maxangle, svScalar zangle1, svScalar zangle2);

   svVector3 * GetPosition() const {return ctrpos;}
   svVector3 * GetVelocity() const {return ctrvel;}
   svScalar  * GetDensity()  const {return ctrden;}
   svScalar  GetBoxDensity(svVector3 l, svVector3 r); //by Henan
   svFloat   * GetHisto() const {return histoden;}
   svInt  GetDataSize() const {return dataSize;}
   svInt GetHistoDataSize() const {return histodatasize;}
   svScalar  GetDenMin(int i)const{return denMin[i];}
   svScalar  GetDenMax(int i) const {return denMax[i];}
   svScalar GetExpMax() const {return expMax;}
   svFloat GetDenRatio(svScalar den, svFloat z);
   int GetLayer(float n){return (n-sortpos[0][2])/zdistance;}
   
   svVector3Array *GetSplitPos(){return zsplitpos;}
   svVector3Array GetSplitPos(int n){return zsplitpos[n];}
   svVector3Array *GetSplitVel(){return zsplitvel;}
   svScalarArray *GetSplitDen(){return zsplitden;}
   svScalarArray *GetSplitExp(){return zsplitexp;}
   svScalarArray *GetSplitCoe(){return zsplitcoe;}
   svIntArray *GetSplitNei1(){return zsplit_neighbor1;}
   svIntArray *GetSplitNei2(){return zsplit_neighbor2;}
   
   void PlaneNeighbors(float z, int x);
   int Interpolation_neighbor(float z, int x, svVector3 p, svVector3 v);
   //svScalar  GetPointDen(svVector3 xx);//wrong by Henan
   svVector3 Interpolation(svScalar &den, svScalar &exp, svScalar &coe, svVector3 xx);//wrong by Henan

   svScalar GetInterDen(svVector3 p);
   void Savetovtk(char *file, svVector3 *p,int num);
   void Savetofile(char *file, svVector3 *p,int num);

   svVector3 *GetXYplane(){return xyplane;}
   svVector3 *GetXZplane(){return xzplane;}
   svVector3 *GetYZplane(){return yzplane;}
   int GetXYsample(){return xysample;}  
   int GetYZsample(){return yzsample;}  
   int GetXZsample(){return xzsample;} 
   float GetMaxZ(){return sortpos[sortpos.size()-1][2];}

   svVector3 *GetSortPos(){return sortPos;}
   svVector3 *GetSortVel(){return sortVel;}
   svVector3 *GetSortVel2(){return sortVel2;}
   svScalar *GetSortDen(){return sortDen;}
   vector<vector<double> >* GetSortVectorPos(){return sortP;}
   vector<vector<double> >* GetSortVectorVel(){return sortV;}
   vector<vector<double> > GetSortVectorDen(){return sortD;}

	svScalar GetMaxDen(){return max_density/pow(10.,(double)DEN_SCALAR);}
	svScalar GetMinDen(){return min_density/pow(10.,(double)DEN_SCALAR);}
	svScalar GetMinDen_Nonzero(){return min_density_nonzero/pow(10.,(double)DEN_SCALAR);}

   int GetZlayer(){return zlayernum;} 
   svVector3 GetMinDistance(){return mindistance;}

	int GetScalar(){return DEN_SCALAR;}

	double GetScalarPow(){double tmppow=-1;for(int iii=0;iii<DEN_SCALAR;iii++)tmppow = tmppow * 10.; return tmppow;}


 protected:
   void findNN(float pos, float nn[2], float *weightToTheFirst);
   void setProfileFromData(char *indir, char *infname, int seedNum);
   void setProfileFromData(char *indir, char *infname, float contourvalue);
   void setProfileFromData(char *indir, char *infname);
   void cleanUp();

 private:
   //QDOTdataNorm *myData;
   svVector3   *ctrpos; 
   svVector3   *ctrvel; 
   svScalar    *ctrden; 
   svScalar	   *ctrexp;
   svScalar    *ctrcoe;
   svInt       dataSize; // size of the data to be drawn
   
   svVector3Array *zsplitpos;
   svVector3Array *zsplitvel;
   svScalarArray *zsplitden;
   svScalarArray *zsplitexp;
   svScalarArray *zsplitcoe;
   svIntArray *zsplit_neighbor1;//selected neighbor for comparison of planes
   svIntArray *zsplit_neighbor2;
   
   //for symmetry
   svVector3 *sortPos;
   svScalar *sortDen;
   svVector3 *sortVel;//after symmetry process
   svVector3 *sortVel2;//original

   vector<vector<double> > sortP[3];
   vector<vector<double> > sortV[3];
   vector<vector<double> > sortD;

   svInt histodatasize;
   svFloat *histoden;
   svFloat *histoROIden;

   svScalar 	*denMin;
   svScalar	*denMax;
   svScalar 	expMax;
   svScalar min_density;
   svScalar max_density;
   svScalar min_density_nonzero;

   vector<svVector3> sortpos;
   vector<svVector3> sortvel;
   vector<int> sortindex;
   float zdistance;

   vector<int> zindex;
   vector<bool> zformat;
   vector<int> yindex;
   vector<int> xindex;
   vector<int> num_z[2];//index in each z

   vector<float> ydistance;
   vector<float> xdistance;

   vector<int> yinz;
   vector<int> xinz;
   vector<int> xiny;
   
   int zlayernum; 
   svVector3 *xyplane;
   int xysample;
   svVector3 *yzplane;
   int yzsample;
   svVector3 *xzplane;
   int xzsample;

   svVector3 interpolationv;
   
   int DEN_SCALAR;//scale density by times the DEN_SCALAR
   svVector3 mindistance;//min of distances. [0]:x, [1]:y, [2]:z
   int zValue; //# of different z values
   int sample_index[2];//provide a sample to show position relations between every point and corresponding neighbors.
};
}
#endif // __SV_QDOT_NORM_H
