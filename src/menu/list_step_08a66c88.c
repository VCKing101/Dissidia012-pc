typedef struct StepList {
    char pad_00[0x28];
    int field_28;
} StepList;

extern void func_08A67844(StepList *self);
extern void func_08A66CE8(StepList *self);

void func_08A66C88(StepList *self)
{
    switch (self->field_28) {
    case 1:
        func_08A67844(self);
        func_08A66CE8(self);
        break;
    case 2:
        func_08A66CE8(self);
        break;
    }
}
