extern "C" {
typedef struct ItemList { char pad_000[0x488]; int count; char items[1][0x40]; } ItemList;
extern char D_089CEA9C[];
extern char *func_08935C14(const char *text, const char *pattern);
char *func_08926A8C(void *self, ItemList *list, int nth)
{
    int found = 0;
    for (int i = 0; i < list->count; i++) {
        if (func_08935C14(list->items[i], D_089CEA9C) != 0) {
            if (found == nth) {
                return list->items[i];
            }
            found++;
        }
    }
    return 0;
}
}
