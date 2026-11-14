typedef struct SlotTable7EB98 {
    unsigned char active;
    char pad_001[0xF];
    int field_10;
    char pad_014[0x4];
    int field_18;
    char pad_01C[0x70];
    unsigned char field_8C;
} SlotTable7EB98;

extern void func_08A7F564(SlotTable7EB98 *self);
extern void func_08A7F674(SlotTable7EB98 *self);
extern void func_08A7F870(SlotTable7EB98 *self);
extern void func_08A7FBF8(SlotTable7EB98 *self);
extern void func_08A7FAB4(SlotTable7EB98 *self);

int func_08A7EB98(SlotTable7EB98 *self)
{
    if (!self->active) {
        return -1;
    }
    func_08A7F564(self);
    func_08A7F674(self);
    func_08A7F870(self);
    func_08A7FBF8(self);
    func_08A7FAB4(self);
    return 0;
}

int func_08A7EBF4(SlotTable7EB98 *self, unsigned char value)
{
    if (!self->active) {
        return -1;
    }
    self->field_8C = value;
    return 0;
}

int func_08A7EC18(SlotTable7EB98 *self)
{
    if (!self->active) {
        return -1;
    }
    return self->field_10;
}

int func_08A7EC38(SlotTable7EB98 *self)
{
    if (!self->active) {
        return -1;
    }
    return self->field_18;
}
