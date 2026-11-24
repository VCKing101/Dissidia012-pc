/* Draws a visible screen sprite: binds its texture, sets the draw state and
   blits its texture rectangle to its screen rectangle. */
typedef struct Blit {
    char pad_00[0xC];
    int x;
    int y;
    int w;
    int h;
    int u;
    int v;
    int uw;
    int vh;
    int color;
    int c0;
    int c1;
    int c2;
    int c3;
    int c4;
    unsigned char visible;
    unsigned char filter;
} Blit;

typedef struct DrawCtx {
    char pad_00[8];
    int texture;
} DrawCtx;

extern DrawCtx *D_089D6D80;

extern void func_08941210(int texture, int filter);
extern void func_089A0384(int state);
extern void func_089A03DC(int state);
extern void func_089A119C(int on);
extern void func_089A12A4(int a, int b, int c, int d, int e);
extern int func_08941250(int texture);
extern int func_0894125C(int texture);
extern void func_08866F50(float u0, float v0, float u1, float v1, int x0, int y0, int x1, int y1, int color);
extern void func_089A0DF4(int state, int on);

void func_088955B8(Blit *self)
{
    int texture;
    float width;
    float height;

    if (self->visible == 0) {
        return;
    }
    texture = D_089D6D80->texture;
    func_08941210(texture, self->filter);
    func_089A0384(4);
    func_089A0384(9);
    func_089A03DC(3);
    func_089A03DC(1);
    func_089A03DC(10);
    func_089A03DC(7);
    func_089A119C(1);
    func_089A12A4(self->c0, self->c1, self->c2, self->c3, self->c4);
    width = func_08941250(texture);
    height = func_0894125C(texture);
    {
        float u0 = self->u / width;
        float u1 = (self->u + self->uw) / width;
        float v0 = self->v / height;
        float v1 = (self->v + self->vh) / height;
        func_08866F50(u0, v0, u1, v1, self->x, self->y, self->x + self->w, self->y + self->h, self->color);
    }
    func_089A119C(0);
    func_089A0DF4(7, 1);
}
