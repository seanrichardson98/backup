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

void init_zbuf() {
    int i,j;
    for (i = 0; i < 800; i++) {
        for (j = 0; j < 800; j++) {
            zbuf[i][j] = 0;
        }
    }
}
//PARAMETRIC EQUATIONS:
void sphere1(double p[3], double u, double v) {
    // 0 < u < 2pi; -pi/2 < v < pi/2;
    p[0] = cos(u)*cos(v);
    p[1] = sin(v);
    p[2] = sin(u)*cos(v);
}
void sphere2(double p[3], double u, double v) {
    // 0 < u < 2pi; -pi/2 < v < pi/2;
    p[0] = sqrt(1-pow(v,2))*sin(u);
    p[1] = v;
    p[2] = sqrt(1-pow(v,2))*cos(u);
}
void hyper(double p[3], double u, double v) {
    // 0 < u < 2pi; -1 < v < 1;
    double r = sqrt(1+pow(v,2));
    p[0] = r*sin(u);
    p[1] = v;
    p[2] = r*cos(u);
}
void draw_point(double p[3]) {
    int x = (400/tan(HA))*(p[0]/p[2])+400;
    int y = (400/tan(HA))*(p[1]/p[2])+400;
    int inside = (x >= 0 && x < 800 && y >= 0 && y < 800);
    if (inside && (zbuf[x][y] == 0 || zbuf[x][y] > p[2])) {
        zbuf[x][y] = p[2];
        G_point(x,y);
    }
}
//LIGHT MODEL:
double light_intensity(double *p, double *n) {
    double SPEC_MAX = (1-AMB-DIFF_MAX);
    double e[3], l[3], r[3];
    //set vecs
    subtract_vec(e, EYE,p); normalize(e);
    subtract_vec(l, LIGHT,p); normalize(l);
    scale_vec(r, n, 2*d_prod(n,l));
    subtract_vec(r,r,l);
    //normal direction check
    double diffuse, specular;
    if (d_prod(e,n) < 0 && d_prod(l,n) < 0)
       scale_vec(n,n,-1);
    //calc
    diffuse = DIFF_MAX*d_prod(n,l);
    specular = SPEC_MAX*pow(d_prod(r,e),SPEC_POW);
    if (diffuse < 0) {diffuse = 0;}
    if (specular < 0) {specular = 0;}
    if (d_prod(e,n)*d_prod(l,n) < 0) { 
        specular = 0; diffuse = 0;
    }
    return AMB + diffuse + specular;
}
void color(double ret[3], double RGB[3], double inten) {
    double white[3] = {1,1,1};
    double black[3] = {0,0,0};
    double hold[3];
    double scale;
    if (inten > IN_COLOR) {
        subtract_vec(hold, white,RGB);
        scale = (inten-IN_COLOR)/(1-IN_COLOR);
    }
    else {
        subtract_vec(hold, black, RGB);
        scale = (IN_COLOR-inten)/(IN_COLOR);
    }
    scale_vec(ret,hold,scale);
    add_vec(ret, ret, RGB);
}
//COLOR PATTERNS:
void cool_color1(double RGB[3], double u, double v, struct Thing3D D) {
    double udiff = D.ur[1]-D.ur[0]; double vdiff = D.vr[1]-D.vr[0]; 
    RGB[0] = (u-D.ur[0])/(udiff); RGB[1] = (v-D.vr[0])/vdiff; RGB[2] = 1-(u-D.ur[0])*(v-D.vr[0])/(udiff*vdiff);
}
void checker(double RGB[3], double u, double v, struct Thing3D D) {
    double udiff = D.ur[1]-D.ur[0]; double vdiff = D.vr[1]-D.vr[0]; 
    int freq = 20;
    int a = (u-D.ur[0])/udiff*freq;
    int b = (v-D.vr[0])/vdiff*freq;
    if ((a+b)%2) {copy_vec(RGB, D.color1);} 
    else {copy_vec(RGB, D.color2);}
}
void plain(double RGB[3], double u, double v, struct Thing3D D) {
    copy_vec(RGB, D.color1);
}

