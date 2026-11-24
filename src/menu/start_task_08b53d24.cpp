// Starts the screen task (storing the start page when valid) unless the shared flag is set, in which case it calls the fallback.
extern "C" {
void func_0886503C(void *t, void *fn1, void *fn2, int a);
void func_0886504C(void *t, void *owner, int a);
void func_088650A4(void *t);
void func_08B53FA4(void);
void func_08B53FC4(void);
void func_089E0BDC(int a, int b);
extern unsigned *D_089D726C;
struct Pg53D24 { short page; short sub; };
extern Pg53D24 D_089D8590;
extern int D_089E75C4;
}

struct S53D24 {
    char pad[0x100];
    char task[1];
    static bool Has(unsigned f, unsigned m) { return (f & m) != 0; }
};

extern "C" void func_08B53D24(S53D24 *self, int page)
{
    if (S53D24::Has(*D_089D726C, 0x800000) == true) {
        int g = D_089E75C4;
        int zero = 0;
        func_089E0BDC(g, zero);
    } else {
        char *t;
        if (page >= 0) {
            D_089D8590.page = page;
            D_089D8590.sub = 0;
        }
        t = self->task;
        func_0886503C(t, (void *)func_08B53FA4, (void *)func_08B53FC4, 0);
        func_0886504C(t, self, 0x100);
        func_088650A4(t);
    }
}
