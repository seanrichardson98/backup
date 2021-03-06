#include <stdio.h>
#include <FPT.h>

#include <D3d_matrix.h>
#include <vecops.h>
#include <patterns.h>
#include <objs.h>

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
//PATTERNS:
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

//LIGHT MODEL:
double light_intensity(double p[3], double n[3]) {
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
    double r = 5;
    eye[0] = r*sin(t);
    eye[1] = 0;
    eye[2] = r*cos(t);
}
void eye_path2(double eye[3], double t) {
    double r = 7;
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
    S.eq = &sphere_par1;  
    S.pat = &plain;
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
    double res = 0.05;
    //init
    G_init_graphics(800,800);
    G_rgb(0,0,0);
    G_clear();
    init_zbuf();
    
    double a = 5;
    double hl = (a-sqrt(3))/2;
    double ty = -2;

    double p1[3] = {0,ty,-a/2+tz};
    double p2[3] = {0,sqrt(3)/2*a+ty,tz};
    double p3[3] = {a/2,ty,(sqrt(3)-1)/2*a+tz};
    double p4[3] = {-a/2,ty,(sqrt(3)-1)/2*a+tz};

    struct Thing3D sphere1 = make_sphere(res);
    D3d_translate(sphere1.m, sphere1.minv, p1[0], p1[1], p1[2]);

    struct Thing3D sphere2 = make_sphere(res);
    D3d_translate(sphere2.m, sphere2.minv, p2[0], p2[1], p2[2]);

    struct Thing3D sphere3 = make_sphere(res);
    D3d_translate(sphere3.m, sphere3.minv, p3[0], p3[1], p3[2]);

    struct Thing3D sphere4 = make_sphere(res);
    D3d_translate(sphere4.m, sphere4.minv, p4[0],p4[1],p4[2]);

    struct Thing3D hyper1 = make_hyper(res);
    D3d_scale(hyper1.m, hyper1.minv, 0.5, hl, 0.5);
    D3d_rotate_x(hyper1.m, hyper1.minv, +M_PI/6);
    D3d_translate(hyper1.m, hyper1.minv, (p1[0]+p2[0])/2, (p1[1]+p2[1])/2, (p1[2]+p2[2])/2);
        
    struct Thing3D hyper2 = make_hyper(res);
    D3d_scale(hyper2.m, hyper2.minv, 0.5, hl, 0.5);
    D3d_rotate_x(hyper2.m, hyper2.minv, +M_PI/2);
    D3d_rotate_y(hyper2.m, hyper2.minv, +M_PI/6);
    D3d_translate(hyper2.m,hyper2.minv, (p1[0]+p3[0])/2, (p1[1]+p3[1])/2, (p1[2]+p3[2])/2);
 
    struct Thing3D hyper3 = make_hyper(res);
    D3d_scale(hyper3.m, hyper3.minv, 0.5, hl, 0.5);
    D3d_rotate_x(hyper3.m, hyper3.minv, +M_PI/2);
    D3d_rotate_y(hyper3.m, hyper3.minv, -M_PI/6);
    D3d_translate(hyper3.m,hyper3.minv, (p1[0]+p4[0])/2, (p1[1]+p4[1])/2, (p1[2]+p4[2])/2);
 
    struct Thing3D hyper4 = make_hyper(res);
    D3d_scale(hyper4.m, hyper4.minv, 0.5, hl, 0.5);
    D3d_rotate_x(hyper4.m, hyper4.minv, +M_PI/2);
    D3d_rotate_y(hyper4.m, hyper4.minv, +M_PI/2);
    D3d_translate(hyper4.m,hyper4.minv, (p3[0]+p4[0])/2, (p3[1]+p4[1])/2, (p3[2]+p4[2])/2);
 
    struct Thing3D hyper5 = make_hyper(res);
    D3d_scale(hyper5.m, hyper5.minv, 0.5, hl, 0.5);
    D3d_rotate_z(hyper5.m, hyper5.minv, -M_PI/6);
    D3d_rotate_y(hyper5.m, hyper5.minv, +M_PI/6);
    D3d_translate(hyper5.m,hyper5.minv, (p2[0]+p4[0])/2, (p2[1]+p4[1])/2, (p2[2]+p4[2])/2);
 
    struct Thing3D hyper6 = make_hyper(res);
    D3d_scale(hyper6.m, hyper6.minv, 0.5, hl, 0.5);
    D3d_rotate_z(hyper6.m, hyper6.minv, +M_PI/6);
    D3d_rotate_y(hyper6.m, hyper6.minv, -M_PI/6);
    D3d_translate(hyper6.m,hyper6.minv, (p3[0]+p2[0])/2, (p3[1]+p2[1])/2, (p3[2]+p2[2])/2);

    struct Thing3D hypers[6] = {hyper1, hyper2, hyper3, hyper4, hyper5, hyper6};
    for (i = 0; i < 6; i++) {
        hypers[i].pat = &u_stripes;
        copy_vec(hypers[i].color1,BLUE); copy_vec(hypers[i].color2,WHITE);
    }
    hyper1.pat = &u_stripes;
    copy_vec(hyper1.color1,BLUE); copy_vec(hyper1.color2,WHITE);
     
    struct Thing3D collection[10] = {sphere1, sphere2, sphere3, sphere4, hyper1, hyper2, hyper3, hyper4, hyper5, hyper6};
  
    for (i = 0; i < 4; i++) {
        collection[i].pat = &checker;
        copy_vec(collection[i].color1,RED);
        copy_vec(collection[i].color2,BLACK);
    }   
    for (i = 4; i < 10; i++) {
        collection[i].pat = &all;
    }

    coi[0] = 0; coi[1] = 0; coi[2] = 0;

    char fname[100];
    double t;
    int check = 0;
    int ilo = 0; int ihi = 100;
    for (i = ilo; i < ihi; i++) {
        t = i*2*M_PI/ihi;
        printf("i:%d\n",i);
        G_rgb(0,0,0);
        G_clear();
        init_zbuf();
       
        coi_path2(coi,t); 
        eye_path2(eye,t);
        up[0] = eye[0]+0; up[1] = eye[1]+1; up[2] = eye[2]+0;
        D3d_make_identity(view);
        D3d_make_identity(vinv);
        D3d_view(view,vinv,eye,coi,up);
       
        mat_mult_all(view, collection, 10);
        D3d_mat_mult_pt(LIGHT,view,LIGHT);

        draw_all(collection,10);
        if (!check) {G_wait_key(); check = 1;}       
    
        sprintf(fname,"tetrahedron/pic%04d.xwd",i);
        G_save_image_to_file(fname);
        
        mat_mult_all(vinv, collection, 10);
        D3d_mat_mult_pt(LIGHT,vinv,LIGHT);

        G_wait_key();  
    }
    printf("Done\n"); 
    while (1) { 
        char q = G_wait_key();
        if (q == 'q') break;
    }
}
