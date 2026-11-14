typedef struct ItemBase {
    void *vtable;
    unsigned char active;
} ItemBase;

extern char D_089CCB48[];

ItemBase *func_088ECE20(ItemBase *item)
{
    item->vtable = D_089CCB48;
    item->active = 0;
    return item;
}
