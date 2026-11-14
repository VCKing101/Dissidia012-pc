typedef struct RefreshMenu {
    char pad_00[0x2C];
    unsigned int field_2C;
} RefreshMenu;

extern int func_08AB6DAC(RefreshMenu *self);
extern int func_08AB6D58(RefreshMenu *self);
extern void func_08A6FC94(int handle);
extern int func_08AB73A0(RefreshMenu *self, int kind);
extern void func_08AB58E0(RefreshMenu *self, int kind, int value);
extern void func_08AB5D58(RefreshMenu *self);

void func_08AB6970(RefreshMenu *self)
{
    int handle;
    if (func_08AB6DAC(self) == 1) {
        return;
    }
    handle = func_08AB6D58(self);
    if ((self->field_2C & 4) != 0) {
        int target = handle;
        func_08A6FC94(target);
    }
    func_08AB58E0(self, 5, func_08AB73A0(self, 5) | 8);
    func_08AB5D58(self);
}
