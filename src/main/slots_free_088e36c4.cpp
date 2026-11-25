extern "C" void func_088DA060(int);
extern "C" void func_0887BA5C(int);
extern "C" void func_088E052C(int, int);
struct Res98 { short a; short b; int f4; };
struct Slot98 {
    Res98 *p[8];
    int q[8];
    unsigned char b40;
    signed char b41;
    unsigned char b42;
    unsigned char b43;
    static void kill(int a, Slot98 *s, int j) { if (a != 0) { func_088DA060(a); s->p[j]->f4 = 0; } }
    static void kill2(int b, Slot98 *s, int j) { if (b != 0) { func_0887BA5C(b); s->q[j] = 0; } }
};
struct Own98 { int f0; char pad[0x6A8 - 4]; Slot98 slot[4]; };
extern "C" void func_088E36C4(Own98 *self);
extern "C" void func_088E36C4(Own98 *self)
{
    for (int i = 0; i < 4; i++) {
        Slot98 *s = &self->slot[i];
        if (s->b42 != 0) {
            if (s->b43 != 0) {
                func_088E052C(self->f0, s->b41);
            }
            for (int j = 0; j < 8; j++) {
                if (s->p[j] == 0) {
                    continue;
                }
                {
                    int a = s->p[j]->f4;
                    if (a != 0) {
                        Slot98::kill(a, s, j);
                    }
                    int b = s->q[j];
                    if (b != 0) {
                        Slot98::kill2(b, s, j);
                    }
                    Res98 *r = s->p[j];
                    r->a = -1;
                    r->b = -1;
                    r->f4 = 0;
                    s->p[j] = 0;
                }
            }
        }
    }
}
