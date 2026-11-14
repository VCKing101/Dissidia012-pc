/* Waits two frames, then plays the sound for the panel's mode and clears its state. */
typedef struct WaitPanel {
    char pad_00[0x18];
    int field_18;
    char pad_1C[0x28 - 0x1C];
    int field_28;
    int field_2C;
} WaitPanel;

extern void *D_08A4FFF4;
extern void func_08A4D824(void *sound, int kind);

void func_08A5F5F4(WaitPanel *self)
{
    if (self->field_18 < 2) {
        self->field_18 = self->field_18 + 1;
        return;
    }
    {
        switch (self->field_2C) {
        case 0:
            func_08A4D824(D_08A4FFF4, 0);
            break;
        case 1:
            func_08A4D824(D_08A4FFF4, 1);
            break;
        }
        self->field_28 = 0;
    }
}
