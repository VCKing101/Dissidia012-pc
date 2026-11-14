typedef struct ToggleState {
    char pad_000[0x470];
    int field_470;
    int field_474;
    char pad_478[0x884 - 0x478];
    int field_884;
} ToggleState;

typedef struct ToggleOwner {
    char pad_000[0x14C];
    ToggleState *field_14C;
} ToggleOwner;

extern void func_088D9F5C(int target, int kind, int value);

void func_08ABCCDC(ToggleOwner *self)
{
    int kind = 0x25;
    int on = 0;
    if (self->field_14C->field_884 != 0) {
        kind = 0x24;
        on = 1;
    }
    func_088D9F5C(self->field_14C->field_470, kind, 1);
    func_088D9F5C(self->field_14C->field_474, on, 1);
}
