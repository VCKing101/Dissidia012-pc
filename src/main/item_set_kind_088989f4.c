/* Sets the kind of the item with the key: its index, or -1. */
typedef struct Item088989F4 {
    char pad_00[0x22];
    short kind;
    char pad_24[0x28];
} Item088989F4;

typedef struct Obj088989F4 {
    char pad_00[0x70];
    Item088989F4 *items;
} Obj088989F4;

int func_08898654(Obj088989F4 *o, int key);

int func_088989F4(Obj088989F4 *o, int key, int kind)
{
    int idx;
    if (o->items != 0) {
        idx = func_08898654(o, key);
        if (idx >= 0) {
            o->items[idx].kind = kind;
            return idx;
        }
    }
    return -1;
}
