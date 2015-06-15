#include <iostream>
#include <fstream>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define RADS 0.01744444444 //pi/180

using namespace std;

float CC(float a, float d)
{
        float r;
        r = a * cos(d * RADS);
        return r;
}

float SS(float a, float d)
{
        float r;
        r = a * sin(d * RADS);
        return r;
}

struct svVector4
{
	float r;
	float g;
	float b;
	float a;
};

struct svVector3
{
	float x;
	float y;
	float z;
};

svVector4 *line2rgb(svVector3 *vector, int datasize)
{
	 if(datasize<1)
        {
                cerr<<"Insufficient number of arguments"<<endl;
        }

        float x, y, z;
        float xx2, yy2, zz2;
        float xx3, yy3, zz3;
        float zz4;
        float xy, xz, yz;
        float X, Y, Z;

        float *hh1 = new float[datasize];
        float *hh2 = new float[datasize];
        float *hh3 = new float[datasize];
        float *hh4 = new float[datasize];
        float *hh5 = new float[datasize];
        float *hh6 = new float[datasize];
        float *hh7 = new float[datasize];
        float *hh8 = new float[datasize];
        float *hh9 = new float[datasize];
        float *hh10 = new float[datasize];
        float *hh11 = new float[datasize];
        float *hh12 = new float[datasize];

        svVector4 *C = new svVector4[datasize];
        svVector4 tmp;

        float s0 = -23.0;
        float s1 = 227.9;
        float s2 = 251.0;
        float s3 = 125.0;

        float ss23, ss45, ss67, ss89;
        float cc23, cc45, cc67, cc89;
        float w_x = 4.1925;
        float trl_x = -2.0425;

        float w_y = 4.0217 ;
        float trl_y = -1.8541 ;

        float w_z = 4.0694;
        float trl_z = -2.1899;

        ss23 = SS(2.71, s0); cc23 = CC(2.71, s0);
        ss45 = SS(2.12, s1); cc45 = CC(2.12, s1);
        ss67 = SS(.972, s2); cc67 = CC(.972, s2);
        ss89 = SS(.868, s3); cc89 = CC(.868, s3);

        for(int i=0;i<datasize;i++)
        {
                x = vector[i].x;
                y = vector[i].y;
                z = vector[i].z;
                xx2 = x * x;
                yy2 = y * y;
                zz2 = z * z;
                xx3 = x * xx2;
                yy3 = y * yy2;
                zz3 = z * zz2;
                zz4 = zz2 * zz2;
                xy = x * y;
                xz = x * z;
                yz = y * z;

                hh1[i] = 0.5 * (3 * zz2 - 1)/1.58;
                hh2[i] = 3 * xz/2.745;
                hh3[i] = 3 * yz/2.745;
                hh4[i] = 1.5 * (xx2 - yy2)/2.745;
                hh5[i] = 6 * xy/5.5;
                hh6[i] = (1/1.176) * 0.125 * (35 * zz4 - 30 * zz2 + 3);
                hh7[i] = 2.5 * x * (7 * zz3 - 3*z)/3.737;
                hh8[i] = 2.5 * y * (7 * zz3 - 3*z)/3.737;
                hh9[i] = ((xx2 - yy2) * 7.5 * (7 * zz2 - 1))/15.85;
                hh10[i] = ((2 * xy)* (7.5 * (7 * zz2 - 1)))/15.85;
                hh11[i] = 105 * ( 4 * xx3 * z - 3 * xz * (1 - zz2))/59.32;
                hh12[i] = 105 * ((-4) * yy3 * z + 3 * yz * (1 - zz2))/59.32;
        }

        ss23 = SS(2.71, s0); cc23 = CC(2.71, s0);
        ss45 = SS(2.12, s1); cc45 = CC(2.12, s1);
        ss67 = SS(.972, s2); cc67 = CC(.972, s2);
        ss89 = SS(.868, s3); cc89 = CC(.868, s3);

        for(int i=0;i<datasize;i++)
        {
                X = 0.0;
                X =X+ hh2[i]* cc23;
                X =X+ hh3[i]* ss23;
                X =X+ hh5[i]* cc45;
                X =X+ hh4[i]* ss45;
                X =X+ hh7[i]* cc67;
                X =X+ hh8[i]* ss67;
                X =X+ hh10[i]* cc89;
                X =X+ hh9[i]*  ss89;

                Y = 0.0;
                Y =Y+ hh2[i]* (-1)*ss23;
                Y =Y+ hh3[i]* cc23;
                Y =Y+ hh5[i]* (-1)*ss45;
                Y =Y+ hh4[i]* cc45;
                Y =Y+ hh7[i]* (-1)*ss67;
                Y =Y+ hh8[i]* cc67;
                Y =Y+ hh10[i]* (-1)*ss89;
                Y =Y+ hh9[i]*  cc89;

                Z = 0.0;
                Z =Z+ hh1[i] *  (-2.8);
                Z =Z+ hh6[i] *  (-0.5);
                Z =Z+ hh11[i] *  0.3;
                Z =Z+ hh12[i] * (-2.5);

                tmp.a = 1;
                tmp.r = 0.9 * abs((X-trl_x)/w_x) + 0.05;
                tmp.g = 0.9 * abs((Y-trl_y)/w_y) + 0.05;
                tmp.b = 0.9 * abs((Z-trl_z)/w_z) + 0.05;

                C[i]=tmp;

        }
        delete [] hh1;
        delete [] hh2;
        delete [] hh3;
        delete [] hh4;
        delete [] hh5;
        delete [] hh6;
        delete [] hh7;
        delete [] hh8;
        delete [] hh9;
        delete [] hh10;
        delete [] hh11;
        delete [] hh12;

        return C;
}
void convertcolor(char *indir, char *inf, char *outdir, char *outf)
{
	char infilename[50];
	char outfilename[50];
	
	sprintf(infilename, "%s/%s", indir, inf);
	sprintf(outfilename, "%s/%s", outdir, outf);

	cerr<<infilename<<" "<<outfilename<<endl;

        int datasize;
        int num;
        svVector3 *pos1, *pos2;
        svVector3 tmp;

        ifstream infile(infilename);

        infile>>datasize;

        svVector3 * vector = new svVector3[datasize];
        svVector4 * color = new svVector4[datasize];
        pos1 = new svVector3[datasize];
        pos2 = new svVector3[datasize];

	float sum;
        for(int i=0;i<datasize;i++)
        {
                infile>>num;
                infile>>pos1[i].x>>pos1[i].y>>pos1[i].z;
                infile>>pos2[i].x>>pos2[i].y>>pos2[i].z;

                tmp.x = pos2[i].x - pos1[i].x; 
                tmp.y = pos2[i].y - pos1[i].y;
                tmp.z = pos2[i].z - pos1[i].z;

                //tmp.normalize();
		sum = pow(tmp.x,2.) + pow(tmp.y,2.) + pow(tmp.z,2.);
		sum = sqrt(sum);
		tmp.x = tmp.x/sum;
		tmp.y = tmp.y/sum;
		tmp.z = tmp.z/sum;

                vector[i] = tmp;
        }

        color = line2rgb(vector, datasize);
        infile.close();

        ofstream outfile;
        outfile.open(outfilename);

        outfile<<datasize<<endl;
        for(int i=0;i<datasize;i++)
        {
                outfile<<num<<endl;
                outfile<<pos1[i].x<<" "<<pos1[i].y<<" "<<pos1[i].z<<" "
                        <<color[i].r<<" "<<color[i].g<<" "<<color[i].b<<endl;
                outfile<<pos2[i].x<<" "<<pos2[i].y<<" "<<pos2[i].z<<" "
                        <<color[i].r<<" "<<color[i].g<<" "<<color[i].b<<endl;
        }


        outfile.close();

}

int main(int argc, char** argv)
{
	char *indir;
	char *infile;
	char *outdir;	
	char *outfile;

	if(argc!=5)
	{
		cerr << "Usage:  ./boyssurface  [Original Data DirName] [Original Data FileName] [New Data DirName] [New Data FileName] " << endl;  
		exit(0);
	}
	else
	{
		indir = strdup(argv[1]);
		infile = strdup(argv[2]);
		outdir = strdup(argv[3]);
		outfile = strdup(argv[4]);
		convertcolor(indir, infile, outdir, outfile);
	}
}


