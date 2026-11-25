extern int func_088DEC0C(void);
extern int func_088DF7EC(int);
extern void func_088DA060(int);
extern unsigned func_088DF2F8(int);
extern int func_088DF860(int);
extern int func_088DF8B4(int);
extern void func_088DA0A8(int, int, void *, void *);
extern void func_088D9B84(int, void *);
extern void func_088D9AF4(int, void *);
extern int D_089CC2B8[];
extern int D_089CC2C0[];
extern int D_089CC2C8[];

typedef struct O98 { int f0; int f4; int f8; int fC; int f10; } O98;

int func_088DEA98(O98 *self)
{
    int h;
    unsigned idx;
    int k;
    int v;
    char buf[64];
    char tail[4];
    h = func_088DEC0C();
    if (h == 0) {
        return -1;
    } else {
        if (self->f4 != 0) {
            h = func_088DF7EC(self->f10);
            if (h != 0) {
                int w;
                idx = func_088DF2F8(self->fC);
                if (6 < idx) {
                    return -2;
                }
                int one;
                k = func_088DF860(self->f10);
                one = 1;
                w = idx * 4;
                if (k == one) {
                    func_088DA0A8(h, *(int *)(w + (int)D_089CC2B8), buf, tail);
                    v = self->f4;
                } else {
                    k = func_088DF8B4(self->f10);
                    if (k == one) {
                        func_088DA0A8(h, *(int *)(w + (int)D_089CC2C0), buf, tail);
                        v = self->f4;
                    } else {
                        func_088DA0A8(h, *(int *)(w + (int)D_089CC2C8), buf, tail);
                        v = self->f4;
                    }
                }
                func_088D9B84(v, buf);
                func_088D9AF4(self->f4, tail);
            } else {
                v = self->f4;
                if (v != 0) {
                    func_088DA060(v);
                    self->f4 = 0;
                }
            }
        }
        return 0;
    }
}
