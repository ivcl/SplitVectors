#include "dataProcess.h"
#include "CIEColor2.h"
namespace __em_lib{

void Cluster::ReadColors(int mode)
{
	CIEColor2 *cc = new CIEColor2();

	spiralcolors_mag[0].clear();
	spiralcolors_mag[1].clear();
	spiralcolors_mag[2].clear();
	spiralcolors_vel[0].clear();
	spiralcolors_vel[1].clear();
	spiralcolors_vel[2].clear();
	spiralcolors_vel_hue.clear();


	double t,h1,h2;
	if(mode == 1)
	{
		h1 = 0;
		h2 = 300;
	}
	else if(mode == 2)
	{
		h1 = 180;
		h2 = 540;
	}
	t =0;
	float degree = 1./((double)clusternum_mag[0]-1.);
	double h;
	double hue;
	for(int i=0;i<clusternum_mag[0];i++)
	{
		double v = t;

		vector<double> rgb;
		h = h1 + t * (h2 - h1);
		rgb = cc->MultiplehueSequential(v, 0.6, 0.25, 0.75, hue, h,0.15);
		double r,g,b;
		r = rgb[0];g=rgb[1];b=rgb[2];
		double r2,g2,b2;
		ColorBlindSafe(r,g,b,r2,g2,b2);
		spiralcolors_mag[0].push_back(r2);
		spiralcolors_mag[1].push_back(g2);
		spiralcolors_mag[2].push_back(b2);
		t = t + degree;
	}

	vector<double> c[3];
	int sum = spiralcolors_mag[0].size();//cerr<<sum<<endl;
	for(int i=0;i<sum;i++)
	{
		c[0].push_back(spiralcolors_mag[0][sum-i-1]);
		c[1].push_back(spiralcolors_mag[1][sum-i-1]);
		c[2].push_back(spiralcolors_mag[2][sum-i-1]);
	}
	for(int i=0;i<sum;i++)
	{
		spiralcolors_mag[0][i] = c[0][i];
		spiralcolors_mag[1][i] = c[1][i];
		spiralcolors_mag[2][i] = c[2][i];
		//cerr<<c[0][i]<<endl;
	}
	c[0].clear();c[1].clear();c[2].clear();

	t =0;
	degree = 1./((double)clusternum_vel[0]-1.);
	for(int i=0;i<clusternum_vel[0];i++)
	{
		double v;
		if(mode == 1)v = 0.5;
		else v = t;

		vector<double> rgb;
		h = h1 + t * (h2 - h1);
		rgb = cc->MultiplehueSequential(v, 1., 0.75, 0.88, hue, h, 0.15);
		double r,g,b;
		r = rgb[0];g=rgb[1];b=rgb[2];
		double r2,g2,b2;
		ColorBlindSafe(r,g,b,r2,g2,b2);
		spiralcolors_vel[0].push_back(r2);
		spiralcolors_vel[1].push_back(g2);
		spiralcolors_vel[2].push_back(b2);
		//cerr<<hue<<endl;
		spiralcolors_vel_hue.push_back(hue);
		t = t + degree;
	}

	sum = spiralcolors_vel[0].size();//cerr<<sum<<endl;
	vector<double> hh;
	for(int i=0;i<sum;i++)
	{
		c[0].push_back(spiralcolors_vel[0][sum-i-1]);
		c[1].push_back(spiralcolors_vel[1][sum-i-1]);
		c[2].push_back(spiralcolors_vel[2][sum-i-1]);
		hh.push_back(spiralcolors_vel_hue[sum-i-1]);
	}
	for(int i=0;i<sum;i++)
	{
		spiralcolors_vel[0][i] = c[0][i];
		spiralcolors_vel[1][i] = c[1][i];
		spiralcolors_vel[2][i] = c[2][i];
		spiralcolors_vel_hue[i] = hh[i];
	}
	c[0].clear();c[1].clear();c[2].clear();hh.clear();

	delete [] cc;
	hh.clear();
}

void Cluster::SetROIColors()
{
	CIEColor2 *cc = new CIEColor2();

	spiralcolors_mag_in[0].clear();
	spiralcolors_mag_in[1].clear();
	spiralcolors_mag_in[2].clear();
	spiralcolors_vel_in[0].clear();
	spiralcolors_vel_in[1].clear();
	spiralcolors_vel_in[2].clear();
	spiralcolors_vel_in_hue.clear();

	double t,h1,h2;
	h1 = 0;
	h2 = 360;

	t =0;
	float degree = 1./((double)clusternum_mag_in[0]-1.);
	double h;
	double hue;
	for(int i=0;i<clusternum_mag_in[0];i++)
	{
		vector<double> rgb;
		h = h1 + t * (h2 - h1);
		rgb = cc->MultiplehueSequential(t, 0.6, 0.25, 0.75, hue, h,0.15);
		double r,g,b;
		r = rgb[0];g=rgb[1];b=rgb[2];
		double r2,g2,b2;
		ColorBlindSafe(r,g,b,r2,g2,b2);
		spiralcolors_mag_in[0].push_back(r2);
		spiralcolors_mag_in[1].push_back(g2);
		spiralcolors_mag_in[2].push_back(b2);
		//spiralcolors_mag_in_hue.push_back(hue);
		t = t + degree;
	}

	vector<double> c[3];
	int sum = spiralcolors_mag_in[0].size();//cerr<<sum<<endl;
	for(int i=0;i<sum;i++)
	{
		c[0].push_back(spiralcolors_mag_in[0][sum-i-1]);
		c[1].push_back(spiralcolors_mag_in[1][sum-i-1]);
		c[2].push_back(spiralcolors_mag_in[2][sum-i-1]);
	}
	for(int i=0;i<sum;i++)
	{
		spiralcolors_mag_in[0][i] = c[0][i];
		spiralcolors_mag_in[1][i] = c[1][i];
		spiralcolors_mag_in[2][i] = c[2][i];
		//cerr<<c[0][i]<<endl;
	}
	c[0].clear();c[1].clear();c[2].clear();

	t =0;
	degree = 1./((double)clusternum_vel_in[0]-1.);
	for(int i=0;i<clusternum_vel_in[0];i++)
	{
		vector<double> rgb;
		h = h1 + t * (h2 - h1);
		rgb = cc->MultiplehueSequential(t, 0.6, 0.25, 0.75, hue, h, 0.15);
		double r,g,b;
		r = rgb[0];g=rgb[1];b=rgb[2];
		double r2,g2,b2;
		ColorBlindSafe(r,g,b,r2,g2,b2);
		spiralcolors_vel_in[0].push_back(r2);
		spiralcolors_vel_in[1].push_back(g2);
		spiralcolors_vel_in[2].push_back(b2);
		spiralcolors_vel_in_hue.push_back(hue);
		//cerr<<hue<<endl;
		t = t + degree;
	}

	sum = spiralcolors_vel_in[0].size();//cerr<<sum<<endl;
	for(int i=0;i<sum;i++)
	{
		c[0].push_back(spiralcolors_vel_in[0][sum-i-1]);
		c[1].push_back(spiralcolors_vel_in[1][sum-i-1]);
		c[2].push_back(spiralcolors_vel_in[2][sum-i-1]);
	}
	for(int i=0;i<sum;i++)
	{
		spiralcolors_vel_in[0][i] = c[0][i];
		spiralcolors_vel_in[1][i] = c[1][i];
		spiralcolors_vel_in[2][i] = c[2][i];
	}
	c[0].clear();c[1].clear();c[2].clear();

	delete [] cc;
}

//bool Cluster::scompare1(int a, int b){return magrange[0][a]>magrange[0][b];}
//bool Cluster::scompare2(int a, int b){return magrange[0][a]<=magrange[0][b];}

void Cluster::MagProcess()
{
	//cerr<<"1"<<endl;
	double tmprange[2];
	tmprange[0]=9e+15;
	tmprange[1]=-9e+15;
	magrange[0].clear();
	magrange[1].clear();

	density_color[0].clear();
	density_color[1].clear();
	density_color[2].clear();
	IDX_mag_order.clear();

	for(int i=0;i<clusternum_mag[0];i++)
	{
		magrange[0].push_back(tmprange[0]);
		magrange[1].push_back(tmprange[1]);
	}//cerr<<"1"<<endl;

	//bound of each existing cluster
	for(int i=0;i<dataSize;i++)
	{
		//cerr<<i<<" "<<IDX_Mag[i]<<endl;
		if(IDX_Mag[i]!=-1)
		{
			if(density[i] < magrange[0][IDX_Mag[i]])
				magrange[0][IDX_Mag[i]] = density[i];
			if(density[i] > magrange[1][IDX_Mag[i]])
				magrange[1][IDX_Mag[i]] = density[i];
		}
	}	//cerr<<"1"<<endl;
	/*for(int i=0;i<clusternum_mag[0];i++)
	{
		cerr<<magrange[0][i]<<" "<<magrange[1][i]<<endl;
	}*/
	//find if # of cluster is correct
	vector<int> replace_error;
	if(clusternum_error_mag!=0)
	{
		cerr<<clusternum_error_mag<<endl;
		vector<int> missing_index;
		for(int i=0;i<clusternum_mag[0];i++)
		{
			bool missing = false;
			for(int j=0;j<dataSize;j++)
			{
				if(i==IDX_Mag[j])
				{
					missing = true;
					break;
				}
			}
			if(!missing)
				missing_index.push_back(i);
		}
		for(int i=0;i<magrange[0].size();i++)
		{
			//cerr<<magrange[1][i]<<" "<<magrange[0][i]<<endl;
			if(fabs(-magrange[0][i]+magrange[1][i])>0.000001)
			{
				replace_error.push_back(i);//cerr<<i<<endl;
				if(replace_error.size()==clusternum_error_mag)
					break;
			}
		}
		int count = 1;
		for(int i=0;i<replace_error.size();i++)
		{
			double ave = magrange[0][replace_error[i]]+magrange[1][replace_error[i]];
			ave = ave/2.;
			cerr<<"average: "<<ave<<endl;
			for(int j=0;j<dataSize;j++)
			{//cerr<<i<<" "<<j<<endl;
				if(IDX_Mag[j]==replace_error[i]
				&& density[j]>=ave)
				{
					IDX_Mag[j] = missing_index[count-1];
					//cerr<<IDX_Mag[j]<<" ";
				}
			}//cerr<<endl;
			count ++;
		}
		missing_index.clear();

		/*for(int i=0;i<dataSize;i++)
		{//cerr<<"dd"<<i<<endl;
			if(IDX_Mag[i]>=clusternum_mag[0]-clusternum_error_mag)
			{
				if(density[i] < magrange[0][IDX_Mag[i]])
					magrange[0][IDX_Mag[i]] = density[i];
				if(density[i] > magrange[1][IDX_Mag[i]])
					magrange[1][IDX_Mag[i]] = density[i];

			}
		}*/
		for(int i=0;i<clusternum_mag[0];i++)
		{
			magrange[0][i]=tmprange[0];
			magrange[1][i]=tmprange[1];
		}//cerr<<"1"<<endl;
		for(int i=0;i<dataSize;i++)
		{
			//cerr<<i<<" "<<IDX_Mag[i]<<endl;
			if(IDX_Mag[i]!=-1)
			{
				if(density[i] < magrange[0][IDX_Mag[i]])
					magrange[0][IDX_Mag[i]] = density[i];
				if(density[i] > magrange[1][IDX_Mag[i]])
					magrange[1][IDX_Mag[i]] = density[i];
			}
		}	//cerr<<"1"<<endl;
	}//cerr<<"1"<<endl;
	/*for(int i=0;i<clusternum_mag[0];i++)
	{
		cerr<<magrange[0][i]<<" "<<magrange[1][i]<<endl;
	}*/
	//fix range //need to be improved
	IDX_sort_mag.clear();
	for(int i=0;i<clusternum_mag[0];i++)
	{
		IDX_sort_mag.push_back(i);
	}

	vector<double> tmpmagrange[2];
	for(int i=0;i<magrange[0].size();i++)
	{
		tmpmagrange[0].push_back(magrange[0][i]);
		tmpmagrange[1].push_back(magrange[1][i]);
	}

	sort(magrange[0].begin(),magrange[0].end());

	for(int i=0;i<clusternum_mag[0];i++)
	{
		for(int j=0;j<clusternum_mag[0];j++)
		{
			if(fabs(magrange[0][i] - tmpmagrange[0][j])<0.0001)
			//&& fabs(magrange[1][i] - tmpmagrange[1][j])<0.0001)
			{
				IDX_sort_mag[i] = j;//cerr<<j<<endl;
				break;
			}
		}
	}//cerr<<"q"<<endl;

	magrange[0].clear();
	magrange[1].clear();
	for(int i=0;i<tmpmagrange[0].size();i++)
	{
		magrange[0].push_back(tmpmagrange[0][i]);
		magrange[1].push_back(tmpmagrange[1][i]);
	}

	/*for(int i=0;i<clusternum_mag[0];i++)
	{
		cerr<<IDX_sort_mag[i]<<" "<<magrange[0][IDX_sort_mag[i]]<<" "<<magrange[1][IDX_sort_mag[i]]<<endl;
	}*/
	//sort(IDX_sort_mag.begin(),IDX_sort_mag.end(),scompare2);
	vector<int> tmpindex;
	vector<bool> tmpl;
	int count = 0;
	for(int i=1;i<clusternum_mag[0];i++)
	{
		if(magrange[0][IDX_sort_mag[i]]<magrange[1][IDX_sort_mag[i-1]])
		{
			if(magrange[1][IDX_sort_mag[i]]<magrange[1][IDX_sort_mag[i-1]])
			{	
				tmpl.push_back(1);//cerr<<"ha"<<endl;
			}
			else
				tmpl.push_back(0);
			tmpindex.push_back(i);
		}
	}
	for(int j=0;j<tmpindex.size();j++)
	{
		for(int i=0;i<dataSize;i++)
		{
			if(!tmpl[j])
			{
				if(IDX_Mag[i] == IDX_sort_mag[tmpindex[j]])
				{
					if(density[i]<magrange[1][IDX_sort_mag[tmpindex[j]-1]])
						IDX_Mag[i] = IDX_sort_mag[tmpindex[j]-1];
				}
			}
			else
			{
				if(IDX_Mag[i] == IDX_sort_mag[tmpindex[j]])
				{
					if(density[i]<(magrange[1][IDX_sort_mag[tmpindex[j]]]/2.
						+ magrange[0][IDX_sort_mag[tmpindex[j]]]/2.))
						IDX_Mag[i] = IDX_sort_mag[tmpindex[j]-1];
				}	
				else if(IDX_Mag[i] == IDX_sort_mag[tmpindex[j]-1])
				{
					if(density[i]>(magrange[1][IDX_sort_mag[tmpindex[j]-1]]/2.
						+ magrange[0][IDX_sort_mag[tmpindex[j]-1]]/2.))
						IDX_Mag[i] = IDX_sort_mag[tmpindex[j]];
				}			
			}
		}
	}

	/*for(int i=0;i<clusternum_mag[0];i++)
	{
		cerr<<magrange[0][IDX_sort_mag[i]]<<" "<<magrange[1][IDX_sort_mag[i]]<<endl;
	}*/
	//sort
	IDX_sort_mag.clear();
	magrange[0].clear();
	magrange[1].clear();
	for(int i=0;i<clusternum_mag[0];i++)
	{
		magrange[0].push_back(tmprange[0]);
		magrange[1].push_back(tmprange[1]);
	}
	for(int i=0;i<dataSize;i++)
	{
		if(IDX_Mag[i]!=-1)
		{
			if(density[i] < magrange[0][IDX_Mag[i]])
				magrange[0][IDX_Mag[i]] = density[i];
			if(density[i] > magrange[1][IDX_Mag[i]])
				magrange[1][IDX_Mag[i]] = density[i];

		}
	}

	tmpmagrange[0].clear();
	tmpmagrange[1].clear();
	for(int i=0;i<magrange[0].size();i++)
	{
		tmpmagrange[0].push_back(magrange[0][i]);
		tmpmagrange[1].push_back(magrange[1][i]);
	}
	sort(magrange[0].begin(),magrange[0].end());
	for(int i=0;i<clusternum_mag[0];i++)
	{
		for(int j=0;j<clusternum_mag[0];j++)
		{
			if(magrange[0][i] == tmpmagrange[0][j]
			&& magrange[1][i] == tmpmagrange[1][j])
			{
				IDX_sort_mag[i] = j;
				break;
			}
		}
	}
	magrange[0].clear();
	magrange[1].clear();
	for(int i=0;i<tmpmagrange[0].size();i++)
	{
		magrange[0].push_back(tmpmagrange[0][i]);
		magrange[1].push_back(tmpmagrange[1][i]);
	}

	//sort(IDX_sort_mag.begin(),IDX_sort_mag.end(),scompare1);
	int tmpidxsort[clusternum_mag[0]];
	for(int i=0;i<clusternum_mag[0];i++)
	{
		tmpidxsort[i] = IDX_sort_mag[i];
	}
	for(int i=0;i<clusternum_mag[0];i++)
	{
		IDX_sort_mag[i] = tmpidxsort[clusternum_mag[0]-i-1];
		//cerr<<magrange[0][IDX_sort_mag[i]]<<endl;
	}

	for(int i=0;i<clusternum_mag[0];i++)
	{
		for(int j=0;j<clusternum_mag[0];j++)
		{
			if(i == IDX_sort_mag[j])
			{
				IDX_mag_order.push_back(j);
				break;
			}	
		}
	}

	double r,g,b;
	double h,s,v;

	//int degree = spiralcolors[0].size()/(clusternum_mag[0]+1);	
	//cerr<<spiralcolors[0].size()<<endl;

	for(int i=0;i<clusternum_mag[0];i++)
	{
		h = 60;

		r = spiralcolors_mag[0][i];
		g = spiralcolors_mag[1][i];
		b = spiralcolors_mag[2][i];
		//cerr<<degree*(i+1)<<" "<<r<<" "<<g<<" "<<b<<endl;
		density_color[0].push_back(r);
		density_color[1].push_back(g);
		density_color[2].push_back(b);

	}

	min_degree_mag.clear();
	min_degree_mag.push_back(log10(magrange[1][IDX_sort_mag[0]]));
	for(int i=0;i<clusternum_mag[0];i++)
	{
		min_degree_mag.push_back(log10(magrange[0][IDX_sort_mag[0]]));
	}

	tmpindex.clear();
	tmpl.clear();
	replace_error.clear();
	tmpmagrange[0].clear();
	tmpmagrange[1].clear();

	range_mag[0] = min_density;
	range_mag[1] = max_density;
	widget_t_l = 0;
	widget_t_h = 0;
}
void Cluster::MaginProcess()
{
	//cerr<<"1"<<endl;
	double tmprange[2];
	tmprange[0]=9e+15;
	tmprange[1]=-9e+15;
	magrange_in[0].clear();
	magrange_in[1].clear();

	density_color_in[0].clear();
	density_color_in[1].clear();
	density_color_in[2].clear();
	IDX_mag_order_in.clear();

	for(int i=0;i<clusternum_mag_in[0];i++)
	{
		magrange_in[0].push_back(tmprange[0]);
		magrange_in[1].push_back(tmprange[1]);
	}//cerr<<"1"<<endl;

	//bound of each existing cluster
	for(int i=0;i<dataSize;i++)
	{
		//cerr<<i<<" "<<IDX_Mag[i]<<endl;
		if(IDX_Mag_In[i]!=-1)
		{
			if(density[i] < magrange_in[0][IDX_Mag_In[i]])
				magrange_in[0][IDX_Mag_In[i]] = density[i];
			if(density[i] > magrange_in[1][IDX_Mag_In[i]])
				magrange_in[1][IDX_Mag_In[i]] = density[i];
		}
	}	//cerr<<"1"<<endl;

	//find if # of cluster is correct
	vector<int> replace_error;
	if(clusternum_error_mag_in!=0)
	{
		cerr<<clusternum_error_mag_in<<endl;
		vector<int> missing_index;
		for(int i=0;i<clusternum_mag_in[0];i++)
		{
			bool missing = false;
			for(int j=0;j<dataSize;j++)
			{
				if(i==IDX_Mag_In[j])
				{
					missing = true;
					break;
				}
			}
			if(!missing)
				missing_index.push_back(i);
		}
		for(int i=0;i<magrange_in[0].size();i++)
		{
			//cerr<<magrange_in[1][i]<<" "<<magrange_in[0][i]<<endl;
			if(fabs(-magrange_in[0][i]+magrange_in[1][i])>0.000001)
			{
				replace_error.push_back(i);//cerr<<i<<endl;
				if(replace_error.size()==clusternum_error_mag_in)
					break;
			}
		}
		int count = 1;
		for(int i=0;i<replace_error.size();i++)
		{
			double ave = magrange_in[0][replace_error[i]]+magrange_in[1][replace_error[i]];
			ave = ave/2.;
			cerr<<"average: "<<ave<<endl;
			for(int j=0;j<dataSize;j++)
			{//cerr<<i<<" "<<j<<endl;
				if(IDX_Mag_In[j]==replace_error[i]
				&& density[j]>=ave)
				{
					IDX_Mag_In[j] = missing_index[count-1];
					//cerr<<IDX_Mag[j]<<" ";
				}
			}//cerr<<endl;
			count ++;
		}
		missing_index.clear();

		/*for(int i=0;i<dataSize;i++)
		{//cerr<<"dd"<<i<<endl;
			if(IDX_Mag[i]>=clusternum_mag[0]-clusternum_error_mag)
			{
				if(density[i] < magrange_in[0][IDX_Mag[i]])
					magrange[0][IDX_Mag[i]] = density[i];
				if(density[i] > magrange[1][IDX_Mag[i]])
					magrange[1][IDX_Mag[i]] = density[i];

			}
		}*/
		for(int i=0;i<clusternum_mag_in[0];i++)
		{
			magrange_in[0][i]=tmprange[0];
			magrange_in[1][i]=tmprange[1];
		}//cerr<<"1"<<endl;
		for(int i=0;i<dataSize;i++)
		{
			//cerr<<i<<" "<<IDX_Mag[i]<<endl;
			if(IDX_Mag_In[i]!=-1)
			{
				if(density[i] < magrange_in[0][IDX_Mag_In[i]])
					magrange_in[0][IDX_Mag_In[i]] = density[i];
				if(density[i] > magrange_in[1][IDX_Mag_In[i]])
					magrange_in[1][IDX_Mag_In[i]] = density[i];
			}
		}	//cerr<<"1"<<endl;
	}//cerr<<"1"<<endl;
	/*for(int i=0;i<clusternum_mag[0];i++)
	{
		cerr<<magrange[0][i]<<" "<<magrange[1][i]<<endl;
	}*/

	//fix range //need to be improved
	IDX_sort_mag_in.clear();
	for(int i=0;i<clusternum_mag_in[0];i++)
	{
		IDX_sort_mag_in.push_back(i);
	}

	vector<double> tmpmagrange_in[2];
	for(int i=0;i<magrange_in[0].size();i++)
	{
		tmpmagrange_in[0].push_back(magrange_in[0][i]);
		tmpmagrange_in[1].push_back(magrange_in[1][i]);
	}

	sort(magrange_in[0].begin(),magrange_in[0].end());

	for(int i=0;i<clusternum_mag_in[0];i++)
	{
		for(int j=0;j<clusternum_mag_in[0];j++)
		{
			if(fabs(magrange_in[0][i] - tmpmagrange_in[0][j])<0.0001)
			//&& fabs(magrange[1][i] - tmpmagrange[1][j])<0.0001)
			{
				IDX_sort_mag_in[i] = j;//cerr<<j<<endl;
				break;
			}
		}
	}//cerr<<"q"<<endl;

	magrange_in[0].clear();
	magrange_in[1].clear();
	for(int i=0;i<tmpmagrange_in[0].size();i++)
	{
		magrange_in[0].push_back(tmpmagrange_in[0][i]);
		magrange_in[1].push_back(tmpmagrange_in[1][i]);
	}

	/*for(int i=0;i<clusternum_mag[0];i++)
	{
		cerr<<IDX_sort_mag[i]<<" "<<magrange[0][IDX_sort_mag[i]]<<" "<<magrange[1][IDX_sort_mag[i]]<<endl;
	}*/

	//sort(IDX_sort_mag.begin(),IDX_sort_mag.end(),scompare2);
	vector<int> tmpindex;
	vector<bool> tmpl;
	int count = 0;
	for(int i=1;i<clusternum_mag_in[0];i++)
	{
		if(magrange_in[0][IDX_sort_mag_in[i]]<magrange_in[1][IDX_sort_mag_in[i-1]])
		{
			if(magrange_in[1][IDX_sort_mag_in[i]]<magrange_in[1][IDX_sort_mag_in[i-1]])
			{	
				tmpl.push_back(1);//cerr<<"ha"<<endl;
			}
			else
				tmpl.push_back(0);
			tmpindex.push_back(i);
		}
	}
	for(int j=0;j<tmpindex.size();j++)
	{
		for(int i=0;i<dataSize;i++)
		{
			if(!tmpl[j])
			{
				if(IDX_Mag_In[i] == IDX_sort_mag_in[tmpindex[j]])
				{
					if(density[i]<magrange_in[1][IDX_sort_mag_in[tmpindex[j]-1]])
						IDX_Mag_In[i] = IDX_sort_mag_in[tmpindex[j]-1];
				}
			}
			else
			{
				if(IDX_Mag_In[i] == IDX_sort_mag_in[tmpindex[j]])
				{
					if(density[i]<(magrange_in[1][IDX_sort_mag_in[tmpindex[j]]]/2.
						+ magrange_in[0][IDX_sort_mag_in[tmpindex[j]]]/2.))
						IDX_Mag_In[i] = IDX_sort_mag_in[tmpindex[j]-1];
				}	
				else if(IDX_Mag_In[i] == IDX_sort_mag_in[tmpindex[j]-1])
				{
					if(density[i]>(magrange_in[1][IDX_sort_mag_in[tmpindex[j]-1]]/2.
						+ magrange_in[0][IDX_sort_mag_in[tmpindex[j]-1]]/2.))
						IDX_Mag_In[i] = IDX_sort_mag_in[tmpindex[j]];
				}			
			}
		}
	}

	/*for(int i=0;i<clusternum_mag[0];i++)
	{
		cerr<<magrange[0][IDX_sort_mag[i]]<<" "<<magrange[1][IDX_sort_mag[i]]<<endl;
	}*/
	
	//sort
	IDX_sort_mag_in.clear();
	magrange_in[0].clear();
	magrange_in[1].clear();
	for(int i=0;i<clusternum_mag_in[0];i++)
	{
		magrange_in[0].push_back(tmprange[0]);
		magrange_in[1].push_back(tmprange[1]);
	}
	for(int i=0;i<dataSize;i++)
	{
		if(IDX_Mag_In[i]!=-1)
		{
			if(density[i] < magrange_in[0][IDX_Mag_In[i]])
				magrange_in[0][IDX_Mag_In[i]] = density[i];
			if(density[i] > magrange_in[1][IDX_Mag_In[i]])
				magrange_in[1][IDX_Mag_In[i]] = density[i];

		}
	}
	
	tmpmagrange_in[0].clear();
	tmpmagrange_in[1].clear();
	for(int i=0;i<magrange_in[0].size();i++)
	{
		tmpmagrange_in[0].push_back(magrange_in[0][i]);
		tmpmagrange_in[1].push_back(magrange_in[1][i]);
	}
	sort(magrange_in[0].begin(),magrange_in[0].end());

	for(int i=0;i<clusternum_mag_in[0];i++)
	{
		for(int j=0;j<clusternum_mag_in[0];j++)
		{
			if(magrange_in[0][i] == tmpmagrange_in[0][j]
			&& magrange_in[1][i] == tmpmagrange_in[1][j])
			{
				IDX_sort_mag_in[i] = j;
				break;
			}
		}
	}
	magrange_in[0].clear();
	magrange_in[1].clear();
	for(int i=0;i<tmpmagrange_in[0].size();i++)
	{
		magrange_in[0].push_back(tmpmagrange_in[0][i]);
		magrange_in[1].push_back(tmpmagrange_in[1][i]);
	}

	//sort(IDX_sort_mag.begin(),IDX_sort_mag.end(),scompare1);
	int tmpidxsort[clusternum_mag_in[0]];
	for(int i=0;i<clusternum_mag_in[0];i++)
	{
		tmpidxsort[i] = IDX_sort_mag_in[i];
	}
	for(int i=0;i<clusternum_mag_in[0];i++)
	{
		IDX_sort_mag_in[i] = tmpidxsort[clusternum_mag_in[0]-i-1];
		//cerr<<magrange[0][IDX_sort_mag[i]]<<endl;
	}

	for(int i=0;i<clusternum_mag_in[0];i++)
	{
		for(int j=0;j<clusternum_mag_in[0];j++)
		{
			if(i == IDX_sort_mag_in[j])
			{
				IDX_mag_order_in.push_back(j);
				break;
			}	
		}
	}

	double r,g,b;
	double h,s,v;

	//int degree = spiralcolors[0].size()/(clusternum_mag[0]+1);	
	//cerr<<spiralcolors[0].size()<<endl;

	for(int i=0;i<clusternum_mag_in[0];i++)
	{
		h = 60;

		r = spiralcolors_mag_in[0][i];
		g = spiralcolors_mag_in[1][i];
		b = spiralcolors_mag_in[2][i];
		//cerr<<degree*(i+1)<<" "<<r<<" "<<g<<" "<<b<<endl;
		density_color_in[0].push_back(r);
		density_color_in[1].push_back(g);
		density_color_in[2].push_back(b);

	}

	tmpindex.clear();
	tmpl.clear();
	replace_error.clear();
	tmpmagrange_in[0].clear();
	tmpmagrange_in[1].clear();
}

void Cluster::VelProcess(bool existing)
{
	vector<double> zangle;
	vector<double> xyangle;
	vector<double> min_degree;
	vector<int> vel_layer_index;
	vector<double> tmp_order;
	vector<int> tmp_z_order;
	
	velocity_order.clear();

	velocity_color[0].clear();
	velocity_color[1].clear();
	velocity_color[2].clear();
	velocity_hue.clear();

	double r,g,b;
	double h,s,v;

	if(!existing){
		velocity_center[0].clear();
		velocity_center[1].clear();
		velocity_center[2].clear();
		//calculate centers in each cluster
		for(int i=0;i<clusternum_vel[0];i++)
		{
			velocity_center[0].push_back(0);
			velocity_center[1].push_back(0);
			velocity_center[2].push_back(0);
		}//cerr<<"done"<<endl;
		for(int i=0;i<clusternum_vel[0];i++)
		{
			qdotem->getmu(velocity_center[0][i],velocity_center[1][i],velocity_center[2][i],i);
			//cerr<<velocity[0][i]<<endl;
		}
	}
	//cerr<<"done"<<endl;
	//colors
	for(int i=0;i<clusternum_vel[0];i++)
	{
		double z = acos(velocity_center[2][i])/PI * 180;
		double xy = acos(velocity_center[0][i])/PI * 180;
		if(velocity_center[1][i]<=0&&velocity_center[0][i]>=0)
			xy = 180 - xy;
		else  if(velocity_center[1][i]<=0&&velocity_center[0][i]<=0)
			xy = 180 - xy;

		z = (int)(z/10);
		zangle.push_back(z);
		xyangle.push_back(xy);	
	}
	for(int i=0;i<19;i++)
	{
		vel_layer_index.push_back(0);	
	}
	for(int i=0;i<19;i++)
		min_degree.push_back(92);
	whole_min_degree = 92;

	for(int i=0;i<clusternum_vel[0];i++)
	{
		for(int j=i+1;j<clusternum_vel[0];j++)	
		{
			if((int)zangle[j]==(int)zangle[i])
			{
				double dd = abs(xyangle[i] - xyangle[j]);
				if(dd<min_degree[(int)zangle[j]]&&dd>0)
				{	min_degree[(int)zangle[j]] = dd;
				if(min_degree[(int)zangle[j]]<whole_min_degree)
					whole_min_degree = min_degree[(int)zangle[j]];}
			}
		}
		vel_layer_index[(int)zangle[i]]++;
		tmp_order.push_back(0);
	}

	for(int i=0;i<clusternum_vel[0];i++)
	{
		for(int j=i+1;j<clusternum_vel[0];j++)	
		{
			if((int)zangle[j]==(int)zangle[i])
			{
				if(xyangle[j]<xyangle[i])
					tmp_order[i]++;
				else
					tmp_order[j]++;
			}
			//if((int)zangle[j]==(int)zangle[i])
		}

		velocity_order.push_back(0);
	}

	//sort(tmp_z_order.begin(),tmp_z_order.end());

	for(int i=0;i<clusternum_vel[0];i++)
	{
		int sum = 0;
		for(int j=0;j<clusternum_vel[0];j++)
		{
			if((int)zangle[i]>(int)zangle[j])
				sum ++;
		}
		velocity_order[i] = sum + vel_layer_index[(int)zangle[i]]-tmp_order[i]-1;
		//cerr<<order[i]<<endl;
	}
	
	//for(int i=0;i<19;i++)
	//	cerr<<min_degree[i]<<" ";
	//cerr<<endl;
	//int degree = spiralcolors[0].size()/(clusternum_vel[0]+1);
	for(int i=0;i<clusternum_vel[0];i++)
	{
		//cerr<<velocity_order[i]<<endl;
		r = spiralcolors_vel[0][velocity_order[i]];
		g = spiralcolors_vel[1][velocity_order[i]];
		b = spiralcolors_vel[2][velocity_order[i]];
		//cerr<<r<<" "<<g<<" "<<b<<endl;

		velocity_color[0].push_back(r);
		velocity_color[1].push_back(g);
		velocity_color[2].push_back(b);
		velocity_hue.push_back(spiralcolors_vel_hue[velocity_order[i]]);
		//cerr<<min_degree[zangle[i]]<<" "<<h<<" "<<s<<" "<<endl;
	}

	zangle.clear();
	xyangle.clear();
	min_degree.clear();
	vel_layer_index.clear();
	tmp_order.clear();
	tmp_z_order.clear();
}

void Cluster::VelinProcess()
{
	vector<double> zangle;
	vector<double> xyangle;
	vector<double> min_degree;
	vector<int> vel_layer_index;
	vector<double> tmp_order;
	vector<int> tmp_z_order;
	
	velocity_order_in.clear();
	velocity_center_in[0].clear();
	velocity_center_in[1].clear();
	velocity_center_in[2].clear();
	velocity_color_in[0].clear();
	velocity_color_in[1].clear();
	velocity_color_in[2].clear();
	velocity_hue_in.clear();

	double r,g,b;
	double h,s,v;

	//calculate centers in each cluster
	for(int i=0;i<clusternum_vel_in[0];i++)
	{
		velocity_center_in[0].push_back(0);
		velocity_center_in[1].push_back(0);
		velocity_center_in[2].push_back(0);
	}//cerr<<"done"<<endl;
	for(int i=0;i<clusternum_vel_in[0];i++)
	{
		qdotem_in->getmu(velocity_center_in[0][i],velocity_center_in[1][i],velocity_center_in[2][i],i);
		//cerr<<velocity[0][i]<<endl;
	}
	//cerr<<"done"<<endl;
	//colors
	for(int i=0;i<clusternum_vel_in[0];i++)
	{
		double z = acos(velocity_center_in[2][i])/PI * 180;
		double xy = acos(velocity_center_in[0][i])/PI * 180;
		if(velocity_center_in[1][i]<=0&&velocity_center_in[0][i]>=0)
			xy = 180 - xy;
		else  if(velocity_center_in[1][i]<=0&&velocity_center_in[0][i]<=0)
			xy = 180 - xy;

		z = (int)(z/10);
		zangle.push_back(z);
		xyangle.push_back(xy);	
	}
	for(int i=0;i<19;i++)
	{
		vel_layer_index.push_back(0);	
	}
	for(int i=0;i<19;i++)
		min_degree.push_back(92);
	whole_min_degree = 92;

	for(int i=0;i<clusternum_vel_in[0];i++)
	{
		for(int j=i+1;j<clusternum_vel_in[0];j++)	
		{
			if((int)zangle[j]==(int)zangle[i])
			{
				double dd = abs(xyangle[i] - xyangle[j]);
				if(dd<min_degree[(int)zangle[j]]&&dd>0)
				{	min_degree[(int)zangle[j]] = dd;
				if(min_degree[(int)zangle[j]]<whole_min_degree)
					whole_min_degree = min_degree[(int)zangle[j]];}
			}
		}
		vel_layer_index[(int)zangle[i]]++;
		tmp_order.push_back(0);
	}

	for(int i=0;i<clusternum_vel_in[0];i++)
	{
		for(int j=i+1;j<clusternum_vel_in[0];j++)	
		{
			if((int)zangle[j]==(int)zangle[i])
			{
				if(xyangle[j]<xyangle[i])
					tmp_order[i]++;
				else
					tmp_order[j]++;
			}
			//if((int)zangle[j]==(int)zangle[i])
		}

		velocity_order_in.push_back(0);
	}

	//sort(tmp_z_order.begin(),tmp_z_order.end());

	for(int i=0;i<clusternum_vel_in[0];i++)
	{
		int sum = 0;
		for(int j=0;j<clusternum_vel_in[0];j++)
		{
			if((int)zangle[i]>(int)zangle[j])
				sum ++;
		}
		velocity_order_in[i] = sum + vel_layer_index[(int)zangle[i]]-tmp_order[i]-1;
		//cerr<<order[i]<<endl;
	}
	
	//for(int i=0;i<19;i++)
	//	cerr<<min_degree[i]<<" ";
	//cerr<<endl;
	//int degree = spiralcolors[0].size()/(clusternum_vel[0]+1);
	for(int i=0;i<clusternum_vel_in[0];i++)
	{
		//cerr<<velocity_order[i]<<endl;
		r = spiralcolors_vel_in[0][velocity_order_in[i]];
		g = spiralcolors_vel_in[1][velocity_order_in[i]];
		b = spiralcolors_vel_in[2][velocity_order_in[i]];
		//cerr<<r<<" "<<g<<" "<<b<<endl;

		velocity_color_in[0].push_back(r);
		velocity_color_in[1].push_back(g);
		velocity_color_in[2].push_back(b);
		velocity_hue_in.push_back(spiralcolors_vel_in_hue[velocity_order_in[i]]);
		//cerr<<min_degree[zangle[i]]<<" "<<h<<" "<<s<<" "<<endl;
	}

	zangle.clear();
	xyangle.clear();
	min_degree.clear();
	vel_layer_index.clear();
	tmp_order.clear();
	tmp_z_order.clear();
}
bool unique_function (double i, double j) {
  return (fabs(i-j)<=0.0001);
}
void Cluster::SetEMdata(vector<vector<double> >p[3], vector<vector<double> > v[3], vector<vector<double> > d, int zValue,
		int num_mag_cluster)
{
	if(dataSize!=0)
	{
		cleanup_nocolor();
	}

	cerr<<"Start detecting the symmetric mode .... Several minutes are needed...."<<endl;


	vector<int> tmpzi[zValue];
	//vector<int> tmpzi2[zValue];
	vector<float> tmpvel[3][zValue];
	vector<float> tmppos[3][zValue];//cerr<<"1"<<endl;

	int count = 0;
	int countz[zValue];
	int sum=0;
	bool flag = false;

	int mm =0;

	for(int i=0;i<zValue;i++)
	{	
		for(int j=0;j<p[0][i].size();j++)
		{
			//mm++;
			tmpzi[i].push_back(-1);
		}
		countz[i]=0;
	}
	//unique_index.resize(mm);

	dataSize = 0;
	for(int i=0;i<zValue;i++)
	{	
		//cerr<<i<<endl;	
		count = 0;
		for(int j=0;j<p[0][i].size();j++)
		{
			
				//dataSize ++;
				for(int k=j+1;k<p[0][i].size();k++)
				{
					if(fabs(p[0][i][j]+p[0][i][k])<0.0001
					&& fabs(p[1][i][j]-p[1][i][k])<0.0001)//y
					{
						if(fabs(v[0][i][j]+v[0][i][k])<0.0001
						&& fabs(v[1][i][j]-v[1][i][k])<0.0001)
						{
							flag = true;
						}
					}
					else if(fabs(p[1][i][j]+p[1][i][k])<0.0001
					&& fabs(p[0][i][j]-p[0][i][k])<0.0001)//x
					{
						if(fabs(v[1][i][j]+v[1][i][k])<0.0001
						&& fabs(v[0][i][j]-v[0][i][k])<0.0001)
						{
							flag = true;
						}
					}
					else if(fabs(p[1][i][j]+p[1][i][k])<0.0001
					&& fabs(p[0][i][j]+p[0][i][k])<0.0001)//origin
					{
						if(fabs(v[1][i][j]+v[1][i][k])<0.0001
						&& fabs(v[0][i][j]+v[0][i][k])<0.0001)
						{
							flag = true;
						}
					}
					else if(fabs(p[1][i][j]-p[0][i][k])<0.0001
					&& fabs(p[0][i][j]-p[1][i][k])<0.0001)//y=x
					{
						if(fabs(v[1][i][j]-v[0][i][k])<0.0001
						&& fabs(v[0][i][j]-v[1][i][k])<0.0001)
						{
							flag = true;
						}
					}
					else if(fabs(p[1][i][j]+p[0][i][k])<0.0001
					&& fabs(p[0][i][j]+p[1][i][k])<0.0001)//y=-x
					{
						if(fabs(v[1][i][j]+v[0][i][k])<0.0001
						&& fabs(v[0][i][j]+v[1][i][k])<0.0001)
						{
							flag = true;
						}
					}

					if(flag==true)
					{
						if(tmpzi[i][j]==-1&&tmpzi[i][k]==-1)
						{
							count ++;
							sum ++;
							countz[i]++;
							tmpzi[i][j] = count;
							tmpzi[i][k] = count;
							//tmpzi2[i].push_back(count);
							tmpvel[0][i].push_back(v[0][i][j]);
							tmpvel[1][i].push_back(v[1][i][j]);
							tmpvel[2][i].push_back(v[2][i][j]);
							tmppos[0][i].push_back(p[0][i][j]);
							tmppos[1][i].push_back(p[1][i][j]);
							tmppos[2][i].push_back(p[2][i][j]);
						}
						else if(tmpzi[i][j]!=-1)
						{
							tmpzi[i][k] = tmpzi[i][j];
						}
						else if(tmpzi[i][k]!=-1)
						{
							tmpzi[i][j] = tmpzi[i][k];
						}
					}
					flag = false;
				}
				if(tmpzi[i][j]==-1)
				{
				
					countz[i]++;
					count++;
					sum ++;
					tmpzi[i][j] = count;
					//tmpzi2[i].push_back(count);
					tmpvel[0][i].push_back(v[0][i][j]);
					tmpvel[1][i].push_back(v[1][i][j]);
					tmpvel[2][i].push_back(v[2][i][j]);
					tmppos[0][i].push_back(p[0][i][j]);
					tmppos[1][i].push_back(p[1][i][j]);
					tmppos[2][i].push_back(p[2][i][j]);
				}
			//}
			//if(tmpzi[i][j]==1)
			//cerr<<v[0][i][j]<<" "<<v[1][i][j]<<" "<<v[2][i][j]<<" "<<endl;
		}
	}

	//update 
	for(int i=0;i<zValue;i++)
	{
		for(int j=0;j<p[0][i].size();j++)
		{
			if(tmpzi[i][j]!=-1)
			{	
				if(v[0][i][j]>tmpvel[0][i][tmpzi[i][j]-1])
				{
					tmppos[0][i][tmpzi[i][j]-1] = p[0][i][j];
					tmppos[1][i][tmpzi[i][j]-1] = p[1][i][j];
					tmppos[2][i][tmpzi[i][j]-1] = p[2][i][j];
					tmpvel[0][i][tmpzi[i][j]-1] = v[0][i][j];
					tmpvel[1][i][tmpzi[i][j]-1] = v[1][i][j];
					tmpvel[2][i][tmpzi[i][j]-1] = v[2][i][j];
				}	
				else if(v[0][i][j]==tmpvel[0][i][tmpzi[i][j]-1]
					&& v[1][i][j]>tmpvel[1][i][tmpzi[i][j]-1])
				{
					tmppos[0][i][tmpzi[i][j]-1] = p[0][i][j];
					tmppos[1][i][tmpzi[i][j]-1] = p[1][i][j];
					tmppos[2][i][tmpzi[i][j]-1] = p[2][i][j];
					tmpvel[0][i][tmpzi[i][j]-1] = v[0][i][j];
					tmpvel[1][i][tmpzi[i][j]-1] = v[1][i][j];
					tmpvel[2][i][tmpzi[i][j]-1] = v[2][i][j];
				}
			}
		}
	}

	//restore
	int ii = 0;
	velocity_unique.resize(sum);
	velocity_density.resize(sum);
	unique_index.resize(sum);
	for(int i=0;i<sum;i++)
	{
		velocity_unique[i].resize(3);
		//velocity_unique[i][0] = 0;
		//velocity_unique[i][1] = 0;
		//velocity_unique[i][2] = 0;
	}

	max_density = -9e+9;
	min_density = 9e+9;

	for(int i=0;i<zValue;i++)
	{
		for(int j=0;j<p[0][i].size();j++)
		{
			//sym_index.push_back(tmpzi[i][j]+ii-1);
			//if(i==3)cerr<<tmpzi[i][j]+ii-1<<" ";
			//if(in[i][j])
			//{
			dataSize++;
			vector<double> pp;

			pp.push_back(p[0][i][j]);
			pp.push_back(p[1][i][j]);
			pp.push_back(p[2][i][j]);			
			position.push_back(pp);
			pp.clear();

			pp.push_back(v[0][i][j]);
			pp.push_back(v[1][i][j]);
			pp.push_back(v[2][i][j]);			
			velocity.push_back(pp);	
			pp.clear();	

			if(d[i][j]>=max_density)
				max_density = d[i][j];
			if(d[i][j]<=min_density)
				min_density = d[i][j];

			density.push_back(d[i][j]);

			pp.push_back(tmpvel[0][i][tmpzi[i][j]-1]);
			pp.push_back(tmpvel[1][i][tmpzi[i][j]-1]);
			pp.push_back(tmpvel[2][i][tmpzi[i][j]-1]);	

			if(tmpzi[i][j]!=-1)
			{
				pp.push_back(tmpvel[0][i][tmpzi[i][j]-1]);
				pp.push_back(tmpvel[1][i][tmpzi[i][j]-1]);
				pp.push_back(tmpvel[2][i][tmpzi[i][j]-1]);			
			}
			else
			{
				pp.push_back(v[0][i][j]);
				pp.push_back(v[1][i][j]);
				pp.push_back(v[2][i][j]);
			}
			velocity2.push_back(pp);

					
			velocity_unique[tmpzi[i][j]-1+ii] = pp;
			velocity_index.push_back(tmpzi[i][j]-1+ii);
			unique_index[tmpzi[i][j]-1+ii].push_back(dataSize-1);
			pp.clear();	
			//}

		}
		ii = ii + countz[i];
	}
/*
	vector<vector<double> > tmp_unique;
	int array[sum];
	for(int i=0;i<sum;i++)
		array[i] = -1;
	int mm = 0;
	int ucount = 0;
	for(int i=0;i<sum;i++)
	{
		if(!(velocity_unique[i][0]==0&&velocity_unique[i][1]==0&&velocity_unique[i][2]==0))
		{
			tmp_unique.push_back(velocity_unique[i]);		
			array[i] = ucount;
			ucount ++;
		}
		else
		{
			mm++;
		}
	}*/

	/*cerr<<velocity.size()<<" "<<velocity_unique.size()<<" "<<sum<<" "<<ii<<endl;
	ofstream outfile("c_v");
	for(int i=0;i<dataSize;i++)
		outfile<<position[i][0]<<" "<<position[i][1]<<" "<<position[i][2]<<" "
			<<velocity[i][0]<<" "<<velocity[i][1]<<" "<<velocity[i][2]<<" "<<endl;
	outfile.close();*/

	/*for(int i=0;i<zValue;i++)
	{
		for(int j=0;j<tmpvel[0][i].size();j++)
		{
			vector<double> pp;

			pp.push_back(tmpvel[0][i][j]);
			pp.push_back(tmpvel[1][i][j]);
			pp.push_back(tmpvel[2][i][j]);			
			velocity2.push_back(pp);
			pp.clear();
		}
	}*/

	/*ii=0;
	for(int i=0;i<zValue;i++)
	{
		for(int j=0;j<tmpvel[0][i].size();j++)
		{
			vector<double> pp;

			pp.push_back(tmpvel[0][i][j]);
			pp.push_back(tmpvel[1][i][j]);
			pp.push_back(tmpvel[2][i][j]);			
			velocity2[tmpzi2[i][j]+ii-1]=pp;
			pp.clear();
		}
		ii = ii + countz[i];
	}*/

	//unique density
	density_unique = density;
	//cerr<<density_unique.size()<<endl;
	std::sort(density_unique.begin(),density_unique.end());
	std::vector<double>::iterator it;
	it=std::unique(density_unique.begin(), density_unique.end());
	density_unique.resize( std::distance(density_unique.begin(),it) );
	unique_index2.resize(density_unique.size());
	
	//cerr<<density_unique.size()<<endl;
	/*if(density_unique.size()==109)
		for(int i=0;i<109;i++)
			cerr<<density_unique[i]<<endl;*/

	for(int i=0;i<dataSize;i++)
	{
		it = find(density_unique.begin(),density_unique.end(),density[i]);
		int index = std::distance(density_unique.begin(),it);
		density_index.push_back(index);
		unique_index2[index].push_back(i);
	}

	//release
	for(int i=0;i<zValue;i++)
	{
		tmpzi[i].clear();
		tmpvel[0][i].clear();
		tmpvel[1][i].clear();
		tmpvel[2][i].clear();
		//tmpzi2[i].clear();
		tmppos[0][i].clear();
		tmppos[1][i].clear();
		tmppos[2][i].clear();
	}

	clusternum_mag[0] = num_mag_cluster;
	clusternum_vel[0] = 0;
	
	IDX_Vel = new int[dataSize];
	IDX_Mag = new int[dataSize];
	IDX_Vel_In = new int[dataSize];
	IDX_Mag_In = new int[dataSize];

	for(int i=0;i<dataSize;i++)
	{
		IDX_Vel[i] = -1;
		IDX_Mag[i] = -1;
		IDX_Mag_In[i] = -1;
		IDX_Vel_In[i] = -1;
	}

	range_mag[0] = min_density;
	range_mag[1] = max_density;

	/*int *IDX_tmp = new int[density_unique.size()];

	for(int i=0;i<dataSize;i++)
	{
		IDX_Vel[i] = -1;
	}

	//qdotem->SetData(position,density_unique,velocity2,dataSize,density_unique.size());
	qdotem->SetData(position,density_unique,velocity_unique,velocity_unique.size(),density_unique.size());
	
	clusternum_error_mag = 1;
	cout<<"EM for Gaussian mixture: running ... "<<endl;
	clusternum_error_mag = qdotem->EM(clusternum_mag[0],IDX_tmp,false,true);
	for(int i=0;i<dataSize;i++)
	{
		IDX_Mag[i] = IDX_tmp[density_index[i]];
		velocity_density[velocity_index[i]].push_back(IDX_Mag[i]);
	}
	delete [] IDX_tmp;
	//cerr<<"1"<<endl;
	//cerr<<clusternum_error_mag<<endl;
	//ReadColors();
	MagProcess();*/
}
void Cluster::SetEMdata2(vector<vector<double> >p[3],vector<vector<double> > v[3], vector<vector<double> > d, int zValue,
		int num_mag_cluster)
{
	if(dataSize!=0)
	{
		cleanup_nocolor();
	}

	int ii = 0;

	dataSize = 0;
	max_density = -9e+9;
	min_density = 9e+9;

	for(int i=0;i<zValue;i++)
	{
		for(int j=0;j<p[0][i].size();j++)
		{
			//sym_index.push_back(tmpzi[i][j]+ii-1);
			//if(i==3)cerr<<tmpzi[i][j]+ii-1<<" ";

			vector<double> pp;

			pp.push_back(p[0][i][j]);
			pp.push_back(p[1][i][j]);
			pp.push_back(p[2][i][j]);			
			position.push_back(pp);
			pp.clear();

			pp.push_back(v[0][i][j]);
			pp.push_back(v[1][i][j]);
			pp.push_back(v[2][i][j]);			
			velocity.push_back(pp);	
			velocity2.push_back(pp);

			if(d[i][j]>=max_density)
				max_density = d[i][j];
			if(d[i][j]<=min_density)
				min_density = d[i][j];
			density.push_back(d[i][j]);

			velocity_unique.push_back(pp);
			velocity_index.push_back(ii);
			pp.clear();
	
			ii++;
			dataSize++;

		}
	}
	velocity_density.resize(dataSize);
	unique_index.resize(dataSize);
	//unique density
	density_unique = density;
	//cerr<<density_unique.size()<<endl;
	std::sort(density_unique.begin(),density_unique.end());
	std::vector<double>::iterator it;
	it=std::unique(density_unique.begin(), density_unique.end());
	density_unique.resize( std::distance(density_unique.begin(),it) );
	unique_index2.resize(density_unique.size());	

	for(int i=0;i<dataSize;i++)
	{
		it = find(density_unique.begin(),density_unique.end(),density[i]);
		int index = std::distance(density_unique.begin(),it);
		density_index.push_back(index);
		unique_index2[index].push_back(i);
		unique_index[i].push_back(i);
	}


	clusternum_mag[0] = num_mag_cluster;
	clusternum_vel[0] = 0;
	
	IDX_Vel = new int[dataSize];
	IDX_Mag = new int[dataSize];
	IDX_Vel_In = new int[dataSize];
	IDX_Mag_In = new int[dataSize];

	for(int i=0;i<dataSize;i++)
	{
		IDX_Vel[i] = -1;
		IDX_Mag[i] = -1;
		IDX_Mag_In[i] = -1;
		IDX_Vel_In[i] = -1;
	}
}
void Cluster::DrawMagLegend()
{
    /*GLdouble vmat[16], pmat[16], sx, sy, sz;
    GLint    viewport[4];

    glGetDoublev(GL_MODELVIEW_MATRIX, vmat);
    glGetDoublev(GL_PROJECTION_MATRIX, pmat);
    glGetIntegerv(GL_VIEWPORT, viewport);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,(GLfloat) viewport[2], 0.0, (GLfloat) viewport[3]);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_LIGHTING);*/

	/*glColor3f(1,1,1);
	
	sprintf(str,"Magnitude");
	glRasterPos2f((GLfloat)viewport[2]*9./10.,(GLfloat) viewport[3]/2.-20);
		for(int j=0;j<strlen(str);j++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[j]);*/
	glPushMatrix();


	//glTranslatef(viewport[2]/158., (GLfloat) viewport[3]-(GLfloat) viewport[3]/42.,0.);
	//glScalef(10.,20.,1.);
	char str[50];
	float x = 0;
	float y = 0;
	widget_l[0] = x;
	widget_l[1] = y;
	float size = 1;	
	m_length = 1;

	glColor3f(1.,1.,1.);
	//cerr<<min_density<<" "<<magrange[0][IDX_sort_mag[clusternum_mag[0]-1]]<<endl;
	if(fabs(min_density - magrange[0][IDX_sort_mag[clusternum_mag[0]-1]])<=0.00001)
	{}
	else
	{
		sprintf(str,"%6.2e",min_density/pow(10.,d_scale));
		glRasterPos2f(x + size*1.1, y-size/2. );
		for(int j=0;j<strlen(str);j++)
		     glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

		glBegin(GL_LINES);
		glVertex2f(x+size/2.,y);
		glVertex2f(x+size/2.,y+size);
		glEnd();
		glBegin(GL_LINES);
		glVertex2f(x,y);
		glVertex2f(x+size,y);
		glEnd();
		y = y + size;
		m_length = m_length + 1.;
	}
	for(int i=0;i<clusternum_mag[0];i++)
	{
		glColor3f(density_color[0][clusternum_mag[0]-i-1],
			density_color[1][clusternum_mag[0]-i-1],
			density_color[2][clusternum_mag[0]-i-1]);

		glBegin(GL_QUADS);
		glVertex2f(x,y);
		glVertex2f(x+size,y);
		glVertex2f(x+size,y+size);
		glVertex2f(x,y+size);
		glEnd();

		//if(i+1>=Index_Vel[0]
		//&&i+1<=Index_Vel[1])
		//	glColor3f(254./255.,178./255.,76./255.);
		//else
			glColor3f(1.,1.,1.);
	
		if(i<clusternum_mag[0]-1)
		sprintf(str,"%6.2e - %6.2e",magrange[0][IDX_sort_mag[clusternum_mag[0]-i-1]]/pow(10.,d_scale), 
				magrange[0][IDX_sort_mag[clusternum_mag[0]-(i+1)-1]]/pow(10.,d_scale));
		else
		sprintf(str,"%6.2e - %6.2e",magrange[0][IDX_sort_mag[clusternum_mag[0]-i-1]]/pow(10.,d_scale), 
				magrange[1][IDX_sort_mag[clusternum_mag[0]-i-1]]/pow(10.,d_scale));

		glRasterPos2f(x + size*1.1, y );
		for(int j=0;j<strlen(str);j++)
		     glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

		sprintf(str,"%d",i+1);
		glRasterPos2f(x - size*1.3, y);
		for(int j=0;j<strlen(str);j++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

		y = y + size;
		m_length = m_length + 1.;
	}
	glColor3f(1.,1.,1.);
	if(fabs(max_density - magrange[1][IDX_sort_mag[0]])<=0.00001)
	{}
	else
	{
		sprintf(str,"%6.2e",max_density/pow(10.,d_scale));
		glRasterPos2f(x + size*1.1, y + size );
		for(int j=0;j<strlen(str);j++)
		     glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

		glBegin(GL_LINES);
		glVertex2f(x+size/2.,y);
		glVertex2f(x+size/2.,y+size);
		glEnd();
		glBegin(GL_LINES);
		glVertex2f(x,y+size);
		glVertex2f(x+size,y+size);
		glEnd();
		y = y + size;
		m_length = m_length + 1.;
	}
	widget_h[0]=x;
	widget_h[1] = y;
	glPopMatrix();

  /*  glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(pmat);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(vmat);	*/
}

void Cluster::RenderWidget()
{
	glEnable(GL_BLEND);

	float l = 1;
	char str[20];

	glPushMatrix();

	if(widget_select[0])
		glColor4f(1.,1.,0.,0.5);
	else
		glColor4f(1.,1.,1.,0.5);
	glTranslatef(0.,widget_t_l,0);
	glBegin(GL_TRIANGLES);
	glVertex2f(widget_l[0],widget_l[1]);
	glVertex2f(widget_l[0]-l,widget_l[1]+l/sqrt(3.));
	glVertex2f(widget_l[0]-l,widget_l[1]-l/sqrt(3.));	
	glEnd();	

	glColor3f(1.,1.,1.);
	sprintf(str,"%6.2e",range_mag[0]/pow(10.,d_scale));
	glRasterPos2f(widget_l[0]-l-1.,widget_l[1]-l/sqrt(3.));
	for(int j=0;j<strlen(str);j++)
		   glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

	glPopMatrix();

	glPushMatrix();

	if(widget_select[1])
		glColor4f(1.,1.,0.,0.5);
	else
		glColor4f(1.,1.,1.,0.5);
	glTranslatef(0.,widget_t_h,0);
	glBegin(GL_TRIANGLES);
	glVertex2f(widget_h[0],widget_h[1]);
	glVertex2f(widget_h[0]-l,widget_h[1]+l/sqrt(3.));
	glVertex2f(widget_h[0]-l,widget_h[1]-l/sqrt(3.));	
	glEnd();	

	glColor3f(1.,1.,1.);
	sprintf(str,"%6.2e",range_mag[1]/pow(10.,d_scale));
	glRasterPos2f(widget_h[0]-l-1.,widget_h[1]-l/sqrt(3.));
	for(int j=0;j<strlen(str);j++)
		   glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

	glPopMatrix();
}
void Cluster::selectwidget(int x, int y, int x2, int y2)
{
	if(x>=widget_l[0]-2.&&x<=widget_l[0]+1.
		&&y>=widget_l[1]-1.+widget_t_l&&y<=widget_l[1]+1.+widget_t_l)
	{
		widget_select[0]=true;
	}
	else 
	{
		widget_select[0]=false;
	}

	if(x>=widget_h[0]-2.&&x<=widget_h[0]+1.
		&&y>=widget_h[1]-1.+widget_t_h&&y<=widget_h[1]+1.+widget_t_h)
	{
		widget_select[1]=true;
	}
	else 
	{
		widget_select[1]=false;
	}		
	
	if(widget_select[1]&&widget_select[0])
	{
		if(fabs(widget_t_l)>fabs(widget_t_h))
		{
			widget_select[1]=false;
		}
		else
		{
			widget_select[0]=false;
		}
	}
	GetMagRange();
	mouse_x = x2;
	mouse_y = y2;
}

void Cluster::GetMagRange()
{
	float min = widget_h[1] - widget_l[1];
	min = min/(float)clusternum_mag[0];
	
	if(widget_select[0])
	{
		float index = widget_t_l/min;
		if(index == clusternum_mag[0])index = index - 1;
		//index = (clusternum_mag[0] - index-1);

		float distance = widget_t_l - min * (int)index;

		int index2 = (int)(clusternum_mag[0] - (int)index);
		range_mag[0] = min_degree_mag[index2] 
			+ (min_degree_mag[index2-1] - min_degree_mag[index2]) * distance/min;
		range_mag[0] = pow(10.,range_mag[0]);
	}
	if(widget_select[1])
	{
		float index = widget_t_h/min;
		if(index == clusternum_mag[0])index = index - 1;
		//index = (clusternum_mag[0] - index-1);

		float distance = widget_t_h - min * (int)index;

		int index2 = (int)(index);
		range_mag[1] = min_degree_mag[index2] 
			- (min_degree_mag[index2+1] - min_degree_mag[index2]) * distance/min;
		range_mag[1] = pow(10.,range_mag[1]);
	}
}

void Cluster::movewidget(int x,int y)
{
	float deltay = y - mouse_y;

	if(widget_select[0])
	{
		if((widget_l[1] + deltay + widget_t_l) >= widget_l[1]
		&& (widget_l[1] + deltay + widget_t_l) <= (widget_h[1] + widget_t_h))
		{
			widget_t_l  = widget_t_l + deltay;
		}
	}	

	if(widget_select[1])
	{
		if((widget_h[1] + deltay + widget_t_h) <= widget_h[1]
		&& (widget_h[1] + deltay + widget_t_h) >= (widget_l[1] + widget_t_l))
		{
			widget_t_h  = widget_t_h + deltay;
		}
	}

	mouse_x = x;
	mouse_y = y;
}

void Cluster::DrawMagLegend_in()
{
	glPushMatrix();


	//glTranslatef(viewport[2]/158., (GLfloat) viewport[3]-(GLfloat) viewport[3]/42.,0.);
	//glScalef(10.,20.,1.);
	char str[50];
	float x = 0;
	float y = 0;
	float size = 1;	
	m_length_in = 1;

	glColor3f(1.,1.,1.);

	for(int i=0;i<clusternum_mag_in[0];i++)
	{
		glColor3f(density_color_in[0][clusternum_mag_in[0]-i-1],
			density_color_in[1][clusternum_mag_in[0]-i-1],
			density_color_in[2][clusternum_mag_in[0]-i-1]);

		glBegin(GL_QUADS);
		glVertex2f(x,y);
		glVertex2f(x+size,y);
		glVertex2f(x+size,y+size);
		glVertex2f(x,y+size);
		glEnd();


		glColor3f(1.,1.,1.);
	
		if(i<clusternum_mag_in[0]-1)
		sprintf(str,"%6.2e - %6.2e",magrange_in[0][IDX_sort_mag_in[clusternum_mag_in[0]-i-1]]/pow(10.,d_scale), 
				magrange_in[0][IDX_sort_mag_in[clusternum_mag_in[0]-(i+1)-1]]/pow(10.,d_scale));
		else
		sprintf(str,"%6.2e - %6.2e",magrange_in[0][IDX_sort_mag_in[clusternum_mag_in[0]-i-1]]/pow(10.,d_scale), 
				magrange_in[1][IDX_sort_mag_in[clusternum_mag_in[0]-i-1]]/pow(10.,d_scale));

		glRasterPos2f(x + size*1.1, y );
		for(int j=0;j<strlen(str);j++)
		     glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

		sprintf(str,"%d",i+1);
		glRasterPos2f(x - size*1.3, y);
		for(int j=0;j<strlen(str);j++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

		y = y + size;
		m_length_in = m_length_in + 1.;
	}

	glPopMatrix();

}
void Cluster::mag_display(bool *in) {
  	
    glPointSize(4);

  float s = 0.5;

   for(int i=0;i<position.size();i++)
   {
	   if(position[i][2]==zmin)
	   {

			if(IDX_Mag[i]!=-1&&in[i]&&IDX_Mag_In[i]==-1)
			{
				glColor3f(density_color[0][IDX_mag_order[IDX_Mag[i]]],
					density_color[1][IDX_mag_order[IDX_Mag[i]]],
					density_color[2][IDX_mag_order[IDX_Mag[i]]]);
			}
			else if(IDX_Mag[i]!=-1&&!in[i]&&IDX_Mag_In[i]==-1)
			     glColor4f(density_color[0][IDX_mag_order[IDX_Mag[i]]],
					density_color[1][IDX_mag_order[IDX_Mag[i]]],
					density_color[2][IDX_mag_order[IDX_Mag[i]]],0.1);
			else if(IDX_Mag[i]!=-1&&in[i]&&IDX_Mag_In[i]!=-1)
			{
				glColor3f(density_color_in[0][IDX_mag_order_in[IDX_Mag_In[i]]],
					density_color_in[1][IDX_mag_order_in[IDX_Mag_In[i]]],
					density_color_in[2][IDX_mag_order_in[IDX_Mag_In[i]]]);
			}
			else if(IDX_Mag[i]!=-1&&!in[i]&&IDX_Mag_In[i]!=-1)
				glColor4f(density_color_in[0][IDX_mag_order_in[IDX_Mag_In[i]]],
					density_color_in[1][IDX_mag_order_in[IDX_Mag_In[i]]],
					density_color_in[2][IDX_mag_order_in[IDX_Mag_In[i]]],0.1);			
  
		if(IDX_Mag[i]!=-1){//IDX_Mag[i]!=-1&&IDX_Vel[i]!=-1){
		   glBegin(GL_LINES);

		   glVertex3f(position[i][0],position[i][1],position[i][2]);
		   glVertex3f(position[i][0]+s*velocity[i][0],
			position[i][1]+s*velocity[i][1],position[i][2]+s*velocity[i][2]);

		   glEnd();
		glPointSize(4);
		 glBegin(GL_POINTS);
		   glVertex3f(position[i][0]+s*velocity[i][0],
		position[i][1]+s*velocity[i][1],
		position[i][2]+s*velocity[i][2]);glEnd();}
		/*else //if(IDX_Mag[i]!=-1)
		{
			glPointSize(1);
			
		
		 glBegin(GL_POINTS);
		   glVertex3f(position[i][0],
		position[i][1],
		position[i][2]);glEnd();
		}*/
	   }
   }

   glPointSize(1.);

}
void Cluster::mag_display2() {
  	
    glPointSize(4);

  float s = 0.5;

   for(int i=0;i<position.size();i++)
   {


			if(IDX_Mag[i]!=-1)
			{
				glColor3f(density_color[0][IDX_mag_order[IDX_Mag[i]]],
					density_color[1][IDX_mag_order[IDX_Mag[i]]],
					density_color[2][IDX_mag_order[IDX_Mag[i]]]);
			}
			   else
				   glColor4f(1.,1.,1.,0.1);
  
		if(IDX_Mag[i]!=-1&&IDX_Vel[i]!=-1){
		   glBegin(GL_LINES);

		   glVertex3f(position[i][0],position[i][1],position[i][2]);
		   glVertex3f(position[i][0]+s*velocity[i][0],
			position[i][1]+s*velocity[i][1],position[i][2]+s*velocity[i][2]);

		   glEnd();
		glPointSize(4);
		 glBegin(GL_POINTS);
		   glVertex3f(position[i][0]+s*velocity[i][0],
		position[i][1]+s*velocity[i][1],
		position[i][2]+s*velocity[i][2]);glEnd();}
		else if(IDX_Mag[i]!=-1)
		{
			glPointSize(1);
		 glBegin(GL_POINTS);
		   glVertex3f(position[i][0]+velocity[i][0],
		position[i][1]+velocity[i][1],
		position[i][2]+velocity[i][2]);glEnd();
			
		}

	   
   }

   glPointSize(1.);

}
void Cluster::DrawVelLegend(int n)
{
	glEnable(GL_DEPTH_TEST);
	//glPushMatrix();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glLineWidth(1);
        glColor3f(1.,1,1);

        glBegin(GL_LINES);
        glVertex3f(-1.2,0,0);
        glVertex3f(1.2,0,0);
        glEnd();
        glRasterPos3f(1.2,0,0);
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'x');

	glPushMatrix();
	glTranslatef(1.2,0.,0.);
	glRotatef(90,0.,1.,0.);
	glutSolidCone(0.1,0.2,4,4);
	glPopMatrix();

        glBegin(GL_LINES);
        glVertex3f(0,-1.2,0);
        glVertex3f(0,1.2,0);
        glEnd();
        glRasterPos3f(0,1.2,0);
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'y');

	glPushMatrix();
	glTranslatef(0.,1.2,0.);
	glRotatef(-90,1.,0.,0.);
	glutSolidCone(0.1,0.2,4,4);
	glPopMatrix();

        glBegin(GL_LINES);
        glVertex3f(0,0,-1.2);
        glVertex3f(0,0,1.2);
        glEnd();
        glRasterPos3f(0,0,1.2);
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'z');

