typedef struct MenuItems {
    char pad_000[0x18];
    short *items[0x201];
    int index;
} MenuItems;

/* Id of the current item, -1 when the slot is empty. */
int func_08AF5AC0(MenuItems *self)
{
    short **items = self->items;
    short *item = items[self->index];
    int result = -1;
    if (item != 0) {
        result = *item;
    }
    return result;
}
