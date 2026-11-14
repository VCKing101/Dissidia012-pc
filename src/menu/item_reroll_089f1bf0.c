/* Screen0B6A0: picks the entry of a result item (func_089F1A28, first
   with the bonus, then without when func_089FF65C refuses it) and
   computes its value. */
typedef struct Pick29 {
    char pad_00[0x10];
    char field_10[0x6];
    short entry;        /* +0x16 */
} Pick29;

typedef struct ResultItem29 {
    char pad_00[0x1];
    unsigned char kind;
    char pad_02[0xC];
    short field_0E;
    char pad_10[0x2];
    short entry;        /* +0x12 */
    short value;        /* +0x14 */
} ResultItem29;

typedef struct ItemWork29 {
    char pad_0000[0x3294];
    unsigned char bonus;
} ItemWork29;

typedef struct ItemScreen29 {
    char pad_000[0x14C];
    ItemWork29 *work;
} ItemScreen29;

extern Pick29 *func_089F1A28(ItemScreen29 *self, ResultItem29 *item, int id, int bonus);
extern int func_089FF65C(ItemScreen29 *self, ResultItem29 *item, char *pick);
extern int func_089F050C(ItemScreen29 *self, ResultItem29 *items, short entry, unsigned char bonus,
                         int index, int kind);

void func_089F1BF0(ItemScreen29 *self, ResultItem29 *item, short id)
{
    Pick29 *pick = func_089F1A28(self, item, id, 1);

    if (func_089FF65C(self, item, pick->field_10) == 0) {
        pick = func_089F1A28(self, item, id, 0);
        func_089FF65C(self, item, pick->field_10);
    }
    item->entry = pick->entry;
    item->value = func_089F050C(self, item, item->entry, self->work->bonus, 0, -1);
    item->kind = item->field_0E;
}