	glPushMatrix();
	glTranslatef(0.,0.,1.2);
	glutSolidCone(0.1,0.2,4,4);
	glPopMatrix();

	//glScalef(20.,20.,20.);
	glPointSize(2.);
	char str[10];
	//int dataSize = qdotField->GetDataSize();

	for(int i=0;i<dataSize;i++)
	{
		if(IDX_Vel[i]!=-1)
		{
			glColor3f(velocity_color[0][IDX_Vel[i]],
				velocity_color[1][IDX_Vel[i]],velocity_color[2][IDX_Vel[i]]);

			glBegin(GL_POINTS);
			//cerr<<velocity[i][0]<<" ";
			glVertex3f(velocity[i][0],velocity[i][1],velocity[i][2]);
			glEnd();

			if(i==n)
			{
				glColor3f(0.,0.,0.);
				glBegin(GL_LINES);
				glVertex3f(velocity[i][0],velocity[i][1],velocity[i][2]);
				glVertex3f(velocity[i][0]*1.5,velocity[i][1]*1.5,velocity[i][2]*1.5);
				glEnd();
			}
		}
	}

		
	/*for(int i=0;i<dataSize;i++)
	{
		if(IDX_Vel[i]!=-1)
		{
			glColor3f(velocity_color[0][IDX_Vel[i]],
				velocity_color[1][IDX_Vel[i]],velocity_color[2][IDX_Vel[i]]);

			glBegin(GL_POINTS);
			//cerr<<velocity[i][0]<<" ";
			glVertex3f(velocity[i][0],velocity[i][1],velocity[i][2]);
			glEnd();
		}
	}*/
	//glutSolidSphere(1,10,10);

