#include <FPT.h>
#include <D3d_matrix.h>
#include <mytools.h>

int back_elim(int f, int i, int dir);

int HEIGHT = 1000;
int WIDTH = 1000;

double HA = M_PI/3;

int numpoints[10];
double x[10][5000], y[10][5000], z[10][5000];
int numpolys[10];
int psize[10][5000];
int con[10][5000][50];
double cent[10][3];
FILE *q;

void draw(int f,int dir) {
  int i,j;
  G_rgb(1,0,0);
  for (i = 0; i < numpolys[f]; i++) {
    double xtemp[1000], ytemp[1000];
    for (j = 0; j < psize[f][i]; j++) {
      double xp = x[f][con[f][i][j]];
      double yp = y[f][con[f][i][j]];
      double zp = z[f][con[f][i][j]];
      xtemp[j] = WIDTH*(xp/zp)/tan(HA) + WIDTH/2;
      ytemp[j] = HEIGHT*(yp/zp)/tan(HA) + HEIGHT/2;
    }
    int be = back_elim(f,i, dir);
    if (be) {
      G_polygon(xtemp,ytemp,psize[f][i]);
    }
  }
}
int back_elim(int f,int i, int dir) {
  double cross[3];
  int p0 = con[f][i][0];
  int p1 = con[f][i][1];
  int p2 = con[f][i][2];
  double u[3] = {x[f][p0]-x[f][p1],y[f][p0]-y[f][p1],z[f][p0]-z[f][p1]};
  double v[3] = {x[f][p2]-x[f][p1],y[f][p2]-y[f][p1],z[f][p2]-z[f][p1]};
  x_prod(cross,u,v);
  double w[3] = {x[f][p1],y[f][p1],z[f][p1]};
  double dot = d_prod(cross,w,3);
  double to_cent[3] = {cent[f][0]-x[f][p1],cent[f][1]-y[f][p1],cent[f][2]-z[f][p1]};
  if (dir < 0)     return (dot < 0);
  else             return (dot > 0);
}
int main(int argc, char ** argv) {
  int f,i,j,n;
  
  //Reading:
  for (f = 0; f < argc-1; f++) {
    char temp[20] = "xyz/";
    strcat(temp,argv[f+1]);
    q = fopen(temp,"r");
    if (q == NULL) {
      printf("can't open file %d\n",f+1);
      exit(0);
    }
    fscanf(q,"%d", &numpoints[f]);
    for (i = 0; i < numpoints[f]; i++) {
      fscanf(q,"%lf %lf %lf",&x[f][i],&y[f][i],&z[f][i]);
    }
    fscanf(q,"%d",&numpolys[f]);
    for (i = 0; i < numpolys[f]; i++) {
      fscanf(q,"%d",&psize[f][i]);
      for (j = 0; j < psize[f][i]; j++) {
	fscanf(q,"%d",&con[f][i][j]);
      }
    }
  }
  //Drawing:
  G_init_graphics(HEIGHT,WIDTH);
  G_rgb(0,0,0);
  G_clear();
  G_rgb(1,0,0);
  //Set centers:
  for (f = 0; f < 10; f++) {
    n = numpoints[f];
    cent[f][0] = (max(x[f],n) + min(x[f],n))/2;
    cent[f][1] = (max(y[f],n) + min(y[f],n))/2;
    cent[f][2] = (max(z[f],n) + min(z[f],n))/2;
  }
  double m[4][4];
  double minv[4][4];
  D3d_make_identity(m);
  D3d_make_identity(minv);
  f = 0;
  int disp[10];
  for (i = 0; i < 10; i++) {disp[i] = 0;}
  int dir = -1;
  while (1) {
    char c = G_wait_key();
    int num = c-48;
    char opp;
    D3d_make_identity(m);
    D3d_make_identity(minv);
    if (num >= 0 && num <= 9) {
      f = num;
      if (opp == '+') {disp[f] = 1;}
      if (opp == '-') {disp[f] = 0;}
    }
    else if (c == 't' || c == 'r' || c == '+' || c == '-' || c == 's') {
      opp = c;
    }
    else if (c == 'f') {
      dir*=-1;
    }
    else {
      if (opp == 't') {
	if (c == 'z') {D3d_translate(m,minv,0,0,1);}
	if (c == 'Z') {D3d_translate(m,minv,0,0,-1);}
	if (c == 'y') {D3d_translate(m,minv,0,1,0);}
	if (c == 'Y') {D3d_translate(m,minv,0,-1,0);}
	if (c == 'x') {D3d_translate(m,minv,1,0,0);}
	if (c == 'X') {D3d_translate(m,minv,-1,0,0);}
      }
      if (opp == 'r') {
	D3d_translate(m,minv,-cent[f][0],-cent[f][1],-cent[f][2]);
	if (c == 'z') {D3d_rotate_z(m,minv,M_PI/90);}
	if (c == 'Z') {D3d_rotate_z(m,minv,-M_PI/90);}
	if (c == 'y') {D3d_rotate_y(m,minv,M_PI/90);}
	if (c == 'Y') {D3d_rotate_y(m,minv,-M_PI/90);}
	if (c == 'x') {D3d_rotate_x(m,minv,M_PI/90);}
	if (c == 'X') {D3d_rotate_x(m,minv,-M_PI/90);}
	D3d_translate(m,minv,cent[f][0],cent[f][1],cent[f][2]);
      }
      if (opp == 's') {
	D3d_translate(m,minv,-cent[f][0],-cent[f][1],-cent[f][2]);
	if (c == '>') {D3d_scale(m,minv,1.1,1.1,1.1);}
	if (c == '<') {D3d_scale(m,minv,0.9,0.9,0.9);}
	D3d_translate(m,minv,cent[f][0],cent[f][1],cent[f][2]);
      }
    }
    D3d_mat_mult_points(x[f],y[f],z[f],m,x[f],y[f],z[f],numpoints[f]);
    D3d_mat_mult_pt(cent[f],m,cent[f]);
    G_rgb(0,0,0);
    G_clear();
    G_rgb(1,0,0);
    for (i = 0; i < 10; i++) {
      if (disp[i]) {draw(i,dir);}
    }
  }
}
