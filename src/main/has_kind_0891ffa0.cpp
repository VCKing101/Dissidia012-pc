/* True when one of the four equipped slots resolves to a record whose kind byte equals the given value. */
struct Rec { char pad[0xD]; char kind; };
struct Pair { Rec *p; int x; };
struct Owner { char pad[0xC]; int base; };
extern "C" int func_088B7B18(int slot, int i);
extern "C" Pair func_088B97E8(int id);
extern "C" int func_0891FFA0(Owner *self, int want)
{
    Pair c;
    int b = self->base + 0x10;
    int slot = b + *(char *)(b + 0x763) * 0xC6;
    for (int i = 0; i < 4; i++) {
        int id = func_088B7B18(slot, i);
        if (id != -1) {
            c = func_088B97E8(id);
            Rec *pp = c.p;
            if (pp != 0) {
                if (pp->kind == want) {
                    return 1;
                }
            }
        }
    }
    return 0;
}
