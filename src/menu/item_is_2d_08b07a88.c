/* True when the current item's kind is 0x2D. */
struct Item08B07A88 {
    char pad_0[0xC];
    short field_C;
};

struct State08B07A88 {
    char pad_000[0x490];
    struct Item08B07A88 *field_490;
};

struct Owner08B07A88 {
    char pad_000[0x138];
    struct State08B07A88 *field_138;
};

unsigned char func_08B07A88(struct Owner08B07A88 *self)
{
    if (self->field_138->field_490->field_C == 0x2D) {
        return 1;
    }
    return 0;
}