	//glPopMatrix();
	glPointSize(1.);
	
}
void Cluster::DrawVelLegend_in()
{
	glEnable(GL_DEPTH_TEST);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glLineWidth(1);
        glColor3f(1.,1,1);

        glBegin(GL_LINES);
        glVertex3f(-1.2,0,0);
        glVertex3f(1.2,0,0);
        glEnd();
        glRasterPos3f(1.2,0,0);
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'x');

	glPushMatrix();
	glTranslatef(1.2,0.,0.);
	glRotatef(90,0.,1.,0.);
	glutSolidCone(0.1,0.2,4,4);
	glPopMatrix();

        glBegin(GL_LINES);
        glVertex3f(0,-1.2,0);
        glVertex3f(0,1.2,0);
        glEnd();
        glRasterPos3f(0,1.2,0);
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'y');

	glPushMatrix();
	glTranslatef(0.,1.2,0.);
	glRotatef(-90,1.,0.,0.);
	glutSolidCone(0.1,0.2,4,4);
	glPopMatrix();

        glBegin(GL_LINES);
        glVertex3f(0,0,-1.2);
        glVertex3f(0,0,1.2);
        glEnd();
        glRasterPos3f(0,0,1.2);
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'z');

	glPushMatrix();
	glTranslatef(0.,0.,1.2);
	glutSolidCone(0.1,0.2,4,4);
	glPopMatrix();

	//glScalef(20.,20.,20.);
	glPointSize(2.);
	char str[10];
	//int dataSize = qdotField->GetDataSize();

	for(int i=0;i<dataSize;i++)
	{
		if(IDX_Vel_In[i]!=-1)
		{
			glColor3f(velocity_color_in[0][IDX_Vel_In[i]],
				velocity_color_in[1][IDX_Vel_In[i]],velocity_color_in[2][IDX_Vel_In[i]]);

			glBegin(GL_POINTS);
			//cerr<<velocity[i][0]<<" ";
			glVertex3f(velocity[i][0],velocity[i][1],velocity[i][2]);
			glEnd();
		}
	}

		
	/*for(int i=0;i<dataSize;i++)
	{
		if(IDX_Vel[i]!=-1)
		{
			glColor3f(velocity_color[0][IDX_Vel[i]],
				velocity_color[1][IDX_Vel[i]],velocity_color[2][IDX_Vel[i]]);

			glBegin(GL_POINTS);
			//cerr<<velocity[i][0]<<" ";
			glVertex3f(velocity[i][0],velocity[i][1],velocity[i][2]);
			glEnd();
		}
	}*/
	//glutSolidSphere(1,10,10);

	//glPopMatrix();
	glPointSize(1.);
	
}
void Cluster::vel_display(bool *in) { 

   glPointSize(4);
	float s =0.5;

   for(int i=0;i<position.size();i++)
   {
	   if(position[i][2]==zmin)
	   {

			if(IDX_Vel[i]!=-1&&velocity_color[0].size()>0&&in[i]&&IDX_Vel_In[i]==-1)
			{		   glColor3f(velocity_color[0][IDX_Vel[i]],
				velocity_color[1][IDX_Vel[i]],
				velocity_color[2][IDX_Vel[i]]);
			}
			else if(IDX_Vel[i]!=-1&&velocity_color[0].size()>0&&IDX_Vel_In[i]==-1)
			{		   glColor4f(velocity_color[0][IDX_Vel[i]],
				velocity_color[1][IDX_Vel[i]],
				velocity_color[2][IDX_Vel[i]],0.1);
			}
			else if(IDX_Vel[i]!=-1&&velocity_color[0].size()>0&&in[i]&&IDX_Vel_In[i]!=-1)
			{		   glColor3f(velocity_color_in[0][IDX_Vel_In[i]],
				velocity_color_in[1][IDX_Vel_In[i]],
				velocity_color_in[2][IDX_Vel_In[i]]);
			}
			else if(IDX_Vel[i]!=-1&&velocity_color[0].size()>0&&IDX_Vel_In[i]!=-1)
			{	glColor4f(velocity_color_in[0][IDX_Vel_In[i]],
				velocity_color_in[1][IDX_Vel_In[i]],
				velocity_color_in[2][IDX_Vel_In[i]],0.1);
			}
		if(IDX_Vel[i]!=-1&&velocity_color[0].size()>0){
		glPointSize(4);
		glBegin(GL_POINTS);	

		   glVertex3f(position[i][0]+s*velocity[i][0],
		position[i][1]+s*velocity[i][1],
		position[i][2]+s*velocity[i][2]);

		   glEnd();
		   
		   glBegin(GL_LINES);

		   glVertex3f(position[i][0],position[i][1],position[i][2]);
		   glVertex3f(position[i][0]+s*velocity[i][0],
			position[i][1]+s*velocity[i][1],position[i][2]+s*velocity[i][2]);

		   glEnd();}//}
		/* else if(IDX_Vel[i]==-1)
			{		   glColor4f(1,1,1,0.1);
		glPointSize(1.);
		glBegin(GL_POINTS);	

		   glVertex3f(position[i][0]+s*velocity[i][0],
		position[i][1]+s*velocity[i][1],
		position[i][2]+s*velocity[i][2]);

		   glEnd();
		   
		   glBegin(GL_LINES);

		   glVertex3f(position[i][0],position[i][1],position[i][2]);
		   glVertex3f(position[i][0]+s*velocity[i][0],
			position[i][1]+s*velocity[i][1],position[i][2]+s*velocity[i][2]);

		   glEnd();}*/
	   }
   }
   glPointSize(1.);


}
void Cluster::vel_display2() { //need to be improved!!!

   glPointSize(4);
	float s =0.5;


   for(int i=0;i<position.size();i++)
   {
	   if(position[i][2]==-7.75)
	   {

			if(IDX_Vel[i]!=-1&&velocity_color[0].size()>0)
			{		   glColor3f(velocity_color[0][IDX_Vel[i]],
				velocity_color[1][IDX_Vel[i]],
				velocity_color[2][IDX_Vel[i]]);
	
				glBegin(GL_POINTS);	

				   glVertex3f(position[i][0]+s*velocity[i][0],
				position[i][1]+s*velocity[i][1],
				position[i][2]+s*velocity[i][2]);

				   glEnd();
				   
				   glBegin(GL_LINES);

				   glVertex3f(position[i][0],position[i][1],position[i][2]);
				   glVertex3f(position[i][0]+s*velocity[i][0],
					position[i][1]+s*velocity[i][1],position[i][2]+s*velocity[i][2]);

				   glEnd();
			}
	   }
   }
   glPointSize(1.);


}
/*
void Cluster::GetVelClusters(int c1, int c2)
{
	vector< vector<double> >p_vel;
	vector< vector<double> >v_vel;
	//vector<int> v_index;
	vector<double>d_vel;

	string tmp2;
	float tmpnum2;

	if(c2>=c1&&c1>=1&&c2<=clusternum_mag[0])
	{	
		for(int i=0;i<dataSize;i++)
			IDX_Vel[i] = -1;

		//p_vel.clear();
		v_vel.clear();
		//d_vel.clear();
		//v_index.clear();

		int count =0;
		bool flag = false;
		for(int i=0;i<dataSize;i++)
		{
			flag = false;
			if(IDX_mag_order[IDX_Mag[i]]<=clusternum_mag[0]-c1
			&&IDX_mag_order[IDX_Mag[i]]>=clusternum_mag[0]-c2)
			{
				p_vel.push_back(position[i]);

					v_vel.push_back(velocity2[i]);
					//v_index.push_back(sym_index[i]);
					count ++;
				//}
				d_vel.push_back(1);
				
			}
		}
		cerr<<"count"<<count<<endl;
		qdotem->SetData(p_vel,d_vel,v_vel,v_vel.size(),v_vel.size());
		
		if(clusternum_vel[0]>=0&&clusternum_vel[0]<=50&&clusternum_vel[0]<=count)
		{  
			int *IDX_tmp = new int[v_vel.size()];
			cout<<"EM for Gaussian mixture: running ... "<<endl;
		 	qdotem->EM(clusternum_vel[0],IDX_tmp,false,false);

		 	int ii=0;
			int j=0;
		  	for(int i=0;i<dataSize;i++)
		  	{
				if(IDX_mag_order[IDX_Mag[i]]<=clusternum_mag[0]-c1
				&&IDX_mag_order[IDX_Mag[i]]>=clusternum_mag[0]-c2)
				{

					IDX_Vel[i] = IDX_tmp[j];j++;
					//cerr<<IDX_Vel[i]<<" ";
				}
			}		
			VelProcess();	
			delete [] IDX_tmp;	
		}
		else
			cout<<"You input two invalid numbers "<<clusternum_vel[0]<<endl;
	}
	else
	{		
		cout<<"You input two invalid numbers "<<c1<<" "<<c2<<endl;
	}
	//VelProcess();

}*/

