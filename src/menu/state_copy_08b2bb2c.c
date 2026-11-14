/* Copies the saved selection block out, marks it valid and adds the current
   page. */
struct Block08B2BB2C {
    unsigned char field_0;
    char pad_1[3];
    int field_4;
    char data[0x12C - 8];
};

struct State08B2BB2C {
    char pad_000[0x740];
    int field_740;
    char pad_744[0x8DC - 0x744];
    struct Block08B2BB2C field_8DC;
};

struct Owner08B2BB2C {
    char pad_000[0xA18];
    struct State08B2BB2C *field_A18;
};

void func_08B2BB2C(struct Owner08B2BB2C *self, struct Block08B2BB2C *out)
{
    int page;
    *out = self->field_A18->field_8DC;
    page = self->field_A18->field_740;
    out->field_0 = 1;
    out->field_4 = page;
}
