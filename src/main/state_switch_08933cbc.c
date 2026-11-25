typedef struct Obj { char pad[0x134]; unsigned state; } Obj;
extern int D_089E09C0;
extern int D_089E330C;
extern int D_08AC4560;
extern void func_089DE910(int a, int b);
extern void func_089DE94C(int a);
extern void func_089DEBDC(int a);
extern void func_08ABC158(int a, int b, int c, int d);
extern void func_08ABC1D0(int a, int b, int c);

void func_08933CBC(Obj *self) {
    switch (self->state) {
    case 0:
        func_089DEBDC(D_089E330C);
        break;
    case 2:
        func_089DE910(D_089E09C0, 0);
        func_089DE94C(D_089E09C0);
        break;
    case 1:
        break;
    case 3:
        break;
    case 4:
        func_08ABC158(D_08AC4560, 1, 2, 1);
        func_08ABC1D0(D_08AC4560, 0, 0);
        break;
    case 5:
        func_08ABC158(D_08AC4560, 1, 2, 0);
        func_08ABC1D0(D_08AC4560, 0, 0);
        break;
    }
}
