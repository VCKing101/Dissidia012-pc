/* Sets the byte value of entry i when the entry table exists and i is in range. */
struct Info { int counts[2]; };
struct Entry { char data_00[0x20]; unsigned short value; char data_rest[0x3C - 0x20 - 2]; };
struct Table {
    char data_00[0x58];
    Info *info;
    char data_5C[0x5C - 0x5C];
    union { Entry *entries; void *data; };
    static bool valid(Table *t, int i) {
        if (t->data != 0 && i >= 0 && i < t->info->counts[1]) {
            return true;
        }
        return false;
    }
};

extern "C" {
void func_08897BFC(Table *t, int i, unsigned char value)
{
    if (Table::valid(t, i)) {
        t->entries[i].value = value;
    }
}
}
