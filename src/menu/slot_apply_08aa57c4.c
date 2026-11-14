/* Copies the chosen slot's three values into the work block, refreshes the
   model and its parts, then closes the picker. */
struct ApplySlot08AA57C4 {
    char pad_000[0xF0];
    int field_0F0;
    char pad_0F4[0x104 - 0xF4];
    int field_104;
    char pad_108[0x118 - 0x108];
    int field_118;
};

struct ApplyRow08AA57C4 {
    char data[0x154];
};

struct ApplyState08AA57C4 {
    char pad_0000[0x710];
    signed char field_710;
    char pad_0711[3];
    signed char field_714;
    signed char field_715;
    char pad_0716[0x750 - 0x716];
    struct ApplyRow08AA57C4 field_750[17];
    char pad_end[0x1E74 - 0x750 - 17 * 0x154];
    void *field_1E74;
};

struct ApplyPage08AA57C4 {
    char pad_0000[0x1EE4];
    struct ApplyState08AA57C4 *field_1EE4;
};

extern int func_08AB48F4(void *model);
extern struct ApplySlot08AA57C4 *func_08AA58C4(void);
extern void func_08AB4C30(void *model, int part, int a, int b, int c);
extern void func_08AB1748(void *values);
extern void func_08AB16A8(void *values);
extern void func_08AB1610(void *values);
extern void func_08AB6B90(void *model, int part, struct ApplyRow08AA57C4 *row);
extern void func_08AA4744(struct ApplyPage08AA57C4 *self);

void func_08AA57C4(struct ApplyPage08AA57C4 *self)
{
    int busy = func_08AB48F4(self->field_1EE4->field_1E74);
    struct ApplySlot08AA57C4 *slot = func_08AA58C4();
    if (busy == 0) {
        int first[1];
        int third[1];
        int second[2];
        first[0] = self->field_1EE4->field_710;
        third[0] = self->field_1EE4->field_715;
        second[0] = self->field_1EE4->field_714;
        slot->field_0F0 = first[0];
        slot->field_118 = third[0];
        slot->field_104 = second[0];
        func_08AB4C30(self->field_1EE4->field_1E74, 0, first[0], second[0], third[0]);
        func_08AB1748(&self->field_1EE4->field_710);
        func_08AB16A8(&self->field_1EE4->field_710);
        func_08AB1610(&self->field_1EE4->field_710);
        if (slot->field_118 > 0) {
            func_08AB6B90(self->field_1EE4->field_1E74, 0, &self->field_1EE4->field_750[slot->field_118]);
        }
        func_08AA4744(self);
    }
}
