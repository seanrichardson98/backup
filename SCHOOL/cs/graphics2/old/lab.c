#include<FPT.h>
#include<D3d_matrix.h>

int draw_circle() {
    G_rgb(1,0,0);  
    double t,x,y;
    for (t = 0.25*M_PI; t < 1.5*M_PI; t += 0.01) {
        x = 50*cos(t)+300;
        y = 100*sin(t)+500;
        G_point(x,y);
    }
    return 1;
}
int draw_sum4() {
    G_rgb(0,1,0);
    double t,x,y;
    double hi = 1.75*M_PI;
    double lo = 0.5*M_PI;
    for (t = 0.0; t < M_PI/2; t += 0.01) {
        x = 30*sqrt(fabs(cos(t)));
        y = 60*sqrt(fabs(sin(t)));
        if (t >= lo) G_point(x+300,y+300);
        if (-t+2.0*M_PI < hi) G_point(x+300,-y+300);
        G_point(-x+300,y+300);       
        G_point(-x+300,-y+300);
    }
    return 1;
}
int draw_square() {
    G_rgb(1,1,0);
    double p[3],q[3];
    double t,x,y;
    double m[4][4], minv[4][4];
    D3d_make_identity(m); D3d_make_identity(m);
    int tlist[10]; double plist[10]; int num = 0;
    tlist[num] = SX; plist[num] = 150; num++; 
    tlist[num] = SY; plist[num] = 100; num++;
    tlist[num] = TX; plist[num] = 500; num++;
    tlist[num] = TY; plist[num] = 500; num++;
    D3d_make_movement_sequence_matrix(m,minv, num, tlist, plist) ;
    for (t = 0; t < M_PI/2; t += 0.01) {
        
    }
}
void circle(double p[3], double t) {
    p[0] = cos(t); p[1] = sin(t); p[2] = 0;
}
void sum4(double p[3], double t) {
    p[0] = sqrt(abs(cos(t)));
    p[1] = sqrt(abs(sin(t)));
    p[2] = 0;
}
void square(double p[3], double t) {
    p[0] = pow(cos(t),2);
    p[1] = pow(sin(t),2);
    p[2] = 0;
}
void astroid(double p[3], double t) {
    p[0] = pow(cos(t),4);
    p[1] = pow(sin(t),4);
    p[2] = 0;
}
void hyperbola(double p[3], double t) {
    p[0] = cosh(u); p[1] = sinh(u); p[2] = 0;
}
void parabola(double p[3], double t) {
    p[0] = u; p[1] = pow(u,2); p[2] = 0;
}
void lemon(double p[3], double t) {
    p[0] = pow(cos(t),3);
    p[1] = sin(t); p[2] = 0;
}
int draw(double m[3][3], char eq[], double hi, double lo) {
    double p[3], q[3];
    double t,x,y;
    for (t = 0; t < M_PI/2; t += 0.01) {
        if (eq == "circle") circle(p,t);
        if (eq == "sum4") sum4(p,t);
        if (eq == "square") square(p,t);
        if (eq == "astroid") astroid(p,t);
        if (eq == "hyperbola") hyperbola(p,t);
        if (eq == "parabola") parabola(p,t);
        if (eq == "lemon") lemon(p,t);
         
    }
}
int draw_astroid() {
    G_rgb(0,0,1);
    double p[3],q[3];
    double t,x,y;
    double m[4][4], minv[4][4];
    D3d_make_identity(m);
    D3d_make_identity(minv);
    int tlist[10]; double plist[10]; int num = 0;
    D3d_scale(m,minv,80,40,1);
    D3d_rotate_z(m,minv,45*M_PI/180);
    D3d_translate(m,minv,500,300,0);
    for (t = 0; t < M_PI/2; t += 0.01) {
        p[0] = pow(cos(t),4);
        p[1] = pow(sin(t),4);
        p[2] = 0;

        D3d_mat_mult_pt(q,m,p);
        G_point(q[0],q[1]);

        p[0] = -p[0]; 
        D3d_mat_mult_pt(q,m,p);
        G_point(q[0],q[1]);

        p[1] = -p[1];
        D3d_mat_mult_pt(q,m,p);
        G_point(q[0],q[1]);       

        p[0] = -p[0];
        D3d_mat_mult_pt(q,m,p);
        G_point(q[0],q[1]); 
    }
}
int main() {
    G_init_graphics(800,800);
    G_rgb(0,0,0);
    G_clear(); 
    draw_circle();
    draw_sum4();
    draw_astroid();
    G_wait_key();
}

