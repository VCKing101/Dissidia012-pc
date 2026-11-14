typedef struct ItemList { char pad_000[0x488]; int count; char items[1][0x40]; } ItemList;
extern void func_08926640(void *a, void *b, void *item);
void func_089266F0(void *a, int unused1, void *b, int unused3, ItemList *list)
{
    int i;
    for (i = 0; i < list->count; i++) {
        func_08926640(a, b, list->items[i]);
    }
}
