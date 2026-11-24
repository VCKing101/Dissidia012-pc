// On first call runs the pending-start hook and sets the started flag, then polls the object at +0x1C; -1 when it reports 0.
struct S { char pad[0x1C]; int obj; char pad2[0x20]; int hook; unsigned char started; };
extern void func_089411D8(struct S *, int);
extern int func_08943124(int, int);
int func_08941888(struct S *self, int x) {
    int r;
    if (self->started == 0) {
        if (self->hook != 0) {
            func_089411D8(self, self->hook);
        }
        self->started = 1;
    }
    r = func_08943124(self->obj, x);
    if (r == 0) {
        return -1;
    }
    return r;
}
