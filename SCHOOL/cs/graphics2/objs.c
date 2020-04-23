#include <stdio.h>
#include <math.h>

#include <vecops.h>

//QUADRATIC:
double small_pos_quad(double a, double b, double c) {
//returns the smallest positive solution to quadratic
    double sol[2],t;
    double root = pow(b,2)-4*a*c;
    if (root < 0) return -1;
    sol[0] = (-b + sqrt(root))/(2*a);
    sol[1] = (-b - sqrt(root))/(2*a);
    if (sol[0] > 0 && sol[1] > 0)
        {t = sol[sol[1] < sol[0]];}
    else if (sol[0]*sol[1] < 0) 
        {t = sol[sol[0] < sol[1]];}
    else {return -1;}
    return t;
}

//CIRCLE:
void circle_par(double p[3], double t) {
    //par: (x,y) = (cos(t), sin(t))
    p[0] = cos(t);
    p[1] = sin(t);
    p[2] = 0;
}
int circle_int(double solOb[3], double p1[3], double p2[3]) {
    //imp: x^2+y^2 = 1
    //returns OBJECT space sol w/ OBJ space p1,p2.
    double t;
    double a,b,c,dx,x0,dy,y0;
    dx = p2[0]-p1[0]; dy = p2[1]-p1[1];
    x0 = p1[0]; y0 = p1[1];
    //quadractic
    a = (dx*dx+dy*dy);
    b = (dx*x0+dy*y0)*2;
    c = (x0*x0+y0*y0)-1;
    //solve:
    t = small_pos_quad(a,b,c);
    if (t == -1) return 0;
    solOb[0] = dx*t+x0;
    solOb[1] = dy*t+y0;
    solOb[2] = 0;
    //printf("here\n");
    return 1;
}
int circle_norm(double norm[3], double p[3]) {
    norm[0] = 2*p[0]; norm[1] = 2*p[1]; norm[2] = 0;
}
//PLANE:
void plane_par(double p[3], double u, double v) {
    p[0] = u; p[1] = v; p[2] = 0;
}
int plane_int(double solOb[3], double p1[3], double p2[3]) {
    double t,dx,x0,dy,y0,dz,z0;
    dx = p2[0]-p1[0]; x0 = p1[0];
    dy = p2[1]-p1[1]; y0 = p1[1];
    dz = p2[2]-p1[2]; z0 = p1[2];
    t = -z0/dz;
    solOb[0] = dx*t+x0;
    solOb[1] = dy*t+y0;
    solOb[2] = dz*t+z0;
    if (fabs(solOb[0]) > 1 || fabs(solOb[1]) > 1)
        return 0;
    return 1;
}
int plane_norm(double norm[3], double p[3]) {
    norm[0] = 0; norm[1] = 0; norm[2] = -1;
}
//SPHERE:
void sphere_par1(double p[3], double u, double v) {
    // 0 < u < 2pi; -pi/2 < v < pi/2;
    p[0] = cos(u)*cos(v);
    p[1] = sin(v);
    p[2] = sin(u)*cos(v);
}
void sphere_par2(double p[3], double u, double v) {
    // 0 < u < 2pi; -pi/2 < v < pi/2;
    p[0] = sqrt(1-pow(v,2))*sin(u);
    p[1] = v;
    p[2] = sqrt(1-pow(v,2))*cos(u);
}
int sphere_int(double solOb[3], double p1[3], double p2[3]) {
    //imp: x^2+y^2 = 1
    //returns OBJECT space sol w/ OBJ space p1,p2.
    double t;
    double a,b,c,dx,x0,dy,y0,dz,z0;
    dx = p2[0]-p1[0]; x0 = p1[0];
    dy = p2[1]-p1[1]; y0 = p1[1];
    dz = p2[2]-p1[2]; z0 = p1[2];
    //quadractic
    a = (dx*dx+dy*dy+dz*dz);
    b = (dx*x0+dy*y0+dz*z0)*2;
    c = (x0*x0+y0*y0+z0*z0)-1;
    //solve:
    t = small_pos_quad(a,b,c);
    if (t == -1) return 0;
    solOb[0] = dx*t+x0;
    solOb[1] = dy*t+y0;
    solOb[2] = dz*t+z0;
    //printf("here\n");
    return 1;
}
int sphere_norm(double norm[3], double p[3]) {
    norm[0] = 2*p[0]; norm[1] = 2*p[1]; norm[2] = 2*p[2];
}
//HYPERBOLA:
void hyper_par(double p[3], double u, double v) {
    double r = sqrt(1+pow(v,2));
    p[0] = r*sin(u);
    p[1] = v;
    p[2] = r*cos(u);

}
int hyper_int(double solOb[3], double p1[2], double p2[3]) {
    //returns OBJECT space sol w/ OBJ space p1,p2.
    double hold0[3], hold1[3];
    double a,b,c,dx,x0,dy,y0,dz,z0;
    dx = p2[0]-p1[0]; x0 = p1[0];
    dy = p2[1]-p1[1]; y0 = p1[1];
    dz = p2[2]-p1[2]; z0 = p1[2];
    //quadractic
    a = (+dx*dx-dy*dy+dz*dz);
    b = (+dx*x0-dy*y0+dz*z0)*2;
    c = (+x0*x0-y0*y0+z0*z0)-1;
    //solve:

    double sol[2],t;
    double root = pow(b,2)-4*a*c;
    if (root < 0) return 0;
    sol[0] = (-b + sqrt(root))/(2*a);
    sol[1] = (-b - sqrt(root))/(2*a);
    //holds are in 
    t = sol[0];
    hold0[0] = dx*t+x0;
    hold0[1] = dy*t+y0;
    hold0[2] = dz*t+z0;
    t = sol[1];
    hold1[0] = dx*t+x0;
    hold1[1] = dy*t+y0;
    hold1[2] = dz*t+z0;

    if (fabs(hold0[1]) >= 1 && fabs(hold1[1]) >= 1)
        return 0;
    if (fabs(hold0[1]) >= 1) sol[0] = 100000;
    if (fabs(hold1[1]) >= 1) sol[1] = 100000;
    if (sol[0] > 0 && sol[1] > 0)
        {t = sol[sol[1] < sol[0]];}
    else if (sol[0]*sol[1] < 0) 
        {t = sol[sol[0] < sol[1]];}
    else {return 0;}
    if (t == -1) return 0;
    solOb[0] = dx*t+x0;
    solOb[1] = dy*t+y0;
    solOb[2] = dz*t+z0;
    //print_vec(solOb); printf("\t");
    if (fabs(solOb[1]) > 1) return 0;
    //printf("here\n");
    return 1;
}
int hyper_norm(double norm[3], double p[3]) {
    //printf("here\n");
    norm[0] = 2*p[0]; norm[1] = -2*p[1]; norm[2] = 2*p[2];
    //norm[0] = 1; norm[1] = 0; norm[2] = 0;
}
