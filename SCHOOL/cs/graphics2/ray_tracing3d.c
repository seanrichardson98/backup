#include <stdio.h>
#include <FPT.h>

#include <D3d_matrix.h>
#include <vecops.h>
#include <objs.h>

//#define WIDTH 800;
//#define HEIGHT 800;

double HA = M_PI/4;
double AMB = 0.2;
double DIFF_MAX = 0.5;
double SPEC_POW = 30;
double LIGHT[3] = /*{100,200,-50};*/ {0,0,-50};
double EYE[3] = {0,0,0};
double IN_COLOR = 0.7;

typedef void (*ParaEquation)();
typedef int  (*Intersection)();
typedef int  (*NormalEquation)();
struct Thing3D {
    double m[4][4];
    double minv[4][4];
    double ur[3]; //{ulo, uhi, ustep}
    double vr[3]; //{vlo, vhi, vstep}
    ParaEquation par;
    Intersection inter;
    NormalEquation norm;
    double RGB[3];
    double color[3];
    double reflectivity;
};
void draw_point(double p[3]) {
    int x = (400/tan(HA))*(p[0]/p[2])+400;
    int y = (400/tan(HA))*(p[1]/p[2])+400;
    G_point(x,y);
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
//STUFF:
int inter_primary(double solW[3], double p1W[3], double p2W[3], struct Thing3D *objs, int n) {
    int i;
    int num_low;
    double dist_low = 10000;
    double p1Ob[3], p2Ob[3];
    double temp[3], disp[3], temp_dist;
    int has_sol = 0;
    for (i = 0; i < n; i++) {
        //printf("%d\n",i);
        //print_vec(p1W); print_vec(p2W); printf("\n");
        D3d_mat_mult_pt(p1Ob,objs[i].minv,p1W);
        D3d_mat_mult_pt(p2Ob,objs[i].minv,p2W);
        //print_vec(p1Ob); print_vec(p2Ob); printf("\n");
        if (objs[i].inter(temp,p1Ob,p2Ob)) {
            //temp is solOb then solW
            D3d_mat_mult_pt(temp,objs[i].m,temp);
            subtract_vec(disp, p1W, temp);
            temp_dist = mag_vec(disp);
            if (temp_dist < dist_low && temp_dist > 0.01) {
                copy_vec(solW,temp);
                dist_low = temp_dist;
                num_low = i;
                has_sol = 1;
            }
        }
    }
    if (has_sol) return num_low;
    else return -1;
}

void reflection_color (double rgb[3], double solW[3], double p1W[3], double p2W[3], struct Thing3D *objs, int n, int depth) {
    //printf("beginning\n") ;
    int i,num;
    double solOb[3];
    double normW[3], normOb[3];
    double tran[4][4];
    double RGB[3];
    struct Thing3D hit_obj; 
    RGB[0] = 1; RGB[1] = 0; RGB[2] = 0;

    int soln_num = inter_primary(solW,p1W,p2W,objs,n);
    //printf("%d",soln_num);
    if (soln_num != -1) {
        hit_obj = objs[soln_num];
        D3d_mat_mult_pt(solOb,hit_obj.minv,solW);
        hit_obj.norm(normOb,solOb);
        D3d_transpose(tran,hit_obj.minv);
        D3d_mat_mult_pt(normW,tran,normOb);
        normalize(normW);
        //printf("in\n");
        //p[0] = u; p[1] = v; p[2] = solW[2];   
        copy_vec(EYE,p1W);
        double inten = light_intensity(solW,normW);
        EYE[0] = 0; EYE[1] = 0; EYE[2] = 0;
        if (inten < 0 || inten > 1) printf("%lf", inten);
        rgb[0] = inten; rgb[1] = inten; rgb[2] = inten;     //printf("%lf...",inten);
        //printf(" W:"); print_vec(normW);
        //printf(" Ob:"); print_vec(normOb);

        /*printf("-----\n");
        D3d_print_mat(hit_obj.minv);
        printf("\n");
        D3d_print_mat(tran);
        printf("-----\n");*/

        G_rgb(rgb[0], rgb[1], rgb[2]);
        //printf("%lf\t",inten);
        color(rgb,hit_obj.RGB,inten);
        //printf("%lf", inten);

    }
    else {rgb[0] = 0; rgb[1] = 0; rgb[2] = 0;}

    if (soln_num != -1 && hit_obj.reflectivity != 0 && depth < 100) {
        //find reflection and set p2
        double p2W_new[3];
        //printf("%d",soln_num);
        double l[3], r[3];
        subtract_vec(l,p1W,solW); normalize(l);
        normalize(normW);
        scale_vec(r, normW, 2*d_prod(normW,l));
        subtract_vec(r,r,l);
        add_vec(p2W_new,r,solW);
        double p1W_new[3];
        copy_vec(p1W_new, solW);
        //printf("rgb: "); print_vec(rgb); printf("\n"); 
        double old_rgb[3];
        copy_vec(old_rgb,rgb);
        //print_vec(rgb); printf("->");
        reflection_color(rgb,solW,p1W_new,p2W_new,objs,n,depth+1);
        scale_vec(old_rgb,old_rgb,1-hit_obj.reflectivity);
        scale_vec(rgb,rgb,hit_obj.reflectivity);
        add_vec(rgb,rgb,old_rgb);
        //print_vec(rgb);
        
        //p1W[0] = 0; p1W[1] = 0; p1W[2] = 0;
        //printf("here7\n");
    } 

}
/*int reflection_intersection(struct Thing3D *objs, double p1W[3], double p2W[3],) {
      
}*/

struct Thing3D make_sphere(double res) {
    struct Thing3D S;
    D3d_make_identity(S.m);
    D3d_make_identity(S.minv);
    S.ur[0] = 0;      S.vr[0] = -M_PI/2;
    S.ur[1] = 2*M_PI; S.vr[1] = M_PI/2;
    S.ur[2] = res;    S.vr[2] = res;
    S.par = &sphere_par1;
    S.inter = &sphere_int;
    S.norm = &sphere_norm;
    S.reflectivity = 0;
    S.RGB[0] = 1; S.RGB[1] = 0; S.RGB[2] = 0;
    return S;
}
struct Thing3D make_hyper(double res) {
    struct Thing3D H;
    D3d_make_identity(H.m);
    D3d_make_identity(H.minv);
    H.ur[0] = 0;      H.vr[0] = -1;
    H.ur[1] = 2*M_PI; H.vr[1] = 1;
    H.ur[2] = res;    H.vr[2] = res;
    H.par = &hyper_par;
    H.inter = &hyper_int;
    H.norm = &hyper_norm;
    H.reflectivity = 0;
    H.RGB[0] = 1; H.RGB[1] = 0; H.RGB[2] = 0;
    return H;
}
struct Thing3D make_plane(double res) {
    struct Thing3D P;
    D3d_make_identity(P.m);
    D3d_make_identity(P.minv);
    P.ur[0] = -1;     P.vr[0] = -1;
    P.ur[1] =  1;     P.vr[1] = 1;
    P.ur[2] = res;    P.vr[2] = res;
    P.par = &plane_par;
    P.inter = &plane_int;
    P.norm = &plane_norm;
    P.reflectivity = 0;
    P.RGB[0] = 1; P.RGB[1] = 0; P.RGB[2] = 0;
    return P;
}
int main() {
    int tlist[100];
    double plist[100];
    int i,num;
    double t,tlo,thi,tinc;
    double p[3],p1[3],p2[3];
    double p1W[3], p2W[3], p1Ob[3], p2Ob[3];
    double solW[3], solOb[3];
    double normW[3], normOb[3];
    double tran[4][4];
    double rgb[3], RGB[3];
    double res = 0.01;

    double BLUE[3] = {0,0,1};
    double GREEN[3] = {0,1,0};

    G_init_graphics(800,800);
    G_rgb(0,0,0); 
    G_clear();
    G_rgb(1,1,1);
    
    struct Thing3D sphere1 = make_sphere(res);
    num = 0;
    tlist[num] = SY; plist[num] = 2; num++;
    tlist[num] = TZ; plist[num] = 4; num++;
    tlist[num] = TX; plist[num] = -2; num++;
    tlist[num] = TY; plist[num] = -2; num++;
    sphere1.reflectivity = 0;
    copy_vec(sphere1.RGB,GREEN);
    D3d_make_movement_sequence_matrix(sphere1.m,sphere1.minv, num, tlist, plist);
    /*
    struct Thing3D sphere2 = make_sphere(res);
    num = 0;
    tlist[num] = TZ; plist[num] = 4; num++;
    tlist[num] = TX; plist[num] = 2; num++;
    tlist[num] = TY; plist[num] = 2; num++;
    D3d_make_movement_sequence_matrix(sphere2.m,sphere2.minv, num, tlist, plist);

    struct Thing3D sphere4 = make_sphere(res);
    num = 0;
    tlist[num] = TZ; plist[num] = 4; num++;
    tlist[num] = TX; plist[num] = -2; num++;
    tlist[num] = TY; plist[num] = 2; num++;
    D3d_make_movement_sequence_matrix(sphere4.m,sphere4.minv, num, tlist, plist);
    struct Thing3D sphere3 = make_sphere(res);
    num = 0;
    tlist[num] = SY; plist[num] = 2; num++;
    tlist[num] = TZ; plist[num] = 4; num++;
    tlist[num] = TX; plist[num] = 2; num++;
    tlist[num] = TY; plist[num] = -2; num++;
    D3d_make_movement_sequence_matrix(sphere3.m,sphere3.minv, num, tlist, plist);*/
    struct Thing3D hyper1 = make_hyper(res);
    num = 0;
    tlist[num] = RX; plist[num] = 45; num++;
    tlist[num] = TZ; plist[num] = 5; num++;
    tlist[num] = TX; plist[num] = 3; num++;
    tlist[num] = TY; plist[num] = 3; num++;
    D3d_make_movement_sequence_matrix(hyper1.m,hyper1.minv, num, tlist, plist);

    struct Thing3D plane1 = make_plane(res);
    num = 0;
    tlist[num] = SY; plist[num] = 5; num++;
    tlist[num] = SX; plist[num] = 5; num++;
    tlist[num] = RX; plist[num] = 0; num++;
    tlist[num] = TZ; plist[num] = 10; num++;
    tlist[num] = TX; plist[num] = 0; num++;
    tlist[num] = TY; plist[num] = 0; num++;
    plane1.reflectivity = 0.8; 
    copy_vec(plane1.RGB,BLUE);
    D3d_make_movement_sequence_matrix(plane1.m,plane1.minv, num, tlist, plist);

    struct Thing3D plane2 = make_plane(res);
    num = 0;
    tlist[num] = SY; plist[num] = 5; num++;
    tlist[num] = SX; plist[num] = 5; num++;
    tlist[num] = RX; plist[num] = 0; num++;
    tlist[num] = TZ; plist[num] = -10; num++;
    tlist[num] = TX; plist[num] = 0; num++;
    tlist[num] = TY; plist[num] = 0; num++;
    plane2.reflectivity = 0.8;
    copy_vec(plane2.RGB,BLUE);
    D3d_make_movement_sequence_matrix(plane2.m,plane2.minv, num, tlist, plist);
   
    int num_objects = 4;
    struct Thing3D objs[4] = {plane1, sphere1, hyper1, plane2};
 
    RGB[0] = 1; RGB[1] = 0; RGB[2] = 0;
    p1W[0] = 0; p1W[1] = 0; p1W[2] = 0;
    double u,v;
    for (u = 0; u < 800; u++) {
        for (v = 0; v < 800; v++) {
           //printf("%lf,%lf\n",u,v);
           //SET P2
           double H = tan(HA); double zb = 1;
           double xb = (u-800/2)*(H/(800/2));
           double yb = (v-800/2)*(H/(800/2));
           p2W[0] = xb; p2W[1] = yb; p2W[2] = zb;
           //print_vec(p2); printf("\n");
           //CALCULATIONS: 
           reflection_color(rgb,solW,p1W,p2W,objs,num_objects,0);
           G_rgb(rgb[0],rgb[1],rgb[2]);
           //printf("(%lf,%lf): ",u,v);
           //print_vec(rgb); printf("\n");
           G_point(u,v);
        }
    } 
    while (1) {if (G_wait_key() == 'q') break;}
}