void draw_object(struct Thing3D D) {
    double RGB[3], rgb[3];
    double u,v;
    double p[3], q[3], r[3], n[3];
    double cent[3] = {0,0,0};
    D3d_mat_mult_pt(cent,D.m,cent);
    double du = 0.001; double dv = 0.001;
    for (u = D.ur[0]; u <= D.ur[1]; u += D.ur[2]) {
        for (v = D.vr[0]; v <= D.vr[1]; v += D.vr[2]) {
            //point:
            D.eq(p,u,v); D3d_mat_mult_pt(p,D.m,p);
            //normal:
            D.eq(q,u+du,v); D3d_mat_mult_pt(q,D.m,q);
            D.eq(r,u,v+dv); D3d_mat_mult_pt(r,D.m,r);
            subtract_vec(q,q,p);
            subtract_vec(r,r,p);
            x_prod(n,q,r);
            normalize(n);
            //light and color
            D.pat(RGB,u,v,D);
            //cool_color1(RGB,u,v);
            double intensity = light_intensity(p,n);
            color(rgb,RGB,intensity); 
            G_rgb(rgb[0],rgb[1],rgb[2]);
            draw_point(p);
        }
    }

}

void rand_draw( double m[4][4], double RGB[3], void (*object)(), double u_lo, double u_hi, double v_lo, double v_hi) {
    double rgb[3];
    double u,v,i;
    double p[3];
    double cent[3] = {0,0,0};
    D3d_mat_mult_pt(cent,m,cent);
    for (i = 0; i < 10000; i++) {
        u = drand48()*(u_hi-u_lo)+u_lo;
        v = drand48()*(v_hi-v_lo)+v_lo;
        object(p,u,v);
        D3d_mat_mult_pt(p,m,p);
        double intensity = light_intensity(p,cent);
        color(rgb,RGB,intensity); 
        G_rgb(rgb[0],rgb[1],rgb[2]);
        draw_point(p);
    }
}
void eye_path1(double eye[3], double t) {
    double r = 4;
    eye[0] = r*sin(t);
    eye[1] = 0;
    eye[2] = r*cos(t);
}
void make_hyper(struct Thing3D D) {
     
}
void make_sphere(struct Thing3D D, double res) {
    D3d_make_identity(D.m);
    D3d_make_identity(D.minv);
    D.ur[0] = 0;      D.vr[0] = -M_PI/2;
    D.ur[1] = 2*M_PI; D.vr[1] = M_PI/2;
    D.ur[2] = res;    D.vr[2] = res;
    D.eq = &sphere1;  D.pat = &plain;
    D.color1[0] = 0; D.color1[1] = 0; D.color1[2] = 0; 
    D.color2[0] = 0; D.color2[1] = 0; D.color2[2] = 0; 
}
int main() {
    //vars:
    int i;
    double view[4][4], vinv[4][4];
    double coi[3], up[3], eye[3];
    //colors
    double RED[3]   = {1,0,0};
    double GREEN[3] = {0,1,0};
    double BLUE[3]  = {0,0,1};
    double BLACK[3] = {0,0,0};
    double WHITE[3] = {1,1,1};
    //specifications:
    double s = 1;
    double tz = 0;
    double ty = 1.5;
    double res = 0.005;
    //init
    G_init_graphics(800,800);
    G_rgb(0,0,0);
    G_clear();
    init_zbuf();

    struct Thing3D test_sphere;
    D3d_make_identity(test_sphere.m);
    D3d_make_identity(test_sphere.minv);
    D3d_translate(test_sphere.m,test_sphere.minv,0,-(ty+sqrt(3)/2),tz);
    test_sphere.ur[0] = 0;      test_sphere.vr[0] = -M_PI/2;
    test_sphere.ur[1] = 2*M_PI; test_sphere.vr[1] = M_PI/2;
    test_sphere.ur[2] = res;    test_sphere.vr[2] = res;
    test_sphere.eq = &sphere1;
    test_sphere.pat = &plain;
    copy_vec(test_sphere.color1, RED);

    struct Thing3D top_sphere;
    D3d_make_identity(top_sphere.m);
    D3d_make_identity(top_sphere.minv);
    D3d_translate(top_sphere.m,top_sphere.minv,0, (ty+sqrt(3)/2),tz);
    top_sphere.ur[0] = 0;      top_sphere.vr[0] = -M_PI/2;
    top_sphere.ur[1] = 2*M_PI; top_sphere.vr[1] = M_PI/2;
    top_sphere.ur[2] = res;  top_sphere.vr[2] = res;
    top_sphere.eq = &sphere1;
    top_sphere.pat = &checker;
    copy_vec(top_sphere.color1, RED);
    copy_vec(top_sphere.color2, BLACK);

    struct Thing3D bottom_sphere;
    D3d_make_identity(bottom_sphere.m);
    D3d_make_identity(bottom_sphere.minv);
    D3d_translate(bottom_sphere.m,bottom_sphere.minv,0,-(ty+sqrt(3)/2),tz);
    bottom_sphere.ur[0] = 0;      bottom_sphere.vr[0] = -M_PI/2;
    bottom_sphere.ur[1] = 2*M_PI; bottom_sphere.vr[1] = M_PI/2;
    bottom_sphere.ur[2] = res;  bottom_sphere.vr[2] = res;
    bottom_sphere.eq = &sphere1;
    bottom_sphere.pat = &checker;
    copy_vec(bottom_sphere.color1, RED);
    copy_vec(bottom_sphere.color2, BLACK);
    
    struct Thing3D hyperbola;
    D3d_make_identity(hyperbola.m);
    D3d_make_identity(hyperbola.minv);
    D3d_scale(hyperbola.m,hyperbola.minv,.5,ty,.5);
    D3d_translate(hyperbola.m,hyperbola.minv,0,0,tz);
    hyperbola.ur[0] = 0;      hyperbola.vr[0] = -1;
    hyperbola.ur[1] = 2*M_PI; hyperbola.vr[1] = 1;
    hyperbola.ur[2] = res;  hyperbola.vr[2] = res;
    hyperbola.eq = &hyper;
    hyperbola.pat = &cool_color1; 
    copy_vec(hyperbola.color1, BLUE);

    printf("2\n");

    draw_object(top_sphere);
    draw_object(bottom_sphere);
    draw_object(hyperbola);

    printf("3\n");

    G_wait_key();
    //draw_object(test_sphere); G_wait_key();

    coi[0] = 0; coi[1] = 0; coi[2] = 0;

    char fname[100];
    double t;
    int ilo = 0; int ihi = 100;
    for (i = ilo; i < ihi; i++) {
        t = i*2*M_PI/ihi;
        printf("i:%d\n",i);
        //printf("t:%lf\n",t);
        //clear
        G_rgb(0,0,0);
        G_clear();
        init_zbuf();
        //view matrix:
        coi[0] = 0; coi[1] = 0; coi[2] = 0;
        eye_path1(eye,t);
        up[0] = eye[0]+0; up[1] = eye[1]+1; up[2] = eye[2]+0;
        D3d_make_identity(view);
        D3d_make_identity(vinv);
        //D3d_translate(view,vinv,0,0,4);
        //printf("up1: "); print_vec(up) ; printf("\n");
        D3d_view(view,vinv,eye,coi,up);

        //printf("eye: "); print_vec(eye); printf("\n");
        //printf("up2: "); print_vec(up) ; printf("\n");
        //mat_mult:

        //D3d_print_mat(hyperbola.m); //    

        D3d_mat_mult(top_sphere.m,view,top_sphere.m);
        D3d_mat_mult(bottom_sphere.m,view,bottom_sphere.m);
        D3d_mat_mult(hyperbola.m,view,hyperbola.m);
        D3d_mat_mult_pt(LIGHT,view,LIGHT);
        
        draw_object(top_sphere);
        draw_object(bottom_sphere);
        draw_object(hyperbola);
        //save
        sprintf(fname,"2sphere_hyper/pic%04d.xwd",i);
        G_save_image_to_file(fname);
        //inv_mat:
        D3d_mat_mult(hyperbola.m,vinv,hyperbola.m);
        D3d_mat_mult(top_sphere.m,vinv,top_sphere.m);
        D3d_mat_mult(bottom_sphere.m,vinv,bottom_sphere.m);
        D3d_mat_mult_pt(LIGHT,vinv,LIGHT);


        //D3d_print_mat(hyperbola.m); //

        //G_wait_key();
    } 

    while (1) { 
        char q = G_wait_key();
        if (q == 'q') break;
    }
}
