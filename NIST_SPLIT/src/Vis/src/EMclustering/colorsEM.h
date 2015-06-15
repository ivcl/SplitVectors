
namespace __em_lib {

void RGB2LAB(double R, double G, double B, double &l, double &a, double &b);
void ColorBlindSafe(double r, double g, double b, double &r2, double &g2, double &b2);
void HSVtoRGB( double &r, double &g, double &b, double h, double s, double v );
void LAB2RGB2(double l, double a, double b, double &R, double &G, double &B);

}

