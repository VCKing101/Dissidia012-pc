typedef struct ListItem {
    char data[0x18];
} ListItem;

typedef struct ItemList {
    ListItem items[20];
} ItemList;

/* Item index, or item 0 when the index is out of range. */
ListItem *func_08A3D720(ItemList *self, unsigned int index)
{
    ListItem *list[20] = { &self->items[0], &self->items[1], &self->items[2], &self->items[3], &self->items[4], &self->items[5], &self->items[6], &self->items[7], &self->items[8], &self->items[9], &self->items[10], &self->items[11], &self->items[12], &self->items[13], &self->items[14], &self->items[15], &self->items[16], &self->items[17], &self->items[18], &self->items[19] };
    if ((int)index < 0 || index >= 20) {
        index = 0;
    }
    return list[index];
}
