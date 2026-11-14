/* Returns a voice to the pool when it belongs to it. */
struct Pool088A2104 {
    char storage[0x1700];
    void *items[0x40];
    int count;

    static unsigned char owns(Pool088A2104 *pool, void *p)
    {
        int inside = 0;
        if ((char *)p >= (char *)pool && (char *)p < (char *)pool->items) {
            inside = 1;
        }
        return inside;
    }
};

extern "C" {

extern Pool088A2104 *D_089D6C90;

void func_088A2104(void *p)
{
    Pool088A2104 *pool = D_089D6C90;
    if (Pool088A2104::owns(pool, p) == 1) {
        int *count = &pool->count;
        pool->items[(*count)++] = p;
    }
}

}
