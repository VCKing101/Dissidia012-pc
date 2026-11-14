typedef struct ToggleState {
    char pad_000[0x448];
    int field_448;
    int field_44C;
    char pad_450[0x8B0 - 0x450];
    int field_8B0;
} ToggleState;

typedef struct ToggleOwner {
    char pad_000[0x14C];
    ToggleState *field_14C;
} ToggleOwner;

extern void func_088D9F5C(int target, int kind, int value);

void func_08ABCB48(ToggleOwner *self)
{
    int kind = 0x3;
    int on = 0;
    if (self->field_14C->field_8B0 != 0) {
        kind = 0x2;
        on = 1;
    }
    func_088D9F5C(self->field_14C->field_448, kind, 1);
    func_088D9F5C(self->field_14C->field_44C, on, 1);
}
