/* Releases two handles, two resources and a flagged object, then clears them. */
struct O { int h0; int h1; char sub[8]; int r[2]; int pad; unsigned char f; };
extern "C" void func_0887BA5C(int);
extern "C" void func_088DA060(int);
extern "C" void func_088DAB44(int);
extern "C" void func_088D0370(int, int, unsigned char);
extern "C" void func_0887B3B8(void *);
extern int D_089D7830;
extern "C" void func_0891E5E4(O *self)
{
    for (int i = 0; i < 2; i++) {
        if (self->r[i]) { func_0887BA5C(self->r[i]); self->r[i] = 0; }
    }
    if (self->h1) { func_088DA060(self->h1); self->h1 = 0; }
    if (self->h0) { func_088DAB44(self->h0); self->h0 = 0; }
    unsigned char c = 1; int a = 0x23; func_088D0370(D_089D7830, a, c);
    if (self->f == 1) { func_0887B3B8(self->sub); self->f = 0; }
}
