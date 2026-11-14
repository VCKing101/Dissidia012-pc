typedef struct Entry0884BB00 {
    int field_00;
    float value;
    unsigned char unknown_08[0xC];
    int key;
    unsigned char unknown_18[0xC8];
} Entry0884BB00;

typedef struct List0884BB00 {
    Entry0884BB00 *entries;
    int count;
} List0884BB00;

void func_0884BB00(List0884BB00 *list, int unused, int key, float value) {
    int i;
    for (i = 0; i < list->count; i++) {
        if (list->entries[i].key == key) {
            list->entries[i].value = value;
        }
    }
}
