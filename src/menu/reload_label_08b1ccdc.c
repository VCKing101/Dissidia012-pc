/* Frees the label handle in the selected slot, formats a new name from the current entry, creates the label and registers it. */
extern void func_0887BA5C(void *h);
extern void func_0893476C(char *buf, const char *fmt, int arg);
extern void func_0887DF14(void *g, void *dst, int n, char *src);
extern void *func_0887D868(void *g, int a, void *name, int b, int c, int d, int e, int f);
extern void func_088D428C(void *a, void *b, const char *c, int d);
extern void func_08B1CDF0(void *self);
extern const char D_08B22A50[];
extern const char D_08B224D0[];
extern unsigned short D_08B228A8;
extern void *D_089D6998;

typedef struct R1CCDC {
    char pad0[0x404];
    char at404[0x80];
    char pad484[0x884 - 0x484];
    void *handle[4];
    char pad894[0xA76 - 0x894];
    char name[0x80];
    char padAF6[0x1C10 - 0xAF6];
    int val[8];
    char pad1C30[0x80F8 - 0x1C30];
    int cur;
} R1CCDC;
typedef struct S1CCDC { char pad[0x138]; R1CCDC *rec; } S1CCDC;

void func_08B1CCDC(S1CCDC *self)
{
    char buf[64];
    if (self->rec->handle[D_08B228A8] != 0) {
        func_0887BA5C(self->rec->handle[D_08B228A8]);
        self->rec->handle[D_08B228A8] = 0;
    }
    func_0893476C(buf, D_08B22A50, self->rec->val[self->rec->cur]);
    func_0887DF14(D_089D6998, self->rec->name, 0x80, buf);
    {
        char *nm = self->rec->name;
        void *h = func_0887D868(D_089D6998, 0, nm, -1, 0, 0, 0, 0);
        self->rec->handle[D_08B228A8] = h;
    }
    func_088D428C(self->rec->at404, self->rec->handle, D_08B224D0, 1);
    func_08B1CDF0(self);
}
