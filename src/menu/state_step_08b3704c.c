/* Menu state step: waits on a pending handle, then sets the next state from a lookup result. */
typedef struct { int f0; int f4; } D;
typedef struct { int f0; char p[0xC]; int h10; int f14; int f18; int f1C; int f20; } C;
typedef struct { char p[0xC4]; C *c; char q[8]; D *d; } S;
typedef struct { char p[0x10]; int h10; } I;
typedef struct { char pad[4]; short h4; } G;
extern G D_09B6DAC0;
extern int func_088A751C(int);
extern void func_089234C0(int);
extern int func_08B39D30(S *);
extern void func_08B3A008(S *, int);
extern void func_08B3A1CC(S *, int);
extern int func_08B396E0(S *, int);
extern int func_08B45200(int);
extern void func_08B46584(int, int);
extern int func_08B468F8(int);
void func_08B3704C(S *self) {
    int r, v;
    if (self->d->f4 != 0) {
        if (func_088A751C(self->d->f4) != 0) {
            return;
        } else {
            self->d->f4 = 0;
        }
    }
    v = func_08B45200(D_09B6DAC0.h4);
    func_08B46584(self->c->h10, v);
    r = func_08B468F8(self->c->h10);
    if (r == 0) {
        func_08B3A1CC(self, D_09B6DAC0.h4);
        self->c->f0 = 6;
        if (func_08B39D30(self) == 0) {
            func_08B3A008(self, 1);
            if (func_08B396E0(self, 0) != 0) {
                self->c->f0 = 0x23;
            }
        }
    } else {
        func_089234C0(r);
        self->c->f20 = r;
        self->c->f1C = 2;
        self->c->f0 = 0x18;
    }
}
