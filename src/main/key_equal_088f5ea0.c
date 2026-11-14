/* True when both keys have the same id and the same 6 data bytes. */
typedef struct NetKey {
    int id;
    unsigned char data[6];
} NetKey;

extern int func_08935490(const void *a, const void *b, int size);

int func_088F5EA0(NetKey *a, NetKey *b)
{
    if (a->id != b->id) {
        return 0;
    }
    if (func_08935490(a->data, b->data, 6) != 0) {
        return 0;
    }
    return 1;
}
