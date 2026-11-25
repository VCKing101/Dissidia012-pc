// Builds a default loadout record, lets func_08B3ABE0 fill it from the table at D_09B6DAC0 + 0x12, then applies the table's pair at +0x94 and two of the record's fields to the target.
struct Loadout08B3B158 {
    short a;
    short b;
    short c;
    short d;
    short e;
    short f;
    short g[6];
    short h;
    Loadout08B3B158() {
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
struct Tab08B3B158 { char pad[0x12]; char at12[0x82]; short h94; short h96; };
extern "C" {
extern Tab08B3B158 D_09B6DAC0;
void func_08B3ABE0(void *self, Loadout08B3B158 *l, int a2, char *tab);
void func_08B42DF0(void *t, int a1, int a2, int a3);
void func_08B42E48(void *t, int a1, int a2, int a3);
void func_08B42E7C(void *t, int a1, int a2);
void func_08B42E98(void *t, int a1, int a2);
void func_08B3B158(void *self, int a1, int a2, void *t)
{
    Loadout08B3B158 l;
    int p = D_09B6DAC0.h94;
    int q = D_09B6DAC0.h96;
    func_08B3ABE0(self, &l, a2, D_09B6DAC0.at12);
    func_08B42DF0(t, a1, a2, -1);
    func_08B42E48(t, a1, p, q);
    func_08B42E7C(t, a1, l.a);
    func_08B42E98(t, a1, l.b);
}
}
