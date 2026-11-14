/* Builds the cursor's link list for the current layout and applies it. */
typedef struct PairPanel {
    char pad_000[0xD5C];
    char field_D5C[0x4];
    int field_D60;
    char pad_D64[0xE34 - 0xD64];
    int field_E34[3];
    int field_E40;
    unsigned char field_E44;
} PairPanel;

extern void func_088D4788(void *cursor, int *links, int count);

void func_08A62A3C(PairPanel *self)
{
    int count = 0;
    if (self->field_E44 != 0) {
        switch (self->field_E40) {
        case 1:
            break;
        case 2:
            self->field_D60 = 0;
            self->field_E34[count++] = 1;
            self->field_E34[count++] = 2;
            break;
        case 3:
            self->field_D60 = 2;
            self->field_E34[count++] = 0;
            self->field_E34[count++] = 1;
            break;
        case 4:
            self->field_D60 = 1;
            self->field_E34[count++] = 0;
            self->field_E34[count++] = 2;
            break;
        }
    }
    func_088D4788(self->field_D5C, self->field_E34, count);
}
