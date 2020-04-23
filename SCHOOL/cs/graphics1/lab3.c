#include <FPT.h>
#include <D2d_matrix.h>
#include <mytools.h>

int HEIGHT;
int WIDTH;
int BOX = 500;
int X_CENTER = 500;
int Y_CENTER = 500;

int is_inside(double x, double y, double *X, double *Y, int n) {
  double cent[2];  double p[2],p0[2],p1[2];
  int i;
  p[0] = x; p[1] = y;
  center(cent,X,Y,n);
  //G_fill_polygon(X,Y,n);
  int inside = 1;
  printf("-----\n");
  for (i = 0; i < n; i++) {
    p0[0] = X[i]; p0[1] = Y[i];
    p1[0] = X[(i+1)%n]; p1[1] = Y[(i+1)%n];
    printf("%d\n",line_side(p,p0,p1)*line_side(cent,p0,p1));
    if (line_side(p,p0,p1) != line_side(cent,p0,p1)) {
      inside = 0;
      break;
    }
  }
  return inside;
}
int main(int argc, char ** argv) {
  double p[2], pi[2], pf[2];
  int numpoints[10];
  double x[10][1000], y[10][1000];
  int numpolys[10];
  int psize[10][5000];
  int con[10][5000][20];
  double red[10][5000],grn[10][5000],blue[10][5000];
  double mat[3][3];
  double minv[3][3];
  int i,j,k,s;
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

  //Draw window:
  double window_x[100];
  double window_y[100];
  int wind_n = 0;
  double wind_cent[2];

  G_fill_rectangle(0,0,20,20);
  i = 0;
  while (1) {
    G_wait_click(p);
    if (p[0]>0 && p[0]<20 && p[1]>0 && p[1]<20) {
      G_line(window_x[i-1],window_y[i-1],window_x[0],window_y[0]);
      break;
    }
    window_x[i] = p[0];
    window_y[i] = p[1];
    if (i > 0) {
      G_line(window_x[i-1],window_y[i-1],window_x[i],window_y[i]);
    }
    i++;
  }
  G_rgb(1,1,1);
  G_fill_rectangle(0,0,20,20);
  G_rgb(0,0,0);
  wind_n = i;

  center(wind_cent, window_x, window_y, wind_n);
  G_fill_circle(wind_cent[0], wind_cent[1], 5);
 
  G_polygon(window_x,window_y,wind_n);
  int prev = -1;
  while (1) {
    printf("1\n");
    G_rgb(1,1,1);
    int m = G_wait_key();
    int rot = 0;
    if (m==prev) {
      rot = 1;
    }
    prev = m;
    G_clear();
    G_rgb(0,0,0);
    G_polygon(window_x,window_y,wind_n);
    m-=48;
    if (rot) {
      D2d_make_identity(mat);
      D2d_make_identity(minv);
      D2d_translate(mat,minv,-1*X_CENTER,-1*Y_CENTER);
      D2d_rotate(mat,minv,M_PI/30);
      D2d_translate(mat,minv,X_CENTER,Y_CENTER);
      D2d_mat_mult_points(x[m],y[m],mat,x[m],y[m],numpoints[m]);
    }
    //for each polygon
    for (i = 0; i < numpolys[m]; i++) {
      printf("2\n");
      double xtemp[1000];
      double ytemp[1000];
      double xp[1000];
      double yp[1000];
      int poly_n = psize[m][i];
      int temp_n = poly_n;
      printf("poly_n:%d\n",poly_n);
      for (j = 0; j < poly_n; j++) {
	xtemp[j] = x[m][con[m][i][j]];
	ytemp[j] = y[m][con[m][i][j]];
      }
      for (j = 0; j < wind_n; j++) {
	double pc[2], pl[2];
	copy_matrix(xp,xtemp,temp_n);
	copy_matrix(yp,ytemp,temp_n);
	poly_n = temp_n;
	pi[0] = window_x[j]; pi[1] = window_y[j];
	pf[0] = window_x[(j+1)%wind_n]; pf[1] = window_y[(j+1)%wind_n];
	int goodness;
	int prevgood;
	temp_n = 0;
	//for each point in poly:
	for (k = 0; k < poly_n; k++) {
	  pl[0] = xp[k]; pl[1] = yp[k];
	  pc[0] = xp[(k+1)%poly_n];
	  pc[1] = yp[(k+1)%poly_n];
	  prevgood = line_side(pl,pi,pf)*line_side(wind_cent,pi,pf);
	  goodness = line_side(pc,pi,pf)*line_side(wind_cent,pi,pf);
	  if (prevgood != 1 && goodness == 1) {
	    intersec(p,pi,pf,pc,pl);
	    xtemp[temp_n] = p[0];
	    ytemp[temp_n] = p[1];
	    xtemp[temp_n+1] = pc[0];
	    ytemp[temp_n+1] = pc[1];
	    temp_n += 2;
	  }
	  else if (prevgood != 1 && goodness != 1) {printf("2!\n");}
	  else if (prevgood == 1 && goodness == 1) {
	    xtemp[temp_n] = pc[0];
	    ytemp[temp_n] = pc[1];
	    temp_n += 1;
	  }
	  else if (prevgood == 1 && goodness != 1) {
	    intersec(p,pi,pf,pc,pl);
	    xtemp[temp_n] = p[0];
	    ytemp[temp_n] = p[1];
	    temp_n += 1;
	  }
	}	
      }
      G_rgb(red[m][i],grn[m][i],blue[m][i]);
      G_fill_polygon(xtemp,ytemp,temp_n);
    }
    G_rgb(0,0,0);
    G_polygon(window_x,window_y,wind_n);
  }
}
