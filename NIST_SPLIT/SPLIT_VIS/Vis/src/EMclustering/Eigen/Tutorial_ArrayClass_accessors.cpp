#include "Dense"
#include <iostream>
#include <cmath>
#include <math.h>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h> 

using namespace Eigen;
using namespace std;
#define PI 3.1415926

struct gaussian_model{
	MatrixXf mu;
	MatrixXf *sigma;
	VectorXf weight;
};
int ck;




VectorXf logsumexp(MatrixXf x, int dim)
{
	int r = x.rows();
	int c = x.cols();

	VectorXf y(r);
	MatrixXf tmp1(r,c);
	VectorXf tmp2(r);
	VectorXf s(r);

	y = x.rowwise().maxCoeff();cerr<<"y"<<y<<endl<<endl;
	x = x.colwise() - y;	
	cerr<<"x"<<x<<endl<<endl;
	tmp1 = x.array().exp();	
	cerr<<"t"<<tmp1<<endl<<endl;
	tmp2 = tmp1.rowwise().sum();	
	cerr<<"t"<<tmp2<<endl<<endl;
	s = y.array() + tmp2.array().log();

	for(int i=0;i<s.size();i++)
	{
		if(!isfinite(s(i)))
		{
			s(i) = y(i);
		}
	}

	y.resize(0);
	tmp1.resize(0,0);
	tmp2.resize(0);
	
	return s;
}

VectorXf loggausspdf(MatrixXf x, VectorXf mu, MatrixXf sigma)
{
	//cerr<<x<<endl<<endl;
	//cerr<<mu<<endl<<endl;
	//cerr<<sigma<<endl<<endl;

	int d = x.rows();
	int c = x.cols();
	int r_sigma = sigma.rows();
	int c_sigma = sigma.cols();

	MatrixXf tmpx = x.colwise() - mu;

	MatrixXf u1 = sigma.llt().matrixL() ;
	MatrixXf u2 = u1.adjoint();cerr<<u2<<endl;
	
	MatrixXf Q = u1.jacobiSvd(ComputeThinU | ComputeThinV).solve(tmpx);
	cerr<<"q"<<Q<<endl;
	VectorXf q = Q.cwiseProduct(Q).colwise().sum();//cerr<<"q"<<q<<endl;
	VectorXf tmp1 = u2.diagonal();
	tmp1 = tmp1.array().log();
	double c1 = tmp1.sum() * 2;
	double c2 = d * log(2*PI);//cerr<<c1+c2<<endl;
	
	VectorXf y = -(c1+c2)/2. - q.array()/2.;
	
	tmpx.resize(0,0);
	u1.resize(0,0);
	u2.resize(0,0);
	Q.resize(0,0);
	q.resize(0);
	tmp1.resize(0);

	return y;
}

gaussian_model maximization(MatrixXf x, MatrixXf r)
{
	int d = x.rows();
	int n = x.cols();
	int k = r.cols();
	//cerr<<x<<endl;
	VectorXf nk = r.colwise().sum();
	VectorXf w = nk/n;
	MatrixXf tmp1 = x * r;
	VectorXf tmp2 = nk.array().inverse();
	//cerr<<tmp2<<endl<<endl;
	MatrixXf mu = tmp1 * tmp2.asDiagonal() ;
	MatrixXf *sigma = new MatrixXf[k];
	for(int i=0;i<k;i++)
		sigma[i].setZero(d,d);
	MatrixXf sqrtr = r.cwiseSqrt();
	MatrixXf xo(d,n);
	MatrixXf tmp3;
	tmp3.setIdentity(d,d);
	
	for(int i=0;i<k;i++)
	{
		xo = x.colwise() - mu.col(i);
		VectorXf tmp4 = sqrtr.col(i);
		tmp4 = tmp4.adjoint();
		xo = xo* tmp4.asDiagonal();
		sigma[i] = xo*xo.adjoint()/nk(i);
		sigma[i] = sigma[i] + tmp3*1e-6; 
		//cerr<<sigma[i]<<endl<<endl;
	}

	gaussian_model model;
	model.mu = mu;
	model.sigma = new MatrixXf[k];
	for(int i=0;i<k;i++)
		model.sigma[i] = sigma[i];
	
	model.weight = w;
	
	nk.resize(0);
	w.resize(0);
	tmp1.resize(0,0);
	tmp2.resize(0);
	tmp3.resize(0,0);
	mu.resize(0,0);
	for(int i=0;i<k;i++)
		sigma[i].resize(0,0);
	delete [] sigma;
	sqrtr.resize(0,0);
	xo.resize(0,0);
	tmp3.resize(0,0);

	ck = k;//;;;;;;;;;
	cerr<<"---"<<endl;
	model.weight = model.weight.adjoint();
	cerr<<model.weight<<endl<<endl;
	cerr<<model.mu<<endl<<endl;
	for(int i=0;i<k;i++)
	{
		cerr<<model.sigma[i]<<endl<<endl;
	}
	return model;
}

