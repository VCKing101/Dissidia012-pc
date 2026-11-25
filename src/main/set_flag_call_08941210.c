/* Stores (flag == 1) at 0x40 of the object at 0x14, then calls func_0897371C(obj, 1, 1, 1, 1). */
typedef struct { char pad[0x40]; int w40; } In08941210;
typedef struct { char pad[0x14]; In08941210 *in; } Obj08941210;
extern void func_0897371C(In08941210 *, int, int, int, int);
void func_08941210(Obj08941210 *self, unsigned char flag) {
    int r = flag == 1;
    In08941210 *i = self->in;
    i->w40 = r;
    func_0897371C(self->in, 1, 1, 1, 1);
}
