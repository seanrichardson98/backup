#include <FPT.h>
#include <D2d_matrix.h>
#include <mytools.h>

int HEIGHT;
int WIDTH;
int BOX = 500;
int X_CENTER = 500;
int Y_CENTER = 500;

int main(int argc, char ** argv) {
  double p[2];
  int numpoints[10];
  double x[10][1000], y[10][1000];
  int numpolys[10];
  int psize[10][5000];
  int con[10][5000][20];
  double red[10][5000],grn[10][5000],blue[10][5000];
  double mat[3][3];
  double minv[3][3];
  
  int i;
  int j;
  FILE *q;

  G_init_graphics(1000,1000);

  //Reading in info:
  int f;
  for (f = 0; f < argc-1; f++) {
    char temp[10] = "xy/";
    //printf("%s",temp);
    strcat(temp,argv[f+1]);
    printf("%s\n",temp);   
    q = fopen(temp,"r");
    if (q == NULL) {
      printf("can't open file\n");
      exit(0);
    }
    fscanf(q,"%d",&numpoints[f]);
    for (i = 0; i < numpoints[f]; i++) {
      fscanf(q,"%lf %lf",&x[f][i],&y[f][i]);
    }
    fscanf(q,"%d",&numpolys[f]);
    for (i = 0; i < numpolys[f]; i++) {
      fscanf(q,"%d",&psize[f][i]);
      for (j = 0; j < psize[f][i]; j++) {
	fscanf(q,"%d",&con[f][i][j]);
      }
    }
    for (i = 0; i < numpolys[f]; i++) {
      fscanf(q,"%lf %lf %lf",&red[f][i],&grn[f][i],&blue[f][i]);
    }
    fclose(q);
  }


  for (f = 0; f < argc-1; f++) {
    D2d_make_identity(mat);
    D2d_make_identity(minv);
    int n = numpoints[f];
    double x_diff = max(x[f],n) - min(x[f],n);
    double y_diff = max(y[f],n) - min(y[f],n);
    double scale = BOX/x_diff;
    D2d_scale(mat,minv,scale,scale);
    double x_mid = (max(x[f],n)*scale+min(x[f],n)*scale)/2.0;
    double y_mid = (max(y[f],n)*scale+min(y[f],n)*scale)/2.0;
    D2d_translate(mat,minv,X_CENTER-x_mid,Y_CENTER-y_mid);
    D2d_mat_mult_points(x[f],y[f],mat,x[f],y[f],n);
    
    //    translate(x[f],y[f],n,500,500);
  }
  
  int prev = -1;
  while (1) {
    G_rgb(1,1,1);
    int m = G_wait_key();
    int rot = 0;
    if (m==prev) {
      rot = 1;
    }
    prev = m;
    G_clear();
    m-=48;
    if (rot) {
      D2d_make_identity(mat);
      D2d_make_identity(minv);
      D2d_translate(mat,minv,-1*X_CENTER,-1*Y_CENTER);
      D2d_rotate(mat,minv,M_PI/90);
            D2d_translate(mat,minv,X_CENTER,Y_CENTER);
      D2d_mat_mult_points(x[m],y[m],mat,x[m],y[m],numpoints[m]);
      //      rotate(x[m],y[m],numpoints[m],M_PI/90);
    }
    for (i = 0; i < numpolys[m]; i++) {
      double xtemp[1000];
      double ytemp[1000];
      for (j = 0; j < psize[m][i]; j++) {
	xtemp[j] = x[m][con[m][i][j]];
	ytemp[j] = y[m][con[m][i][j]];
      }
      G_rgb(red[m][i],grn[m][i],blue[m][i]);
      G_fill_polygon(xtemp,ytemp,psize[m][i]);
    }
  }
}
