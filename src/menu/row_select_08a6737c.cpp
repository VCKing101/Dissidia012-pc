/* Deselects every visible row, then selects the current one. */
class SelectRow {
public:
    char pad_00[0xC];
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
};

struct SelectRows {
    char pad_000[0x50];
    int field_50;
    char pad_054[0x4];
    int field_58;
    char pad_05C[0x128 - 0x5C];
    SelectRow *field_128[16];
};

extern "C" {
void func_08A6737C(SelectRows *self)
{
    int current = self->field_50;
    int i;
    for (i = 0; i <= self->field_58; i++) {
        self->field_128[i]->v3();
    }
    self->field_128[current]->v2();
}
}
