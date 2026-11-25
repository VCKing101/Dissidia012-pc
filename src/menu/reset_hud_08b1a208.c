typedef struct B2 {
    char pad[0x404]; char s404[0x7BC - 0x404]; int w7BC[5]; int w7D0[5]; char pad7E4[0xCE8 - 0x7E4]; int wCE8; int wCEC; char padCF0[0xD18 - 0xCF0];
    int wD18[5]; int wD2C[5]; int wD40[5];
} B2;
typedef struct Wk { char pad[0x138]; B2 *b; } Wk;
extern char D_08B1C778[];
extern int func_08B0E058(void);
extern void func_0887BA5C(int);
extern int func_0887E534(int);
extern void func_088D428C(void *, void *, void *, int);
extern void func_088D9F5C(int, int, int);
extern void func_0887B944(int);
extern void func_0887B924(int);
extern void func_088D44EC(void *, void *, void *);
extern void func_08B0E084(Wk *);
void func_08B1A208(Wk *self)
{
    int sel;
    int i;
    sel = func_08B0E058();
    if (self->b->wCE8 != 0) {
        func_0887BA5C(self->b->wCE8);
        self->b->wCE8 = 0;
    }
    if (self->b->wCEC != 0) {
        func_0887BA5C(self->b->wCEC);
        self->b->wCEC = 0;
    }
    if (self->b->wD40[sel] != 0) {
        self->b->wCE8 = func_0887E534(self->b->wD40[sel]);
        self->b->wCEC = func_0887E534(self->b->wD40[sel]);
    }
    func_088D428C(self->b->s404, (char *)self->b + 0x884, D_08B1C778, 1);
    for (i = 0; i < 5; i++) {
        func_088D9F5C(self->b->w7BC[i], 3, 1);
        func_088D9F5C(self->b->w7D0[i], 4, 1);
        if (self->b->wD18[i] != 0) {
            func_0887B944(self->b->wD18[i]);
        }
        if (self->b->wD2C[i] != 0) {
            func_0887B944(self->b->wD2C[i]);
        }
    }
    func_088D9F5C(self->b->w7BC[sel], 2, 1);
    if (self->b->wD18[sel] != 0) {
        func_0887B924(self->b->wD18[sel]);
    }
    func_088D44EC(self->b->s404, (char *)self->b + 0x884, D_08B1C778);
    func_08B0E084(self);
}
