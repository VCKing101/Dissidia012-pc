/* Menu page: when neither mode is blocked, loads a scene and stores two sizes from the loader. */
typedef struct { char p[0xC0C]; int fC0C; int fC10; } B;
typedef struct { char p[0xC18]; B *b; } S;
typedef struct { char p[0x134]; int h134; } G;
extern int D_08ACA508;
extern G *D_08AC7500;
extern int func_088F175C(int);
extern int func_08862C40(int, int);
extern int func_0885C6CC(int, int, int, int, int, int);
extern void func_08862854(int, int, int, int);
extern void func_0885C538(int, int, int);
extern void func_088CAF64(void *);
extern void func_088CAF74(void *, int, int);
extern void func_088CAF90(void *);
extern void func_088CB00C(void *);
extern void func_088CB054(void *);
extern int func_088F15A8(int);
void func_08AC86C0(S *self) {
    int obj[2];
    int zero;
    int s2, s4;
    self->b->fC0C = 0;
    self->b->fC10 = 0;
    if (func_088F175C(0) != 0) {
        return;
    } else {
        if (func_088F175C(1) != 0) {
            return;
        }
        s2 = func_08862C40(D_08ACA508, 1);
        zero = 0;
        s4 = func_0885C6CC(D_08AC7500->h134, s2, 0, 0, zero, 0);
        func_08862854(D_08ACA508, s4, s2, 1);
        func_088CAF64(obj);
        func_088CAF74(obj, s4, s2);
        func_088CB00C(obj);
        self->b->fC0C = func_088F15A8(0);
        self->b->fC10 = func_088F15A8(1);
        func_088CB054(obj);
        func_088CAF90(obj);
        func_0885C538(D_08AC7500->h134, s4, 0);
    }
}
