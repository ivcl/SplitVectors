#include "svBoySurface.h"
#include <cmath>


#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define RADS 0.01744444444 //pi/180

using namespace std;
namespace __svl_lib{
svFloat CC(svFloat a, svFloat d)
{
        svFloat r;
        r = a * cos(d * RADS);
        return r;
}

svFloat SS(svFloat a, svFloat d)
{
        svFloat r;
        r = a * sin(d * RADS);
        return r;
}
svBoySurface::svBoySurface(int dataSize)
{
        colors = new svVector4[dataSize];
	colornum = dataSize;
}

svVector4Array * svBoySurface::line2rgb(svVector3Array *vector, int datasize)
{
	 if(datasize<1)
        {
                cerr<<"Insufficient number of arguments"<<endl;
        }

        svFloat x, y, z;
        svFloat xx2, yy2, zz2;
        svFloat xx3, yy3, zz3;
        svFloat zz4;
        svFloat xy, xz, yz;
        svFloat X, Y, Z;

        svFloat *hh1 = new svFloat[datasize];
        svFloat *hh2 = new svFloat[datasize];
        svFloat *hh3 = new svFloat[datasize];
        svFloat *hh4 = new svFloat[datasize];
        svFloat *hh5 = new svFloat[datasize];
        svFloat *hh6 = new svFloat[datasize];
        svFloat *hh7 = new svFloat[datasize];
        svFloat *hh8 = new svFloat[datasize];
        svFloat *hh9 = new svFloat[datasize];
        svFloat *hh10 = new svFloat[datasize];
        svFloat *hh11 = new svFloat[datasize];
        svFloat *hh12 = new svFloat[datasize];

        svVector4Array *C = new svVector4Array[1];
        svVector4 tmp;

        svFloat s0 = -23.0;
        svFloat s1 = 227.9;
        svFloat s2 = 251.0;
        svFloat s3 = 125.0;

        svFloat ss23, ss45, ss67, ss89;
        svFloat cc23, cc45, cc67, cc89;
       svFloat w_x = 4.1925;
        svFloat trl_x = -2.0425;

        svFloat w_y = 4.0217 ;
        svFloat trl_y = -1.8541 ;

        svFloat w_z = 4.0694;
        svFloat trl_z = -2.1899;


        ss23 = SS(2.71, s0); cc23 = CC(2.71, s0);
        ss45 = SS(2.12, s1); cc45 = CC(2.12, s1);
        ss67 = SS(.972, s2); cc67 = CC(.972, s2);
        ss89 = SS(.868, s3); cc89 = CC(.868, s3);
        for(int i=0;i<datasize;i++)
        {
                x = vector[0][i][0];
                y = vector[0][i][1];
                z = vector[0][i][2];
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

                tmp[3] = 1;
                tmp[0] = 0.9 * fabs((X-trl_x)/w_x) + 0.05;
                tmp[1] = 0.9 * fabs((Y-trl_y)/w_y) + 0.05;
                tmp[2] = 0.9 * fabs((Z-trl_z)/w_z) + 0.05;

                C[0].add(tmp);

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
svVector4 * svBoySurface::line2rgb(svVector3 *vector, int datasize)
{
	 if(datasize<1)
        {
                cerr<<"Insufficient number of arguments"<<endl;
        }

        svFloat x, y, z;
        svFloat xx2, yy2, zz2;
        svFloat xx3, yy3, zz3;
        svFloat zz4;
        svFloat xy, xz, yz;
        svFloat X, Y, Z;

        svFloat *hh1 = new svFloat[datasize];
        svFloat *hh2 = new svFloat[datasize];
        svFloat *hh3 = new svFloat[datasize];
        svFloat *hh4 = new svFloat[datasize];
        svFloat *hh5 = new svFloat[datasize];
        svFloat *hh6 = new svFloat[datasize];
        svFloat *hh7 = new svFloat[datasize];
        svFloat *hh8 = new svFloat[datasize];
        svFloat *hh9 = new svFloat[datasize];
        svFloat *hh10 = new svFloat[datasize];
        svFloat *hh11 = new svFloat[datasize];
        svFloat *hh12 = new svFloat[datasize];

        svVector4 *C = new svVector4[datasize];
        svVector4 tmp;

        svFloat s0 = -23.0;
        svFloat s1 = 227.9;
        svFloat s2 = 251.0;
        svFloat s3 = 125.0;

        svFloat ss23, ss45, ss67, ss89;
        svFloat cc23, cc45, cc67, cc89;
       svFloat w_x = 4.1925;
        svFloat trl_x = -2.0425;

        svFloat w_y = 4.0217 ;
        svFloat trl_y = -1.8541 ;

        svFloat w_z = 4.0694;
        svFloat trl_z = -2.1899;


        ss23 = SS(2.71, s0); cc23 = CC(2.71, s0);
        ss45 = SS(2.12, s1); cc45 = CC(2.12, s1);
        ss67 = SS(.972, s2); cc67 = CC(.972, s2);
        ss89 = SS(.868, s3); cc89 = CC(.868, s3);
        for(int i=0;i<datasize;i++)
        {
                x = vector[i][0];
                y = vector[i][1];
                z = vector[i][2];
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

                tmp[3] = 1;
                tmp[0] = 0.9 * fabs((X-trl_x)/w_x) + 0.05;
                tmp[1] = 0.9 * fabs((Y-trl_y)/w_y) + 0.05;
                tmp[2] = 0.9 * fabs((Z-trl_z)/w_z) + 0.05;

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
void svBoySurface::convertcolor(char *indir, char *inf, char *outdir, char *outf)
{
	char infilename[50];
	char outfilename[50];
	
	sprintf(infilename, "%s%s", indir, inf);
	sprintf(outfilename, "%s%s", outdir, outf);

        int datasize;
        int num;
        svVector3 *pos1, *pos2;
        svVector3 tmp;

        ifstream infile(infilename);

        infile>>datasize;

        svVector3Array * vector = new svVector3Array[1];
        svVector4Array * color = new svVector4Array[1];
        pos1 = new svVector3[datasize];
        pos2 = new svVector3[datasize];


        for(int i=0;i<datasize;i++)
        {
                infile>>num;
                infile>>pos1[i][0]>>pos1[i][1]>>pos1[i][2];
                infile>>pos2[i][0]>>pos2[i][1]>>pos2[i][2];

                tmp[0] = pos2[i][0] - pos1[i][0];
                tmp[1] = pos2[i][1] - pos1[i][1];
                tmp[2] = pos2[i][2] - pos1[i][2];

                tmp.normalize();

                vector[0].add(tmp);
        }

        color = line2rgb(vector, datasize);
        infile.close();

        ofstream outfile;
        outfile.open(outfilename);

        outfile<<datasize<<endl;
        for(int i=0;i<datasize;i++)
        {
                outfile<<num<<endl;
                outfile<<pos1[i][0]<<" "<<pos1[i][1]<<" "<<pos1[i][2]<<" "
                        <<color[0][i][0]<<" "<<color[0][i][1]<<" "<<color[0][i][2]<<endl;
                outfile<<pos2[i][0]<<" "<<pos2[i][1]<<" "<<pos2[i][2]<<" "
                        <<color[0][i][0]<<" "<<color[0][i][1]<<" "<<color[0][i][2]<<endl;
        }


        outfile.close();

}
void svBoySurface::readcolor(char *dir, char *file)
{
  long double dummy;
  long double ln;
  int i;

  svVector3 pos;
  svVector3 vel;
  svVector4 col;

  col[3]=1;

  svChar filename[2056];
  sprintf(filename, "%s/%s", dir, file);

  ifstream infile(filename);
  if(infile.fail())
  {
    cerr << file << "not found.\n";
        cerr << "Load QDOT data failed. prog exit\n" <<endl;
        exit(-1);
  }
  else
  {
        cerr << "Loading data: " << filename << endl;
  }

  infile>>ln;
  for(i=0;i<ln;i++)
  {
        infile>>dummy;
        infile>>pos[0]>>pos[1]>>pos[2]>>col[0]>>col[1]>>col[2];
        infile>>vel[0]>>vel[1]>>vel[2]>>col[0]>>col[1]>>col[2];

        col[0] = col[0] * 255;
        col[1] = col[1] * 255;
        col[2] = col[2] * 255;
        col[3] = col[3] * 255;

        colors[i]=col;
  };

    infile.close();
  
}
void svBoySurface::readlegend(char *dir, char *file)
{
        svScalar ln1,ln2;
        svVector3 pos;
        svVector4 col;

        col[3] = 1;

        svChar filename[2056];
        sprintf(filename, "%s/%s", dir, file);
        ifstream infile(filename);
        if(infile.fail())
        {
                cerr << file << "not found.\n";
                exit(-1);
        }
        else
        {
                cerr << "Loading data: " << filename << endl;
        }

        infile>>ln1;

        legendPos = new svVector3[LEGEND_1*LEGEND_2];
        legendCol = new svVector4[LEGEND_1*LEGEND_2];

        for(int i=0; i<LEGEND_1; i++)
        {
                infile>>ln2;
                for(int j=0;j<LEGEND_2;j++)
                {
                        infile>>pos[0]>>pos[1]>>pos[2]>>col[0]>>col[1]>>col[2];

                        legendPos[i*LEGEND_2+j] = pos;
                        legendCol[i*LEGEND_2+j] = col;
                }
        }
        infile.close();
}
void svBoySurface::RenderLegend()
{
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

        glLineWidth(1);
        glColor3f(1.,1,1);

        glLineWidth(1);
        int i,j;
        for(i=0;i<LEGEND_1;i++)
        {
                for(j=0;j<LEGEND_2-1;j++)
                {
                        if(i<LEGEND_1-1)
                        {glBegin(GL_TRIANGLES);
                        glColor4f(legendCol[j+i*LEGEND_2][0],legendCol[j+i*LEGEND_2][1],legendCol[j+i*LEGEND_2][2],legendCol[j+i*LEGEND_2][3]);
                        glVertex3f(legendPos[j+i*LEGEND_2][0],legendPos[j+i*LEGEND_2][1],legendPos[j+i*LEGEND_2][2]);

                        glColor4f(legendCol[j+i*LEGEND_2+1+LEGEND_2][0],legendCol[j+i*LEGEND_2+1+LEGEND_2][1],legendCol[j+i*LEGEND_2+1+LEGEND_2][2],legendCol[j+i*LEGEND_2+1+LEGEND_2][3]);
                        glVertex3f(legendPos[j+i*LEGEND_2+1+LEGEND_2][0],legendPos[j+i*LEGEND_2+1+LEGEND_2][1],legendPos[j+i*LEGEND_2+1+LEGEND_2][2]);

                        glColor4f(legendCol[j+i*LEGEND_2+1][0],legendCol[j+i*LEGEND_2+1][1],legendCol[j+i*LEGEND_2+1][2],legendCol[j+i*LEGEND_2+1][3]);
                        glVertex3f(legendPos[j+i*LEGEND_2+1][0],legendPos[j+i*LEGEND_2+1][1],legendPos[j+i*LEGEND_2+1][2]);


                        glEnd();
                        glBegin(GL_TRIANGLES);
                        glColor4f(legendCol[j+i*LEGEND_2][0],legendCol[j+i*LEGEND_2][1],legendCol[j+i*LEGEND_2][2],legendCol[j+i*LEGEND_2][3]);
                        glVertex3f(legendPos[j+i*LEGEND_2][0],legendPos[j+i*LEGEND_2][1],legendPos[j+i*LEGEND_2][2]);

                        glColor4f(legendCol[j+i*LEGEND_2+LEGEND_2][0],legendCol[j+i*LEGEND_2+LEGEND_2][1],legendCol[j+i*LEGEND_2+LEGEND_2][2],legendCol[j+i*LEGEND_2+LEGEND_2][3]);
                        glVertex3f(legendPos[j+i*LEGEND_2+LEGEND_2][0],legendPos[j+i*LEGEND_2+LEGEND_2][1],legendPos[j+i*LEGEND_2+LEGEND_2][2]);

                        glColor4f(legendCol[j+i*LEGEND_2+1+LEGEND_2][0],legendCol[j+i*LEGEND_2+1+LEGEND_2][1],legendCol[j+i*LEGEND_2+1+LEGEND_2][2],legendCol[j+i*LEGEND_2+1+LEGEND_2][3]);
                        glVertex3f(legendPos[j+i*LEGEND_2+1+LEGEND_2][0],legendPos[j+i*LEGEND_2+1+LEGEND_2][1],legendPos[j+i*LEGEND_2+1+LEGEND_2][2]);


                        glEnd();}
                        else
                        {glBegin(GL_TRIANGLES);
                        glColor4f(legendCol[j+i*LEGEND_2][0],legendCol[j+i*LEGEND_2][1],legendCol[j+i*LEGEND_2][2],legendCol[j+i*LEGEND_2][3]);
                        glVertex3f(legendPos[j+i*LEGEND_2][0],legendPos[j+i*LEGEND_2][1],legendPos[j+i*LEGEND_2][2]);

                        glColor4f(legendCol[j+1][0],legendCol[j+1][1],legendCol[j+1][2],legendCol[j+1][3]);
                        glVertex3f(legendPos[j+1][0],legendPos[j+1][1],legendPos[j+1][2]);

                        glColor4f(legendCol[j+i*LEGEND_2+1][0],legendCol[j+i*LEGEND_2+1][1],legendCol[j+i*LEGEND_2+1][2],legendCol[j+i*LEGEND_2+1][3]);
                        glVertex3f(legendPos[j+i*LEGEND_2+1][0],legendPos[j+i*LEGEND_2+1][1],legendPos[j+i*LEGEND_2+1][2]);


                        glEnd();
                        glBegin(GL_TRIANGLES);
                        glColor4f(legendCol[j+i*LEGEND_2][0],legendCol[j+i*LEGEND_2][1],legendCol[j+i*LEGEND_2][2],legendCol[j+i*LEGEND_2][3]);
                        glVertex3f(legendPos[j+i*LEGEND_2][0],legendPos[j+i*LEGEND_2][1],legendPos[j+i*LEGEND_2][2]);

                        glColor4f(legendCol[j][0],legendCol[j][1],legendCol[j][2],legendCol[j][3]);
                        glVertex3f(legendPos[j][0],legendPos[j][1],legendPos[j][2]);

                        glColor4f(legendCol[j+1][0],legendCol[j+1][1],legendCol[j+1][2],legendCol[j+1][3]);
                        glVertex3f(legendPos[j+1][0],legendPos[j+1][1],legendPos[j+1][2]);


                        glEnd();}
                }

        }


        glDisable(GL_BLEND);


}
  

}
