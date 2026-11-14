typedef struct Item { char data_00[0x16]; signed char kind; } Item;
typedef struct Book { char data_00[0x5A]; unsigned char slots[64]; } Book;
extern Item *func_088B1D9C(int id);

int func_088B7820(Book *b, int id)
{
    Item *item = func_088B1D9C(id);
    if (item != 0 && item->kind >= 2 && item->kind < 5) {
        int i;
        for (i = 0; i < 64; i++) {
            if (b->slots[i] == 0xFF) {
                b->slots[i] = id & 0xFF;
                return 1;
            }
        }
    }
    return 0;
}
