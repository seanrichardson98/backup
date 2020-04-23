#include <FPT.h>
#include <D3d_matrix.h>
#include <mytools.h>

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

typedef
struct {
  int objnum ;
  int polynum ;
  double dist ;
} THING;

THING things[5000];
int thing_index = 0;

void thingify(int f) {
  int p,i;
  double xp[100], yp[100], zp[100];
  for (p = 0; p < numpolys[f]; p++) {
    for (i = 0; i < psize[f][p]; i++) {
      xp[i] = x[f][con[f][p][i]];
      yp[i] = y[f][con[f][p][i]];
      zp[i] = z[f][con[f][p][i]];   
    }
    int n = psize[f][p];
    double x_cent = (max(xp,n)+min(xp,n))/2;
    double y_cent = (max(yp,n)+min(yp,n))/2;
    double z_cent = (max(zp,n)+min(zp,n))/2;
    double d = sqrt(pow(x_cent,2)+pow(y_cent,2)+pow(z_cent,2));
    things[thing_index].objnum  = f;
    things[thing_index].polynum = p;
    things[thing_index].dist    = d;
    thing_index++;
  }
}

int compare (const void *p, const void *q) {
  THING *a, *b;

  a = (THING*)p ;
  b = (THING*)q ;

  if  (((*a).dist) < ((*b).dist)) return 1;
  else if (((*a).dist) > ((*b).dist)) return -1;
  else return 0;
}

void draw_poly(int f, int p) {
  int i;
  double xtemp[1000], ytemp[1000];
  for (i = 0; i < psize[f][p]; i++) {
    double xp = x[f][con[f][p][i]];
    double yp = y[f][con[f][p][i]];
    double zp = z[f][con[f][p][i]]; 
    xtemp[i] = WIDTH*(xp/zp)/tan(HA) + WIDTH/2;
    ytemp[i] = HEIGHT*(yp/zp)/tan(HA) + HEIGHT/2;
  }
  G_rgb(f%3/2,(f+1)%3/2,(f+2)%3/2);
  G_fill_polygon(xtemp,ytemp,psize[f][p]);
  G_rgb(0,0,0);
  G_polygon(xtemp,ytemp,psize[f][p]);
}

void draw_object(int f) {
  int i;
  for (i = 0; i < numpolys[f]; i++) {
    draw_poly(f,i);
  }
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
    //print_arr(x[f],numpoints[f]);
    D3d_mat_mult_pt(cent[f],m,cent[f]);
    G_rgb(0,0,0);
    G_clear();
    G_rgb(1,0,0);
    thing_index = 0;
    for (i = 0; i < 10; i++) {
      if (disp[i]) {
	thingify(i);
      }
    }
    qsort(things,thing_index,sizeof(THING),compare);
    for (i = 0; i < thing_index; i++) {
      draw_poly(things[i].objnum, things[i].polynum);
    }
  }
}
