/* Matches a (kind, id) key; id -1 matches any id. */
typedef struct KeyedB {
    char pad_00[8];
    unsigned short id;
    unsigned char kind;
} KeyedB;

int func_088DE1E8(KeyedB *item, unsigned char kind, int id)
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