void Cluster::Savemagtofile(char *file)
{
	ofstream infile(file);
	
	infile<<clusternum_mag[0]<<endl;;
	for(int i=0;i<dataSize;i++)
	{
		infile<<IDX_Mag[i]<<endl;
	}

	infile.close();
}
void Cluster::Saveveltofile(char *file)
{
	ofstream infile(file);
	
	infile<<clusternum_vel[0]<<endl;
	for(int i=0;i<dataSize;i++)
	{
		infile<<IDX_Vel[i]<<endl;
	}
	infile<<endl;

	for(int i=0;i<clusternum_vel[0];i++)
	{
		infile<<velocity_center[0][i]<<" "<<velocity_center[1][i]<<" "<<velocity_center[2][i]<<endl;	
	}

	infile.close();
}
void Cluster::GetMagClusters_file(char *file)
{clusternum_error_mag = 0;
	ifstream infile(file);
	
	float store;
	infile>>store;
	clusternum_mag[0] = store;
	for(int i=0;i<dataSize;i++)
	{
		infile>>store;
		IDX_Mag[i] = store;
	}
	MagProcess();

	infile.close();

}

void Cluster::GetVelClusters_file(char *file)
{
	ifstream infile(file);

	float store;
	infile>>store;
	clusternum_vel[0] = store;
	for(int i=0;i<dataSize;i++)
	{
		infile>>store;
		IDX_Vel[i] = store;
	}

	velocity_center[0].clear();
	velocity_center[1].clear();
	velocity_center[2].clear();
	for(int i=0;i<clusternum_vel[0];i++)
	{
		double t[3];
		infile>>t[0]>>t[1]>>t[2];
		velocity_center[0].push_back(t[0]);
		velocity_center[1].push_back(t[1]);
		velocity_center[2].push_back(t[2]);
		//calculate centers in each cluster
	
	}

	VelProcess(true);

	infile.close();

}

