/* Runs the step routine for the current state (1..3) followed by the shared refresh pair. */
extern void func_08B50AB8(void *self);
extern void func_08B50AFC(void *self);
extern void func_08B50B50(void *self);
extern void func_08B5087C(void *self);
extern void func_08B50BFC(void *self);
typedef struct Z509F4 { char pad[0xC]; short kind; } Z509F4;
typedef struct S509F4 { char pad[0x28]; int state; char pad2[4]; Z509F4 *z; } S509F4;
void func_08B509F4(S509F4 *self)
{
    switch (self->state) {
    case 1:
        func_08B50AB8(self);
        func_08B50AFC(self);
        break;
    case 2:
        func_08B50B50(self);
        if (self->z != 0 && self->z->kind != 2) {
            func_08B5087C(self);
        }
        func_08B50AB8(self);
        func_08B50AFC(self);
        break;
    case 3:
        func_08B50BFC(self);
        func_08B50AB8(self);
        func_08B50AFC(self);
        break;
    }
}
