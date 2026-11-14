/* Index of the item matching the key, or -1. */
struct Item08897D50 {
    char data[0x3C];
};

struct Info08897D50 {
    int count;
};

struct Obj08897D50 {
    char pad_00[0x58];
    Info08897D50 *info;
    char pad_5C[0x10];
    Item08897D50 *items;
};

extern "C" {

int func_08935970(Item08897D50 *item, void *key);

int func_08897D50(Obj08897D50 *o, void *key)
{
    if (o->info != 0) {
        for (int i = 0; i < o->info->count; i++) {
            if (func_08935970(&o->items[i], key) == 0) {
                return i;
            }
        }
    }
    return -1;
}

}
