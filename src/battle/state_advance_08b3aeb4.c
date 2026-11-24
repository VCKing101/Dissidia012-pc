/* Marks the object busy and runs func_08B3F034; on 1, records the previous state and switches to state 3 unless already in 2. */
struct Obj { char pad[0x108]; int state; char pad1[4]; int prev; char pad2[5]; unsigned char busy; char pad3[0x66]; char sub[1]; };
extern int func_08B3F034(void *, struct Obj *, int, int, int);
extern void func_08B3B3EC(struct Obj *, int, int);
int func_08B3AEB4(struct Obj *self, int a, int b, int c)
{
    int one = 1;
    self->busy = one;
    if (func_08B3F034(self->sub, self, a, b, c) == 1) {
        int st = self->state;
        if (st == 2) {
            return one;
        }
        if (st != 3) {
            self->prev = st;
        }
        func_08B3B3EC(self, 3, 0);
        return one;
    }
    return 0;
}
