/* Runs the common update, then the handler of steps 1-4. */
typedef struct StepOwner {
    char pad_00[0x2C];
    int field_2C;
} StepOwner;

extern void func_08A5A228(StepOwner *self);
extern void func_08A5A2C4(StepOwner *self);
extern void func_08A5AC08(StepOwner *self);
extern void func_08A5A31C(StepOwner *self);
extern void func_08A5A35C(StepOwner *self);

void func_08A5A178(StepOwner *self)
{
    switch (self->field_2C) {
    case 1:
        func_08A5A228(self);
        func_08A5A2C4(self);
        break;
    case 2:
        func_08A5A228(self);
        func_08A5AC08(self);
        break;
    case 3:
        func_08A5A228(self);
        func_08A5A31C(self);
        break;
    case 4:
        func_08A5A228(self);
        func_08A5A35C(self);
        break;
    }
}
