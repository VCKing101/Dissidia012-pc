typedef struct ListEntry48 {
    char data[0x48];
} ListEntry48;

typedef struct EntryList48 {
    unsigned char active;
    unsigned int count;
    ListEntry48 *entries;
} EntryList48;

extern int func_08AC068C(ListEntry48 *entry);

int func_08AC0E4C(EntryList48 *self, unsigned int index)
{
    if (self->active == 0) {
        return -1;
    }
    if (index >= self->count) {
        return -2;
    }
    func_08AC068C(&self->entries[index]);
    return 0;
}
