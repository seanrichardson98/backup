#include <FPT.h>

int main(int argc, char ** argv) {
  //Reading:
  q = fopen("filename","r");
  fscanf(q,"%d", &var);
  fclose(q);
  //Writing:
  f = fopen ("filename","w");
  fprintf(f,"%d",var);
  //Draw:
  G_init_graphics(HEIGHT,WIDTH);
  G_rgb(0,0,0);
  G_clear();
  G_fill_polygon(xs,ys,n);
  G_polygon(xs,ys,n);
  G_line(x1,y1,x2,y2);
  G_fill_circle(x,y,r);
}
