/* Destructor of a large screen object: releases its base, runs the three member-array destructors and frees itself when the delete flag is set. */
extern "C" {
void func_08882158(void *p);
void func_0899372C(void *arr, int n, int size, void *dtor, int a, int b);
void func_088DE0BC(void);
void func_088DCB5C(void);
void func_088D99B0(void);
void func_08992F04(void *p);
}
struct Big {
    void *base;
    char p0[0x3374 - 4];
    char m3[0x42C4 - 0x3374];
    char m3a[0x1EA * 0x44];
    char p1[0xC500 - 0x3374 - (0x42C4 - 0x3374) - 0x1EA * 0x44];
    char m2[0xC780 - 0xC500];
    char m2a[0x50 * 0x74];
    char p2[0xEBD4 - 0xC780 - 0x50 * 0x74];
    char m1[0xF444 - 0xEBD4];
    char m1a[0x10E * 0x14];
    static void run(Big *self, int flags)
    {
        func_08882158(self->base);
        if (self->m1 != 0) {
            func_0899372C(self->m1a, 0x10E, 0x14, (void *)func_088DE0BC, 0, 0);
        }
        if (self->m2 != 0) {
            func_0899372C(self->m2a, 0x50, 0x74, (void *)func_088DCB5C, 0, 0);
        }
        if (self->m3 != 0) {
            func_0899372C(self->m3a, 0x1EA, 0x44, (void *)func_088D99B0, 0, 0);
        }
        if (flags & 1) {
            func_08992F04(self);
        }
    }
};
extern "C" void func_088D14B8(Big *self, int flags)
{
    if (self != 0) {
        Big::run(self, flags);
    }
}
