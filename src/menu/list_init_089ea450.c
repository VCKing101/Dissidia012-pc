typedef struct MenuListState {
    char pad_00[0x8];
    char field_08[0xC];
    char field_14[0x14];
    char field_28[0x8];
    char field_30[0xC];
    int field_3C;
    int field_40;
    int field_44;
} MenuListState;

extern void func_0887B1E4(MenuListState *self);
extern void func_08935828(void *dst, int value, int size);

MenuListState *func_089EA450(MenuListState *self, int owner) {
    func_0887B1E4(self);
    self->field_3C = 0;
    self->field_40 = 0;
    self->field_44 = owner;
    func_08935828(self->field_08, 0, 0xC);
    func_08935828(self->field_14, 0, 0x14);
    func_08935828(self->field_28, 0, 0x8);
    func_08935828(self->field_30, 0, 0xC);
    return self;
}
