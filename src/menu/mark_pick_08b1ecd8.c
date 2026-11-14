/* Clears the mark of the item in whichever list the page shows. */
struct Item08B1ECD8 {
    char pad_0[6];
    short field_6;
    int field_8;
};

struct State08B1ECD8 {
    char pad_0000[0x80F8];
    int field_80F8;
};

struct Owner08B1ECD8 {
    char pad_000[0x138];
    struct State08B1ECD8 *field_138;
};

extern void func_08B1EBF8(struct Owner08B1ECD8 *self, short id);
extern void func_08B1EC68(struct Owner08B1ECD8 *self, short id);

void func_08B1ECD8(struct Owner08B1ECD8 *self, struct Item08B1ECD8 *item)
{
    if (self->field_138->field_80F8 == 0) {
        func_08B1EBF8(self, item->field_6);
    } else {
        func_08B1EC68(self, item->field_8);
    }
}
