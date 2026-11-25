typedef struct O { char pad[0x14]; char sub[0x24]; int h38; int h3c; int h40; } O;
extern void func_08895FE4(O *o);
extern void func_088A2664(int a, float f);
extern void func_088963B8(O *o);
extern void func_0889F830(void *a, int b);
void func_0889661C(O *o) {
    func_08895FE4(o);
    func_088A2664(0x18703, 0.0f);
    func_088A2664(0x18704, 0.0f);
    func_088963B8(o);
    if (o->h3c != 0) {
        func_088A2664(o->h3c, 0.0f);
        o->h3c = 0;
    }
    if (o->h40 != 0) {
        func_088A2664(o->h40, 0.0f);
        o->h40 = 0;
    }
    if (o->h38 != 0) {
        func_0889F830(o->sub, 0);
    }
}
