/* True when the current list is loaded and the cell under the cursor has
   flag 0x800. */
struct Cell08B06AF4 {
    unsigned short field_0;
    unsigned short field_2;
    char pad_4[0x14 - 4];
};

struct List08B06AF4 {
    struct Cell08B06AF4 cells[800];
};

struct State08B06AF4 {
    char pad_00000[0x31BC];
    int field_31BC[(0x3264 - 0x31BC) / 4];
    struct List08B06AF4 field_3264[(0x1A964 - 0x3264) / 16000];
    char pad_lists[0x1A964 - 0x3264 - ((0x1A964 - 0x3264) / 16000) * 16000];
    unsigned short field_1A964[(0x1F8EC - 0x1A964) / 0x640][0x320];
    char pad_rows[0x1F8EC - 0x1A964 - ((0x1F8EC - 0x1A964) / 0x640) * 0x640];
    int field_1F8EC;
    char pad_1F8F0[0x1F918 - 0x1F8F0];
    int field_1F918;
};

struct Owner08B06AF4 {
    char pad_000[0x138];
    struct State08B06AF4 *field_138;
};

int func_08B06AF4(struct Owner08B06AF4 *self)
{
    int set = 0;
    if (self->field_138->field_31BC[self->field_138->field_1F8EC] != 0) {
        unsigned short index = self->field_138->field_1A964[self->field_138->field_1F8EC][self->field_138->field_1F918];
        struct Cell08B06AF4 *cell = &self->field_138->field_3264[self->field_138->field_1F8EC].cells[index];
        if ((cell->field_2 & 0x800) != 0) {
            set = 1;
        }
    }
    return set;
}
