extern unsigned char *D_089D69E0;
extern int D_089D7830;
extern void func_088CF6CC(int a, int b);
extern void func_088CF750(int a, int b);
typedef struct M { char pad[0x2C]; int a; int b; } M;
void func_08913B58(M *self, int mode) {
    self->a = mode;
    self->b = mode;
    switch (mode) {
    case 1:
        { int zero = 0; *D_089D69E0 = zero; }
        func_088CF6CC(D_089D7830, 0xFF);
        break;
    case 2:
        *D_089D69E0 = 1;
        func_088CF6CC(D_089D7830, 2);
        { int z2 = 0; func_088CF750(D_089D7830, z2); }
        break;
    case 4:
    case 8:
        *D_089D69E0 = 1;
        func_088CF6CC(D_089D7830, 0xFF);
        break;
    }
}
