#include <FPT.h>
#include <D3d_matrix.h>

int main(int argc, char ** argv) {
  int i,j;
  double x[1000], y[1000], z[1000];
  int con[100][10];
  int numpolys = 10;
  double rad = 100;
  double depth = 100;
  double P[3] = {rad,0,0};
  double m[4][4], minv[4][4];
  double t[4][4], tinv[4][4];
  double r[4][4], rinv[4][4];
  double s[4][4], sinv[4][4];
  D3d_make_identity(m);
  D3d_make_identity(t);
  D3d_make_identity(r);
  D3d_make_identity(tinv);
  D3d_rotate_z(m,minv,2*M_PI/((double)numpolys));
  D3d_translate(s,sinv,0,0,-100);
  //D3d_rotate_y(r,rinv,M_PI);
  D3d_print_mat(r);
  int ind = 0;
  for (i = 0; i < numpolys; i++) {
    double f = (double)i/((double)numpolys);
    D3d_make_identity(r);
    D3d_make_identity(rinv);
    D3d_rotate_x(r,rinv,2*M_PI/((double)numpolys));
    D3d_make_identity(t);
    D3d_make_identity(tinv);
    D3d_translate(t,tinv,-rad*cos(f*2*M_PI),-rad*sin(f*2*M_PI),-50);
    
    D3d_mat_mult_pt(P,t,P);
    D3d_mat_mult_pt(P,r,P);
    x[ind] = P[0]; y[ind] = P[1]; z[ind] = P[2]; ind++;
    D3d_mat_mult_pt(P,rinv,P);
    D3d_mat_mult_pt(P,s,P);
    D3d_mat_mult_pt(P,r,P);
    x[ind] = P[0]; y[ind] = P[1]; z[ind] = P[2]+depth; ind++;
    D3d_mat_mult_pt(P,rinv,P);
    D3d_mat_mult_pt(P,sinv,P);
    D3d_mat_mult_pt(P,tinv,P);
    printf("[%lf,%lf,%lf]\n",P[0],P[1],P[2]);
    
    D3d_mat_mult_pt(P,m,P);

    D3d_mat_mult_pt(P,t,P);
    D3d_mat_mult_pt(P,r,P);
    x[ind] = P[0]; y[ind] = P[1]; z[ind] = P[2]; ind++;
    D3d_mat_mult_pt(P,rinv,P);
    D3d_mat_mult_pt(P,s,P);
    D3d_mat_mult_pt(P,r,P);
    x[ind] = P[0]; y[ind] = P[1]; z[ind] = P[2]+depth; ind++;
    D3d_mat_mult_pt(P,rinv,P);
    D3d_mat_mult_pt(P,sinv,P);
    D3d_mat_mult_pt(P,tinv,P);
    printf("[%lf,%lf,%lf]\n",P[0],P[1],P[2]);
  }
  for (i = 0; i < numpolys; i++) {
    int n = numpolys*4;
    ind+=4;
    con[i][0] = ind%n;
    con[i][1] = (ind+1)%n;
    con[i][2] = (ind+2)%n;
    con[i][3] = (ind+3)%n;
  }
  FILE *f;
  f = fopen ("test.xyz","w");
  fprintf(f,"%d\n",numpolys*4);
  for (i = 0; i < numpolys*4; i++) {
    fprintf(f,"\t%lf\t%lf\t%lf\n",x[i],y[i],z[i]);
  }
  fprintf(f,"%d\n",numpolys);
  for (i = 0; i < numpolys; i++) {
    fprintf(f,"4\t");
    for (j = 0; j < 4; j++) {
      fprintf(f,"%d ",con[i][j]);
    }
    fprintf(f,"\n");
  }
}
