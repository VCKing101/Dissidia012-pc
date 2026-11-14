/* Waits two frames, then runs the callback for the panel's mode and clears its state. */
typedef struct CallPanel {
    char pad_00[0x18];
    int field_18;
    char pad_1C[0x2C - 0x1C];
    int field_2C;
    int field_30;
    char pad_34[0xD4 - 0x34];
    void (*field_D4)(void);
    void (*field_D8)(void);
    void (*field_DC)(void);
} CallPanel;

void func_08A688B4(CallPanel *self)
{
    if (self->field_18 < 2) {
        self->field_18 = self->field_18 + 1;
        return;
    }
    {
        switch (self->field_2C) {
        case 0:
            self->field_D4();
            break;
        case 1:
            self->field_D8();
            break;
        case 2:
            self->field_DC();
            break;
        }
        self->field_30 = 0;
    }
}