void Cluster::GetVelClusters(int c1, int c2)
{
	Index_Vel[0] = c1;
	Index_Vel[1] = c2;

	vector< vector<double> >p_vel;
	vector< vector<double> >v_vel;
	vector<double>d_vel;
	vector<int> v_index;

	string tmp2;
	float tmpnum2;
	//cerr<<c1<<" "<<c2<<endl;

	if(c2>=c1&&c1>=1&&c2<=clusternum_mag[0])
	{	
		for(int i=0;i<dataSize;i++)
			IDX_Vel[i] = -1;


		int count =0;
		bool flag = false;

		for(int i=0;i<velocity_unique.size();i++)
		{
			flag = false;
			bool in = false;
			for(int t=0;t<velocity_density[i].size();t++)
			if(IDX_mag_order[velocity_density[i][t]]<=clusternum_mag[0]-c1
			&&IDX_mag_order[velocity_density[i][t]]>=clusternum_mag[0]-c2)
			{
				//cerr<<velocity_density[i].size()<<" ";
				in = true;break;
			}
			if(in)
			{
				p_vel.push_back(position[i]);
				v_vel.push_back(velocity_unique[i]);
				//count ++;
				d_vel.push_back(1.);
				v_index.push_back(i);
				
			}
		}//cerr<<v_vel.size()<<endl;
		//cerr<<"count"<<count<<endl;
		qdotem->SetData(p_vel,d_vel,v_vel,v_vel.size(),v_vel.size());
		
		if(clusternum_vel[0]>=2&&clusternum_vel[0]<=v_vel.size())
		{  
			int *IDX_tmp = new int[v_vel.size()];
			int *IDX_tmp2 = new int[velocity_unique.size()];
			cout<<"EM for Gaussian mixture: running ... "<<endl;
		 	qdotem->EM(clusternum_vel[0],IDX_tmp,false,false);
			//cerr<<v_vel.size()<<endl;
		 	int ii=0;
			int j=0;
			for(int i=0;i<velocity_unique.size();i++)
			{
				IDX_tmp2[i] = -1;
			}//cerr<<v_vel.size()<<endl;
			for(int i=0;i<v_index.size();i++)
			{
				IDX_tmp2[v_index[i]] = IDX_tmp[i];

			}//cerr<<v_vel.size()<<endl;
		  	for(int i=0;i<dataSize;i++)
		  	{
				if(IDX_mag_order[IDX_Mag[i]]<=clusternum_mag[0]-c1
				&&IDX_mag_order[IDX_Mag[i]]>=clusternum_mag[0]-c2)
				{
					IDX_Vel[i] = IDX_tmp2[velocity_index[i]];
				}
			}	//cerr<<v_vel.size()<<endl;	
			VelProcess(false);	
			delete [] IDX_tmp;
			delete [] IDX_tmp2;	
		}
		else
			cout<<"You input two invalid numbers "<<clusternum_vel[0]<<endl;
	}
	else
	{		
		cout<<"You input two invalid numbers "<<c1<<" "<<c2<<endl;
	}

	for(int i=0;i<d_vel.size();i++)
	{
		v_vel[i].clear();
		p_vel[i].clear();
	}
	d_vel.clear();
	v_vel.clear();
	p_vel.clear();
	v_index.clear();
	//VelProcess();

}
void Cluster::GetMagClusters()
{
	Index_Vel[0] = -1;
	Index_Vel[1] = -1;

	if(clusternum_mag[0]>=2&&clusternum_mag[0]<=density_unique.size())
	{
		int *IDX_tmp = new int[density_unique.size()];

		qdotem->SetData(position,density_unique,velocity_unique,velocity_unique.size(),density_unique.size());
		cout<<"EM for Gaussian mixture: running ... "<<endl;
		clusternum_error_mag = qdotem->EM(clusternum_mag[0],IDX_tmp,false,true);
		//cerr<<"1"<<endl;
		for(int i=0;i<velocity_density.size();i++)
			velocity_density[i].clear();
		for(int i=0;i<dataSize;i++)
		{
			IDX_Mag[i] = IDX_tmp[density_index[i]];
			velocity_density[velocity_index[i]].push_back(IDX_Mag[i]);
		}//cerr<<"1"<<endl;
		MagProcess();

	}
	else
	{
		cout<<"You input a invalid number "<<clusternum_mag[0]<<endl;
	}
	//MagProcess();	
	cerr<<"Magnitude clustering ... done"<<endl;
}
void Cluster::GetVelClusters()
{
	Index_Vel[0] = -1;
	Index_Vel[1] = -1;

	vector< vector<double> >p_vel;
	vector< vector<double> >v_vel;
	vector<double>d_vel;
	vector<int> v_index;

	string tmp2;
	float tmpnum2;

	if(clusternum_vel[0]>=2&&clusternum_vel[0]<=velocity_unique.size())
	{	
		for(int i=0;i<dataSize;i++)
		{
			IDX_Vel[i] = -1;
		}


		for(int i=0;i<velocity_unique.size();i++)
		{
			p_vel.push_back(position[i]);
			v_vel.push_back(velocity_unique[i]);
			d_vel.push_back(density[i]);
			v_index.push_back(i);

		}

		qdotem->SetData(p_vel,d_vel,v_vel,v_vel.size(),d_vel.size());
		
		if(clusternum_vel[0]>=2&&clusternum_vel[0]<=v_vel.size())
		{  
			int *IDX_tmp = new int[v_vel.size()];
			int *IDX_tmp2 = new int[velocity_unique.size()];
			cout<<"EM for Gaussian mixture: running ... "<<endl;
			//cerr<<clusternum_vel[0]<<endl;
		 	qdotem->EM(clusternum_vel[0],IDX_tmp,false,false);
			//cerr<<v_vel.size()<<endl;
		 	int ii=0;
			int j=0;
			for(int i=0;i<velocity_unique.size();i++)
			{
				IDX_tmp2[i] = -1;
			}//cerr<<v_vel.size()<<endl;
			for(int i=0;i<v_index.size();i++)
			{
				IDX_tmp2[v_index[i]] = IDX_tmp[i];

			}//cerr<<v_vel.size()<<endl;
			//int count = 0;
		  	for(int i=0;i<dataSize;i++)
		  	{

				IDX_Vel[i] = IDX_tmp2[velocity_index[i]];//count++;
				
			}	//cerr<<v_vel.size()<<endl;	
//cerr<<count<<endl;
			VelProcess(false);	
			delete [] IDX_tmp;
			delete [] IDX_tmp2;	
		}
		else
			cout<<"You input a invalid number "<<clusternum_vel[0]<<endl;
	}
	else
	{		
		cout<<"You input a invalid number "<<clusternum_vel[0]<<endl;
	}
//cerr<<v_vel.size()<<endl;
	for(int i=0;i<d_vel.size();i++)
	{
		v_vel[i].clear();
		p_vel[i].clear();
	}
	d_vel.clear();
	v_vel.clear();
	p_vel.clear();
	v_index.clear();
	//VelProcess();
}
void Cluster::GetROIMagClusters(vector<bool> in)
{
	Index_Vel[0] = -1;
	Index_Vel[1] = -1;

	//Index_Vel[0] = 1;
	//Index_Vel[1] = clusternum_mag[0];

	vector< vector<double> >p_vel;
	vector< vector<double> >v_vel;
	vector<double>d_vel;
	vector<int> d_index;

	string tmp2;
	float tmpnum2;
	cerr<<clusternum_mag_in[0]<<endl;
	if(clusternum_mag_in[0]>=2&&clusternum_mag_in[0]<=density_unique.size())
	{	//cerr<<"1"<<endl;
		for(int i=0;i<dataSize;i++)
		{
			IDX_Mag_In[i] = -1;
		}//cerr<<"1"<<endl;
		

		//int count =0;
		//bool flag = false;
		for(int i=0;i<density_unique.size();i++)
		{
			bool inbox = false;
			for(int t=0;t<unique_index2[i].size();t++)
			{
				if(in[unique_index2[i][t]])
				{
					inbox = true;break;
				}
			}
			if(inbox)
			{
				p_vel.push_back(position[i]);
				v_vel.push_back(velocity[i]);
				d_vel.push_back(density_unique[i]);
				d_index.push_back(i);

			}
		}//cerr<<d_vel.size()<<endl;
		qdotem_in->SetData(p_vel,d_vel,v_vel,v_vel.size(),d_vel.size());
		
		if(clusternum_mag_in[0]>=2&&clusternum_mag_in[0]<=d_vel.size())
		{  
			int *IDX_tmp = new int[d_vel.size()];
			int *IDX_tmp2 = new int[density_unique.size()];
			cout<<"EM for Gaussian mixture: running ... "<<endl;
			//cerr<<clusternum_mag[0]<<endl;
		 	clusternum_error_mag_in = qdotem_in->EM(clusternum_mag_in[0],IDX_tmp,false,true);
			//cerr<<v_vel.size()<<endl;
		 	int ii=0;
			int j=0;
			//for(int i=0;i<velocity_density_in.size();i++)
			//	velocity_density_in[i].clear();
			for(int i=0;i<density_unique.size();i++)
			{
				IDX_tmp2[i] = -1;
			}//cerr<<v_vel.size()<<endl;
			for(int i=0;i<d_index.size();i++)
			{
				IDX_tmp2[d_index[i]] = IDX_tmp[i];

			}//cerr<<v_vel.size()<<endl;
		  	for(int i=0;i<dataSize;i++)
		  	{
				if(in[i])
				{
					IDX_Mag_In[i] = IDX_tmp2[density_index[i]];
					//velocity_density_in[velocity_index[i]].push_back(IDX_Mag_In[i]);
				}
				//else
				//	velocity_density[velocity_index[i]].push_back(-1);
			}	//cerr<<v_vel.size()<<endl;	
			MaginProcess();	
			delete [] IDX_tmp;
			delete [] IDX_tmp2;	
		}
		else
			cout<<"You input a invalid number "<<clusternum_mag_in[0]<<endl;
	}
	else
	{		
		cout<<"You input a invalid number "<<clusternum_mag_in[0]<<endl;
	}
	//cerr<<"done"<<endl;

	for(int i=0;i<d_vel.size();i++)
	{
		v_vel[i].clear();
		p_vel[i].clear();
	}
	d_vel.clear();
	v_vel.clear();
	p_vel.clear();
	d_index.clear();
	//cerr<<"done"<<endl;
	//VelProcess();
}
void Cluster::GetROIVelClusters(vector<bool> in)
{
	Index_Vel[0] = -1;
	Index_Vel[1] = -1;

	vector< vector<double> >p_vel;
	vector< vector<double> >v_vel;
	vector<double>d_vel;
	vector<int> v_index;

	string tmp2;
	float tmpnum2;

	if(clusternum_vel_in[0]>=2&&clusternum_vel_in[0]<=velocity_unique.size())
	{	
		for(int i=0;i<dataSize;i++)
		{
			IDX_Vel_In[i] = -1;
		}

		//int count =0;
		bool flag = false;
		for(int i=0;i<velocity_unique.size();i++)
		{
			flag = false;
			bool inbox = false;
			for(int t=0;t<unique_index[i].size();t++)
			{
				if(in[unique_index[i][t]])
				{
					inbox = true;break;
				}
			}
			if(inbox)
			{
				p_vel.push_back(position[i]);
				v_vel.push_back(velocity_unique[i]);
				d_vel.push_back(density[i]);
				v_index.push_back(i);
				
			}
		}
		//cerr<<velocity_unique.size()<<" "<<v_vel.size()<<endl;
		//cerr<<"count"<<count<<endl;
		qdotem_in->SetData(p_vel,d_vel,v_vel,v_vel.size(),d_vel.size());
		
		if(clusternum_vel_in[0]>=2&&clusternum_vel_in[0]<=v_vel.size())
		{  
			int *IDX_tmp = new int[v_vel.size()];
			int *IDX_tmp2 = new int[velocity_unique.size()];
			cout<<"EM for Gaussian mixture: running ... "<<endl;
			//cerr<<clusternum_vel[0]<<endl;
		 	qdotem_in->EM(clusternum_vel_in[0],IDX_tmp,false,false);
			//cerr<<v_vel.size()<<endl;
		 	int ii=0;
			int j=0;
			for(int i=0;i<velocity_unique.size();i++)
			{
				IDX_tmp2[i] = -1;
			}//cerr<<v_vel.size()<<endl;
			for(int i=0;i<v_index.size();i++)
			{
				IDX_tmp2[v_index[i]] = IDX_tmp[i];

			}//cerr<<v_vel.size()<<endl;
			//int count = 0;
		  	for(int i=0;i<dataSize;i++)
		  	{
				if(in[i])
				{
					IDX_Vel_In[i] = IDX_tmp2[velocity_index[i]];//count++;
				}
			}	//cerr<<v_vel.size()<<endl;	
//cerr<<count<<endl;
			VelinProcess();	
			delete [] IDX_tmp;
			delete [] IDX_tmp2;	
		}
		else
			cout<<"You input a invalid number "<<clusternum_vel_in[0]<<endl;
	}
	else
	{		
		cout<<"You input a invalid number "<<clusternum_vel_in[0]<<endl;
	}
//cerr<<v_vel.size()<<endl;
	for(int i=0;i<d_vel.size();i++)
	{
		v_vel[i].clear();
		p_vel[i].clear();
	}
	d_vel.clear();
	v_vel.clear();
	p_vel.clear();
	v_index.clear();
	//VelProcess();
}
/*
void Cluster::GetROIVelClusters2(vector<bool> in)//no detect symmetry
{
	vector< vector<double> >p_vel;
	vector< vector<double> >v_vel;
	vector<double>d_vel;
	vector<int> v_index;

	string tmp2;
	float tmpnum2;

	if(clusternum_vel[0]>=1&&clusternum_vel[0]<=50&&clusternum_vel[0]<=velocity_unique.size())
	{	
		for(int i=0;i<dataSize;i++)
		{
			IDX_Vel[i] = -1;
		}

		//int count =0;
		bool flag = false;
		for(int i=0;i<velocity_unique.size();i++)
		{
			flag = false;
			bool inbox = false;
			for(int t=0;t<unique_index[i].size();t++)
			{
				if(in[unique_index[i][t]])
				{
					inbox = true;break;
				}
			}
			if(inbox)
			{
				p_vel.push_back(position[i]);
				v_vel.push_back(velocity[i]);
				d_vel.push_back(density[i]);
				v_index.push_back(i);
				
			}
		}
		//cerr<<"count"<<count<<endl;
		qdotem->SetData(p_vel,d_vel,v_vel,v_vel.size(),d_vel.size());
		
		if(clusternum_vel[0]>=1&&clusternum_vel[0]<=50&&clusternum_vel[0]<=v_vel.size())
		{  
			int *IDX_tmp = new int[v_vel.size()];
			int *IDX_tmp2 = new int[velocity_unique.size()];
			cout<<"EM for Gaussian mixture: running ... "<<endl;
		 	qdotem->EM(clusternum_vel[0],IDX_tmp,false,true);
			//cerr<<v_vel.size()<<endl;
		 	int ii=0;
			int j=0;
			for(int i=0;i<velocity_unique.size();i++)
			{
				IDX_tmp2[i] = -1;
			}//cerr<<v_vel.size()<<endl;
			for(int i=0;i<v_index.size();i++)
			{
				IDX_tmp2[v_index[i]] = IDX_tmp[i];

			}//cerr<<v_vel.size()<<endl;
		  	for(int i=0;i<dataSize;i++)
		  	{
				if(in[i])
				{
					IDX_Vel[i] = IDX_tmp2[velocity_index[i]];
				}
			}	//cerr<<v_vel.size()<<endl;	
			VelProcess();	
			delete [] IDX_tmp;
			delete [] IDX_tmp2;	
		}
		else
			cout<<"You input a invalid number "<<clusternum_vel[0]<<endl;
	}
	else
	{		
		cout<<"You input a invalid number "<<clusternum_vel[0]<<endl;
	}

	for(int i=0;i<d_vel.size();i++)
	{
		v_vel[i].clear();
		p_vel[i].clear();
	}
	d_vel.clear();
	v_vel.clear();
	p_vel.clear();
	v_index.clear();
	//VelProcess();
}*/
void Cluster::GetMagColors(double *r, double *g, double *b)
{
	for(int i=0;i<dataSize;i++)
	{
		if(IDX_Mag[i]!=-1&&IDX_Mag_In[i]==-1)
		{
		r[i] = density_color[0][IDX_mag_order[IDX_Mag[i]]];
		g[i] = density_color[1][IDX_mag_order[IDX_Mag[i]]];
		b[i] = density_color[2][IDX_mag_order[IDX_Mag[i]]];
		}
		else if(IDX_Mag[i]!=-1&&IDX_Mag_In[i]!=-1)
		{
		r[i] = density_color_in[0][IDX_mag_order_in[IDX_Mag_In[i]]];
		g[i] = density_color_in[1][IDX_mag_order_in[IDX_Mag_In[i]]];
		b[i] = density_color_in[2][IDX_mag_order_in[IDX_Mag_In[i]]];
		}
		else
		{
			r[i] = -1;
			g[i] = -1;
			b[i] = -1;
		}
	}
}

