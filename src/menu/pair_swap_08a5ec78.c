typedef struct PairPanel {
    char pad_000[0x48];
    int field_48;
    int field_4C;
    char pad_050[0x170 - 0x50];
    unsigned char field_170;
} PairPanel;

extern void func_088D9CD8(int handle);
extern void func_088D9CFC(int handle);

void func_08A5EC78(PairPanel *self)
{
    if (self->field_170) {
        func_088D9CD8(self->field_48);
        func_088D9CFC(self->field_4C);
    } else {
        func_088D9CFC(self->field_48);
        func_088D9CD8(self->field_4C);
    }
}
