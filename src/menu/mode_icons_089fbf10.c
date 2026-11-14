/* Sets the mode icons unless the mode is 0 or 3; mode 2 also sets a third icon. */
typedef struct ModeState {
    char pad_0000[0x38A0];
    int field_38A0;
    char pad_38A4[0x38AC - 0x38A4];
    void *field_38AC;
    void *field_38B0;
    char pad_38B4[0x38C8 - 0x38B4];
    void *field_38C8;
} ModeState;

typedef struct ModeOwner {
    char pad_000[0x14C];
    ModeState *field_14C;
} ModeOwner;

extern void func_088D9F5C(void *icon, int frame, int one);

void func_089FBF10(ModeOwner *self)
{
    int mode = self->field_14C->field_38A0;
    if (mode != 0) {
        if (mode != 3) {
            func_088D9F5C(self->field_14C->field_38AC, 0x1E, 1);
            func_088D9F5C(self->field_14C->field_38C8, 0x22, 1);
            if (self->field_14C->field_38A0 == 2) {
                func_088D9F5C(self->field_14C->field_38B0, 3, 1);
            }
        }
    }
}
