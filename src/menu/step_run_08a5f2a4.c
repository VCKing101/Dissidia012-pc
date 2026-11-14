/* Runs the handler of steps 1-5 (all but step 5 after the common update). */
typedef struct StepOwner {
    char pad_00[0x28];
    int field_28;
} StepOwner;

extern void func_08A5F364(StepOwner *self);
extern void func_08A5F4C8(StepOwner *self);
extern void func_08A5F3B8(StepOwner *self);
extern void func_08A5F528(StepOwner *self);
extern void func_08A5F574(StepOwner *self);
extern void func_08A5F5F4(StepOwner *self);

void func_08A5F2A4(StepOwner *self)
{
    switch (self->field_28) {
    case 1:
        func_08A5F364(self);
        func_08A5F4C8(self);
        break;
    case 2:
        func_08A5F364(self);
        func_08A5F3B8(self);
        break;
    case 3:
        func_08A5F364(self);
        func_08A5F528(self);
        break;
    case 4:
        func_08A5F364(self);
        func_08A5F574(self);
        break;
    case 5:
        func_08A5F5F4(self);
        break;
    }
}