MatrixXf expectation(MatrixXf x, gaussian_model model, double &llh)
{
	cerr<<"===="<<endl;
	MatrixXf mu = model.mu;
	MatrixXf *sigma = new MatrixXf[ck];
	sigma =  model.sigma;
	VectorXf w = model.weight;

	cerr<<mu<<endl;
	cerr<<w<<endl<<endl;
	for(int i=0;i<ck;i++)cerr<<sigma[i]<<endl;
	cerr<<endl;

	int n = x.cols();
	int k = mu.cols();
	MatrixXf logrho(n,k);
	logrho.setZero(n,k);
	//cerr<<logrho<<endl;
	
cerr<<logrho<<endl<<endl;
	for(int i=0;i<k;i++)
	{
		//cerr<<i<<endl;
		logrho.col(i) = loggausspdf(x,mu.col(i),sigma[i]);
		//cerr<<mu.col(i)<<endl;
	}

	cerr<<logrho<<endl<<endl;
	
	w = w.array().log();//cerr<<w<<endl<<endl;
	MatrixXf tmp1;
	tmp1 = logrho.rowwise() + w.adjoint();
	logrho = tmp1;cerr<<logrho<<endl<<endl;
	VectorXf t = logsumexp(logrho,2);cerr<<t<<endl<<endl;
	llh = t.sum()/n;cerr<<llh<<endl<<endl;
	MatrixXf logr = logrho.colwise() - t;cerr<<logr<<endl<<endl;
	MatrixXf r = logr.array().exp();cerr<<r<<endl<<endl;

	logrho.resize(0,0);
	mu.resize(0,0);
	w.resize(0);
	for(int i=0;i<ck;i++)//..................
		sigma[i].resize(0,0);
	delete [] sigma;
	
	cerr<<r<<endl<<endl;
	cerr<<llh<<endl;

	return r;
}
vector<int> unique_u;
vector<int> unique_index;
VectorXi unique(VectorXi l, VectorXi &l2)
{
	unique_u.clear();
	unique_index.clear();
	int d = l.size();

	unique_u.push_back(l(0));
	bool flag;
	for(int i=1;i<d;i++)
	{
		int size = unique_u.size();
		flag = true;
		for(int j=0;j<size;j++)
		{
			if(l(i)==unique_u[j])
			{
				flag = false;
				break;
			}
		}
		if(flag)
		{
			unique_u.push_back(l(i));
			//cerr<<l(i)<<endl;
		}
	}//cerr<<unique_u.size()<<" "<<unique_index.size()<<endl;

	for(int i=0;i<d;i++)
	{
		for(int j=0;j<unique_u.size();j++)
		{
			if(l(i)==unique_u[j])
				{unique_index.push_back(j);}//cerr<<unique_u.size()<<" "<<unique_index.size()<<endl;}
		}
	}

	/*sort(unique_index.begin(),unique_index.end(),myfunction);
	sort(unique_u.begin(),unique_u.end());*/
	VectorXi u(unique_u.size());//cerr<<unique_u.size()<<" "<<unique_index.size()<<endl;

	for(int i=0;i<unique_u.size();i++)
	{
		u(i) = unique_u[i];
	}//cerr<<unique_u.size()<<" "<<unique_index.size()<<endl;
	for(int i=0;i<unique_index.size();i++)
	{
		l2(i) = unique_index[i];//cerr<<unique_u.size()<<" "<<unique_index.size()<<endl;
	}
//cerr<<l2<<endl;
	return u;
}
MatrixXf initialization(MatrixXf x, int k)
{
  srand (time(NULL));

	int n = x.cols();
	int d = x.rows();
	//cerr<<n<<"d "<<d<<endl;

	VectorXi idx(k);
	//idx.setRandom(k);
	//cerr<<idx<<endl;
	idx(0) = rand()%n;
	for(int i=1;i<k;i++)
	{
		int random = rand()%n;
		cerr<<random<<endl;
		bool flag2 = false;
		while(!flag2)
		{
			random = rand()%n;
			flag2 = true;
			for(int j=0;j<i;j++)
			{
				if(idx(j)==random)
				{	
				
					flag2 = false;
					break;
				}
			}
		}
		idx(i) = random;
	}
	cerr<<"idx"<<idx<<endl<<endl;

	int tmpsize = idx.size();
	MatrixXf m(d,tmpsize);
	for(int i=0;i<tmpsize;i++)
		m.col(i) = x.col(idx(i));
	//cerr<<m<<endl<<endl;

	VectorXf tmp1 = m.cwiseProduct(m).colwise().sum();
	//cerr<<tmp1<<endl<<endl;
	tmp1 = tmp1.adjoint()/2;
	//cerr<<tmp1<<endl<<endl;
	MatrixXf tmp2 = m.adjoint() * x;
	//cerr<<tmp2<<endl<<endl;
	MatrixXf tmp3 = tmp2.colwise() - tmp1;
	//cerr<<tmp3<<endl<<endl;
	double tmp4;
	int tmp5;
	int tmpd = tmp3.cols();
	VectorXi label(tmpd);
	for(int i=0;i<tmpd;i++)
	{
		tmp4 = tmp3.col(i).maxCoeff(&tmp5);
		label(i) = tmp5;
	}
	//cerr<<label<<endl<<endl;

	VectorXi tmp6(label.size());
	VectorXi u = unique(label, tmp6);
	cerr<<u<<endl<<endl;
	//cerr<<tmp6<<endl<<endl;

	
	while(k!=u.size())
	{
		cerr<<"-----------"<<endl;
		cerr<<k<<" "<<u.size()<<endl;
		//idx.setRandom(k);
		idx(0) = rand()%n;
		for(int i=1;i<k;i++)
		{
			int random = rand()%n;
			bool flag2 = false;
			while(!flag2)
			{random = rand()%n;
				flag2 = true;
				for(int j=0;j<i;j++)
				{
					if(idx(j)==random)
					{
						flag2 = false;
						break;
					}
				}
			}
			idx(i) = random;
		}
		cerr<<"idx"<<idx<<endl<<endl;;

		for(int i=0;i<tmpsize;i++)
			m.col(i) = x.col(idx(i));
		//cerr<<m<<endl<<endl;

		tmp1 = m.cwiseProduct(m).colwise().sum();
		//cerr<<tmp1<<endl<<endl;
		tmp1 = tmp1.adjoint()/2;
		//cerr<<tmp1<<endl<<endl;
		tmp2 = m.adjoint() * x;
		//cerr<<tmp2<<endl<<endl;
		tmp3 = tmp2.colwise() - tmp1;
		//cerr<<tmp3<<endl<<endl;
		tmpd = tmp3.rows();
		for(int i=0;i<tmpd;i++)
		{
			tmp4 = tmp3.row(i).maxCoeff(&tmp5);
			label(i) = tmp5;
		}
		cerr<<"label"<<label<<endl<<endl;
		cerr<<"end"<<endl;
		u = unique(label, tmp6);
		label = tmp6;
		cerr<<"u"<<u<<endl<<endl;
		//cerr<<tmp6<<endl<<endl;
	}cerr<<k<<" "<<u.size()<<endl;

	MatrixXf r(n,k);
	r.setZero(n,k);
	int j=0;
	for(int i=0;i<n;i++)
	{
		r(i,label(j)) = 1;
		j++;
	}

	cerr<<r<<endl;

	return r;
}
void EM(int k, int *IDX, MatrixXf x)
{
	cout<<"EM for Gaussian mixture: running ... "<<endl;
	MatrixXf r = initialization(x,k);
	VectorXi label(r.rows());
	for(int i=0;i<r.rows();i++)
	{
		int index;
		float tmp1 = r.row(i).maxCoeff(&index);
		label(i) = index;
	}cerr<<label<<endl;
	VectorXi tmpp(label.size()); 
	VectorXi tmp2 = unique(label,tmpp);
	int tmpd = tmp2.size();  cerr<<tmpd<<endl;
	MatrixXf tmpr(r.rows(),tmpd);
	for(int i=0;i<tmpd;i++)
	{
		tmpr.col(i) = r.col(tmp2(i));
	}cerr<<"done1"<<endl;
	r.resize(r.rows(),tmpd);
	r = tmpr;
	float tol = 1e-10;
	int max = 500;
	double llh = -9e+9;
	bool converged = false;
	int t = 1;
	cerr<<"done1"<<endl;
	while(!converged&&t<max)
	{
		t = t + 1;
		gaussian_model model = maximization(x,r);
		float tmpllh = llh;
		r = expectation(x,model,llh);
		VectorXi u = unique(label,tmpp);
		if(r.cols()!=u.size())
		{
			tmpr.resize(r.rows(),u.size());
			for(int i=0;i<u.size();i++)
			{
				tmpr.col(i) = r.col(tmp2(i));
			}
			r.resize(r.rows(),u.size());
			r = tmpr;
		}
		else
		{
			if((llh - tmpllh)<tol*abs(llh))
				converged = true;
			else
				converged = false;
		}
		cerr<<"t"<<t<<endl;
	}
	cerr<<label<<endl;
	/*if (converged)
    		cerr<<"Converged in %d steps.\n",t-1);
	else
    		printf("Not converged in %d steps.\n",max);*/
	cerr<<t-1<<endl;
	
}

