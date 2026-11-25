struct Root088F2444 {
    char pad[0x40];
    unsigned char kind;
    unsigned char count;
    unsigned char mode;
    unsigned char hold;
    unsigned char pad44;
    unsigned char c45;
    unsigned char c46;
    unsigned char c47;
};
extern "C" void func_088F203C(void);
extern "C" int func_088C6AA0(int);
extern "C" void func_088F20E8(void);
extern "C" void func_088F229C(int, int);
extern char *D_089D712C;
extern unsigned *D_089D726C;
extern "C" void func_088F2444(void);
extern "C" void func_088F2444(void)
{
    Root088F2444 *s = (Root088F2444 *)(D_089D712C + 0x1DE98);
    int done = 0;
    if (s->mode == 2) {
        func_088F203C();
    }
    s->c46 = 1;
    s->count = s->count + 1;
    int need = func_088C6AA0(s->kind);
    if (s->count >= need) {
        switch (s->kind) {
        case 0:
            *D_089D726C = *D_089D726C | 0x400;
            break;
        case 1:
            *D_089D726C = *D_089D726C | 0x800;
            break;
        case 2:
            *D_089D726C = *D_089D726C | 0x1000;
            break;
        }
        s->count = 0;
        done = 1;
    }
    int h = s->hold + 1;
    s->c45 = 0;
    if (h > 99) {
        h = 100;
    }
    s->hold = h;
    func_088F20E8();
    if (done) {
        func_088F229C(s->kind, 0);
    }
    if (s->c47 != 0) {
        func_088F229C(s->kind, 1);
        s->c47 = 0;
    }
}
