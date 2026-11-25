/* Per-frame update of two flag bytes: handles pending close (0x3889) and open request (0x3888). */
typedef struct MfObj {
    char pad0[0x3888];
    unsigned char f3888;
    unsigned char f3889;
} MfObj;
typedef struct MfMode { char pad0[0x4C]; int mode; } MfMode;
extern void *D_08B05258;
extern int D_089D7260;
extern unsigned char *D_089D7280;
extern MfMode *D_089D6A10;
extern int func_0888B014(void);
extern int func_088AF29C(int a);
extern int func_08B05008(void *p);
extern void func_08B050A8(void *p, int a);
extern void func_08B05164(void *p);
extern void func_0888AD50(void);
extern void func_08B081D4(MfObj *o, int a);

int func_08B07E28(MfObj *self)
{
    void *p = D_08B05258;
    int one = 1;
    if (self->f3889 == one) {
        if (func_0888B014() == one) {
            self->f3889 = 0;
            if (func_088AF29C(D_089D7260) == one) {
                if (func_08B05008(p) != 3) {
                    func_08B050A8(p, 3);
                }
            }
        }
    }
    if (self->f3888 == one && *D_089D7280 == 0) {
        func_0888AD50();
        self->f3888 = 0;
        self->f3889 = 1;
    }
    if (self->f3889 == 0) {
        if (self->f3888 == 0) {
            if (D_089D6A10->mode == 5) {
                func_08B081D4(self, 0x13);
            } else {
                func_08B081D4(self, 0x11);
            }
            if (func_08B05008(p) < 4u) {
                func_08B05164(p);
            }
        }
    }
    return 0;
}
