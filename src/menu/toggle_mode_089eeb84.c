/* Flips the panel between its two modes and switches both icons to match. */
typedef struct ModePanel {
    int field_00;
    void *field_04;
    void *field_08;
    char pad_0C[0x1C - 0xC];
    int field_1C;
} ModePanel;

extern void func_088D9F5C(void *icon, int frame, int one);

void func_089EEB84(ModePanel *self)
{
    self->field_1C = self->field_1C == 0;
    switch (self->field_1C) {
    case 0:
        func_088D9F5C(self->field_04, 0x25, 1);
        func_088D9F5C(self->field_08, 0, 1);
        break;
    case 1:
        func_088D9F5C(self->field_04, 0x24, 1);
        func_088D9F5C(self->field_08, 1, 1);
        break;
    }
}
