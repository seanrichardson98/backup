#include <stdio.h>
#include <FPT.h>

#include <D3d_matrix.h>
#include <vecops.h>

int WIDTH = 800;
int HEIGHT = 800;

void color2(double rgb[3], double u, double v) {
    double m[4][4], minv[4][4];
    double a = 1;
    double b = 2;
    rgb[0] = (a*cos(u)+b)*cos(v);
    rgb[1] = (a*cos(u)+b)*sin(v);
    rgb[2] = a*sin(u);
    D3d_make_identity(m);
    D3d_make_identity(minv);
    D3d_scale(m,minv,1.0/6,1.0/6,0.5);
    D3d_scale(m,minv,.75,.75,.75);
    D3d_rotate_z(m,minv,M_PI/2);
    //D3d_rotate_y(m,minv,M_PI/4);
    //D3d_rotate_x(m,minv,M_PI/4);
    D3d_translate(m,minv,.5,0,0);
    D3d_mat_mult_pt(rgb,m,rgb);  

}


void valentines(double rgb[3], double u, double v) {
    double m[4][4], minv[4][4];
    double a = 1;
    double b = 2;
    rgb[0] = (a*cos(u)+b)*cos(v);
    rgb[1] = (a*cos(u)+b)*sin(v);
    rgb[2] = a*sin(u);
    D3d_make_identity(m);
    D3d_make_identity(minv);
    D3d_scale(m,minv,1.0/6,1.0/6,0.5);
    D3d_scale(m,minv,.5,.5,.5);
    //D3d_rotate_x(m,minv,M_PI/4);
    //D3d_rotate_y(m,minv,-M_PI/4);
    D3d_translate(m,minv,.5,0,.5);
    D3d_mat_mult_pt(rgb,m,rgb);  

}


void green_blue(double rgb[3], double u, double v) {
    double m[4][4], minv[4][4];
    double a = 1;
    double b = 2;
    rgb[0] = (a*cos(u)+b)*cos(v);
    rgb[1] = (a*cos(u)+b)*sin(v);
    rgb[2] = a*sin(u);
    D3d_make_identity(m);
    D3d_make_identity(minv);
    D3d_scale(m,minv,1.0/6,1.0/6,0.5);
    D3d_scale(m,minv,.5,.5,.5);
    //D3d_rotate_x(m,minv,M_PI/4);
    //D3d_rotate_y(m,minv,-M_PI/4);
    D3d_translate(m,minv,0,.5,.5);
    D3d_mat_mult_pt(rgb,m,rgb);  
}


void fall(double rgb[3], double u, double v) {
    double m[4][4], minv[4][4];
    double a = 1;
    double b = 2;
    rgb[0] = (a*cos(u)+b)*cos(v);
    rgb[1] = (a*cos(u)+b)*sin(v);
    rgb[2] = a*sin(u);
    D3d_make_identity(m);
    D3d_make_identity(minv);
    D3d_scale(m,minv,1.0/6,1.0/6,0.5);
    D3d_scale(m,minv,.5,.5,.5);
    //D3d_rotate_x(m,minv,M_PI/4);
    //D3d_rotate_y(m,minv,-M_PI/4);
    D3d_translate(m,minv,.5,.5,0);
    D3d_mat_mult_pt(rgb,m,rgb);  
}

void all(double rgb[3], double u, double v) {
    double m[4][4], minv[4][4];
    double a = 1;
    double b = 2;
    rgb[0] = (a*cos(u)+b)*cos(v);
    rgb[1] = (a*cos(u)+b)*sin(v);
    rgb[2] = a*sin(u);
    D3d_make_identity(m);
    D3d_make_identity(minv);
    D3d_scale(m,minv,1.0/6,1.0/6,0.5);
    D3d_translate(m,minv,.5,.5,.5);
    D3d_mat_mult_pt(rgb,m,rgb);  
} 

int main() {
    G_init_graphics(800,800);
    G_rgb(0,0,0);
    G_clear();
    int i,j;
    double u,v;
    double rgb[3];
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            u = (double)i*2*M_PI/HEIGHT;
            v = (double)j*2*M_PI/WIDTH;
            all(rgb,u,v);
            G_rgb(rgb[0],rgb[1],rgb[2]);
            //G_rgb(1,0,1);
            G_point(i,j); 
        }
    }
    while (1) {G_wait_key();}
}
