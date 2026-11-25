typedef struct E1 { char pad[0xC]; short s; } E1;
typedef struct Top { char pad[0x4F4]; E1 *e; } Top;
typedef struct Blk {
    int w0; char s4[0x400]; char s404[0x480]; char s884[0x4F4 - 0x884 + 0x1000];
} Blk;
typedef struct Wk { char pad[0x138]; char *b; } Wk;
extern void *D_08B01CF8;
extern char D_08B1BBF4[], D_08B1BC1C[], D_08B1BC4C[], D_08B1BCBC[], D_08B1BE1A[];
extern int func_08B0DF10(Wk *);
extern void func_088D3F00(void *, void *, void *);
extern void func_088D44EC(void *, void *, void *);
extern void func_08B077BC(Wk *);
extern void func_08AFE69C(void *, int);
extern void func_08B07DE4(Wk *);
void func_08B052B8(Wk *self)
{
    int r;
    char *b;
    char *a;
    char *c;
    int i;
    if (((Top *)self->b)->e->s != -1) {
        r = func_08B0DF10(self);
        b = self->b;
        a = b + 4;
        c = b + 0x404;
        if (r != 0) {
            func_088D3F00(b + 4, c, D_08B1BC1C);
            func_088D44EC(self->b + 0x404, self->b + 0x884, D_08B1BC4C);
            b = self->b;
        } else {
            func_088D3F00(a, c, D_08B1BBF4);
            b = self->b;
        }
        func_088D3F00(b + 4, b + 0x404, D_08B1BCBC);
        {
            char *p = self->b;
            char *q = p + 0x404;
            func_088D3F00(p + 4, q, D_08B1BE1A);
        }
        b = self->b;
        func_088D3F00(b + 4, b + 0x404, b + 0x1FA24);
        for (i = 0; i < 10; i++) {
            b = self->b;
            func_088D3F00(b + 4, b + 0x404, b + i * 0x40 + 0x1FA7C);
            b = self->b;
            func_088D44EC(b + 0x404, b + 0x884, b + i * 0x48 + 0x1FCFC);
        }
        func_08B077BC(self);
        r = func_08B0DF10(self);
        if (r == 0) {
            func_08AFE69C(D_08B01CF8, 0);
        }
        func_08B07DE4(self);
    }
}
