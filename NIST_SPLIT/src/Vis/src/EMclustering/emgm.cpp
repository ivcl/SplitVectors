
#define PI 3.1415926

#include "emgm.h"
namespace __em_lib {
void EMclustering::SetData(vector<vector<double> > pos, vector<double> den, vector<vector<double> > vel, int num,int dennum)
{
	if(dataPos.size()!=0)
		cleanup();

	dataSize = num;

	for(int i=0;i<num;i++)
	{	//cerr<<i<<endl;
		
		dataVel.push_back(vel[i]);
		//dataDen.push_back(den[i]);
	}

	for(int i=0;i<dennum;i++)
	{	//cerr<<i<<endl;
		dataPos.push_back(pos[i]);
		dataDen.push_back(den[i]);
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

	MatrixXf tmpx(x.rows(),x.cols());
	tmpx = x.colwise() - mu;

	MatrixXf u1(r_sigma,c_sigma);
	u1 = sigma.llt().matrixL() ;
	MatrixXf u2(u1.cols(),u1.rows());
	u2 = u1.adjoint();//cerr<<u2<<endl;
	
	MatrixXf Q(u2.cols(),tmpx.cols());
	Q = u1.jacobiSvd(ComputeThinU | ComputeThinV).solve(tmpx);
	//cerr<<"q"<<Q<<endl;
	VectorXf q(Q.cols());
	q = Q.cwiseProduct(Q).colwise().sum();//cerr<<"q"<<q<<endl;
	VectorXf tmp1(u2.rows());
	tmp1 = u2.diagonal();
	tmp1 = tmp1.array().log();
	double c1 = tmp1.sum() * 2;
	double c2 = d * log(2*PI);//cerr<<c1+c2<<endl;
	
	VectorXf y(q.size());
	y = -(c1+c2)/2. - q.array()/2.;
	
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
	VectorXf nk(r.rows());
	nk = r.colwise().sum();
	VectorXf w(nk.size());
	w = nk/n;
	MatrixXf tmp1(x.rows(),r.cols());
	tmp1 = x * r;
	VectorXf tmp2(nk.size());
	tmp2 = nk.array().inverse();
	//cerr<<tmp2<<endl<<endl;
	MatrixXf mu(x.rows(),r.cols());
	mu = tmp1 * tmp2.asDiagonal() ;
	MatrixXf *sigma = new MatrixXf[k];
	for(int i=0;i<k;i++)
		sigma[i].setZero(d,d);
	MatrixXf sqrtr(r.rows(),r.cols());
	sqrtr = r.cwiseSqrt();
	MatrixXf xo(d,n);
	MatrixXf tmp3(d,d);
	tmp3.setIdentity(d,d);
	
	for(int i=0;i<k;i++)
	{
		xo = x.colwise() - mu.col(i);
		VectorXf tmp4(sqrtr.rows());
		tmp4 = sqrtr.col(i);
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
	MatrixXf mu(model.mu.rows(),model.mu.cols());
	mu = model.mu;
	MatrixXf *sigma;
	sigma = new MatrixXf[clusternum];
	//for(int i=0;i<clusternum;i++)
	//	sigma[i] =  model.sigma[i];
	//cerr<<"1"<<endl;
	sigma = model.sigma;
	VectorXf w(model.weight.size());
	w = model.weight;

	//cerr<<mu<<endl;
	//cerr<<w<<endl<<endl;
	//for(int i=0;i<clusternum;i++)cerr<<sigma[i]<<endl;
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
	MatrixXf tmp1(logrho.rows(),logrho.cols());
	tmp1 = logrho.rowwise() + w.adjoint();
	logrho = tmp1;//cerr<<logrho<<endl<<endl;
	VectorXf t(logrho.rows());
	t = logsumexp(logrho,2);//cerr<<t<<endl<<endl;
	llh = t.sum()/n;//cerr<<llh<<endl<<endl;
	MatrixXf logr(logrho.rows(),logrho.cols());
	logr = logrho.colwise() - t;//cerr<<logr<<endl<<endl;
	MatrixXf r(logrho.rows(),logrho.cols());
	r = logr.array().exp();//cerr<<r<<endl<<endl;

	logrho.resize(0,0);
	mu.resize(0,0);
	w.resize(0);
	//for(int i=0;i<clusternum;i++)//..................
	//	sigma[i].resize(0,0);
	delete [] sigma;
	tmp1.resize(0,0);
	t.resize(0);
	logr.resize(0,0);
	
	//cerr<<r<<endl<<endl;
	//cerr<<llh<<endl;

	return r;
}

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
}
bool EMclustering::ifequal(int a, int b, MatrixXf x)
{
	bool flag = true;
	for(int i=0;i<x.rows();i++)
	{
		//cerr<<x.rows()<<" "<<x.cols()<<" "<<i<<" "<<a<<" "<<b<<endl;
		if(x(i,a)!=x(i,b))
		{
			flag = false;
		}
	}

	return flag;
}

MatrixXf EMclustering::kmeans(MatrixXf x, int k)
{
	//cerr<<x<<endl;
	srand (1);

	int n = x.cols();
	int d = x.rows();

	VectorXi idx(k);

	idx(0) = rand()%n;
	int count2 =1;
	for(int i=1;i<k;i++)
	{//cerr<<i<<endl;
		int tmpi = rand()%n;
		bool flag = true;
		while(flag)
		{
			flag = false;
			tmpi = rand()%n;
			for(int j=0;j<count2;j++)
			{
				if(idx(j) == tmpi
				|| ifequal(idx(j),tmpi,x))	
				{
					flag = true;
					break;
				}
			}
		}
		idx(i) = tmpi;
		count2++;
	}
	//cerr<<idx<<endl;
	
	MatrixXf a(d,k);
	MatrixXf D(n,k);
	VectorXi tmpr(n);
	VectorXi tmpcount(k);


	for(int i=0;i<k;i++)
	{
		a.col(i) = x.col(idx(i));
	}
	/*for(int i=0;i<k;i++)
		cerr<<a(0,i)<<" ";
	cerr<<endl;*/
	int count = 0;

	while(count<500)
	{
		for(int i=0;i<k;i++)
		{
			D.col(i) = (x.colwise() - a.col(i)).colwise().squaredNorm().transpose();
		}
		for(int i=0;i<n;i++)
		{
			D.row(i).minCoeff(&tmpr(i));
		}	
		//cerr<<D<<endl;
		a.setZero(d,k);
		tmpcount.setZero(k);
		for(int i=0;i<n;i++)
		{

			a.col(tmpr(i)) = (a.col(tmpr(i)).array() + x.col(i).array()).matrix(); 		
			tmpcount(tmpr(i))++;
		}
		//cerr<<a<<endl;
		for(int i=0;i<k;i++)
		{
			a.col(i) = (a.col(i).array()/tmpcount(i)).matrix();
		}
		//cerr<<a<<endl;		
		//cerr<<tmpr<<endl;

		count ++;
	}


	MatrixXf r(n,k);
	r.setZero(n,k);
	for(int i=0;i<n;i++)
	{
		r(i,tmpr(i)) = 1;
	}
	cerr<<r<<endl;

	idx.resize(0);
	a.resize(0,0);
	D.resize(0,0);
	tmpr.resize(0);
	tmpcount.resize(0);
	

	return r;

}


MatrixXf EMclustering::initialization(MatrixXf x, int k)
{
  srand (1);

	int n = x.cols();
	int d = x.rows();
	//cerr<<n<<"d "<<d<<endl;

	VectorXi idx(k);
	//idx.setRandom(k);
	//cerr<<idx<<endl;
	idx(0) = rand()%n;
	for(int i=1;i<k;i++)
	{
		int random;// = rand()%n;
		//cerr<<random<<endl;
		bool flag2 = true;
		while(flag2)
		{
			random = rand()%n;
			flag2 = false;
			for(int j=0;j<i;j++)
			{
				if(idx(j)==random)
				{	
				
					flag2 = true;
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

	VectorXf tmp1(m.cols());
	tmp1 = m.cwiseProduct(m).colwise().sum();
	//cerr<<tmp1<<endl<<endl;
	tmp1 = tmp1.adjoint()/2;
	//cerr<<tmp1<<endl<<endl;
	MatrixXf tmp2(m.adjoint().rows(),x.cols());
	tmp2 = m.adjoint() * x;
	//cerr<<tmp2<<endl<<endl;
	MatrixXf tmp3(tmp2.rows(),tmp2.cols());
	tmp3 = tmp2.colwise() - tmp1;
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

	int max = 100;
	int t=0;
	while(k!=u.size()&&t<max)
	{
		t++;
		//cerr<<"-----------"<<endl;
		
		//idx.setRandom(k);
		idx(0) = rand()%n;//cerr<<k<<" "<<u.size()<<" "<<idx(0)<<endl;
		for(int i=1;i<k;i++)
		{
			int random;// = rand()%n;
			bool flag2 = true;
			while(flag2)
			{random = rand()%n;
				flag2 = false;
				for(int j=0;j<i;j++)
				{
					if(idx(j)==random||x.col(idx(j))==x.col(random))
					{
						flag2 = true;
						break;
					}
				}
			}
			idx(i) = random;//cerr<<idx(i)<<" ";
		}//cerr<<endl;
		//cerr<<"idx"<<idx<<endl<<endl;;
		//cerr<<"u"<<u<<endl<<endl;
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
		tmpd = tmp3.cols();
		for(int i=0;i<tmpd;i++)
		{
			tmp4 = tmp3.col(i).maxCoeff(&tmp5);
			label(i) = tmp5;
		}
		//cerr<<"label"<<label.size()<<endl<<endl;
		//cerr<<"end"<<endl;
		u = unique(label, tmp6);
		label = tmp6;
		//cerr<<"u"<<u<<endl<<endl;
		//cerr<<tmp6<<endl<<endl;
	}//cerr<<k<<" "<<u.size()<<endl;

	//cerr<<t<<endl;
	//cerr<<u.size()<<endl;

	MatrixXf r(n,k);
	r.setZero(n,k);
	int j=0;
	int tmplabel[k];

	for(int i=0;i<k;i++)
		tmplabel[i] = 0;
	for(int i=0;i<n;i++)
	{
		r(i,label(j)) = 1;
		tmplabel[label(j)] = 1;
		j++;
	}

	//for(int i=0;i<n;i++)
	//{
	//	if(tmplabel[i]==0)
	//	{
	//		r(i,0) = 1;
	//	}
	//}
	j = 0;
	for(int i=0;i<k;i++)
	{
		if(tmplabel[i]==0)
		{
			//cerr<<i<<endl;
			for(int ii=0;ii<n/k;ii++)
			{
				int ttt = rand()%n;
				//for(int jj=0;jj<k;jj++)
				//	r(ttt,jj)=0;
				r(ttt,i) = 1;
			}
			//j ++;//cerr<<j<<endl;
		}
	}

	//cerr<<r.cols()<<endl;


	//cerr<<r<<endl;
	//delete [] tmplabel;

	idx.resize(0);
	m.resize(0,0);
	tmp1.resize(0);
	tmp2.resize(0,0);
	tmp3.resize(0,0);
	tmp6.resize(0);
	u.resize(0);
	label.resize(0);

	return r;
}

int EMclustering::EM(int k, int *IDX, bool spatial, bool att)
{
	clusternum = k;
	MatrixXf x;
	/*if(spatial)
	{
		if(att)
		{
			x.resize(4,dataSize);
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
			x.resize(6,dataSize);
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
	{*/
		if(att)
		{
			x.resize(1,dataDen.size());
			for(int i=0;i<dataDen.size();i++)
			{
				x(0,i) = dataDen[i];
			}
			//cerr<<x;
			//cerr<<endl;
			if(k>dataDen.size())
				return -1;
		}
		else
		{
			x.resize(3,dataSize);
			for(int i=0;i<dataSize;i++)
			{
				x(0,i) = dataVel[i][0];//fabs(cos(-PI/4)*dataVel[i][0] - sin(-PI/4)*dataVel[i][1]);
				x(1,i) = dataVel[i][1];//fabs(sin(-PI/4)*dataVel[i][0] + cos(-PI/4)*dataVel[i][1]);
				x(2,i) = dataVel[i][2];
			}
			if(k>dataSize)
				return -1;
		}
	//}

	//cout<<"EM for Gaussian mixture: running ... "<<endl;
	//cerr<<x<<endl;
	MatrixXf r =initialization(x,k);// kmeans(x,k);//
	//cerr<<"Initialization is Done"<<endl;//cerr<<r<<endl;
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
	r = tmpr;//cerr<<r.cols()<<endl;
	float tol = 1e-10;
	int max = 300;

	double llh = -9e+9;
	bool converged = false;
	int t = 1;
	//cerr<<"done1"<<endl;
	//gaussian_model model;

	int clusternum_error;

	MatrixXf tmpmodel;

	while(!converged&&t<max)
	{
		t = t + 1;
		gaussian_model model = maximization(x,r);//cerr<<t<<" "<<"max"<<endl;
		float tmpllh = llh;
		r = expectation(x,model,llh);//cerr<<t<<" "<<"exp"<<endl;
		for(int i=0;i<r.rows();i++)
		{
			int index;
			float tmp1 = r.row(i).maxCoeff(&index);
			label(i) = index;
		}
		
		VectorXi u = unique(label,tmpp);//cerr<<t<<" "<<u.size()<<" "<<r.cols()<<" "<<r.rows()<<endl;
		clusternum_error = clusternum - u.size();

		if(r.cols()!=u.size())
		{
		/*	tmpr.resize(r.rows(),u.size());
			for(int i=0;i<u.size();i++)
			{
				tmpr.col(i) = r.col(u(i));
			}
			r.resize(r.rows(),u.size());
			r = tmpr;//cerr<<"r"<<endl;*/
		}
		else
		{
			if((llh - tmpllh)<tol*abs(llh))
				converged = true;
			else
				converged = false;
		}
		//cerr<<"t"<<t<<endl;
		//return_model = model;

		tmpmodel.resize(model.mu.rows(),model.mu.cols());
		//return_model = model.mu;
		tmpmodel = model.mu;
		u.resize(0);
		//cerr<<tmpmodel<<endl;
	}
	/*ofstream off2("rr");
	off2<<r.row(0)<<endl;
	for(int i=1;i<r.rows();i++)
		if(r.row(i)!=r.row(i-1))
		{off2<<x.col(i)<<" ";
			off2<<r.row(i)<<endl;}
off2.close();*/
		cerr<<clusternum_error<<endl;
	return_model  = tmpmodel;
	//cerr<<label<<endl;
	if (converged)
    		cerr<<"Converged in "<<t-1<<endl;
	else
    		cerr<<max<<endl;
	//cerr<<t-1<<endl;

	
	for(int i=0;i<label.size();i++)
	{
		IDX[i] = label(i);
		//cerr<<IDX[i]<<" ";
	}//cerr<<endl;

	//cerr<<label.size()<<endl;

	x.resize(0,0);
	r.resize(0,0);
	tmpr.resize(0,0);
	tmpmodel.resize(0,0);
	label.resize(0);
	tmpp.resize(0);
	tmp2.resize(0);
	

	return clusternum_error;
}

void EMclustering::getmu(double &mu, int k)
{
	mu = return_model(k);
}
void EMclustering::getmu(double &m1, double &m2, double &m3, int k)
{
	m1 = return_model(0,k);
	m2 = return_model(1,k);
	m3 = return_model(2,k);
}
void EMclustering::cleanup()
{
	int num = dataPos.size();
	if(num!=0)
	for(int i=0;i<num;i++)
	{
		dataPos[i].clear();
		
	}
	if(dataSize!=0)
	for(int i=0;i<dataSize;i++)
	{
		dataVel[i].clear();
	}
	dataPos.clear();
	dataVel.clear();
	dataDen.clear();
	dataSize = 0;
}
}

