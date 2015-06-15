#include "svQDOTaggregation.h"

using namespace std;

namespace __svl_lib {

svQDOTaggregation::svQDOTaggregation(svVector3Array *pos, svScalarArray *dir, svScalarArray *den,
			svScalarArray *exp, svScalarArray *coe,svVector3Array *col,
                      int num, int layer, int xn, int yn, float xl,float yl)
{
    dataPos = new svVector3Array[layer];
    dataDir = new svScalarArray[layer];
    dataExp = new svScalarArray[layer];
    dataCoe = new svScalarArray[layer];
    dataDen = new svScalarArray[layer];
    if(col!=NULL)
   	 dataCol = new svVector3Array[layer];
    else
	dataCol = NULL;

    dataXY = new svVector3Array[layer];
    dataMax = new svScalarArray[layer];
    dataMaxDen = new svScalarArray[layer];
    dataMaxExp = new svScalarArray[layer];
    dataMaxCoe = new svScalarArray[layer];
    dataMaxPos = new svVector3Array[layer];
    dataMaxPos2 = new svVector3Array[layer];

    dataSize = num;
    zlayer = layer;

	svVector3 min;
	svVector3 max;
    float maxx,maxy;
    float minx,miny;
    maxx = -9e+9;
    maxy = -9e+9;
    minx = 9e+9;
    miny = 9e+9;
	float m;
	int in;
	int in2;

	float m2;
	float m2_1, m2_2;

    for(int i=0;i<layer;i++)
    {
        maxx = -9e+9;
        maxy = -9e+9;
        minx = 9e+9;
        miny = 9e+9;
	m = -9e+9;
	m2 = -9e+9;
        for(int j=0;j<pos[i].size();j++)
        {
            dataPos[i].add(pos[i][j]);
            dataDir[i].add(dir[i][j]);
		dataExp[i].add(exp[i][j]);
		dataCoe[i].add(coe[i][j]);
		dataDen[i].add(den[i][j]);

		if(col!=NULL)
			dataCol[i].add(col[i][j]);

		if(fabs(den[i][j])>m2&&den[i][j]>-9e+9)
		{	m2 = den[i][j];m2_1 = exp[i][j];m2_2=coe[i][j];in2 = j;}
		if(fabs(dir[i][j])>m&&dir[i][j]>-9e+9)
		{	m = dir[i][j];in = j;}
            if(minx>dataPos[i][j][0])
                minx = dataPos[i][j][0];
            if(miny>dataPos[i][j][1])
                miny = dataPos[i][j][1];
            if(maxx<dataPos[i][j][0])
                maxx = dataPos[i][j][0];
            if(maxy<dataPos[i][j][1])
                maxy = dataPos[i][j][1];
        }
	dataMax[i].add(m);
	dataMaxDen[i].add(m2);
	dataMaxExp[i].add(m2_1);
	dataMaxCoe[i].add(m2_2);
	dataMaxPos[i].add(dataPos[i][in]);
	dataMaxPos2[i].add(dataPos[i][in2]);
       /* svVector3 p;
        p[0]=(float)(i%xn) * xl + xl - xl*xn/2.-1;p[1]=-(float)(i/xn) * yl + yl*yn/2.;p[2]=0.;
        dataXY[i].add(p);
        p[0]=(float)(i%xn) * xl- xl*xn/2.-1;p[1]=-(float)(i/xn+1) * yl + yl*yn/2.;p[2]=0.;
        dataXY[i].add(p);*/

        svVector3 p;
        p[0]=minx+xl;p[1]=miny+yl;p[2]=0.;
        dataXY[i].add(p);
        p[0]=minx;p[1]=miny;p[2]=0.;
        dataXY[i].add(p);
       // cerr<<xl<<" "<<yl<<" "<<minx<<" "<<miny<<" "
        //   <<dataXY[i][0][0]<<" "<<dataXY[i][0][1]<<" "<<dataXY[i][1][0]<<" "<<dataXY[i][1][1]<<" "<<endl;
    }

    dataAggPos = NULL;
    dataAggOri = NULL;
	dataAggExp = NULL;
	dataAggCoe = NULL;
	dataAggDen = NULL;
	dataAggCol = NULL;
}

void svQDOTaggregation::SetData(svVector3Array *pos, svScalarArray *dir,svScalarArray *den,
			svScalarArray *exp, svScalarArray *coe,svVector3Array *col,
                                     int num, int layer, int xn, int yn, float xl, float yl)
{
    cleanup();

   /* dataPos = new svVector3Array[layer];
    dataDir = new svScalarArray[layer];
    dataXY = new svVector3Array[layer];
    dataMax = new svScalarArray[layer];
    dataMaxPos = new svVector3Array[layer];
    dataSize = num;
    zlayer = layer;
	float m;int in;
    for(int i=0;i<layer;i++)
    {


        //cerr<<i<<endl;
	m = -9e+9;
        for(int j=0;j<pos[i].size();j++)
        {
            //cerr<<i<<" "<<j<<endl;
            dataPos[i].add(pos[i][j]);
            dataDir[i].add(dir[i][j]);
		if(dir[i][j]>m)
		{	m = dir[i][j];in = j;}
        }
	dataMax[i].add(m);	dataMaxPos[i].add(dataPos[i][in]);
        svVector3 p;
        p[0]=(float)(i%xn) * xl + xl;p[1]=(float)(i%yn) * yl + yl;p[2]=0.;
        dataXY[i].add(p);
        p[0]=(float)(i%xn) * xl;p[1]=(float)(i%yn) * yl;p[2]=0.;
        dataXY[i].add(p);

       // cerr<<xl<<" "<<yl<<" "<<minx<<" "<<miny<<" "
        //   <<dataXY[i][0][0]<<" "<<dataXY[i][0][1]<<" "<<dataXY[i][1][0]<<" "<<dataXY[i][1][1]<<" "<<endl;
    }

    dataAggPos = NULL;
    dataAggOri = NULL;*/
    dataPos = new svVector3Array[layer];
    dataDir = new svScalarArray[layer];
    dataExp = new svScalarArray[layer];
    dataCoe = new svScalarArray[layer];
    dataDen = new svScalarArray[layer];
    dataXY = new svVector3Array[layer];
    dataMax = new svScalarArray[layer];
    dataMaxDen = new svScalarArray[layer];
    dataMaxExp = new svScalarArray[layer];
    dataMaxCoe = new svScalarArray[layer];
    dataMaxPos = new svVector3Array[layer];
    dataMaxPos2 = new svVector3Array[layer];

    if(col!=NULL)
   	 dataCol = new svVector3Array[layer];
    else
	dataCol = NULL;

    dataSize = num;
    zlayer = layer;

	svVector3 min;
	svVector3 max;
    float maxx,maxy;
    float minx,miny;
    maxx = -9e+9;
    maxy = -9e+9;
    minx = 9e+9;
    miny = 9e+9;
	float m;
	int in;
	float m2;
	float m2_1, m2_2;
	int in2;
    for(int i=0;i<layer;i++)
    {
        maxx = -9e+9;
        maxy = -9e+9;
        minx = 9e+9;
        miny = 9e+9;
	m = -9e+9;
	m2 = -9e+9;
        for(int j=0;j<pos[i].size();j++)
        {
            dataPos[i].add(pos[i][j]);
            dataDir[i].add(dir[i][j]);
		dataExp[i].add(exp[i][j]);
		dataCoe[i].add(coe[i][j]);
		dataDen[i].add(den[i][j]);
		if(col!=NULL)
			dataCol[i].add(col[i][j]);
		if(fabs(den[i][j])>m2&&den[i][j]>-9e+9)
		{	m2 = den[i][j];m2_1 = exp[i][j];m2_2=coe[i][j];in2 = j;}
		if(fabs(dir[i][j])>m&&dir[i][j]>-9e+9)
		{	m = dir[i][j];in = j;}
            if(minx>dataPos[i][j][0])
                minx = dataPos[i][j][0];
            if(miny>dataPos[i][j][1])
                miny = dataPos[i][j][1];
            if(maxx<dataPos[i][j][0])
                maxx = dataPos[i][j][0];
            if(maxy<dataPos[i][j][1])
                maxy = dataPos[i][j][1];
        }
	dataMax[i].add(m);
	dataMaxDen[i].add(m2);
	dataMaxExp[i].add(m2_1);
	dataMaxCoe[i].add(m2_2);
	dataMaxPos[i].add(dataPos[i][in]);
	dataMaxPos2[i].add(dataPos[i][in2]);
       /* svVector3 p;
        p[0]=(float)(i%xn) * xl + xl - xl*xn/2.-1;p[1]=-(float)(i/xn) * yl + yl*yn/2.;p[2]=0.;
        dataXY[i].add(p);
        p[0]=(float)(i%xn) * xl- xl*xn/2.-1;p[1]=-(float)(i/xn+1) * yl + yl*yn/2.;p[2]=0.;
        dataXY[i].add(p);*/

        svVector3 p;
        p[0]=minx+xl;p[1]=miny+yl;p[2]=0.;
        dataXY[i].add(p);
        p[0]=minx;p[1]=miny;p[2]=0.;
        dataXY[i].add(p);
       // cerr<<xl<<" "<<yl<<" "<<minx<<" "<<miny<<" "
        //   <<dataXY[i][0][0]<<" "<<dataXY[i][0][1]<<" "<<dataXY[i][1][0]<<" "<<dataXY[i][1][1]<<" "<<endl;
    }

    dataAggPos = NULL;
    dataAggOri = NULL;
	dataAggExp = NULL;
	dataAggCoe = NULL;
	dataAggDen = NULL;
	dataAggCol = NULL;
}

void svQDOTaggregation::SetSampleAggregation(float pixel)
{
    if(dataAggPos!=NULL)
        cleanupaggregation();

    dataAggPos = new svVector3Array[zlayer];
    dataAggOri = new svScalarArray[zlayer];
	dataAggExp = new svScalarArray[zlayer];
	dataAggCoe = new svScalarArray[zlayer];
	dataAggDen = new svScalarArray[zlayer];
	dataAggCol = new svVector3Array[zlayer];
    int i;
    float s = sqrt(pixel);
    float spixel = s;
    //int spixel = (int)s;
    //if(s-spixel>0.01)
     //   spixel = spixel + 1;

    float x,y;
    svScalar ori=-9e+9;
    svVector3 p;
    int indexo;

/*
    for(i=0;i<zlayer;)
    {
        for(int j=0;j<dataPos[i].size();j++)
        {
            x = dataPos[i][j][0];
            y = dataPos[i][j][1];
            if(x>=dataXY[i][1][0]&&x<dataXY[i][1][0]+(float)spixel
                    &&y>=dataXY[i][1][1]&&y<dataXY[i][1][1]+(float)spixel)
            {
                if(dataDir[i][j]>ori)
                {
                    ori = dataDir[i][j];
                    indexo = j;
                }
            }

        }
        p[0] = dataXY[i][1][0] + (float)spixel/2.;
        p[1] = dataXY[i][1][1] + (float)spixel/2.;
        p[2] = 0.;
        dataAggPos[i].add(p);
        dataAggOri[i].add(ori);

	i = i + 3;
    }*/
    for(i=0;i<zlayer;i++)
    {
	int xi = (dataMaxPos[i][0][0] - dataXY[i][1][0])/ spixel;
	int yi = (dataMaxPos[i][0][1] - dataXY[i][1][1])/ spixel;
	int xi2 = (dataMaxPos2[i][0][0] - dataXY[i][1][0])/ spixel;
	int yi2 = (dataMaxPos2[i][0][1] - dataXY[i][1][1])/ spixel;
	if((dataMaxPos[i][0][0]+spixel)>dataXY[i][0][0])
	{
		xi = xi - 1;
	}
	if((dataMaxPos[i][0][1]+spixel)>dataXY[i][0][1])
	{
		yi = yi - 1;
	}
	if((dataMaxPos2[i][0][0]+spixel)>dataXY[i][0][0])
	{
		xi2 = xi2 - 1;
	}
	if((dataMaxPos2[i][0][1]+spixel)>dataXY[i][0][1])
	{
		yi2 = yi2 - 1;
	}
        p[0] = dataXY[i][1][0] + spixel/2.+xi * spixel;//(dataXY[i][1][0] + dataXY[i][0][0])/2.;
        p[1] = dataXY[i][1][1] + spixel/2.+yi * spixel;//(dataXY[i][1][1] + dataXY[i][0][1])/2.;
        p[2] = 0.;
        dataAggPos[i].add(p);
        dataAggOri[i].add(dataMax[i][0]);
	dataAggDen[i].add(dataMaxDen[i][0]);
        dataAggExp[i].add(dataMaxExp[i][0]);
        dataAggCoe[i].add(dataMaxCoe[i][0]);

        p[0] = dataXY[i][1][0] + spixel/2.+xi2 * spixel;//(dataXY[i][1][0] + dataXY[i][0][0])/2.;
        p[1] = dataXY[i][1][1] + spixel/2.+yi2 * spixel;//(dataXY[i][1][1] + dataXY[i][0][1])/2.;
        p[2] = 0.;
        dataAggPos[i].add(p);
        dataAggOri[i].add(dataMax[i][0]);
	dataAggDen[i].add(dataMaxDen[i][0]);
        dataAggExp[i].add(dataMaxExp[i][0]);
        dataAggCoe[i].add(dataMaxCoe[i][0]);
    }
}

void svQDOTaggregation::SetAggregation(float pixel)
{
    if(dataAggPos!=NULL)
        cleanupaggregation();

    dataAggPos = new svVector3Array[zlayer];
    dataAggOri = new svScalarArray[zlayer];
	dataAggExp = new svScalarArray[zlayer];
	dataAggCoe = new svScalarArray[zlayer];
	dataAggDen = new svScalarArray[zlayer];
    int i;
    float s = sqrt(pixel);
    float spixel = s;
    //int spixel = (int)s;
    //if(s-spixel>0.01)
     //   spixel = spixel + 1;

    float x,y;

    svScalar ori=-9e+9;
    svVector3 p;
    svVector4 pp;
    pp[0]=0;pp[1]=0;pp[2]=0;pp[3]=0;
    int indexo;

    int cc = 0;

    for(i=0;i<zlayer;i++)
    {
        x = dataXY[i][1][0];
        y = dataXY[i][1][1];
	//cerr<<x<<" "<<y<<" "<<" "<<dataXY[i][0][0]<<" "<<dataXY[i][0][1]<<endl;
        int count = 0;
        while((y<dataXY[i][0][1])&&((y+spixel)<=dataXY[i][0][1]))
        {
            p[0] = x + spixel/2.;
            p[1] = y + spixel/2.;
            p[2] = 0;
            if(x+spixel<=dataXY[i][0][0])
            {    dataAggPos[i].add(p);
                count ++;cc++;}
            x = x + spixel;
            if(x>=dataXY[i][0][0])
            {
                x = dataXY[i][1][0];
                y = y + spixel;
            }
        }

       // cerr<<i<<" "<<dataAggPos[i].size()<<endl;

        for(int j=0;j<count;j++)
        {
            
            dataAggOri[i].add(-9e+9);
            dataAggDen[i].add(-9e+9);
            dataAggExp[i].add(-9e+9);
            dataAggCoe[i].add(-9e+9);
        }   // cerr<<count<<endl;
    }

	//cerr<<dataAggOri[0][0]<<endl;
    //cerr<<cc<<endl;

    float ex,ey;
    float max = -9e+9;
    for(i=0;i<zlayer;i++)
    {//cerr<<i<<endl;
        float count = (dataXY[i][0][0] - dataXY[i][1][0])/spixel;
        int c = (int)count;
        //if(count-(float)c>0.)
        //    c = c + 1;
        for(int j=0;j<dataPos[i].size();j++)
        {
            ex = dataPos[i][j][0];
            ey = dataPos[i][j][1];
            x = ex - dataXY[i][1][0];
            y = ey - dataXY[i][1][1];
            x = x / spixel;
            y = y / spixel;
            int xi = x;
            int yi = y;
		/*if(xi>c)
			xi = c;
		if(yi>c)
			yi = c;*/
		
            if(ex+spixel>dataXY[i][0][0]&&xi!=0)
                xi = xi -1;
            if(ey+spixel>dataXY[i][0][1]&&yi!=0)
                yi = yi - 1;
            xi = xi + yi * c;
	//cerr<<xi<<" ";

	    //if(xi==dataAggOri[i].size())
		//xi = dataAggOri[i].size()-1;
		//cerr<<j<<" "<<xi<<" "<<x<<" "<<y<<" "<<c<<" "<<ex<<" "<<ey<<" "<<dataXY[i][1][0]<<" "<<dataXY[i][1][1]<<" "<<dataXY[i][0][0]<<" "<<dataXY[i][0][1]<<endl;
	

            if(dataAggOri[i][xi]<fabs(dataDir[i][j])&&dataDir[i][j]>-9e+9)
            {
                dataAggOri[i][xi] = dataDir[i][j];
            }
            if(dataAggDen[i][xi]<fabs(dataDen[i][j])&&dataDen[i][j]>-9e+9)
            {
                dataAggDen[i][xi] = dataDen[i][j];
                dataAggExp[i][xi] = dataExp[i][j];
                dataAggCoe[i][xi] = dataCoe[i][j];
            }

        }
    }
	//cerr<<"done"<<endl;
}

void svQDOTaggregation::cleanupaggregation()
{
    for(int i=0;i<zlayer;i++)
    {
        dataAggPos[i].free();
        dataAggOri[i].free();
     dataAggDen[i].free();
     dataAggCoe[i].free();
     dataAggExp[i].free();
    }

    delete [] dataAggPos;
    delete [] dataAggOri;
    delete [] dataAggDen;
    delete [] dataAggExp;
    delete [] dataAggCoe;

    dataAggPos = NULL;
    dataAggOri = NULL;
    dataAggCoe = NULL;
    dataAggExp = NULL;
    dataAggDen = NULL;
}

void svQDOTaggregation::cleanup()
{
	cleanupaggregation();
    for(int i=0;i<zlayer;i++)
    {
        dataPos[i].free();
        dataDen[i].free();
        dataCoe[i].free();
        dataExp[i].free();
        dataXY[i].free();
        dataDir[i].free();
	dataMax[i].free();
	dataMaxPos[i].free();
	dataMaxPos2[i].free();
	dataMaxCoe[i].free();
	dataMaxExp[i].free();
	dataMaxDen[i].free();
    }

    delete [] dataPos;
    delete [] dataDen;
    delete [] dataExp;
    delete [] dataCoe;
    delete [] dataDir;
    delete [] dataXY;
	delete [] dataMax;
	delete [] dataMaxPos;
	delete [] dataMaxPos2;
	delete [] dataMaxExp;
	delete [] dataMaxCoe;
	delete [] dataMaxDen;
    dataPos = NULL;

    dataDen = NULL;

    dataExp = NULL;

    dataCoe = NULL;
    dataDir = NULL;
    dataXY = NULL;
	dataMax = NULL;
	dataMaxPos = NULL;
	dataMaxPos2 = NULL;
	dataMaxExp = NULL;
	dataMaxCoe = NULL;
	dataMaxDen = NULL;
}


}
