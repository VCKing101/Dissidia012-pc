typedef struct ToggleShow {
    char field_00[4];
    char field_04[0x1C];
    int field_20;
} ToggleShow;

extern void func_088D2F68(char *target, int kind);

void func_08A72B88(ToggleShow *self)
{
    self->field_20 = self->field_20 == 0;
    switch (self->field_20) {
    case 0:
        func_088D2F68(self->field_00, 0x25);
        func_088D2F68(self->field_04, 0);
        break;
    case 1:
        func_088D2F68(self->field_00, 0x24);
        func_088D2F68(self->field_04, 1);
        break;
    }
}
