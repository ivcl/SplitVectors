#include "./Eigen/Dense"
#include <iostream>
#include <cmath>
#include <math.h>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h> 

using namespace Eigen;
using namespace std;

namespace __em_lib {
struct gaussian_model{
	MatrixXf mu;
	MatrixXf *sigma;
	VectorXf weight;
};

class EMclustering{

public:
	EMclustering(){dataSize = 0;}
	~EMclustering(){cleanup();}

	void SetData(vector<vector<double> > pos, vector<double> den, vector<vector<double> > vel, int num,
		int dennum);

	VectorXf logsumexp(MatrixXf x, int dim);
	VectorXf loggausspdf(MatrixXf x, VectorXf mu, MatrixXf sigma);	
	gaussian_model maximization(MatrixXf x, MatrixXf r);
	MatrixXf expectation(MatrixXf x, gaussian_model model, double &llh);
	MatrixXf initialization(MatrixXf x, int k);
	VectorXi unique(VectorXi l,VectorXi &l2);
	int EM(int k, int *IDX, bool spatial, bool att);

	void getmu(double &mu, int k);
	void getmu(double &m1, double &m2, double &m3, int k);
protected:
	void cleanup();
private:
	vector<vector<double> > dataPos;
	vector<double> dataDen;
	vector<vector<double> > dataVel;
	int dataSize;	

	int clusternum;
	MatrixXf return_model;
};

}
