/* Enters the selected mode (0-5); mode 5 also opens its page. False for an invalid mode. */
typedef struct EnterPanel {
    char pad_000[0x28];
    int field_28;
    char pad_02C[0x1E8 - 0x2C];
    unsigned int field_1E8;
} EnterPanel;

extern void func_08A6B8B0(EnterPanel *self);
extern void func_08A6C2F8(EnterPanel *self, unsigned int mode);
extern void func_08A6C3A0(EnterPanel *self);

int func_08A6C1C0(EnterPanel *self)
{
    unsigned int mode = self->field_1E8;
    switch (mode) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        func_08A6B8B0(self);
        func_08A6C2F8(self, mode);
        return 1;
    case 5:
        func_08A6B8B0(self);
        func_08A6C3A0(self);
        self->field_28 = 3;
        return 1;
    }
    return 0;
}
