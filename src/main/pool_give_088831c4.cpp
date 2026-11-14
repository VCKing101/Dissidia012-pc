struct Pool088831C4 {
    unsigned char data[0x1D00];
    void *free[64];
    int count;
};

extern "C" Pool088831C4 *D_089D69C0;

struct Owns088831C4 {
    static unsigned char owns(Pool088831C4 *pool, void *item) {
        int inside = 0;
        if ((unsigned char *)item >= pool->data && (unsigned char *)item < pool->data + 0x1D00) {
            inside = 1;
        }
        return inside;
    }
};

extern "C" {
void func_088831C4(void *item) {
    Pool088831C4 *pool = D_089D69C0;
    if (Owns088831C4::owns(pool, item) == 1) {
        int *count = &pool->count;
        pool->free[(*count)++] = item;
    }
}
}
