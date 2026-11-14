typedef struct PairPanel {
    char pad_000[0x5C];
    int field_5C;
    int field_60;
    char pad_064[0x170 - 0x64];
    unsigned char field_170;
} PairPanel;

extern void func_088D9CD8(int handle);
extern void func_088D9CFC(int handle);

void func_08A5ED84(PairPanel *self)
{
    if (self->field_170) {
        func_088D9CD8(self->field_5C);
        func_088D9CFC(self->field_60);
    } else {
        func_088D9CFC(self->field_5C);
        func_088D9CD8(self->field_60);
    }
}
