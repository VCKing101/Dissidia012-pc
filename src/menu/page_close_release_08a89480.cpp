/* Menu close: when the close flag is set, resets the state and releases the two child objects, the handle and the manager entry, then queues the fade. */
struct W { char p0[8]; char f8[0x400]; int h408; char p1[0xC]; int state; unsigned flags; int timer; };
struct Self { char p[0x138]; void *pool; char p1[0x18]; void *c154; void *c158; char p2[0x1B0]; W *w; };
extern "C" {
extern int D_089D7830;
void func_08A8EC28(void *, int);
void func_0885C538(void *, void *, int);
void func_08A96A88(void);
void func_08A96A3C(void *, int);
void func_0887B1F4(int, int);
void func_088D3A04(void *);
void func_088D0370(int, int, int);
void func_08A89480(Self *self)
{
    int go = (self->w->flags & 0x40000) != 0;
    go = (unsigned char)go;
    if (go) {
        self->w->state = 3;
        self->w->timer = 10000;
        if (self->c154 != 0) {
            func_08A8EC28(self->c154, 2);
            func_0885C538(self->pool, self->c154, 0);
            self->c154 = 0;
        }
        if (self->c158 != 0) {
            func_08A96A88();
            func_08A96A3C(self->c158, 2);
            func_0885C538(self->pool, self->c158, 0);
            self->c158 = 0;
        }
        if (self->w->h408 != 0) {
            func_0887B1F4(self->w->h408, 3);
            self->w->h408 = 0;
        }
        func_088D3A04(self->w->f8);
        {
            int n = 0x1E;
            func_088D0370(D_089D7830, n, 1);
        }
    }
}
}
