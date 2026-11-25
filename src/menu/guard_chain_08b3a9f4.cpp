// Returns 1 when the sub-object accepts, the id is not rejected and the optional slot lookup fails; otherwise 0.
struct Sub { char pad[0x10]; int row; };
struct Owner { char pad0[0xB8]; int slot; char pad1[8]; Sub *sub; };
extern "C" {
extern int func_08B48254(int);
extern int func_08920800(int);
extern int func_08B450FC(int, int);
int func_08B3A9F4(Owner *self, int id, int slot) {
    int slot2; int id2; Owner *o;
    o = self;
    id2 = id;
    slot2 = slot;
    Sub *sub = o->sub;
    if (!func_08B48254(sub->row)) {
        return 0;
    }
    if (func_08920800(id2)) {
        return 0;
    }
    if (slot2 != -1) {
        if (func_08B450FC(o->slot, slot2)) {
            return 0;
        }
    }
    return 1;
}
}
