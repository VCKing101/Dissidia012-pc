/* Zeroes the float at +0xC and the word at +0x10 of each 0x14-byte item. */
struct Item { char pad[0xC]; float f; int w; };
struct List { char *items; int count; };
void func_08B519C4(struct List *self)
{
    int i = 0;
    if (i < self->count) {
        float zero = 0.0f;
        int off = 0;
        do {
            ((struct Item *)(self->items + off))->f = zero;
            i++;
            ((struct Item *)(self->items + off))->w = 0;
            off += 0x14;
        } while (i < self->count);
    }
}