void Cluster::GetMagColors(int n, bool *in, double *d, double *r, double *g, double *b)
{

	for(int i=0;i<n;i++)
	{
		if(!in[i])
		{
			int index = -1;
			for(int j=1;j<clusternum_mag[0];j++)
			{
				if(d[i] <= magrange[0][IDX_sort_mag[j-1]]
				&& d[i] >= magrange[0][IDX_sort_mag[j]])
				{
					index = j-1;
					break;
				}
			}
			if(index==-1)
			{
//if(index==-1)	cerr<<"a "<<d[i]<<" "<<magrange[0][IDX_sort_mag[0]]<<" "<<magrange[0][IDX_sort_mag[1]]<<endl;
				if((d[i] <= magrange[1][IDX_sort_mag[0]]
				&& d[i] >= magrange[0][IDX_sort_mag[0]])
				|| fabs(d[i] - magrange[1][IDX_sort_mag[0]])<=1)
				{
					index = 0;
					//break;
				}	
				if(index==-1)
				{

					if(d[i] <= magrange[1][IDX_sort_mag[clusternum_mag[0]-1]]
					&& d[i] >= magrange[0][IDX_sort_mag[clusternum_mag[0]-1]])
					{
						index = clusternum_mag[0]-1;
						//break;
					}	
				}
			}		
			//cerr<<index<<" ";
			//if(index==-1)
			//	cerr<<d[i]<<" "<<index<<endl;
			r[i] = density_color[0][index];
			g[i] = density_color[1][index];
			b[i] = density_color[2][index];
		}
		else 
		{
			int index = -1;
			for(int j=1;j<clusternum_mag_in[0];j++)
			{
				if(d[i] <= magrange_in[0][IDX_sort_mag_in[j-1]]
				&& d[i] >= magrange_in[0][IDX_sort_mag_in[j]])
				{
					index = j-1;
					break;
				}
			}
			if(index==-1)
			{
//if(index==-1)	cerr<<d[i]<<" "<<magrange_in[0][IDX_sort_mag_in[0]]<<" "<<magrange_in[0][IDX_sort_mag_in[1]]<<endl;
				if((d[i] <= magrange_in[1][IDX_sort_mag_in[0]]
				&& d[i] >= magrange_in[0][IDX_sort_mag_in[0]])
				|| fabs(d[i]- magrange_in[1][IDX_sort_mag_in[0]])<1)
				{
					index = 0;
					//break;
				}	
				if(index==-1)
				{
					
					if(d[i] <= magrange_in[1][IDX_sort_mag_in[clusternum_mag_in[0]-1]]
					&& d[i] >= magrange_in[0][IDX_sort_mag_in[clusternum_mag_in[0]-1]])
					{
						index = clusternum_mag_in[0]-1;
						//break;
					}
				}
			}
			//cerr<<index<<" ";
			r[i] = density_color_in[0][index];
			g[i] = density_color_in[1][index];
			b[i] = density_color_in[2][index];
		}
		//cerr<<r[i]<<" "<<g[i]<<" "<<b[i]<<endl;
	}//cerr<<endl;
}

