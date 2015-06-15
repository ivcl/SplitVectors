//Default knn function in Matlab
//Henan Zhao
//01/15/2015 10:30 a.m.
//create nearest neighbors file

#ifndef __SV_KNN_H
#define __SV_KNN_H

#include "svAnalyticField.h"
#include "svException.h"
#include "svUtil.h"
#include "svArray.h"
#include "engine.h" //matlab

#include <vector>
#include <math.h>

namespace __svl_lib {

class svKNN{
 public:
   svKNN();
   ~svKNN(){delete [] ctrpos;}

   void SetData(svInt num, svVector3Array *pos,
		   int znum);
   
   void KNN_space(char *file);//Defualt function in MATLAB
   void KNN_layer(char *file);
 protected:

 private:
   double  *ctrpos; 
   vector<int> zValue;

   svInt       dataSize;   
};
}
#endif // __SV_KNN_H
