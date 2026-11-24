// Looks up the current entry's record, formats its number, frees three effect handles and creates the effect.
struct Panel {
    char g0[0x54];
    int owner;
    char g1[4];
    int h0;
    int h1;
    int h2;
    char g2[0x104 - 0x68];
    int index;
    int ids[1];
};
struct Rec {
    char g0[2];
    char num;
    char g1;
    short value;
};
extern "C" {
extern void func_0887BA5C(void);
extern int func_0887B3EC(int owner, int a, int b, int c, void *d, int e);
extern int func_0887E534(int h);
extern void func_088D428C(void *a, void *b, void *c, int d);
extern Rec *func_08927D2C(int mgr, int id);
extern void func_089347E8(void *buf, const void *fmt, int n);
extern int D_089D8530;
extern const char D_08A58668[];
}
struct Look { static Rec *Find(int id) { return func_08927D2C(D_089D8530, id); } };
extern "C" void func_08A56868(Panel *self) {
    char buf[0x40];
    char *args[1];
    Rec *r;
    int kind;
    int value;
    int num;
    int h;
    r = Look::Find(*(int *)((char *)self + self->index * 4 + 0x138));
    kind = 8;
    value = 0x2E;
    num = -1;
    if (r != 0) {
        kind = 9;
        value = r->value;
        num = r->num;
    }
    args[0] = buf;
    func_089347E8(buf, D_08A58668, num);
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
    h = func_0887B3EC(self->owner, kind, 0, value, args, 1);
    self->h0 = h;
    h = func_0887E534(h);
    self->h1 = h;
    h = func_0887E534(self->h0);
    self->h2 = h;
    func_088D428C(self, &self->h0, (char *)self + 0x8C, 1);
}
