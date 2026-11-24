// On event -5 notifies the owner at +0x18 and runs the follow-up step; events -6 and -1 do nothing.
struct S { char pad[0x18]; int owner; };
extern void func_08971098(int);
extern void func_08946F88(struct S *);
void func_08946F90(struct S *self, int unused, int ev) {
    switch (ev) {
    case -6:
        break;
    case -5:
        func_08971098(self->owner);
        func_08946F88(self);
        break;
    case -1:
        break;
    default:
        break;
    }
}
