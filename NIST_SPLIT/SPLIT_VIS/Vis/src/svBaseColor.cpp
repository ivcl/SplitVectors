/*
 *  * svBaseColor.cpp
 *   *
 *    */

#include "svBaseColor.h"
#include <iostream>
#include <fstream>
#include <cmath>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "./jcUtil/MGL.h"

using namespace std;

namespace __svl_lib {

void svBaseColor::clean()
{
        if(colors!=NULL)
                delete [] colors;
        if(legendPos!=NULL)
                delete [] legendPos;
        if(legendCol!=NULL)
                delete [] legendCol;
	colorsarray[0].free();
	delete [] colorsarray;
	colorsarray = NULL;
}

void svBaseColor::SetColor(svVector4Array *c, int datasize)
{
        colors = new svVector4[datasize];

        for(int i=0;i<datasize;i++)
        {
                colors[i][0] = c[0][i][0];
                colors[i][1] = c[0][i][1];
                colors[i][2] = c[0][i][2];
                colors[i][3] = c[0][i][3];
        }

	colornum = datasize;
}
void svBaseColor::SetColor(svVector4 *c, int datasize)
{
        colors = new svVector4[datasize];

        for(int i=0;i<datasize;i++)
        {
                colors[i][0] = c[i][0];
                colors[i][1] = c[i][1];
                colors[i][2] = c[i][2];
                colors[i][3] = c[i][3];
        }

	colornum = datasize;
}
svVector3 svBaseColor::RGB2LAB(svVector4 rgb)
{
	svVector3 lab;

	double R = rgb[0];
	double G = rgb[1];
	double B = rgb[2];
	
    	double X =  0.412453 * R + 0.357580 * G + 0.180423 * B;
    	double Y =  0.212671 * R + 0.715160 * G + 0.072169 * B;
    	double Z =  0.019334 * R + 0.119193 * G + 0.950227 * B;

    	double xr = X / 0.950456, yr = Y / 1.000, zr = Z / 1.088854;

    	if(yr > 0.008856) lab[0] = 116.0 * pow(yr, 1.0 / 3.0) - 16.0;
	else lab[0] = 903.3 * yr;

	double fxr, fyr, fzr;

    	if(xr > 0.008856) fxr = pow(xr, 1.0 / 3.0);
	else fxr = 7.787 * xr + 16.0 / 116.0;

	if(yr > 0.008856) fyr = pow(yr, 1.0 / 3.0);
	else fyr = 7.787 * yr + 16.0 / 116.0;

	if(zr > 0.008856) fzr = pow(zr, 1.0 / 3.0);
	else fzr = 7.787 * zr + 16.0 / 116.0;

	lab[1] = 500.0 * (fxr - fyr);
	lab[2] = 200.0 * (fyr - fzr);

	return lab;
}

svVector4 svBaseColor::LAB2RGB(svVector3 lab)
{
	 double X, Y, Z;
   	 double P = (lab[0] + 16.0) / 116.0;

	if(lab[0] > 7.9996) Y = 1.000 * P * P * P;
	else Y = 1.000 * lab[0] / 903.3;

    	double yr = Y / 1.000, fy;
	if(yr > 0.008856) fy = pow(yr, 1.0 / 3.0);
	else fy = 7.787 * yr + 16.0 / 116.0;

	double fx = lab[1] / 500.0 + fy, fz = fy - lab[2] / 200.0;

	if(fx > 0.2069) X = 0.950456 * fx * fx * fx;
	else X = 0.950456 / 7.787 * (fx - 16.0 / 116.0);

	if(fz > 0.2069) Z = 1.088854 * fz * fz * fz;
	else Z = 1.088854 / 7.787 * (fz - 16.0 / 116.0);

    	double R = 3.240479 * X - 1.537150 * Y - 0.498535 * Z;
    	double G = -0.969256 * X + 1.875992 * Y + 0.041556 * Z;
    	double B = 0.055648 * X - 0.204043 * Y + 1.057311 * Z;

	if(R<0) R = 0;
	if(R>1) R = 1;
	if(G<0) G = 0;
	if(G>1) G = 1;
	if(B<0) B = 0;	
	if(B>1) B = 1;

	svVector4 rgb;
	rgb[0]=R;rgb[1]=G;rgb[2]=B;rgb[3]=1;
	
	return rgb;
}

void svBaseColor::ColorBlindSafe()
{
	svVector3 *colorlab = new svVector3[colornum];
	for(int i=0;i<colornum;i++)
	{
		colorlab[i] = RGB2LAB(colors[i]);
	}	

	double phimaxl = 3.1415926/4;
	double phimaxr = 3.1415926/4;

	double phi = 0;
	double tmpa = 0;
	double tmpb = 0;

	for(int i=0;i<colornum;i++)
	{
		tmpa = colorlab[i][1];	
		tmpb = colorlab[i][2];
		if(tmpa!=0)
			phi = phimaxl * (1 - fabs(atan(tmpb/tmpa))/(3.1415926/2));
		colorlab[i][1] = tmpa * cos(phi) - tmpb * sin(phi);
		colorlab[i][2] = tmpa * sin(phi) + tmpb * cos(phi);	
	}

	for(int i=0;i<colornum;i++)
	{
		colors[i] = LAB2RGB(colorlab[i]);
	}

	delete [] colorlab;
}

svVector4Array * svBaseColor::ColorBlindSafe(svVector4Array *c, int datasize)
{
	svVector4Array *tmpc = new svVector4Array[datasize];
	svVector4 col;
        svVector3 *colorlab = new svVector3[colornum];
        for(int i=0;i<colornum;i++)
        {
		col[0] = c[0][i][0];
		col[1] = c[0][i][1];
		col[2] = c[0][i][2];
		col[3] = c[0][i][3];
                colorlab[i] = RGB2LAB(col);
        }

        double phimaxl = 45;
        double phimaxr = 45;

        double phi = 0;
        double tmpa = 0;
        double tmpb = 0;

        for(int i=0;i<colornum;i++)
        {
                tmpa = colorlab[i][1];
                tmpb = colorlab[i][2];
                if(tmpa!=0)
                        phi = phimaxl * (1 - fabs(atan(tmpb/tmpa))/90);
                colorlab[i][1] = tmpa * cos(phi) - tmpb * sin(phi);
                colorlab[i][2] = tmpa * sin(phi) + tmpb * cos(phi);
        }

        for(int i=0;i<colornum;i++)
        {
                col = LAB2RGB(colorlab[i]);
		tmpc[0][i][0] = col[0];
		tmpc[0][i][1] = col[1];
		tmpc[0][i][2] = col[2];
		tmpc[0][i][3] = 1;
        }

        delete [] colorlab;
	
	return tmpc;
}

}
