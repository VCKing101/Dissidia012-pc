/* Screen0B6A0: refreshes how many of an entry's item the player owns
   (kind 1: items, also its price; kind 2: cards; others: none). Entry84
   in menu/auto_region1.h leaves +0x0D and +0x10 in padding. */
typedef struct OwnedEntry29 {
    char pad_00[0xC];
    unsigned char kind;
    unsigned char owned;   /* +0x0D */
    short item;            /* +0x0E */
    short price;           /* +0x10 */
} OwnedEntry29;

extern void *D_089D712C;
extern unsigned char func_088B90C4(void *owned, int item);
extern unsigned char func_088B90F8(void *owned, int item);
extern short func_089FACC8(void *self, int item);

void func_089F1FEC(void *self, OwnedEntry29 *entry)
{
    void *owned = (char *)D_089D712C + 0xF6E0;

    if (entry->kind == 1) {
        entry->owned = func_088B90C4(owned, entry->item);
        entry->price = func_089FACC8(self, entry->item);
    } else if (entry->kind == 2) {
        entry->owned = func_088B90F8(owned, entry->item);
        entry->price = 0;
    } else {
        entry->owned = 0;
        entry->price = 0;
    }
}
