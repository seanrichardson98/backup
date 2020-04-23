
#include <FPT.h>   

int main()
{
   double swidth, sheight ;
   double lowleftx, lowlefty, width, height ;
   double x[10],y[10] ;
   double xx[10],yy[10];
   double xx0,yy0,xx1,yy1,xx2,yy2,xx3,yy3,xx4,yy4;
   double xxx[10],yyy[10];
   double numpoints,q, x0,y0,x1,y1 ;
   double p[2],w[2] ;

   // must do this before you do 'almost' any other
   // graphical tasks 
   swidth = 400 ;  sheight = 400 ;
   G_init_graphics (swidth, sheight) ;


   // draw a point, a line, some rectangles, some triangles 

   G_rgb(1, 0, 0) ;  // red 
   G_point(200, 380) ;

   G_rgb(0, 1, 0) ;  // green
   //   G_line (0, 0, 400, 400) ; // won't show if part is off screen
   G_line (0, 0, 399, 399) ;
   
   G_rgb(1,0,0);
   G_line(399,0,0,399);

   G_rgb(0,0,1);
   G_line(200,0,200,399);

   G_rgb(1,0,1);
   G_line(0,200,399,200);

   G_rgb(0,0,0);
   G_fill_circle(200,200,25);

   G_rgb(1,0,0);
   G_rectangle(170,170,60,60);

   G_rgb(0, 0, 1) ;  // blue 
   lowleftx = 200 ; lowlefty = 50 ; width = 10 ; height = 30 ;
   G_rectangle (lowleftx, lowlefty, width, height) ;
   lowleftx = 250 ; 
   G_fill_rectangle (lowleftx, lowlefty, width, height) ;

   G_rgb(1, 1, 0) ;  // yellow
   G_triangle (10,300,  40,300,  60,250) ;
   G_fill_triangle (10,100,  40,100,  60,150) ;

   G_rgb(1, 0.5, 0) ;  // orange
   G_circle (100, 300, 75) ;
   G_fill_circle (370, 200, 50) ;

   // prints text in your graphics window 
   G_rgb(0, 0, 0) ;  // black
   G_draw_string ("hello",300,100) ;

   // draw a polygon 
   x[0] = 100 ;   y[0] = 100 ;
   x[1] = 100 ;   y[1] = 300 ;
   x[2] = 300 ;   y[2] = 300 ;
   x[3] = 300 ;   y[3] = 100 ;
   x[4] = 200 ;   y[4] = 175 ;
   numpoints = 5 ;
   G_polygon (x,y,numpoints) ;

   xx[0]=150; yy[0]=200;
   xx[1]=200; yy[1]=150;
   xx[2]=250; yy[2]=200;
   xx[3]=200; yy[3]=250;
   G_polygon(xx,yy,4);

   G_rgb(.5,.5,0);
   G_triangle(100,300, 200,350, 300,300);

   

   q = G_wait_key() ; // pause to look ...any key to continue


   G_rgb (0.4, 0.2, 0.1) ; // brown
   G_fill_polygon (x,y,numpoints) ;

   G_rgb (0.5, 0.8, 0.4) ;// what color is this?

   G_wait_click(p) ;   // wait for a mouse click
   x0 = p[0] ; y0 = p[1] ; // extract coordinates 
   G_fill_rectangle (x0-2, y0-2, 4,4) ;// mark the clicked point


   G_wait_click(p) ;  
   x1 = p[0] ; y1 = p[1] ;
   G_fill_rectangle (x1-2, y1-2, 4,4) ;


   G_rgb (0.5, 0.5, 0.5) ; // a grey
   G_line (x0,y0, x1,y1) ;

   G_wait_click(w);
   xx0=w[0];yy0=w[1];
   xxx[0]=xx0;yyy[0]=yy0;
   G_wait_click(w);
   xx1=w[0];yy1=w[1];
   G_wait_click(w);
   xx2=w[0];yy2=w[1];
   G_wait_click(w);
   xx3=w[0];yy3=w[1];
   G_wait_click(w);
   xx4=w[0];yy4=w[1];
G_polygon

   q = G_wait_key() ;    // pause again before exit 

   G_save_image_to_file ("t01c.xwd") ;

   G_close() ; // terminate graphics...probably not fatal if forgotten

}



