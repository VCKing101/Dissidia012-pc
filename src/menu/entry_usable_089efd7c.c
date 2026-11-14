/* Returns 1 when the entry for the kind is usable; entry 12 also needs two or more items. */
typedef struct EntryOwner EntryOwner;

extern int func_089EF880(EntryOwner *self, signed char kind);
extern int func_089EFB50(EntryOwner *self, int entry, int index);
extern int func_089F60FC(EntryOwner *self);

int func_089EFD7C(EntryOwner *self, signed char kind)
{
    int usable = 1;
    int entry = func_089EF880(self, kind);
    if (func_089EFB50(self, entry, -1) == 0) {
        usable = 0;
    }
    if (usable) {
        if (entry == 12 && func_089F60FC(self) < 2) {
            usable = 0;
        }
    }
    return usable;
}
