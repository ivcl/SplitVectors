#include "colorsEM.h"
#include <cmath>
#include <iostream>
using namespace std;

namespace __em_lib {
void RGB2LAB(double R, double G, double B, double &l, double &a, double &b)
{
	double lab[3];


	
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

	l = lab[0];
	a = lab[1];
	b = lab[2];
}

void LAB2RGB(double l, double a, double b, double &R, double &G, double &B)
{
	double lab[3];
	lab[0] = l;
	lab[1] = a;
	lab[2] = b;

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

    	R = 3.240479 * X - 1.537150 * Y - 0.498535 * Z;
    	G = -0.969256 * X + 1.875992 * Y + 0.041556 * Z;
    	B = 0.055648 * X - 0.204043 * Y + 1.057311 * Z;

	if(R<0) R = 0;
	if(R>1) R = 1;
	if(G<0) G = 0;
	if(G>1) G = 1;
	if(B<0) B = 0;	
	if(B>1) B = 1;

	double r1,g1,b1;
	r1 = R;
	g1 = G;
	b1 = B;

//ColorBlindSafe(r1,g1,b1,R,G,B);
}

void ColorBlindSafe(double r, double g, double b, double &r2, double &g2, double &b2)
{
	double colorl, colora, colorb;

	RGB2LAB(r,g,b,colorl,colora,colorb);
		
	double phimaxl = 3.1415926/4;
	double phimaxr = 3.1415926/4;

	double phi = 0;
	double tmpa = 0;
	double tmpb = 0;


		tmpa = colora;	
		tmpb = colorb;
		if(tmpa!=0)
			phi = phimaxl * (1 - abs(atan(tmpb/tmpa))/(3.1415926/2));
		colora = tmpa * cos(phi) - tmpb * sin(phi);
		colorb = tmpa * sin(phi) + tmpb * cos(phi);	

	LAB2RGB(colorl, colora, colorb, r2,g2,b2);

}
void HSVtoRGB( double &r, double &g, double &b, double h, double s, double v )
{
	double hprime = h/60.;
	double c = v * s;//cerr<<c<<endl;
	double mod = fmod(hprime,2.);
	double tmp = 1 - abs(mod-1.);
	double x = c * tmp;//cerr<<x<<endl;

	if(hprime>=0&&hprime<1) 
	{
		r = c;
		g = x;
		b = 0;
	}
	else if(hprime>=1&&hprime<2) 
	{
		r = x;
		g = c;
		b = 0;
	}
	else if(hprime>=2&&hprime<3) 
	{
		r = 0;
		g = c;
		b = x;
	}
	else if(hprime>=3&&hprime<4) 
	{
		r = 0;
		g = x;
		b = c;
	}
	else if(hprime>=4&&hprime<5) 
	{
		r = x;
		g = 0;
		b = c;
	}
	else if(hprime>=5&&hprime<6) 
	{
		r = c;
		g = 0;
		b = x;
	}
	//cerr<<r<<" "<<g<<" "<<b<<endl;
	double m = v - c;//cerr<<m<<endl;
	r = r + m;
	g = g + m;
	b = b + m;//cerr<<r<<" "<<g<<" "<<b<<endl;
	double r1= r;
	double g1 = g;
	double b1 = b;

	ColorBlindSafe(r1,g1,b1,r,g,b);
}
void LAB2RGB2(double l, double a, double b, double &R, double &G, double &B)
{
	double lab[3];
	lab[0] = l;
	lab[1] = a;
	lab[2] = b;

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

    	R = 3.240479 * X - 1.537150 * Y - 0.498535 * Z;
    	G = -0.969256 * X + 1.875992 * Y + 0.041556 * Z;
    	B = 0.055648 * X - 0.204043 * Y + 1.057311 * Z;

	if(R<0) R = 0;
	if(R>1) R = 1;
	if(G<0) G = 0;
	if(G>1) G = 1;
	if(B<0) B = 0;	
	if(B>1) B = 1;

	double r1,g1,b1;
	r1 = R;
	g1 = G;
	b1 = B;

ColorBlindSafe(r1,g1,b1,R,G,B);
}

}
