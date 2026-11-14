/* Item i of a list of unsigned shorts; -1 when out of range. */
typedef struct List088B3B60 {
    char pad_00[0xC];
    unsigned int count;
    unsigned short *items;
} List088B3B60;

int func_088B3B60(List088B3B60 *list, unsigned int i)
{
    if (i >= list->count) {
        return -1;
    }
    return list->items[i];
}
