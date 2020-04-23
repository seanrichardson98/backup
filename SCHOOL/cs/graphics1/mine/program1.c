#include <FPT.h>
double max(double * x, int n) {
  if (n <= 0) {
    return -1;
  }
  int max = x[0];
  int i;
  for (i = 0; i < n; i++) {
    if (x[i] > max) {
      max = x[i];
    }
  }
  return max;
}
double min(double * x, int n) {
  if (n <= 0) {
    return -1;
  }
  int min = x[0];
  int i;
  for (i = 0; i < n; i++) {
    if (x[i] < min) {
      min = x[i];
    }
  }
  return min;
}
void print_arr(double * x, int n) {
  int i;
  for (i = 0; i < n; i++) {
    printf("%d: %lf\t",i,x[i]);
  }
  printf("\n");
}
void sort(double * x, int n) {
  int i;
  for (i = 0; i < n; i++) {
    int j;
    for (j = 0; j < n; j++) {
      double hold;
      if (x[j] > x[i]) {
	hold = x[i];
	x[i] = x[j];
	x[j] = hold;
      }
    }
  }
}

void my_fill_polygon(double * x, double * y, int n) {
  //Loop through each horizontal line
  int r;
  for(r = min(y,n); r < max(y,n)+1; r++) {
    printf("%d:\t",r);
    int i;
    double ints[n];
    int count = 0;
    //Locate intersections:
    for (i = 0; i < n; i++) {
      int j = (i+1)%n;
      if ((r > y[i] && r < y[j]) || (r < y[i] && r > y[j]) && (y[i] != y[j])) {
	ints[count] = (r - y[i])*((x[i]-x[j])/(y[i]-y[j]))+x[i];
	count++;
      }
    }
    //Corners cases:
    for (i = 0; i < n; i++) {
      if (r == y[i]) {
	int h = (i-1)%n; int j = (i+1)%n;
	//dir1 is last non-horiz
	int rev;
	double dir1 = 0;
	for (rev = 0; rev < n; rev++) {
	  dir1 = y[(i-rev+n)%n] - y[(h-rev+n)%n];
	  if (dir1 != 0) {
	    break;
	  }
	}
	double dir2 = y[j] - y[i];
	//printf("%lf %lf ",dir1,dir2);
	if (dir1*dir2 > 0) {
	  printf("X:%lf ",x[i]);
	  ints[count] = x[i];
	  count++;
	}
      }
    }
    //Sort intersections
    sort(ints,count);
    //printf("\t");
    print_arr(ints,count);
    //Fill b/w intersections
    for (i = 0; i < count-1; i+=2) {
      G_line(ints[i],r, ints[i+1],r);
    }
  }
}

void my_polygon(double * x, double * y, int n) {
  int i;
  for (i = 0; i < n; i++) {
    int j = (i+1)%n;
    G_line(x[i],y[i],x[j],y[j]);
  }
}
void draw_star(double * p1, double * p2) {
  double r = sqrt(pow((p1[0]-p2[0]),2)+pow(p1[1]-p2[1],2));
  int i;
  double x[5];
  double y[5];
  for (i=0; i < 5; i++) {
    x[i] = r*(cos(4*M_PI/5*i+M_PI/2))+p1[0];
    y[i] = r*(sin(4*M_PI/5*i+M_PI/2))+p1[1];
  }
  // my_fill_polygon(x,y,5,1000);
  //G_polygon(x,y,5);
}

int main() {
  //printf("0 ");
  double p[2];
  double p1[2];
  double p2[2];
  double x[20];
  double y[20];
  G_init_graphics(1000,1000);
  G_fill_rectangle(0,0,20,20);
  /* while (!(p1[0]>0 && p1[0]<20 && p1[1]>0 && p1[1]<20)) {
    G_wait_click(p1);
    G_wait_click(p2);
    draw_star(p1,p2);
  }*/
  //printf("1 ");
  int i = 0;
  while (1) {
    //printf("2 ");
    G_wait_click(p);
    x[i] = p[0];
    y[i] = p[1];
    if (p[0]>0 && p[0]<20 && p[1]>0 && p[1]<20) {
      break;
    }
    i++;
  }
  printf("i:%d\n",i);
  
  /*int s = 10;
  x[0] = 10*s; x[1] = 20*s; x[2] = 30*s; x[3] = 40*s; x[4] = 20*s;
  y[0] = 10*s; y[1] = 10*s; y[2] = 10*s; y[3] = 5*s; y[4] = 40*s;
  i = 5;*/

  print_arr(x,i);
  print_arr(y,i);

  //print_double_arr(y,i);
  my_polygon(x,y,i);
  G_wait_click(p);
  my_fill_polygon(x,y,i);
  G_wait_click(p);
    //G_circle(p1[0],p1[1],r);
}
