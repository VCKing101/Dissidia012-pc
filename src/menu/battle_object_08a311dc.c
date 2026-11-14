typedef struct MenuBattleObject {
    int field_00;
    char field_04[0x34];
    int field_38;
    char pad_3C[0x4];
    void *field_40;
    int field_44;
} MenuBattleObject;

extern int func_0885C6CC(int handle, int size, int a, int b, int c, int d);
extern void func_08A31180(MenuBattleObject *self);
extern void func_088650A4(void *part);

void func_08A311DC(MenuBattleObject *self, int handle, void *info, int index) {
    int one = 1;

    self->field_38 = handle;
    self->field_00 = func_0885C6CC(handle, 0x4000, 0, 0, one, 0);
    self->field_40 = info;
    self->field_44 = index;
    func_08A31180(self);
    func_088650A4(self->field_04);
}
