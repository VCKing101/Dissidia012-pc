typedef struct ToggleState {
    char pad_000[0x4F8];
    int field_4F8;
    int field_4FC;
    char pad_500[0xBDC - 0x500];
    int field_BDC;
} ToggleState;

typedef struct ToggleOwner {
    char pad_000[0x138];
    ToggleState *field_138;
} ToggleOwner;

extern void func_088D9F5C(int target, int kind, int value);

void func_08A2B328(ToggleOwner *self)
{
    int kind = 0x25;
    int on = 0;
    if (self->field_138->field_BDC != 0) {
        kind = 0x24;
        on = 1;
    }
    func_088D9F5C(self->field_138->field_4F8, kind, 1);
    func_088D9F5C(self->field_138->field_4FC, on, 1);
}
