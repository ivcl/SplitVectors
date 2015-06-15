
#define PI 3.1415926

#include "emgm.h"

void EMclustering::SetData(svVector3 *pos, svVector3 *den, svVector3 *vel, int num)
{
	if(dataSize!=0)
		cleanup();

	dataSize = num;

	dataPos = new svVector3[num];
	dataDen = new svVector3[num];
	dataVel = new svVector3[num];

	for(int i=0;i<num;i++)
	{
		dataPos[i] = pos[i];
		dataDen[i] = den[i];
		dataVel[i] = vel[i];
	}
}

VectorXf EMclustering::logsumexp(MatrixXf x, int dim)
{
	int r = x.rows();
	int c = x.cols();

	VectorXf y(r);
	MatrixXf tmp1(r,c);
	VectorXf tmp2(r);
	VectorXf s(r);

	y = x.rowwise().maxCoeff();//cerr<<"y"<<y<<endl<<endl;
	x = x.colwise() - y;	
	//cerr<<"x"<<x<<endl<<endl;
	tmp1 = x.array().exp();	
	//cerr<<"t"<<tmp1<<endl<<endl;
	tmp2 = tmp1.rowwise().sum();	
	//cerr<<"t"<<tmp2<<endl<<endl;
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

VectorXf EMclustering::loggausspdf(MatrixXf x, VectorXf mu, MatrixXf sigma)
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
	MatrixXf u2 = u1.adjoint();//cerr<<u2<<endl;
	
	MatrixXf Q = u1.jacobiSvd(ComputeThinU | ComputeThinV).solve(tmpx);
	//cerr<<"q"<<Q<<endl;
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

gaussian_model EMclustering::maximization(MatrixXf x, MatrixXf r)
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
	//cerr<<"---"<<endl;
	model.weight = model.weight.adjoint();
	//cerr<<model.weight<<endl<<endl;
	//cerr<<model.mu<<endl<<endl;
	//for(int i=0;i<k;i++)
	//{
	//	cerr<<model.sigma[i]<<endl<<endl;
	//}
	return model;
}

MatrixXf EMclustering::expectation(MatrixXf x, gaussian_model model, double &llh)
{
	//cerr<<"===="<<endl;
	MatrixXf mu = model.mu;
	MatrixXf *sigma = new MatrixXf[ck];
	sigma =  model.sigma;
	VectorXf w = model.weight;

	//cerr<<mu<<endl;
	//cerr<<w<<endl<<endl;
	//for(int i=0;i<ck;i++)cerr<<sigma[i]<<endl;
	//cerr<<endl;

	int n = x.cols();
	int k = mu.cols();
	MatrixXf logrho(n,k);
	logrho.setZero(n,k);
	//cerr<<logrho<<endl;
	
//cerr<<logrho<<endl<<endl;
	for(int i=0;i<k;i++)
	{
		//cerr<<i<<endl;
		logrho.col(i) = loggausspdf(x,mu.col(i),sigma[i]);
		//cerr<<mu.col(i)<<endl;
	}

	//cerr<<logrho<<endl<<endl;
	
	w = w.array().log();//cerr<<w<<endl<<endl;
	MatrixXf tmp1;
	tmp1 = logrho.rowwise() + w.adjoint();
	logrho = tmp1;//cerr<<logrho<<endl<<endl;
	VectorXf t = logsumexp(logrho,2);//cerr<<t<<endl<<endl;
	llh = t.sum()/n;cerr<<llh<<endl<<endl;
	MatrixXf logr = logrho.colwise() - t;//cerr<<logr<<endl<<endl;
	MatrixXf r = logr.array().exp();//cerr<<r<<endl<<endl;

	logrho.resize(0,0);
	mu.resize(0,0);
	w.resize(0);
	for(int i=0;i<ck;i++)//..................
		sigma[i].resize(0,0);
	delete [] sigma;
	
	//cerr<<r<<endl<<endl;
	//cerr<<llh<<endl;

	return r;
}

