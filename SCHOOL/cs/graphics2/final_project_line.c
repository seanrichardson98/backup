#include <stdio.h>
#include <FPT.h>

#include <D3d_matrix.h>
#include <vecops.h>

double HA = M_PI/4;
double zbuf[800][800];
double AMB = 0.2;
double DIFF_MAX = 0.5;
double SPEC_POW = 30;
double LIGHT[3] = {100,200,-50};
double EYE[3] = {0,0,0};
double IN_COLOR = 0.7;

typedef void (*ParaEquation)();
typedef void (*ColorPattern)();
struct Thing3D {
    double m[4][4]; 
    double minv[4][4];
    double ur[3]; // {ulo, uhi, ustep}
    double vr[3]; // {vlo, vhi, vstep}
    ParaEquation eq;
    ColorPattern pat;
    double color1[3];
    double color2[3];
};
struct Thing3D_Moving {
    double m[4][4]; 
    double minv[4][4];
    double ur[3]; // {ulo, uhi, ustep}
    double vr[3]; // {vlo, vhi, vstep}
    double tr[3]; // {tlo, thi, tstep}
    ParaEquation eq;
    ColorPattern pat;
    double color1[3];
    double color2[3];

};

void init_zbuf() {
    int i,j;
    for (i = 0; i < 800; i++) {
        for (j = 0; j < 800; j++) {
            zbuf[i][j] = 0;
        }
    }
}
double get_ak(int k) {
    return 2*(1-pow(-1,k))*sin(k*M_PI/2)/(pow(k,2)*M_PI);
}
int main() {
    G_init_graphics(800,800);
    G_rgb(0,0,0);
    G_clear();
    G_rgb(1,1,1);
    //init_zbuf();
    double t,x,y,sum;
    int k;
    for (t = 0; t < 10; t+=0.01) {
        for (x = 0; x < M_PI; x+=0.01) {
            double y = 0;
            for (k = 1; k < 100; k++) {
                double ak = get_ak(k);
                y += ak*cos(k*t)*sin(k*x);
            }
            G_point(x*100, y*100+200);  
            //printf("(%lf,%lf)\n",x*100,y*100+200);
        }
    G_wait_key();
    G_rgb(0,0,0);
    G_clear();
    G_rgb(1,1,1);
    }
}
