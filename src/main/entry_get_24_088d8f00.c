typedef struct Entry {
    char pad_00[0x18];
    int value;
    int field_1C;
    int field_20;
    int field_24;
} Entry;

extern int func_088D8E18(Entry *entry);

int func_088D8F00(Entry *entry)
{
    if (func_088D8E18(entry) == 0) {
        return 0;
    }
    return entry->field_24;
}
