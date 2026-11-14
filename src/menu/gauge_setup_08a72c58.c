/* Lays out the gauge parts around the base element; the marker style depends on the mode (0 or 1). */
typedef struct GaugeParts {
    char field_00[0x4];
    char field_04[0x4];
    char field_08[0x4];
    char field_0C[0x8];
    char field_14[0x4];
    char field_18[0x8];
    int field_20;
} GaugeParts;

extern void func_088D2FB8(char *part, char *base, int anchor, int mode);
extern void func_08A73C98(char *marker, char *base, int style);
extern void func_088D3450(char *part, char *base, int anchor);

void func_08A72C58(GaugeParts *self)
{
    char *base = self->field_08;
    func_088D2FB8(base, self->field_0C, 0, 3);
    switch (self->field_20) {
    case 0:
        func_08A73C98(self->field_18, base, 4);
        break;
    case 1:
        func_08A73C98(self->field_18, base, 3);
        break;
    }
    func_088D2FB8(self->field_00, base, 4, 3);
    func_088D2FB8(self->field_04, base, 3, 3);
    func_088D3450(self->field_14, base, 5);
}
