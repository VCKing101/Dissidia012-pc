struct P98AE50 { int a, b; int pad[2]; int c, d; };
struct Q98AE50 { char pad[0xB7]; unsigned char m; char pad2[200 - 0xB8]; unsigned char flag; };
extern "C" void func_0898B664(void *, P98AE50 *, Q98AE50 *);
extern "C" void func_0898AB80(void *, Q98AE50 *, int, int, int, int, void *, void *);
extern "C" void func_0898ADE8(void *, int, int);
extern "C" void func_0898C2F0(float, float, void *, Q98AE50 *, int, int, int, int, void *, void *);
extern "C" void func_0898AE1C(void *, int, int);
extern "C" void func_0898BF80(void *, Q98AE50 *, int, int, void *, void *);
extern "C" void func_0898AE50(char *self, P98AE50 *p, Q98AE50 *q)
{
    char *iv = self + 0x40;
    if (q->flag == 0) {
        func_0898B664(self, p, q);
    }
    int fl = q->flag;
    int u = p->c;
    int v = p->d;
    if (fl != 0) {
        func_0898BF80(self, q, u, v, iv, iv);
    } else if (q->m != 0) {
        func_0898C2F0(0.0f, 1.0f, self, q, p->a, p->b, u, v, iv, iv);
        func_0898AE1C(self, p->d, 1);
    } else {
        func_0898AB80(self, q, p->a, p->b, u, v, iv, iv);
        func_0898ADE8(self, p->d, 1);
    }
}
