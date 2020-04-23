#include <stdio.h>
#include <FPT.h>

#include <vecops.h>

double init[3];
double change[3];


void rand_color(double RGB[3], double t) {
    //t from 0 to 1;
    int i;
    for (i = 0; i <= 2; i++) {
        RGB[i] = change[i]*t+init[i];
    }
}
double r2() {
    return (double)rand()/(double)RAND_MAX;
}
void randomize() {
    int i;
    for (i = 0; i <= 2; i++) {
        init[i] = r2();
        change[i] = r2()*(1-init[i]);
    }
}
void color(double RGB[3], double t) {
    RGB[0] = t;
    RGB[1] = 1-t;
    RGB[2] = 0;
}
void test_color(double RGB[3], double u, double v, double r) {
    RGB[0] = 1-r;
    RGB[1] = r;
    RGB[2] = 0;
}

int main() {
    G_init_graphics(800,800);
    
    int xp, yp;
    double u,v,r,t;
    int k,l;
    k = 3;
    l = 3;

    double tstep = 0.1;

    double RGB[3];
    char fname[100];

    for (t = 0; t < 2*M_PI; t += tstep) {
        randomize();
        int i = t/tstep+0.1;
        printf("frame %d\n",i);
        for (xp = 0; xp < 800; xp++) {
            for (yp = 0; yp < 800; yp++) {
                u = xp/800.0*2*M_PI;
                v = yp/800.0*2*M_PI;
                r = cos(t)*cos(k*u)*cos(l*v);
                double inten = (r+1)/2;
                test_color(RGB,u,v,inten);
                //print_vec(RGB);
                G_rgb(RGB[0],RGB[1],RGB[2]);
                //G_rgb(inten,inten,inten);
                G_point(xp,yp);
            }
        }
        sprintf(fname,"flat_torus/pic%04d.xwd",i);
        G_save_image_to_file(fname);
        G_wait_key();
    }
}
