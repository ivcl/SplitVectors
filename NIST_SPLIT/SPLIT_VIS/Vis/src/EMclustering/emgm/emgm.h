#include "Dense"
#include <iostream>
#include <cmath>
#include <math.h>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h> 

using namespace Eigen;
using namespace std;

struct gaussian_model{
	MatrixXf mu;
	MatrixXXf *sigma;
	VectorXf weight;
};

class EMclustering{

public:
	EMclustering(){dataPos=NULL;dataDen=NULL;dataVel=NULL;dataSize=0;}
	~EMclustering(){cleanup();}

	void SetData(svVector3 *pos, svVector3 *den, svVector3 *vel, int num);

	VectorXf logsumexp(MatrixXf x, int dim);
	VectorXf loggausspdf(MatrixXf x, VectorXf mu, MatrixXf sigma);	
	gaussian_model maximization(MatrixXf x, MatrixXf r);
	MatrixXf expectation(MatrixXf x, gaussian_model model, double &llh);
	MatrixXf initialization(MatrixXf x, int k);
	VectorXd unique(VectorXd l,VectorXd &l2);

	MatrixXf initialization(MatrixXf x, int k);
	void EM(int k, int *IDX, bool spatial, bool att);
protected:
	void cleanup();
private:
	svVector3 *dataPos;
	svVector3 *dataDen;
	svVector3 *dataVel;
	svInt dataSize;	

	int clusternum;

};

