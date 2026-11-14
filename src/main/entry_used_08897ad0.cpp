struct Info { int field_00; int field_04; int field_08; int count; };
struct Entry { char data_00[0x20]; short used; char data_22[0x2C - 0x22]; };
struct Table {
    char data_00[0x58]; Info *info; char data_5C[0x68 - 0x5C]; Entry *entries;
    static bool valid(Table *t, int i) {
        if (t->entries != 0 && i >= 0 && i < t->info->count) {
            return true;
        }
        return false;
    }
};
extern "C" {
int func_08897AD0(Table *t, int i)
{
    if (Table::valid(t, i)) {
        if (t->entries[i].used != 0) {
            return 1;
        }
    }
    return 0;
}
}
