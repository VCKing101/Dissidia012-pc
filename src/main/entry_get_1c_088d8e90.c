typedef struct Entry {
    char pad_00[0x18];
    int value;
    int field_1C;
} Entry;

extern int func_088D8E18(Entry *entry);

int func_088D8E90(Entry *entry)
{
    if (func_088D8E18(entry) == 0) {
        return -1;
    }
    return entry->field_1C;
}
