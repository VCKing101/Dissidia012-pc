typedef struct Item08813180 {
    unsigned char data[0x40];
} Item08813180;

typedef struct Pool08813180 {
    Item08813180 items[4];
    int free[4];
    int count;
} Pool08813180;

extern Pool08813180 D_089D47B0;

void func_08813180(Item08813180 *item) {
    int index = item - D_089D47B0.items;
    D_089D47B0.free[D_089D47B0.count++] = index;
}
