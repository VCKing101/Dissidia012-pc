typedef struct Slot { char pad_00[0x14]; float value; char pad_18[8]; } Slot;
typedef struct Bank { char pad_00[0x10]; Slot slots[30]; int field_3D0; int field_3D4; } Bank;
void func_08859D6C(Bank *self)
{
    int i;
    for (i = 0; i < 30; i++) {
        self->slots[i].value = 0.0f;
    }
    self->field_3D4 = 0;
    self->field_3D0 = 0;
}
