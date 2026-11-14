typedef struct ModeOwner {
    char pad_00[0x28];
    int *field_28;
} ModeOwner;

extern void func_08A814D4(ModeOwner *self, unsigned char on, int value)
{
    if (*self->field_28 != 5) {
        return;
    }
    if (on == 1) {
        func_08A81560(self, value, 0);
        func_08A7F96C(self, 0xE, 1, 0);
    } else {
        func_08A7F96C(self, 0xE, 0, 0);
    }
}
