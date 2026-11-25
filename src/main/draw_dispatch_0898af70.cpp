struct P98AF70 { int a, b, c, d, e, f; };
struct Q98AF70 { char pad[0xB7]; unsigned char m; char pad2[200 - 0xB8]; unsigned char flag; };
extern "C" void func_0898B2C4(void *, P98AF70 *, Q98AF70 *);
extern "C" void func_0898AB80(void *, Q98AF70 *, int, int, int, int, void *, void *);
extern "C" void func_0898ADE8(void *, int, int);
extern "C" void func_0898C2F0(float, float, void *, Q98AF70 *, int, int, int, int, void *, void *);
extern "C" void func_0898AE1C(void *, int, int);
extern "C" void func_0898BF80(void *, Q98AF70 *, int, int, void *, void *);
extern "C" void func_0898AF70(char *self, P98AF70 *p, Q98AF70 *q)
{
    char *iv2 = self + 0x60;
    char *iv = self + 0x40;
    if (q->flag == 0) {
        func_0898B2C4(self, p, q);
    }
    int fl = q->flag;
    int u = p->e;
    int v = p->f;
    if (fl != 0) {
        func_0898BF80(self, q, u, v, iv2, iv);
    } else if (q->m != 0) {
        func_0898C2F0(0.0f, 0.5f, self, q, p->a, p->c, u, v, iv, iv2);
        func_0898C2F0(0.5f, 1.0f, self, q, p->c, p->b, p->e, p->f, iv2, iv);
        func_0898AE1C(self, p->f, 2);
    } else {
        func_0898AB80(self, q, p->a, p->c, u, v, iv, iv2);
        func_0898AB80(self, q, p->d, p->b, p->e, p->f, iv2, iv);
        func_0898ADE8(self, p->f, 2);
    }
}
