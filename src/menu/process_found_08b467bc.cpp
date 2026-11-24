/* For each id in the owner list, finds its entry with func_08B48F84 and, when found, runs func_089234C0 on it. */
struct L { char pad[0x1C]; int f1c; char sub[4]; char entries[8]; int items[70]; int count; };
struct Owner { char pad[0x18]; L *list; };
extern "C" char *func_08B48F84(char *entries, int id);
extern "C" void func_089234C0(char *entry);
extern "C" void func_08B467BC(Owner *self)
{
    for (int i = 0, off = 0; i < self->list->count; i++, off += 4) {
        L *list = self->list;
        char *entry = func_08B48F84(list->entries, ((int *)((char *)list + off))[0x2C / 4]);
        if (entry) {
            func_089234C0(entry);
        }
    }
}
