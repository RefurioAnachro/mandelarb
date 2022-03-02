
#include "arf.h"
#include "arb.h"
#include "acb.h"

#include "string.h"

// #include "grt.h"
#include "gr.h"

typedef int bool;
const bool false = 0;
const bool true = 1;

slong prec = 64;
const unsigned MAXITER = 256;
bool mandel = false; // mandelbrot or julia

rgb color(unsigned i) {
    if(i == 0)
        return gr_rgb(255, 255, 255);
     return gr_rgb(i*255/MAXITER*2, i*255/MAXITER*4, i*255/MAXITER);
}

acb_t c, z0, z;

void f(acb_t c, acb_t z) {
    // z = z*z + c;
    acb_mul(z, z, z, prec);
    acb_add(z, z, c, prec);
}

arb_t t, a2, re, im;
void cardioid(acb_t z0, arb_t a) {
    // thanks Chris Thomasson
// 2 cos(a)-cos(2a)
    arb_mul_ui(a2, a, 2, prec);
    
    arb_cos(re, a, prec);
    arb_mul_ui(re, re, 2, prec);
    arb_cos(t, a2, prec);
    arb_sub(re, re, t, prec);

    arb_sin(im, a, prec);
    arb_mul_ui(im, im, 2, prec);
    arb_cos(t, a2, prec);
    arb_sub(im, im, t, prec);

    acb_set_arb_arb(z0, re, im);
    acb_div_ui(z0, z0, 4, prec);
}

arf_t mag;
bool trap(acb_t z) {
    acb_get_abs_lbound_arf(mag, z, prec);
    return arf_cmp_ui(mag, 4) > 0;
}
unsigned iter(unsigned maxiter, acb_t c, acb_t z0) {
    acb_set(z, z0);
    for(unsigned i = 0; i < maxiter; ++i) {
        f(c, z);
        if(trap(z))
            return i;
    }
    return 0;
}
arb_t mx, ax, my, ay;
arb_t zr, zi;
void init_transform(unsigned w, unsigned h, acb_t a, acb_t b) {
    arb_init(mx);
    arb_init(my);
    arb_init(ax);
    arb_init(ay);

    // mx = (b.re - a.re) / w;
    arb_sub(mx, acb_realref(b), acb_realref(a), prec);
    arb_div_ui(mx, mx, w, prec);
    acb_get_real(ax, a);
    
    arb_sub(my, acb_imagref(b), acb_imagref(a), prec);
    arb_div_ui(my, my, h, prec);
    acb_get_imag(ay, a);

    arb_init(zr);
    arb_init(zi);
}
void clear_transform() {
    arb_clear(mx);
    arb_clear(my);
    arb_clear(ax);
    arb_clear(ay);

    arb_clear(zr);
    arb_clear(zi);
}
void transform(acb_t z, unsigned x, unsigned y) {
    // z.re = x * mx + ax;
    arb_set_ui(zr, x);
    arb_mul(zr, zr, mx, prec);
    arb_add(zr, zr, ax, prec);

    arb_set_ui(zi, y);
    arb_mul(zi, zi, my, prec);
    arb_add(zi, zi, ay, prec);

    acb_set_arb_arb(z, zr, zi);
}
void reverse(unsigned *x, unsigned *y, acb_t z) {
    // x = (z.re - ax)/mx;
}
void scan(unsigned w, unsigned h) {
    unsigned i;
    for(unsigned y = 0; y < h; ++y)
        for(unsigned x = 0; x < w; ++x) {
            if(mandel) {
                transform(c, x, y);
                acb_set_d(z0, 0);
            }
            else {
                transform(z0, x, y);
            }
            i = iter(MAXITER, c, z0);
            gr_put(x, y, color(i));
        }
}
void redraw(void) {
    scan(W, H);
}
/*
int acb_set_str_str(acb_t res, char *r, char *i, int prec) {
    acb_t re, im;
    acb_init(re);
    acb_init(im);
    arb_set_str(re, r, prec);
    arb_set_str(im, i, prec);
    int ret = acb_set_arb_arb(res, re, im);
    acb_clear(re);
    acb_clear(im);
    return ret;
}
*/
int main() {
    acb_init(c);
    acb_init(z);
    acb_init(z0);
    arf_init(mag);

    acb_t a, b;
    acb_init(a);
    acb_init(b);
//    acb_set_d_d(a, -2, -1.5);
//    acb_set_d_d(b, 1, 1.5);
    acb_set_d_d(a, -2, -2);
    acb_set_d_d(b, 2, 2);
    init_transform(W, H, a, b);

    // for cardioid():
    arb_init(t);
    arb_init(a2);
    arb_init(re);
    arb_init(im);

    arb_t pi;
    arb_init(pi);
    arb_const_pi(pi, prec);
    
    arb_t angle;
    arb_init(angle);
    arb_set_d(angle, 90);
    arb_div_ui(angle, angle, 180, prec);
    arb_mul(angle, angle, pi, prec);
    cardioid(c, angle);
    gr_init(redraw);

    arb_clear(angle);
    arb_clear(pi);
    
    clear_transform();
    acb_clear(a);
    acb_clear(b);
    
    acb_clear(c);
    acb_clear(z);
    acb_clear(z0);
    arf_clear(mag);
    flint_cleanup();
}