int main()
{
   Matrix3f A;
   A << 1, 2, 1,
        2, 1, 0,
        -1, 1, 2;
   cout << "Here is the matrix A:\n" << A << endl;
   cout << "The determinant of A is " << A.determinant() << endl;
   cout << "The inverse of A is:\n" << A.inverse() << endl;

MatrixXf x(5,5);

/*	int r = x.rows();
	int c = x.cols();

	cerr<<r<<" "<<c<<endl;

	VectorXf y(r);
	MatrixXf tmp1(r,c);
	VectorXf tmp2(r);
	VectorXf s(r);

	y = x.rowwise().maxCoeff();
	x = x.colwise() - y;	
	tmp1 = x.array().exp();	
	tmp2 = tmp1.rowwise().sum();	
	s = y.array() + tmp2.array().log();

	for(int i=0;i<s.size();i++)
	{
		if(!isfinite(s(i)))
		{
			s(i) = y(i);
		}
	}

	y.resize(0);
	tmp1.resize(0,0);
	tmp2.resize(0);
	
	cerr<<s<<endl;*/

	x <<

     1     ,2,     3, 0.4,10,
     7  ,   5    , 6, 1,2,
     10   ,  3   ,  4, 11,7,
     2    , 4  ,   4, 19, 3,
     2     ,3 ,    4, 27,4;
	cerr<<x<<endl;
/*
MatrixXf sigma(5,5);
sigma<<
     1   , -1   , -1  ,  -1   , -1,
    -1    , 2   ,  0  ,   0   ,  0,
    -1   ,  0   ,  3   ,  1   ,  1,
    -1   ,  0   ,  1   ,  4   ,  2,
    -1   ,  0   ,  1   ,  2   ,  5;
VectorXf mu(5);
mu = x.col(1);
//mu<<0.1000  ,  0.4000    ,0.3400,0.5,0.3 ;
	int d = x.rows();
	cerr<<d<<endl;
	int c = x.cols();
	int r_sigma = sigma.rows();
	int c_sigma = sigma.cols();

	MatrixXf tmpx = x.colwise() - mu;
	cerr<<tmpx<<endl;

	MatrixXf u1 = sigma.llt().matrixL() ;
	cerr<<u1<<endl;
	MatrixXf u2 = u1.transpose();
	cerr<<u2<<endl;
	

	MatrixXf Q = u1.jacobiSvd(ComputeThinU | ComputeThinV).solve(tmpx);
	cerr<<Q<<endl;
	VectorXf q = Q.cwiseProduct(Q).colwise().sum();
	cerr<<q<<endl;
	VectorXf tmp1 = u2.diagonal();
	tmp1 = tmp1.array().log();
	double c1 = tmp1.sum() * 2;
	double c2 = d * log(2*PI);
	cerr<<c1+c2<<endl;
	
	VectorXf y = -(c1+c2)/2. - q.array()/2.;
	cerr<<y<<endl;*/

	MatrixXf r(x.cols(),2);
	r<<0,     1  ,
     0   ,  1 ,
     0   ,  1,
     0, 1,
	1,0;
	/*cerr<<r<<endl;
	double llh;

	MatrixXf tmp = initialization(x,2);
	cerr<<"---"<<endl;
	gaussian_model model;
	model = maximization(x,r);
	cerr<<"***"<<endl;
	cerr<<model.sigma[0]<<endl;
	expectation(x,model,llh);
	cerr<<"1"<<endl;*/
	EM(2,NULL,x);
	
}