bool myfunction(int a, int b){return unique_u[a]<unique_b[b];}
bool myfunction2(int a,int b){return a<b;}
vector<int> unique_u;
vector<int> unique_index;

VectorXi EMclustering::unique(VectorXi l, VectorXi &l2)
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
	return u;
}

MatrixXf EMclustering::initialization(MatrixXf x, int k)
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
		//cerr<<random<<endl;
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
	//cerr<<"idx"<<idx<<endl<<endl;

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
	//cerr<<u<<endl<<endl;
	//cerr<<tmp6<<endl<<endl;

	
	while(k!=u.size())
	{
		//cerr<<"-----------"<<endl;
		//cerr<<k<<" "<<u.size()<<endl;
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
		//cerr<<"idx"<<idx<<endl<<endl;;

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
		//cerr<<"label"<<label<<endl<<endl;
		//cerr<<"end"<<endl;
		u = unique(label, tmp6);
		label = tmp6;
		//cerr<<"u"<<u<<endl<<endl;
		//cerr<<tmp6<<endl<<endl;
	}//cerr<<k<<" "<<u.size()<<endl;

	MatrixXf r(n,k);
	r.setZero(n,k);
	int j=0;
	for(int i=0;i<n;i++)
	{
		r(i,label(j)) = 1;
		j++;
	}

	//cerr<<r<<endl;

	return r;
}

void EMclustering::EM(int k, int *IDX, bool spatial, bool att)
{
	if(spatial)
	{
		if(att)
		{
			MatrixXf x(4,dataSize);
			for(int i=0;i<dataSize;i++)
			{
				x(0,i) = dataPos[i][0];
				x(1,i) = dataPos[i][1];
				x(2,i) = dataPos[i][2];
				x(3,i) = dataDen[i];
			}
		}
		else
		{
			MatrixXf x(6,dataSize);
			for(int i=0;i<dataSize;i++)
			{
				x(0,i) = dataPos[i][0];
				x(1,i) = dataPos[i][1];
				x(2,i) = dataPos[i][2];
				x(3,i) = dataVel[i][0];
				x(4,i) = dataVel[i][1];
				x(5,i) = dataVel[i][2];
			}
		}
	}
	else
	{
		if(att)
		{
			MatrixXf x(1,dataSize);
			for(int i=0;i<dataSize;i++)
			{
				x(0,i) = dataDen[i];
			}
		}
		else
		{
			MatrixXf x(3,dataSize);
			for(int i=0;i<dataSize;i++)
			{
				x(0,i) = dataVel[i][0];
				x(1,i) = dataVel[i][1];
				x(2,i) = dataVel[i][2];
			}
		}
	}

	cout<<"EM for Gaussian mixture: running ... "<<endl;
	MatrixXf r = initialization(x,k);
	VectorXi label(r.rows());
	for(int i=0;i<r.rows();i++)
	{
		int index;
		float tmp1 = r.row(i).maxCoeff(&index);
		label(i) = index;
	}//cerr<<label<<endl;
	VectorXi tmpp(label.size()); 
	VectorXi tmp2 = unique(label,tmpp);
	int tmpd = tmp2.size();  //cerr<<tmpd<<endl;
	MatrixXf tmpr(r.rows(),tmpd);
	for(int i=0;i<tmpd;i++)
	{
		tmpr.col(i) = r.col(tmp2(i));
	}//cerr<<"done1"<<endl;
	r.resize(r.rows(),tmpd);
	r = tmpr;
	float tol = 1e-10;
	int max = 500;
	double llh = -9e+9;
	bool converged = false;
	int t = 1;
	//cerr<<"done1"<<endl;
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
		//cerr<<"t"<<t<<endl;
	}
	//cerr<<label<<endl;
	/*if (converged)
    		cerr<<"Converged in %d steps.\n",t-1);
	else
    		printf("Not converged in %d steps.\n",max);*/
	//cerr<<t-1<<endl;

	
	for(int i=0;i<label.size();i++)
	{
		IDX[i] = label(i);
	}

}

