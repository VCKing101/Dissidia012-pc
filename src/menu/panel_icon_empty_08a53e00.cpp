// Rebuilds a panel icon handle: picks the "blank" or normal icon depending on whether slot data is all unset.
extern "C" {
void func_08A53F1C(void *p, int a, int b);
void func_0887BA5C();
int func_0887B3EC(int a, int b, int c, int d, int e, int f);
void func_088D428C(void *a, void *b, void *c, int d);
}
struct Slot {
    short a, b, c, d;
    signed char e, f, pad0, pad1, g, h;
};
struct Panel {
    char g0[8];
    int mgr;
    char g1[0xB4 - 0xC];
    int h0;
    char g2[0x23C - 0xB8];
    Slot **src;
    static bool Blank(Slot *s, int none) {
        if (s->a == none && s->b == 0 && s->c == 0 && s->d == 0 && s->e == none && s->f == none && s->g == none && s->h == none) {
            return true;
        }
        return false;
    }
};
extern "C" void func_08A53E00(Panel *arg) {
    Panel *p = arg;
    int kind;
    int none = -1;
    if (p->h0) { func_0887BA5C(); p->h0 = 0; }
    if (Panel::Blank(*p->src, none)) {
        kind = 0x6B;
        func_08A53F1C(p, 0, 5);
    } else {
        kind = 0x1A;
        func_08A53F1C(p, 0, 4);
    }
    p->h0 = func_0887B3EC(p->mgr, 8, 0, kind, 0, 0);
    func_088D428C((char *)p + 0xC, (char *)p + 0xB4, (char *)p + 0xC0, 1);
}
