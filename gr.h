
extern const unsigned W;
extern const unsigned H;

typedef unsigned rgb;

extern void gr_init(void (*redraw)(void));
extern rgb gr_rgb(unsigned char r, unsigned char g, unsigned char b);
extern void gr_put(unsigned x, unsigned y, unsigned c);
