typedef struct ToggleState {
    char pad_0000[0x9390];
    char field_9390[4];
} ToggleState;

typedef struct ToggleOwner {
    char pad_000[0x154];
    ToggleState *field_154;
} ToggleOwner;

extern void func_088D4FE4(char *gauge);
extern void func_088D50DC(char *gauge);

void func_08A9C5A0(ToggleOwner *self, unsigned char on)
{
    if (on == 1) {
        func_088D4FE4(self->field_154->field_9390);
    } else {
        func_088D50DC(self->field_154->field_9390);
    }
}
