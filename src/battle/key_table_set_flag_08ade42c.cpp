typedef struct KeyTable KeyTable;

typedef struct FlagEntry {
    int id;
    unsigned char flag;
} FlagEntry;

extern "C" void func_08ADE8A0(KeyTable *self, FlagEntry *entry);

struct FlagEntrySet {
    static void set(KeyTable *table, int id, unsigned char flag)
    {
        FlagEntry entry;
        entry.id = id;
        entry.flag = flag;
        func_08ADE8A0(table, &entry);
    }
};

extern "C" void func_08ADE42C(KeyTable *self, int id, unsigned char flag)
{
    FlagEntrySet::set(self, id, flag);
}
