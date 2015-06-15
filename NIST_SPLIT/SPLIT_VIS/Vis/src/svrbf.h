#ifndef __SV_RBF_H
#define __SV_RBF_H

#include "svAnalyticField.h"
#include <math.h>
#include "svException.h"
#include "svUtil.h"
#include "svArray.h"

#define MAX_NUMBER_OF_CRITICAL_POINTS  10

namespace __svl_lib {
class svrbf : public svAnalyticField {
 public:
   svrbf();
   ~svrbf();

   void New(int numvec, 
         svVector3 llbox, svVector3 rbbox,
         svVector3 *pos, 
	 svFloat *posxvalue, svFloat *posyvalue, svFloat *poszvalue,
	 svScalar e, svScalar max_time=0.f);
   // pos: the order shall be x1,y1,z1, x2,y2,z2, x3,y3,z3, ... 
   // posvalue: the order shall be p1,p2,p3...
   void New(char *infname, svVector3 lb, svVector3 rb);
   void New(char *infname);

   void    distanceMatrix0();

   virtual svVector3 GetVector(const svParticle & p);
   virtual svVector3 GetVector(const svVector3& pos, svScalar t);
   virtual void  jacobianMatrix(double x, double y, double z,
                                svMatrix3 &jac);

   void    CalculateRoughStrength(int num_seeds);

   void    gen_rbf(double x,double y,double z,
                   double *vx,double *vy,double *vz);

   //rbf functions
   svFloat    rbfgaussian(svFloat dis);
   void       rbfgaussian2();

   svFloat    rbfgaussianDer(svFloat dis, svFloat dx);
   void       rbfgaussianDer2(svFloat *dis, svFloat *dx, svFloat *EMDer);

   void setEvaluationMatrix(double x, double y, double z);

   void    distanceMatrix(double x, double y, double z/*, svFloat *dis*/);
   // the distances between the evaluation point and the control points
   void    distanceMatrixOneDir(double p, int index, svFloat *ddis);
   // the distance array along one direction

 protected:
   svFloat dot(svFloat *a, svFloat *b, int length)
   {
     svFloat sum=0.;
     for(int i=0; i<length; i++)
       sum += a[i]*b[i];
     return sum;
   };

 private:
   svScalar    epIn;    // power
   svInt       nvIn;    // num of random vectors
   svVector3   *ctrpos; // control points
   svFloat     *ctrxval, *ctryval, *ctrzval; // value at the control points
   svFloat     * wx, *wy, *wz;      // weight

   svFloat     *dis, *EM;
};
}
#endif // __SV_RBF_H
