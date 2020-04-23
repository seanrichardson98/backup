#include <FPT.h>
#include <D3d_matrix.h>
#include <mytools.h>

int HEIGHT = 1000;
int WIDTH = 1000;
int n = 100;

int main() {
  double p[2];
  double xs[500], ys[500], zs[500];
  double x[5000], y[5000], z[5000];
  double mat[4][4];
  double minv[4][4];
  int con[5000][50];
  int i,j;

  printf("Rotations:\t");
  scanf("%d",&n);
  
  //init
  G_init_graphics(HEIGHT,WIDTH);
  G_rgb(0,0,0);
  G_line(500,0,500,HEIGHT);
  G_fill_rectangle(0,0,20,20);

  //read points
  i = 0;
  G_wait_click(p);
  while (!(p[0]>0 && p[0]<20 && p[1]>0 && p[1]<20)) {
    xs[i] = p[0];
    ys[i] = p[1];
    zs[i] = 0;
    if (i > 0) {
      G_line(xs[i-1],ys[i-1],xs[i],ys[i]);
    }
    G_wait_click(p);
    i++;
  }
  int m = i;
  //translate
  D3d_make_identity(mat);
  D3d_make_identity(minv);
  D3d_translate(mat,minv,-WIDTH/2,0,0);
  D3d_translate(mat,minv,0,-(max(ys,m)+min(ys,m))/2,0);
  D3d_mat_mult_points(xs,ys,zs,mat,xs,ys,zs,m);
  
  int ind = 0;
  for (i = 0; i < n; i++) {
    D3d_make_identity(mat);
    D3d_make_identity(minv);
    D3d_rotate_y(mat,minv,2*M_PI/n);
    D3d_mat_mult_points(xs,ys,zs,mat,xs,ys,zs,m);
    for (j = 0; j < m; j++) {
      x[ind] = xs[j];
      y[ind] = ys[j];
      z[ind] = zs[j];
      ind++;
    }
  }
  //con
  ind = 0;
  for (i = 0; i < n; i++) {
    for (j = 0; j < m-1; j++) {
      con[ind][0] = i*m+j;
      con[ind][1] = i*m+j+1;
      con[ind][2] = (i+1)%n*m+j+1;
      con[ind][3] = (i+1)%n*m+j;
      ind++;
    }
  }
  //write
  FILE *f;
  f = fopen ("xyz/test.xyz","w");
  fprintf(f,"%d\n",m*n);
  for (i = 0; i < n*m; i++) {
    fprintf(f,"\t%lf\t%lf\t%lf\n",x[i],y[i],z[i]);
  }
  fprintf(f,"%d\n",n*(m-1));
  for (i = 0; i < n*(m-1); i++) {
    fprintf(f,"4\t");
    for (j = 0; j < 4; j++) {
      fprintf(f,"%d ",con[i][j]);
    }
    fprintf(f,"\n");
  }
}
