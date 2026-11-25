extern "C" void func_088DA060(int);
extern "C" void func_0887BA5C(int);
struct Res98 { int f0; int f4; };
struct Slot98 {
    Res98 *p[8];
    int q[8];
    unsigned char b40;
    unsigned char b41;
    unsigned char b42;
    unsigned char b43;
    static void kill(int a, Slot98 *s, int j) { if (a != 0) { func_088DA060(a); s->p[j]->f4 = 0; } }
    static void kill2(int b, Slot98 *s, int j) { if (b != 0) { func_0887BA5C(b); s->q[j] = 0; } }
};
struct Own98 { char pad[0x6A8]; Slot98 slot[4]; };
extern "C" void func_088E37D4(Own98 *self);
extern "C" void func_088E37D4(Own98 *self)
{
    for (int i = 0; i < 4; i++) {
        Slot98 *s = &self->slot[i];
        if (s->b42 != 0) {
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
                }
            }
        }
        for (int j = 0; j < 8; j++) {
            s->p[j] = 0;
            s->q[j] = 0;
        }
        s->b40 = 0;
        s->b41 = 0;
        s->b43 = 0;
        s->b42 = 0;
    }
}
