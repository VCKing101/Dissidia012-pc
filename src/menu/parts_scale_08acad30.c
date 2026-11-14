/* Resets the scale of both parts of every entry in the D_08ADA584 table
   (0xFFFF-terminated) to 1. */
struct PartPair08ACAD30 {
    int field_00;
    int field_04;
    char pad_08[0x24 - 8];
};

struct State08ACAD30 {
    char pad_000[0x400];
    void *field_400[1];
};

struct Screen08ACAD30 {
    char pad_0000[0xDB30];
    struct State08ACAD30 *field_DB30;
};

extern struct PartPair08ACAD30 D_08ADA584[];
extern void func_088D9C8C(void *part, float x, float y);

void func_08ACAD30(struct Screen08ACAD30 *self)
{
    struct PartPair08ACAD30 *pair = D_08ADA584;
    float one = 1.0f;
    for (;;) {
        if (pair->field_00 == 0xFFFF) {
            break;
        }
        func_088D9C8C(self->field_DB30->field_400[pair->field_00], one, one);
        func_088D9C8C(self->field_DB30->field_400[pair->field_04], one, one);
        pair++;
    }
}
