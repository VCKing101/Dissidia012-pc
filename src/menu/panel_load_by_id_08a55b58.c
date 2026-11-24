/* Frees three effect handles, looks up an effect by id and, when found, creates and shows it. */
typedef struct Panel {
    char g0[0x5C];
    int h0;
    int h1;
    int h2;
    char g1[0x8C - 0x68];
} Panel;
extern void func_0887BA5C(void);
extern int func_0887D868(int mgr, int a, int b, int c, int d, int e, int f, int g);
extern int func_0887E534(int h);
extern int func_0889F3E4(int mgr, int id);
extern int func_0889F4AC(int mgr, int id);
extern void func_088D428C(void *a, void *b, void *c, int d);
extern void func_08A55174(void *p);
extern int D_089D6C30;
extern int D_089D6998;
void func_08A55B58(Panel *arg, int id) {
    Panel *self = arg;
    int found;
    int res;
    int h;
    if (self->h0 != 0) {
        func_0887BA5C();
        self->h0 = 0;
    }
    if (self->h1 != 0) {
        func_0887BA5C();
        self->h1 = 0;
    }
    if (self->h2 != 0) {
        func_0887BA5C();
        self->h2 = 0;
    }
    found = func_0889F3E4(D_089D6C30, id);
    res = func_0889F4AC(D_089D6C30, id);
    if (found == 0) {
        return;
    }
    {
        h = func_0887D868(D_089D6998, 0, res, -1, 0, 0, 0, 0);
        self->h0 = h;
        h = func_0887E534(h);
        self->h1 = h;
        h = func_0887E534(self->h0);
        self->h2 = h;
        func_088D428C(self, &self->h0, (char *)self + 0x8C, 1);
        func_08A55174(self);
    }
}
