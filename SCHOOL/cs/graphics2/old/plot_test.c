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
void torus(double p[3], double u, double v) {
    double a = 1;
    double b = 2;
    p[0] = (a*cos(u)+b)*cos(v);
    p[1] = (a*cos(u)+b)*sin(v);
    p[2] = a*sin(u);
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
void color_torus(double rgb[3], double u, double v) {
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
void v_stripes(double RGB[3], double u, double v, struct Thing3D D) {
    double udiff = D.ur[1]-D.ur[0];
    int freq = 20;
    int a = (u-D.ur[0])/udiff*freq;
    if (a%2) {copy_vec(RGB, D.color1);} 
    else {copy_vec(RGB, D.color2);}
}
void u_stripes(double RGB[3], double u, double v, struct Thing3D D) {
    double vdiff = D.vr[1]-D.vr[0];
    int freq = 20;
    int b = (v-D.vr[0])/vdiff*freq;
    if (b%2) {copy_vec(RGB, D.color1);} 
    else {copy_vec(RGB, D.color2);}
}
void draw_object(struct Thing3D T) {
    double RGB[3], rgb[3];
    double u,v;
    double p[3], q[3], r[3], n[3];
    double cent[3] = {0,0,0};
    D3d_mat_mult_pt(cent,T.m,cent);
    double du = 0.001; double dv = 0.001;
    for (u = T.ur[0]; u <= T.ur[1]; u += T.ur[2]) {
        for (v = T.vr[0]; v <= T.vr[1]; v += T.vr[2]) {
            //point:
            T.eq(p,u,v); D3d_mat_mult_pt(p,T.m,p);
            //normal:
            T.eq(q,u+du,v); D3d_mat_mult_pt(q,T.m,q);
            T.eq(r,u,v+dv); D3d_mat_mult_pt(r,T.m,r);
            subtract_vec(q,q,p);
            subtract_vec(r,r,p);
            x_prod(n,q,r);
            normalize(n);
            //light and color
            T.pat(RGB,u,v,T);
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
    double r = 2;
    eye[0] = r*sin(t);
    eye[1] = 0;
    eye[2] = r*cos(t);
}
void eye_path2(double eye[3], double t) {
    double r = 6;
    eye[0] = r*sin(t);
    eye[1] = sin(5*t);
    eye[2] = r*cos(t);
}
void coi_path1(double coi[3], double t) {
    coi[0] = 0; coi[1] = 0; coi[2] = 0; 
}
void coi_path2(double coi[3], double t) {
    coi[0] = 0;
    coi[1] = sin(5*t);
    coi[2] = 0; 
}
void draw_all(struct Thing3D *collection, int n) {
    int i;
    for (i = 0; i < 10; i++) {
        draw_object(collection[i]);
    }
}
void mat_mult_all(double view[4][4], struct Thing3D *collection, int n) {
    int i;
    for (i = 0; i < n; i++) {
        D3d_mat_mult(collection[i].m,view,collection[i].m);
    }
}
struct Thing3D make_hyper(double res) {
    struct Thing3D H;
    D3d_make_identity(H.m);
    D3d_make_identity(H.minv);
    H.ur[0] = 0;      H.vr[0] = -1;
    H.ur[1] = 2*M_PI; H.vr[1] = 1;
    H.ur[2] = res;    H.vr[2] = res;
    H.eq = &hyper;    H.pat = &plain;
    H.color1[0] = 1; H.color1[1] = 1; H.color1[2] = 1; 
    H.color2[0] = 1; H.color2[1] = 1; H.color2[2] = 1;
    return H;
}
struct Thing3D make_sphere(double res) {
    struct Thing3D S;
    D3d_make_identity(S.m);
    D3d_make_identity(S.minv);
    S.ur[0] = 0;      S.vr[0] = -M_PI/2;
    S.ur[1] = 2*M_PI; S.vr[1] = M_PI/2;
    S.ur[2] = res;    S.vr[2] = res;
    S.eq = &sphere1;  S.pat = &plain;
    S.color1[0] = 1; S.color1[1] = 1; S.color1[2] = 1; 
    S.color2[0] = 1; S.color2[1] = 1; S.color2[2] = 1; 
    return S; 
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
    double res = 0.001;
    //init
    G_init_graphics(800,800);
    G_rgb(0,0,0);
    G_clear();
    init_zbuf();
    
    double a = 5;
    double hl = (a-sqrt(3))/2;
    double ty = -2;

    struct Thing3D obj;
    D3d_make_identity(obj.m);
    D3d_make_identity(obj.minv);
    obj.ur[0] = 0;      obj.vr[0] = -M_PI/2;
    obj.ur[1] = 2*M_PI; obj.vr[1] = M_PI/2;
    obj.ur[2] = res;    obj.vr[2] = res;
    obj.eq = &sphere2;    obj.pat = &color_torus;
    obj.color1[0] = 1; obj.color1[1] = 1; obj.color1[2] = 1; 
    obj.color2[0] = 1; obj.color2[1] = 1; obj.color2[2] = 1;

    //eye[0] = 5; eye[1] = 0; eye[2] = 5;
    //up[0] = eye[0]+0; up[1] = eye[1]+1; up[2] = eye[2]+0;
    //coi[0] = 0; coi[1] = 0; coi[2] = 0;
    D3d_make_identity(view);
    D3d_make_identity(vinv);
    //D3d_view(view,vinv,eye,coi,up);
    //D3d_mat_mult(obj.m,view,obj.m);
    //D3d_mat_mult_pt(LIGHT,view,LIGHT);
    draw_object(obj);
     
    char fname[100];
    double t;
    int ilo = 0; int ihi = 100;
    for (i = ilo; i < ihi; i++) {
        t = i*2*M_PI/ihi;
        printf("i:%d\n",i);
        G_rgb(0,0,0);
        G_clear();
        init_zbuf();
       
        coi_path1(coi,t); 
        eye_path1(eye,t);
        up[0] = eye[0]+0; up[1] = eye[1]+1; up[2] = eye[2]+0;
        D3d_make_identity(view);
        D3d_make_identity(vinv);
        D3d_view(view,vinv,eye,coi,up);
       
        D3d_mat_mult(obj.m, view, obj.m);
        D3d_mat_mult_pt(LIGHT,view,LIGHT);

        draw_object(obj);
        if (i == 0) G_wait_key();        
         
        sprintf(fname,"plot_test/pic%04d.xwd",i);
        G_save_image_to_file(fname);
        
        D3d_mat_mult(obj.m, vinv, obj.m);
        D3d_mat_mult_pt(LIGHT,vinv,LIGHT);
    }

    while (1) { 
        char q = G_wait_key();
        if (q == 'q') break;
    }
}
