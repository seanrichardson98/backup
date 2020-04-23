#include <stdio.h>
#include <FPT.h>
#include <D3d_matrix.h>
#include <vecops.h>

double HA = M_PI/4;
double zbuf[800][800];
double AMB = 0.2;
double DIFF_MAX = 0.5;
double SPEC_POW = 30;
double LIGHT[3] = {100,100,-50};
//double LIGHT[3] = {0,0,-50};
double EYE[3] = {0,0,0};
double IN_COLOR = 0.7;

void init_zbuf() {
    int i,j;
    for (i = 0; i < 800; i++) {
        for (j = 0; j < 800; j++) {
            zbuf[i][j] = 0;
        }
    }
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
double light_intensity(double p[3], double n[3]) {
    normalize(n);
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
    //printf("%lf+%lf+%lf\n",AMB,diffuse,specular);
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
void old_sphere_par(double p[3], double u, double v) {
    // 0 < u < 2pi; -pi/2 < v < pi/2;
    p[0] = cos(u)*cos(v);
    p[1] = sin(v);
    p[2] = sin(u)*cos(v);
}
void sphere_par(double p[3], double r, double u, double v) {
    // 0 < u < 2pi; 0 < v < pi; 0 < r < inf;
    p[0] = r*cos(u)*sin(v);
    p[1] = r*sin(u)*sin(v);
    p[2] = r*cos(v);
}
void color1(double RGB[3], double t) {
    RGB[0] = t;
    RGB[1] = 1-t;
    RGB[2] = 0;
}

double get_r(double t, double u, double v) {
    return (cos(t)+1.5);
}
double r1(double t, double u, double v) {
    return sin(t)*(0.5*sqrt(3/2)*cos(u)+1);
}
double r2(double t, double u, double v) {
   return sin(t)*(0.5*sqrt(5/M_PI)*(3*pow((cos(u)),2)-1)+1); 
}
int main() {
    G_init_graphics(800,800);
    init_zbuf();
    G_rgb(0,0,0);
    G_clear();
    G_rgb(1,1,1);
    
    //general:
    double p[3]; 
    double RGB[3];
    double rgb[3];
    double m[4][4];
    double minv[4][4];
    D3d_make_identity(m);
    D3d_make_identity(minv);

    //ulo, uhi, ustep; v... ; t...
    double ulo = 0; double uhi = 2*M_PI;   double ustep = 0.01;
    double vlo = 0; double vhi = M_PI; double vstep = 0.01;
    double tlo = 0; double thi = 2*M_PI; double tstep = 0.01;
    //r given by u(t,u,v);

    //for normal calculation:
    double du = 0.001; double dv = 0.001;
    double a[3], b[3], n[3];

    //set matrix:
    D3d_translate(m,minv,0,0,0);
    D3d_scale(m,minv,1,1,1);
    
    //set color:
    RGB[0] = 1; RGB[1] = 0; RGB[2] = 0;

    //view stuff:
    double eye[3], up[3], coi[3];
    double view[4][4], vinv[4][4];
    char fname[100]; int i;

    double u,v,t;
    for (t = tlo; t <= thi; t += tstep) { //step through time
        i = t/tstep+0.1;

        printf("frame %d\n",i);

        G_rgb(0,0,0);
        G_clear();
        init_zbuf();

        coi[0] = 0; coi[1] = 0; coi[2] = 0;
        //double r = 5;
        //eye[0] = 5*sin(t); eye[1] = 0; eye[2] = 5*cos(t);
        eye[0] = 0; eye[1] = 0; eye[2] = -2;
        up[0] = eye[0]+0; up[1] = eye[1]+1; up[2] = eye[2]+0;

        D3d_make_identity(view);
        D3d_make_identity(vinv);
        D3d_view(view,vinv,eye,coi,up);
       
        D3d_mat_mult(m, view, m);
        D3d_mat_mult_pt(LIGHT,view,LIGHT);

        for (u = ulo; u <= uhi; u += ustep) { //step through theta
            for (v = vlo; v <= vhi; v += vstep) { //step through phi
                //set p = {x,y,z} point
                double r = r2(t,u,v);
                sphere_par(p,r,u,v);
                D3d_mat_mult_pt(p,m,p);
                //normal:
                sphere_par(a,r,u+du,v); D3d_mat_mult_pt(a,m,a);
                sphere_par(b,r,u,v+dv); D3d_mat_mult_pt(b,m,b);
                subtract_vec(a,a,p);
                subtract_vec(b,b,p);
                x_prod(n,a,b);
                normalize(n);
                //light and color:
                double intensity = light_intensity(p,n);
                color1(RGB,(r+1)/2);
                color(rgb,RGB,intensity); 
                G_rgb(rgb[0],rgb[1],rgb[2]);
                draw_point(p);
            }
            //G_wait_key();
        }

        sprintf(fname,"final_project_dir/pic%04d.xwd",i);
        G_save_image_to_file(fname);
        
        D3d_mat_mult(m,vinv,m);
        D3d_mat_mult_pt(LIGHT,vinv,LIGHT);

        G_wait_key();  
    }
}
