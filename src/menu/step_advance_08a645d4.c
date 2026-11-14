typedef struct StepPanel {
    char pad_00[0x10];
    int field_10;
} StepPanel;

extern void func_08A647BC(StepPanel *self);
extern void func_08A6480C(StepPanel *self);

void func_08A645D4(StepPanel *self)
{
    switch (self->field_10) {
    case 1:
        func_08A647BC(self);
        break;
    case 2:
        func_08A647BC(self);
        func_08A6480C(self);
        break;
    }
}
