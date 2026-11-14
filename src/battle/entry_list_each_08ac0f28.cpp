typedef struct ListEntry48 {
    char data[0x48];
} ListEntry48;

typedef struct EntryList48 {
    unsigned char active;
    int count;
    ListEntry48 *entries;
} EntryList48;

extern "C" {

extern void func_08AC0840(ListEntry48 *entry);

/* Run func_08AC0840 on every entry of an active list. */
int func_08AC0F28(EntryList48 *self)
{
    if (self->active == 0) {
        return -1;
    }
    for (int i = 0; i < self->count; i++) {
        func_08AC0840(&self->entries[i]);
    }
    return 0;
}

}
