/* When the current item has flag 0x20, draws the cursor layer and the frame;
   returns whether it did. */
struct Item08B09154 {
    char pad_00[0x20];
    unsigned short field_20;
};

struct State08B09154 {
    char pad_000[4];
    char field_004[0x400];
    char field_404[0x6E4 - 0x404];
    Item08B09154 *field_6E4;
    char pad_6E8[0x884 - 0x6E8];
    char field_884[4];
};

struct Owner08B09154 {
    char pad_000[0x138];
    State08B09154 *field_138;
};

extern "C" {
extern unsigned short D_08B1C420[];
extern unsigned short D_08B1C3E8[];
void func_088D442C(char *layout, char *cursor, unsigned short *table);
void func_088D3EA0(char *owner, char *layout, unsigned short *table);
}

struct Flags08B09154 {
    static bool has(unsigned int flags, unsigned int mask) { return (flags & mask) != 0; }
};

extern "C" {
unsigned char func_08B09154(Owner08B09154 *self)
{
    unsigned char drawn = 0;
    if (Flags08B09154::has(self->field_138->field_6E4->field_20, 0x20)) {
        func_088D442C(self->field_138->field_404, self->field_138->field_884, D_08B1C420);
        func_088D3EA0(self->field_138->field_004, self->field_138->field_404, D_08B1C3E8);
        drawn = 1;
    }
    return drawn;
}
}
