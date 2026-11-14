typedef struct ToggleState {
    char pad_000[0x460];
    int field_460;
    int field_464;
    char pad_468[0x884 - 0x468];
    int field_884;
} ToggleState;

typedef struct ToggleOwner {
    char pad_000[0x138];
    ToggleState *field_138;
} ToggleOwner;

extern void func_088D9F5C(int target, int kind, int value);

void func_08AB89A8(ToggleOwner *self)
{
    int kind = 0x25;
    int on = 0;
    if (self->field_138->field_884 != 0) {
        kind = 0x24;
        on = 1;
    }
    func_088D9F5C(self->field_138->field_460, kind, 1);
    func_088D9F5C(self->field_138->field_464, on, 1);
}
