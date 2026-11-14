typedef struct StepPanel {
    unsigned char field_00;
    char pad_01[0xF];
    int field_10;
    char pad_14[4];
    int field_18;
} StepPanel;

extern int D_08A4FFF4;
extern void func_088D9F5C(int target, int kind, int value);
extern void func_08A4DE14(int handle);

int func_08A64634(StepPanel *self)
{
    if (self->field_00 == 0) {
        return 0;
    }
    func_088D9F5C(self->field_18, 1, 1);
    func_08A4DE14(D_08A4FFF4);
    self->field_10 = 2;
    return 1;
}
