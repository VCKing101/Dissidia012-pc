/* For each of 45 entries of 0x68 bytes that are active, releases them and clears their state. */
struct Ent { char pad[0x58]; int handle; int pad1; int s60; unsigned char active; char pad2[3]; };
extern "C" void func_088A9A4C(Ent *e);
extern "C" void func_088A522C(void);
extern "C" void func_088A51E4(int h);
extern "C" void func_088A56A8(Ent *base)
{
    for (int i = 0; i < 0x2D; i++) {
        Ent *e = &base[i];
        if (e->active == 1) {
            func_088A9A4C(e);
            e->active = 0;
            e->s60 = 0;
            if (e->handle != 0) {
                func_088A522C();
                func_088A51E4(e->handle);
                e->handle = 0;
            }
        }
    }
}