void Cluster::GetVelColors(double *r, double *g, double *b)
{
	for(int i=0;i<dataSize;i++)
	{
		if(IDX_Vel[i]!=-1&&IDX_Vel_In[i]==-1)
		{
			r[i] = velocity_color[0][IDX_Vel[i]];
			g[i] = velocity_color[1][IDX_Vel[i]];
			b[i] = velocity_color[2][IDX_Vel[i]];
		}
		else if(IDX_Vel[i]!=-1&&IDX_Vel_In[i]!=-1)
		{
			r[i] = velocity_color_in[0][IDX_Vel_In[i]];
			g[i] = velocity_color_in[1][IDX_Vel_In[i]];
			b[i] = velocity_color_in[2][IDX_Vel_In[i]];
		}
		else
		{
			r[i] = -1;
			g[i] = -1;
			b[i] = -1;
		}
	}
}


void Cluster::GetVelHue(double *h)
{
	for(int i=0;i<dataSize;i++)
	{
		if(IDX_Vel[i]!=-1)
		{
			//cerr<<h[20]<<endl;
			h[i] = velocity_hue[IDX_Vel[i]];
		}
		else
			h[i] = -1;
		//cerr<<h[i]<<" ";
	}
}

vector<double> Cluster::GetOrderHue()
{
	return velocity_hue;
}

