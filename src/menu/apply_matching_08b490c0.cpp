/* For each 0x24-byte entry of the list that passes func_08923098 with the key, runs func_089232E8 on it. */
struct List { int count; char *items; };
struct Owner { List *list; };
extern "C" int func_08923098(char *entry, int key);
extern "C" void func_089232E8(char *entry);
extern "C" void func_08B490C0(Owner *self, int key)
{
    for (int i = 0, off = 0; i < self->list->count; i++, off += 0x24) {
        if (func_08923098(self->list->items + off, key)) {
            func_089232E8(self->list->items + off);
        }
    }
}
