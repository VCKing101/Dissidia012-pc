typedef struct Flags { char data_00[0x18]; unsigned char mode; unsigned char dirty; } Flags;
typedef struct Scene { int field_00; char sub[0x4C - 4]; int state; char data_50[4]; int arg; char data_58[0x68 - 0x58]; unsigned char kind; } Scene;
extern Flags *D_089D6A00;
extern int D_089D8508;
extern void func_088876B0(Flags *flags);
extern void func_088650A4(void *sub);
extern int func_0891BA84(int ctx, unsigned char kind, int arg);

void func_08889808(Scene *scene)
{
    Flags *flags = D_089D6A00;
    if (flags->dirty != 0) {
        unsigned char zero = 0;
        flags->dirty = zero;
        func_088876B0(flags);
    }
    if (flags->mode == 1 || scene->state == 9) {
        func_088650A4(scene->sub);
    } else if (func_0891BA84(D_089D8508, scene->kind, scene->arg) == 1) {
        func_088650A4(scene->sub);
    }
}
