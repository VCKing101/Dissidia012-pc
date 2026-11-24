// Removes (releases and clears) every 0x24-byte entry whose linked record carries the given id.
struct Rec { char pad[0x12]; short id; };
struct Entry { char pad[0x20]; Rec *rec; };
struct List { int count; char *items; };
struct Owner { List *list; };
extern "C" {
extern void func_089246C4(void *);
extern void func_089231A8(void *);
void func_08B48CC0(Owner *self, int id) {
    for (int i = 0, off = 0; i < self->list->count; i++, off += 0x24) {
        if (((Entry *)(self->list->items + off))->rec->id == id) {
            func_089246C4(self->list->items + off);
            func_089231A8(self->list->items + off);
        }
    }
}
}
