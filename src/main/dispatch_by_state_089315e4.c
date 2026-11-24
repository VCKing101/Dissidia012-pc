// Runs the per-state step chosen by the state at +0x28 (1..3); state 2 falls into the pair state 1 runs.
struct S { char pad[0x28]; int state; };
extern void func_08931844(struct S *);
extern void func_089316E0(struct S *);
extern void func_08931664(struct S *);
extern void func_0893168C(struct S *);
void func_089315E4(struct S *self) {
    switch (self->state) {
    case 2:
        func_089316E0(self);
    case 1:
        func_08931664(self);
        func_0893168C(self);
        break;
    case 3:
        func_08931844(self);
        break;
    }
}
