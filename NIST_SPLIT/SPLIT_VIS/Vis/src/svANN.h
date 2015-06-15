//ANN library of UMD
//Henan Zhao
//01/16/2015 04:30 p.m.
//create nearest neighbors file
//http://www.cs.umd.edu/~mount/ANN/

#ifndef __SV_ANN_H
#define __SV_ANN_H

#include "svAnalyticField.h"
#include "svException.h"
#include "svUtil.h"
#include "svArray.h"
#include "../ann_1.1.1/include/ANN/ANN.h"

#include <vector>
#include <math.h>
#include <algorithm>

namespace __svl_lib {


class svANN{
 public:
   svANN();
   ~svANN(){delete [] ctrpos;delete [] mapindex;}

   void SetData(svInt num, svVector3Array *pos,
		   int znum);
    void SetData(svInt num, svVector3 *pos);
   void ANN_montecarlo(svInt num, svVector3Array *pos,
		   int znum, float zvalue,
		int *n1, int *n2, svVector3 v);
  
   void ANN_space(char *file);//Defualt function in MATLAB
   void ANN_layer(char *file);
   void ANN_layer_multiple(char *file);
   svVector4* ANN_individual(svVector3 *pos, int num, svVector4 *IDX, bool *in, bool *all_in);
   void ANN_layer_single(int z, char *file);

   void SetNeighborNum(int n){NEIGHBOR_NUM=n;}
 protected:

 private:
   double  *ctrpos; 
   vector<int> zValue;

	svVector3 *datapos;

   svInt       dataSize;   
	svVector3 *mapindex;
	int NEIGHBOR_NUM;
};
}

#endif // __SV_ANN_H
