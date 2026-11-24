/* Frees three effect handles, creates a numbered effect from a format string and refreshes the panel. */
typedef struct Panel {
    char g0[0x5C];
    char a5c[0xC];
    int h0;
    int h1;
    int h2;
    char g1[0x8C - 0x74];
    char a8c[0x104 - 0x8C];
    int index;
} Panel;
extern void func_0887BA5C(void);
extern int func_0887D868(int mgr, int a, void *b, int c, int d, int e, int f, int g);
extern int func_0887E534(int h);
extern void func_088D428C(void *a, void *b, void *c, int d);
extern void func_089347E8(void *buf, const void *fmt, int n);
extern int D_089D6998;
extern const char D_08A58668[];
void func_08A55938(Panel *arg) {
    Panel *self = arg;
    char buf[128];
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
    func_089347E8(buf, D_08A58668, self->index + 1);
    h = func_0887D868(D_089D6998, 0, buf, -1, 0, 0, 0, 0);
    self->h0 = h;
    h = func_0887E534(h);
    self->h1 = h;
    h = func_0887E534(self->h0);
    self->h2 = h;
    func_088D428C(self, self->a5c, self->a8c, 1);
}
