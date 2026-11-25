// Builds a default loadout record, lets func_08B3AC8C fill it for the entry found by func_088BBD8C, then applies it to the target: the entry's halfword when asked (else the self value), the first field and the two at +8 and +0xA.
struct Loadout08B3B244 {
    short a;
    short b;
    short c;
    short d;
    short e;
    short f;
    short g[6];
    short h;
    Loadout08B3B244() {
        a = 0;
        b = 0;
        c = 0;
        d = 0;
        e = -1;
        f = -1;
        h = 0;
        for (int i = 0; i < 6; i++) {
            g[i] = -1;
        }
    }
};
struct Entry08B3B244 { unsigned short id; };
struct Tab08B3B244 { char pad[0x2C]; char at2C[1]; };
extern "C" {
extern Tab08B3B244 D_09B6DAC0;
Entry08B3B244 *func_088BBD8C(int a);
void func_08B3AC8C(void *self, Loadout08B3B244 *l, int id, char *tab, int t0);
void func_08B42F90(void *t, int a1, int v);
void func_08B42EDC(void *t, int a1, int v);
void func_08B42F00(void *t, int a1, int v);
void func_08B42F38(void *t, int a1, int v);
void func_08B42F54(void *t, int a1, int v);
void func_08B3B244(void *self, int a1, int a2, void *t, int t0, unsigned char flag)
{
    Loadout08B3B244 l;
    Entry08B3B244 *en = func_088BBD8C(a2);
    int v;
    func_08B3AC8C(self, &l, en->id, D_09B6DAC0.at2C, t0);
    v = en->id;
    if (flag != 0) {
        func_08B42F90(t, a1, v);
    } else {
        func_08B42EDC(t, a1, v);
    }
    v = l.a;
    func_08B42F00(t, a1, v);
    func_08B42F38(t, a1, l.e);
    func_08B42F54(t, a1, l.f);
}
}
