// Runs the per-state step chosen by the state at +0x28 (states 1..3).
struct S { char pad[0x28]; int state; };
extern void func_08930098(struct S *);
extern void func_089300EC(struct S *);
extern void func_08930128(struct S *);
void func_08930018(struct S *self) {
    switch (self->state) {
    case 1:
        func_08930098(self);
        break;
    case 2:
        func_08930098(self);
        func_089300EC(self);
        break;
    case 3:
        func_08930128(self);
        break;
    }
}
