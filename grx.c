
#include "gr.h"

#include "X11/Xlib.h"

const unsigned W = 500;
const unsigned H = 500;

Display *d;
Window w;
XEvent e;
int s;
GC *gc;

void gr_init(void (*redraw)(void)) {
    d = XOpenDisplay(NULL);
    s = DefaultScreen(d);
    w = XCreateSimpleWindow(d, RootWindow(d, s), 0, 0, W, H, 1, BlackPixel(d, s), WhitePixel(d, s));
    gc = DefaultGC(d, s);
    XSelectInput(d, w, ExposureMask | KeyPressMask);
    XMapWindow(d, w);
    
    while(1) {
        XNextEvent(d, &e);
        if(e.type == Expose)
            redraw();
//        if(e.type == KeyPress)
//            break;
    }

    XCloseDisplay(d);
}
rgb gr_rgb(unsigned char r, unsigned char g, unsigned char b) {
    return r << 16 | g << 8 | b;
}
void gr_put(unsigned x, unsigned y, rgb c) {
    XSetForeground(d, gc, c);
    XFillRectangle(d, w, gc, x, y, 1, 1);
}
