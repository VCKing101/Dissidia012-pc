// Runs the per-state setup pair chosen by the state at +0x28 (states 1..3).
struct S { char pad[0x28]; int state; };
extern void func_0892F93C(struct S *);
extern void func_0892F8C8(struct S *);
extern void func_0892F9A0(struct S *);
extern void func_0892FB14(struct S *);
void func_0892F838(struct S *self) {
    switch (self->state) {
    case 1:
        func_0892F93C(self);
        func_0892F8C8(self);
        break;
    case 2:
        func_0892F9A0(self);
        func_0892F8C8(self);
        break;
    case 3:
        func_0892F8C8(self);
        func_0892FB14(self);
        break;
    }
}
