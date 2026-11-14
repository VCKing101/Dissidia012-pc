/* Copies the three values of each of the five slots into the work block and
   refreshes each slot's parts. */
struct SlotValues08AAB0B8 {
    char pad_000[0xF0];
    int field_0F0[5];
    int field_104[5];
    int field_118[5];
};

struct SlotRow08AAB0B8 {
    signed char field_0;
    char pad_1[3];
    signed char field_4;
    signed char field_5;
    char pad_6[0x16DC - 6];
};

struct SlotState08AAB0B8 {
    char pad_000[0xA58];
    struct SlotRow08AAB0B8 field_A58[5];
};

struct SlotPage08AAB0B8 {
    char pad_0000[0x7CFC];
    struct SlotState08AAB0B8 *field_7CFC;
};

extern struct SlotValues08AAB0B8 *func_08AABC54(void);
extern void func_08AB1748(struct SlotRow08AAB0B8 *row);
extern void func_08AB16A8(struct SlotRow08AAB0B8 *row);
extern void func_08AB1610(struct SlotRow08AAB0B8 *row);

void func_08AAB0B8(struct SlotPage08AAB0B8 *self)
{
    struct SlotValues08AAB0B8 *values = func_08AABC54();
    int i;
    for (i = 0; i < 5; i++) {
        struct SlotRow08AAB0B8 *row = &self->field_7CFC->field_A58[i];
        int first[1];
        int third[1];
        int second[2];
        first[0] = row->field_0;
        third[0] = row->field_5;
        second[0] = row->field_4;
        values->field_0F0[i] = first[0];
        values->field_118[i] = third[0];
        values->field_104[i] = second[0];
        func_08AB1748(row);
        func_08AB16A8(row);
        func_08AB1610(row);
    }
}
