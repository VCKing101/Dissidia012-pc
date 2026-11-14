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

extern void func_08AC16C0(ListEntryE0 *entry);

/* Run func_08AC16C0 on every entry of an active list. */
int func_08AC22BC(EntryListE0_2C *self)
{
    if (self->active == 0) {
        return -1;
    }
    for (int i = 0; i < self->count; i++) {
        func_08AC16C0(&self->entries[i]);
    }
    return 0;
}

}
