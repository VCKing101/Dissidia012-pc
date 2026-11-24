/* Walks the 0x24-byte entries of the list and runs the per-entry routine with the argument. */
struct L { int count; char *items; };
struct Self { L *list; };
extern "C" void func_08924AD8(char *, int);
extern "C" void func_08B48878(Self *self, int arg)
{
    for (int i = 0, off = 0; i < self->list->count; i++, off += 0x24) {
        func_08924AD8(self->list->items + off, arg);
    }
}
