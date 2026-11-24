/* Gathers up to four ids for the owner list's field +0x1C with func_08B4ABD8 and runs func_08B49030 on its entries for each. */
struct L { char pad[0x1C]; int f1c; char sub[4]; char entries[8]; };
struct Owner { char pad[0x18]; L *list; };
extern "C" int func_089212E8(int v);
extern "C" int func_08B4ABD8(char *sub, int v, int *out);
extern "C" void func_08B49030(char *entries, int id);
extern "C" void func_08B462A4(Owner *self)
{
    int ids[4];
    L *l = self->list;
    char *sub = l->sub;
    int n = func_08B4ABD8(sub, func_089212E8(l->f1c), ids);
    for (int i = 0, *p = ids; i < n; i++, p++) {
        func_08B49030(self->list->entries, *p);
    }
}
