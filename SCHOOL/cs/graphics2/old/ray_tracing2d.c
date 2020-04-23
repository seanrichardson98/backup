#include <stdio.h>
#include <FPT.h>

#include <D3d_matrix.h>
#include <vecops.h>
#include <objs.h>

typedef void (*ParaEquation)();
typedef int  (*Intersection)();
typedef int  (*NormalEquation)();
struct Thing2D {
    double m[4][4];
    double minv[4][4];
    double ur[3]; //{ulo, uhi, ustep}
    ParaEquation par;
    Intersection inter;
    NormalEquation norm;
    double color[3];
};

int inter_primary(double solW[3], double p1W[3], double p2W[3], struct Thing2D *objs, int n) {
    int i;
    int num_low;
    double dist_low = -1;
    double p1Ob[3], p2Ob[3];
    double temp[3], disp[3], temp_dist;
    int has_sol = 0;
    for (i = 0; i < n; i++) {
        print_vec(p1W); print_vec(p2W); printf("\n");
        D3d_mat_mult_pt(p1Ob,objs[i].minv,p1W);
        D3d_mat_mult_pt(p2Ob,objs[i].minv,p2W);
        print_vec(p1Ob); print_vec(p2Ob); printf("\n");
        if (objs[i].inter(temp,p1Ob,p2Ob)) {
            D3d_mat_mult_pt(temp,objs[i].m,temp);
            G_rgb(1,0,0);
            G_circle(temp[0],temp[1],2);
            subtract_vec(disp, p2W, temp);
            temp_dist = mag_vec(disp);
            if (dist_low == -1 || temp_dist < dist_low) {
                copy_vec(solW,temp);
                dist_low = temp_dist;
                num_low = i;
                has_sol = 1;
            }
        }
    }
    if (has_sol)
        return num_low;
    return -1;
}
struct Thing2D make_circle(double res) {
    struct Thing2D C;
    D3d_make_identity(C.m);
    D3d_make_identity(C.minv);
    C.ur[0] = 0; C.ur[1] = 2*M_PI; C.ur[2] = res;
    C.par = &circle_par;
    C.inter = &circle_int;
    C.norm = &circle_norm;
    return C;
}
int main() {
    int tlist[100];
    double plist[100];
    int i,num;
    double t,tlo,thi,tinc;
    double p[3];
    double p1W[3], p2W[3], p1Ob[3], p2Ob[3];
    double solW[3], solOb[3];
    double normW[3], normOb[3];
    double /*m[4][4], minv[4][4],*/ tran[4][4];
    double res = 0.01;

    G_init_graphics(800,800);
    G_rgb(0,0,0); 

    struct Thing2D el1 = make_circle(res);
    num = 0;
    tlist[num] = SX; plist[num] = 20; num++;
    tlist[num] = SY; plist[num] = 40; num++;
    tlist[num] = RZ; plist[num] = 30; num++;
    tlist[num] = TX; plist[num] =300; num++;
    tlist[num] = TY; plist[num] =500; num++;
    D3d_make_movement_sequence_matrix(el1.m,el1.minv, num, tlist, plist);

    struct Thing2D el2 = make_circle(res);
    num = 0;
    tlist[num] = SX; plist[num] = 30; num++;
    tlist[num] = SY; plist[num] = 60; num++;
    tlist[num] = RZ; plist[num] = 90; num++;
    tlist[num] = TX; plist[num] =500; num++;
    tlist[num] = TY; plist[num] =200; num++;
    D3d_make_movement_sequence_matrix(el2.m,el2.minv, num, tlist, plist);

    int num_objects = 2;
    struct Thing2D objs[2] = {el1, el2};
   
    //inter_primary(objs,num_objects); 
//DRAW OBJECTS:
    for (i = 0; i < num_objects; i++) {
        tlo = objs[i].ur[0]; thi = objs[i].ur[1]; tinc = objs[i].ur[2];
        for (t = tlo; t < thi; t+=tinc) {
            objs[i].par(p,t);
            D3d_mat_mult_pt(p,objs[i].m,p);
            G_point(p[0],p[1]);
        }
    }
//MAIN LOOP:
    while (1) {
        G_rgb(0,0,0);
    //GET INPUTS:
        G_wait_click(p1W); p1W[2] = 0;
        G_fill_circle(p1W[0],p1W[1], 3);
    /*for (i = 0; i < 1000000; i++) {    
    t = i/1000000.0*2*M_PI;
    p2[0] = cos(t)+p1[0];
    p2[1] = sin(t)+p1[1];
    p2[2] = 0;*/
        G_wait_click(p2W); p2W[2] = 0;
        G_fill_circle(p2W[0],p2W[1], 3);
        D3d_mat_mult_pt(p1Ob, el1.minv, p1W);
        D3d_mat_mult_pt(p2Ob, el1.minv, p2W);

    //GET INTERSECTIONS:
        //D3d_mat_mult_pt(p1Ob, minv, p1W);
        //D3d_mat_mult_pt(p2Ob, minv, p2W);
        int soln_num = inter_primary(solW,p1W,p2W,objs,num_objects);
        D3d_mat_mult_pt(solOb,objs[soln_num].minv,solW);
        G_rgb(0,0,0);
        G_fill_circle(solW[0],solW[1],3);
        print_vec(solW); 
        printf("\n");
        if (soln_num != -1) {
            G_rgb(1,0,1);
            G_line(p1W[0],p1W[1], solW[0],solW[1]);
            G_rgb(0,1,0);
            G_fill_circle(solW[0],solW[1],3);
         
            D3d_transpose(tran, objs[soln_num].minv);
            circle_norm(normOb,solOb);
            D3d_mat_mult_pt(normW,tran,normOb);
            normalize(normW); scale_vec(normW,normW,30);
            G_line(solW[0], solW[1], solW[0]+normW[0], solW[1]+normW[1]);
        }
        else {printf("no int\n");}
    } 
    while (1) {if (G_wait_key() == 'q') break;}
}
