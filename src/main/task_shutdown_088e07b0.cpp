/* If the flag at 0x17B1 is set: clears D_09AFB89C, calls func_088421EC, func_0885A290(D_089D4DC8, field 4) and func_08B532FC(D_08B54330, field 4, 0). */
struct Obj088e07b0 { int pad; int id; };
extern "C" int D_09AFB89C;
extern "C" int D_089D4DC8;
extern "C" int D_08B54330;
extern "C" void func_088421EC(void);
extern "C" void func_0885A290(int a, int id);
extern "C" void func_08B532FC(int a, int id, int z);
extern "C" void func_088E07B0(Obj088e07b0 *self)
{
    Obj088e07b0 *keep[1];
    if (*((unsigned char *)self + 0x17B1)) {
        keep[0] = self;
        D_09AFB89C = 0;
        func_088421EC();
        self = keep[0];
        int id = self->id;
        int g = D_089D4DC8;
        func_0885A290(g, id);
        self = keep[0];
        func_08B532FC(D_08B54330, self->id, 0);
    }
}
