/* Calls func_089243F4 on each 0x24-byte entry of the list at self[0], re-reading the count every pass. */
struct List { int count; char *items; };
struct Owner { List *list; };
extern "C" void func_089243F4(char *entry);
extern "C" void func_08B48A0C(Owner *self)
{
    for (int i = 0, off = 0; i < self->list->count; i++, off += 0x24) {
        func_089243F4(self->list->items + off);
    }
}
