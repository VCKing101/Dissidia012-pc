typedef struct ListEntryE0 {
    char data[0xE0];
} ListEntryE0;

typedef struct EntryListE0_2C {
    unsigned char active;
    char pad_01[0x2B];
    int count;
    ListEntryE0 *entries;
} EntryListE0_2C;

extern "C" {

extern void func_08AC119C(ListEntryE0 *entry);

/* Run func_08AC119C on every entry of an active list. */
int func_08AC20F0(EntryListE0_2C *self)
{
    if (self->active == 0) {
        return -1;
    }
    for (int i = 0; i < self->count; i++) {
        func_08AC119C(&self->entries[i]);
    }
    return 0;
}

}
