// Checks every slot in a range chosen by func_08B0F188; returns 1 when func_08B0F3E0 accepts them all.
struct Kind08B0ED6C { int pad; short kind; };
struct Slot08B0ED6C { Kind08B0ED6C *k; int pad; };
struct Set08B0ED6C { Slot08B0ED6C slots[8]; int pad; int items[1]; };
extern "C" {
extern void func_08B0F188(void *self, int *lo, int *hi, int kind);
extern int func_08B0F3E0(int slot);
int func_08B0ED6C(Set08B0ED6C *self, int idx)
{
    int r[2];
    short *kp = &self->slots[idx].k->kind;
    func_08B0F188(self, &r[0], &r[1], *kp);
    for (int i = r[0]; i < r[1]; i++) {
        if (func_08B0F3E0(self->items[i]) == 0) {
            return 0;
        }
    }
    return 1;
}
}
