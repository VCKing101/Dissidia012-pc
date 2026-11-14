/* Fills the message's five values from the list (0 past its count). */
struct Item08B32164 {
    int field_0;
    int field_4;
};

struct List08B32164 {
    short field_0;
    short field_2;
    char pad_4[0x20 - 4];
    struct Item08B32164 field_20[5];
};

struct State08B32164 {
    char pad_000[0x528];
    struct List08B32164 field_528;
};

struct Owner08B32164 {
    char pad_000[0x138];
    struct State08B32164 *field_138;
};

struct Message08B32164 {
    int field_0;
    int field_4[5];
};

extern void *D_08B01CF8;
extern struct Message08B32164 *func_08AF34A0(void *queue);

void func_08B32164(struct Owner08B32164 *self)
{
    struct Message08B32164 *message = func_08AF34A0(D_08B01CF8);
    struct List08B32164 *list = &self->field_138->field_528;
    int i;
    for (i = 0; i < 5; i++) {
        message->field_4[i] = (i < list->field_2) ? list->field_20[i].field_0 : 0;
    }
}
