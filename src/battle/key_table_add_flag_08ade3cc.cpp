typedef struct KeyTable KeyTable;

typedef struct FlagEntry {
    int id;
    unsigned char flag;
} FlagEntry;

extern "C" {
extern int func_08ADE648(KeyTable *self, int id);
extern void func_08ADE79C(KeyTable *self, FlagEntry *entry);
}

struct FlagEntryAdd {
    static void add(KeyTable *table, int id, unsigned char flag)
    {
        FlagEntry entry;
        entry.id = id;
        entry.flag = flag;
        func_08ADE79C(table, &entry);
    }
};

extern "C" {

/* Add id with its flag unless the table already has it. */
void func_08ADE3CC(KeyTable *self, int id, unsigned char flag)
{
    if (func_08ADE648(self, id) == -1) {
        FlagEntryAdd::add(self, id, flag);
    }
}

}
