#include <stdio.h>
#include <FPT.h>

#include <D3d_matrix.h>
#include <vecops.h>

double HEIGHT = 800;
double WIDTH = 800;

double HA = M_PI/4;
double zbuf[800][800];
double AMB = 0.2;
double DIFF_MAX = 0.5;
double SPEC_POW = 30;
double LIGHT[3] = {100,100,-50};
//double LIGHT[3] = {0,0,-50};
double EYE[3] = {0,0,0};
double IN_COLOR = 0.7;

double TRIANGLES[1000][3][3];

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
int main() {
    double a[3],b[3],c[3];
    a[0] = 1; a[1] = 0; a[2] = 0+2;
    b[0] = 0; b[1] = 1; b[2] = 0+2;
    c[0] = 0; c[1] = 0; c[2] = 1+2;
    
    double deltaC[3],deltaB[3];
    subtract_vec(deltaC,c,a);
    subtract_vec(deltaB,b,a);

    double n[3];
    
    x_prod(n,deltaC,deltaB);
    print_vec(n); printf("\n");
   
    //ray_tracing:
    int xp,yp;
    for (xp = 0; xp < 800; xp++) {
        for (yp = 0; yp < 800; yp++) {
            //Calculate ray vector:
            double p2W[3];
            double H = tan(HA);
            p2W[0] = (xp-WIDTH/2)*(2*H/WIDTH);
            p2W[1] = (yp-HEIGHT/2)*(2*H/HEIGHT);
            p2W[2] = 1;
            //calculate intersection
                // ray: t*p2W + eye = <x,y,z>
                // triangle: n_vec (dot) (<x,y,z> - base) = 0
            double t = d_prod(a,n)/d_prod(p2W,n);
            double point[3];
            scale_vec(point,p2W,t);
            double inten = light_intensity(point,n);
            G_rgb(inten,inten,inten);
            draw_point(point); 
        }       
    }
}
