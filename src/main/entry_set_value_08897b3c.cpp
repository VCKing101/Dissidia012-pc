struct Info { int count; };
struct Entry { char data_00[0x20]; unsigned short value; char data_22[0x3C - 0x22]; };
struct Table {
    char data_00[0x58]; Info *info; char data_5C[0x6C - 0x5C];
    union { Entry *entries; void *data; };
    static bool valid(Table *t, int i) {
        if (t->data != 0 && i >= 0 && i < t->info->count) {
            return true;
        }
        return false;
    }
};
extern "C" {
void func_08897B3C(Table *t, int i, unsigned char value)
{
    if (Table::valid(t, i)) {
        t->entries[i].value = value;
    }
}
}
