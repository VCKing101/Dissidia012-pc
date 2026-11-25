struct Hook08907608 {
    virtual void v0();
};
struct Root08907608;
struct Rec08907608 { int pad; Root08907608 *owner; int idx; };
struct Slot08907608 { char pad[0x2D4]; Rec08907608 *rec; char pad2[0x2E0 - 0x2D8]; };
struct Root08907608 {
    char pad0[4];
    char pad00[0x808 - 4];
    Slot08907608 slots[3];
    char pad3[0x10A8 - 0x808 - 0x2E0 * 3];
    Rec08907608 recs[3];
    char pad4[0x10D8 - 0x10A8 - 0xC * 3];
    char blk[0x1644 - 0x10D8];
    int mode;
    int a1648;
    char pad1[0x1654 - 0x164C];
    int a1654;
    char pad2[0x1660 - 0x1658];
    Hook08907608 *hook;
};
extern "C" void func_089065D4(void *);
extern "C" void func_0890661C(void *, void *);
extern "C" void func_08906684(void *, int, int);
extern "C" void func_0890C404(void *, int);
extern "C" void func_08907608(Root08907608 *self);
extern "C" void func_08907608(Root08907608 *self)
{
    int table[4];
    Hook08907608 *h = self->hook;
    if (h != 0) {
        h->v0();
    }
    int c5 = 0x5000;
    int c4 = 0x400;
    table[0] = c5;
    int c3 = 4;
    table[1] = c4;
    table[2] = c3;
    int i = 0;
    Slot08907608 *slot = 0;
    char *four = (char *)self + 4;
    slot = self->slots;
    Rec08907608 *rec = self->recs;
    Root08907608 *cur = self;
    int *tp = table;
    do {
        if (self->mode == 0) {
            func_089065D4(slot);
        } else {
            func_0890661C(slot, four);
        }
        slot->rec = rec;
        rec->owner = self;
        rec->idx = i;
        func_08906684(slot, cur->a1648, *tp);
        i = i + 1;
        slot = slot + 1;
        rec = rec + 1;
        cur = (Root08907608 *)((char *)cur + 4);
        tp = tp + 1;
    } while (i < 3);
    func_0890C404(self->blk, self->a1654);
}
