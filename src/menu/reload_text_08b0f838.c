/* Frees the text handle, recreates it from a formatted name when `show` is set, then registers the widgets. */
extern void func_0887BA5C(void *h);
extern void func_0893476C(char *buf, const char *fmt, int arg);
extern void func_0887DF14(void *g, void *dst, int n, char *src);
extern void *func_0887D868(void *g, int a, void *name, int b, int c, int d, int e, int f);
extern void func_088D428C(void *a, void *b, const char *c, int d);
extern const char D_08B1C928[];
extern const char D_08B1BC94[];
extern void *D_089D6998;

typedef struct R0F838 {
    char pad0[0x404];
    char at404[0x480];
    char at884[0x520];
    void *hDA4;
} R0F838;
typedef struct S0F838 { char pad[0x138]; R0F838 *rec; } S0F838;

void func_08B0F838(S0F838 *self, int arg, unsigned char show)
{
    char buf[64];
    char name[128];
    if (self->rec->hDA4 != 0) {
        func_0887BA5C(self->rec->hDA4);
        self->rec->hDA4 = 0;
    }
    if (show != 0) {
        func_0893476C(buf, D_08B1C928, arg);
        func_0887DF14(D_089D6998, name, 0x80, buf);
        self->rec->hDA4 = func_0887D868(D_089D6998, 0, name, -1, 0, 0, 0, 0);
    }
    func_088D428C(self->rec->at404, self->rec->at884, D_08B1BC94, 1);
}
