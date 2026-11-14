typedef struct Slot { char pad_00[0x30]; float value; char pad_34[0xC]; } Slot;
typedef struct Bank { char pad_00[0x10]; Slot slots[300]; int field_4B10; int field_4B14; } Bank;
void func_0885AF4C(Bank *self)
{
    int i;
    for (i = 0; i < 300; i++) {
        self->slots[i].value = 0.0f;
    }
    self->field_4B14 = 0;
    self->field_4B10 = 0;
}
