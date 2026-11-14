typedef struct Item08813574 {
    unsigned char data[0x40];
} Item08813574;

typedef struct Pool08813574 {
    Item08813574 items[4];
    int free[4];
    int count;
} Pool08813574;

extern Pool08813574 D_089D47B0;

void func_08993560(void *array, int count, int size, void (*ctor)(void *));
void func_08813004(void *item);

void func_08813574(void) {
    Pool08813574 *pool = &D_089D47B0;
    int i;
    func_08993560(pool->items, 4, 0x40, func_08813004);
    for (i = 0; i < 4; i++) {
        pool->free[i] = i;
    }
    pool->count = 4;
}