int * Cluster::GetIDXMag()
{
	int *idx = new int[dataSize];

	for(int i=0;i<dataSize;i++)
	{
		if(IDX_Mag[i]!=-1&&IDX_Mag_In[i]==-1)
			idx[i] = IDX_mag_order[IDX_Mag[i]];
		else if(IDX_Mag_In[i]!=-1)
			idx[i] = IDX_mag_order_in[IDX_Mag_In[i]];
		else
			idx[i] = -1;
	}
	return idx;
}

int * Cluster::GetIDXVel()
{
	return IDX_Vel;
}

void Cluster::cleanup()
{
	delete [] IDX_Mag;
	delete [] IDX_Vel;
	delete [] IDX_Mag_In;
	delete [] IDX_Vel_In;

	for(int i=0;i<dataSize;i++)
	{
		position[i].clear();
		velocity[i].clear();
		velocity2[i].clear();
		velocity_unique[i].clear();
	}
	cerr<<"!"<<endl;
	position.clear();
	velocity.clear();
	velocity2.clear();
	density.clear();
	density_unique.clear();
	density_index.clear();
	int sum = velocity_unique.size();
	for(int i=0;i<sum;i++)
		velocity_unique[i].clear();
	velocity_unique.clear();
	sum = unique_index.size();
	for(int i=0;i<sum;i++)
		unique_index[i].clear();
	sum = unique_index2.size();
	for(int i=0;i<sum;i++)
		unique_index2[i].clear();
	unique_index.clear();
	unique_index2.clear();
	velocity_index.clear();
	for(int i=0;i<velocity_density.size();i++)
		velocity_density[i].clear();
	velocity_density.clear();
	for(int i=0;i<velocity_density_in.size();i++)
		velocity_density_in[i].clear();
	velocity_density_in.clear();
	cerr<<"!"<<endl;

	density_color[0].clear();
	density_color[1].clear();
	density_color[2].clear();
	velocity_center[0].clear();
	velocity_center[1].clear();
	velocity_center[2].clear();
	velocity_color[0].clear();
	velocity_color[1].clear();
	velocity_color[2].clear();
	velocity_order.clear();	
	velocity_order_in.clear();	
	spiralcolors_mag[0].clear();
	spiralcolors_mag[1].clear();
	spiralcolors_mag[2].clear();
	spiralcolors_vel[0].clear();
	spiralcolors_vel[1].clear();
	spiralcolors_vel[2].clear();
	spiralcolors_vel_hue.clear();
	IDX_sort_mag.clear();
	IDX_mag_order.clear();
	cerr<<"!"<<endl;

	density_color_in[0].clear();
	density_color_in[1].clear();
	density_color_in[2].clear();
	velocity_center_in[0].clear();
	velocity_center_in[1].clear();
	velocity_center_in[2].clear();
	velocity_color_in[0].clear();
	velocity_color_in[1].clear();
	velocity_color_in[2].clear();
	spiralcolors_mag_in[0].clear();
	spiralcolors_mag_in[1].clear();
	spiralcolors_mag_in[2].clear();
	spiralcolors_vel_in[0].clear();
	spiralcolors_vel_in[1].clear();
	spiralcolors_vel_in[2].clear();
	spiralcolors_vel_in_hue.clear();
	IDX_sort_mag_in.clear();
	IDX_mag_order_in.clear();
	cerr<<"!"<<endl;
}

void Cluster::cleanup_nocolor()
{
	delete [] IDX_Mag;
	delete [] IDX_Vel;
	delete [] IDX_Mag_In;
	delete [] IDX_Vel_In;

	for(int i=0;i<dataSize;i++)
	{
		position[i].clear();
		velocity[i].clear();
		velocity2[i].clear();
		//velocity_unique[i].clear();
	}
	cerr<<"!"<<endl;
	position.clear();cerr<<"!"<<endl;
	velocity.clear();cerr<<"!"<<endl;
	velocity2.clear();cerr<<"!"<<endl;
	density.clear();cerr<<"!"<<endl;
	density_unique.clear();cerr<<"!"<<endl;
	density_index.clear();cerr<<"!"<<endl;
	cerr<<"!"<<endl;
	int sum = velocity_unique.size();
	for(int i=0;i<sum;i++)
		velocity_unique[i].clear();
	velocity_unique.clear();
	cerr<<"!"<<endl;
	sum = unique_index.size();
	for(int i=0;i<sum;i++)
		unique_index[i].clear();
	sum = unique_index2.size();
	for(int i=0;i<sum;i++)
		unique_index2[i].clear();
	unique_index.clear();
	unique_index2.clear();
	cerr<<"!"<<endl;
	velocity_index.clear();
	for(int i=0;i<velocity_density.size();i++)
		velocity_density[i].clear();
	velocity_density.clear();
	for(int i=0;i<velocity_density_in.size();i++)
		velocity_density_in[i].clear();
	velocity_density_in.clear();
	cerr<<"!"<<endl;
	velocity_center[0].clear();
	velocity_center[1].clear();
	velocity_center[2].clear();
	velocity_center_in[0].clear();
	velocity_center_in[1].clear();
	velocity_center_in[2].clear();
	cerr<<"!"<<endl;
	velocity_order.clear();	
	velocity_order_in.clear();	
	cerr<<"!"<<endl;
	IDX_sort_mag.clear();
	IDX_mag_order.clear();
	IDX_sort_mag_in.clear();
	IDX_mag_order_in.clear();
	cerr<<"!"<<endl;
}

}
