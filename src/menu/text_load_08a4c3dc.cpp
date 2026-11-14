extern "C" {
extern int D_089D69E8;
extern char D_08A4FFE0[];
extern int func_08885544(int o);
extern void func_088850EC(int o, int kind, void *name);
extern void func_08885340(int o, int a);
extern void func_088854C4(int o, int kind, int id);
extern void func_08885388(int o);
extern void func_088852C4(int o, int a, int b);
}

extern "C" void func_08A4C3DC(void)
{
    if (func_08885544(D_089D69E8) == 0) {
        int kind = 0x1A;
        func_088850EC(D_089D69E8, kind, D_08A4FFE0);
        int none;
        none = 0;
        func_08885340(D_089D69E8, none);
        unsigned short id = 0x64B2;
        func_088854C4(D_089D69E8, kind, id);
        func_08885388(D_089D69E8);
    } else {
        int none = 0;
        int none2 = 0;
        func_088852C4(D_089D69E8, none, none2);
    }
}
