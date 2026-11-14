typedef struct RefreshPanel {
    char pad_000[0xE8];
    int field_E8[6];
    char pad_100[0x10F4 - 0x100];
    int field_10F4;
} RefreshPanel;

extern void func_08A55070(int handle);
extern void func_08A57EB8(RefreshPanel *self);
extern void func_08A550A0(int handle);
extern void func_08A57EF0(RefreshPanel *self);

void func_08A57F28(RefreshPanel *self)
{
    int i;
    for (i = 0; i < 6; i++) {
        func_08A55070(self->field_E8[i]);
    }
    func_08A57EB8(self);
    if (self->field_10F4 >= 0 && self->field_10F4 < 6) {
        func_08A550A0(self->field_E8[self->field_10F4]);
    } else if (self->field_10F4 == 6) {
        func_08A57EF0(self);
    }
}
