/* Index of the item matching the key, or -1 (also when the set is inactive). */
struct Item08897064 {
    char data[0x24];
};

struct Info08897064 {
    int count;
};

struct Set08897064 {
    Info08897064 *info;
    Item08897064 *items;
    char pad_08[0xC];
    unsigned char active;
};

extern "C" {

int func_0893585C(void *key, Item08897064 *item);

int func_08897064(Set08897064 *s, void *key)
{
    if (!s->active) {
        return -1;
    }
    for (int i = 0; i < s->info->count; i++) {
        if (func_0893585C(key, &s->items[i]) == 0) {
            return i;
        }
    }
    return -1;
}

}
