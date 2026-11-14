/* Matches a (kind, id) key; id -1 matches any id. */
typedef struct KeyedA {
    char pad_00[0x66];
    unsigned short id;
    unsigned char kind;
} KeyedA;

int func_088DCDF8(KeyedA *item, unsigned char kind, int id)
{
    int result = 0;

    if (kind == item->kind) {
        if (id != -1) {
            if (item->id == id) {
                result = 1;
            }
        } else {
            result = 1;
        }
    }
    return result;
}
