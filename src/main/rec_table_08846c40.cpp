/* Table of 0x44-byte records: iteration and sorted lookup. */
struct Rec {
    char data[0x44];
};

extern "C" {
int func_08846C0C(const void *a, const void *b);
void *func_08934944(const void *key, const void *base, unsigned int count, unsigned int size,
                    int (*cmp)(const void *, const void *));
}

struct RecTable {
    int field_00;
    int count;
    Rec entries[1];

    static Rec *find(RecTable *t, const void *key)
    {
        return (Rec *)func_08934944(key, t->entries, t->count, sizeof(Rec), func_08846C0C);
    }
};

struct RecView {
    RecTable *table;
};

extern "C" {

Rec *func_08846C40(RecView *view, Rec *cur)
{
    int idx = 0;
    if (cur != 0) {
        idx = cur - view->table->entries + 1;
    }
    if (idx < view->table->count) {
        return &view->table->entries[idx];
    } else {
        return 0;
    }
}

Rec *func_08846C98(RecView *view, const void *key)
{
    return RecTable::find(view->table, key);
}

}
