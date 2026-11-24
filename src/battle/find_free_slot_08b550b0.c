/* Scans the slot ids of record `index` in the table at +0xC and returns the first whose entry in the table at +0x18 has word 4 clear, or -1. */
struct Rec { int count; unsigned short id; };
struct Item { int a; int b; int c; };
struct Obj { char pad[0xC]; struct Rec *recs; char pad2[8]; struct Item *items; };
unsigned func_08B550B0(struct Obj *self, int index)
{
    struct Rec *rec = (struct Rec *)((char *)self->recs + index * 0x2C);
    int i = 0;
    int n = rec->count;
    if (i < n) {
        do {
            if (self->items[rec->id].b == 0) {
                return rec->id;
            }
            i++;
            rec = (struct Rec *)((char *)rec + 2);
        } while (i < n);
    }
    return 0xFFFFFFFF;
}
