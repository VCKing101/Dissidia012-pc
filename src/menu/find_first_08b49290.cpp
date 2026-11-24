/* Walks the 0x24-byte entries of the list; for the first one passing func_089235DC with the key, calls func_089235A8 on it and stops. */
struct List { int count; char *items; };
struct Owner { List *list; };
extern "C" int func_089235DC(char *entry, int key);
extern "C" void func_089235A8(char *entry);
extern "C" void func_08B49290(Owner *self, int key)
{
    for (int i = 0, off = 0; i < self->list->count; i++, off += 0x24) {
        if (func_089235DC(self->list->items + off, key)) {
            func_089235A8(self->list->items + off);
            break;
        }
    }
}
