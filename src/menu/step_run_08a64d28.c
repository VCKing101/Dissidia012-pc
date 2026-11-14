/* Runs the common update and the handler of steps 1-4. */
typedef struct StepOwner {
    char pad_00[0x10];
    int field_10;
} StepOwner;

extern void func_08A64FF4(StepOwner *self);
extern void func_08A65044(StepOwner *self);
extern void func_08A650CC(StepOwner *self);
extern void func_08A65128(StepOwner *self);

void func_08A64D28(StepOwner *self)
{
    switch (self->field_10) {
    case 1:
        func_08A64FF4(self);
        func_08A65044(self);
        break;
    case 2:
        func_08A64FF4(self);
        break;
    case 3:
        func_08A64FF4(self);
        func_08A650CC(self);
        break;
    case 4:
        func_08A64FF4(self);
        func_08A65128(self);
        break;
    }
}
