struct Info { int counts[6]; };
struct Entry { char data_00[0x24]; short flags; char data_26[0x4C - 0x26]; };
struct Table {
    char data_00[0x58]; Info *info; char data_5C[0x70 - 0x5C]; Entry *entries;
    static bool valid(Table *t, int i) {
        if (t->entries != 0 && i >= 0 && i < t->info->counts[5]) {
            return true;
        }
        return false;
    }
};
extern "C" {
int func_0889877C(Table *t, int i)
{
    if (Table::valid(t, i)) {
        if (t->entries[i].flags & 1) {
            return 1;
        }
    }
    return 0;
}
}
