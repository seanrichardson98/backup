#include <stdio.h>
#include <FPT.h>

void string_par(double p[2], double u, double t, double d) {
    p[0] = u/(2*M_PI);
    p[1] = sin(t)*sin(d*u);
}

void draw_point(double p[2]) {
    double x = (p[0])*800;
    double y = (p[1])*300+400;
    G_fill_circle(x,y,5);
}

int main() {
    double p[2];
    char fname[100];
    G_init_graphics(800,800);
    double tlo = 0; double thi = 2*M_PI; double tstep = 0.2;
    double ulo = 0; double uhi = 2*M_PI; double ustep = 0.001;
    double u,t,d;
    int i = 0;
    for (d = 0.5; d <= 2.0; d += 0.5) {
        for (t = tlo; t < thi; t += tstep) {
            G_rgb(1,1,1);
            G_clear();
            G_rgb(0,0,0);
            G_text();
            for (u = ulo; u < uhi; u += ustep) {
                string_par(p,u,t,d);
                draw_point(p);
            }
            G_wait_key();
            sprintf(fname,"string/string%04d.xwd",i);
            G_save_image_to_file(fname);
            printf("pic%d\n",i);
            i++;
        }
    }
}
