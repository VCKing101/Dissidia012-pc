/* Bounds-checked lookup in the entry list at 0x5C, returning the entry's value. */
typedef struct EntryOwner {
    char pad_00[0x5C];
    char list[0x200];
} EntryOwner;

extern int func_088D93E0(void *list);
extern void *func_088D9498(void *list, int index);
extern int func_088D8E58(void *entry);

int func_088D76A8(EntryOwner *owner, int index)
{
    void *list = owner->list;
    int count = func_088D93E0(list);
    void *entry;

    if (index < 0 || index >= count) {
        return -1;
    }
    entry = func_088D9498(list, index);
    if (entry == 0) {
        return -1;
    }
    return func_088D8E58(entry);
}
