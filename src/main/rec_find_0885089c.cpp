/* Sorted lookup in a table of 0x3C-byte records. */
struct MidRec {
    char data[0x3C];
};

extern "C" {
int func_08850870(const void *a, const void *b);
void *func_08934944(const void *key, const void *base, unsigned int count, unsigned int size,
                    int (*cmp)(const void *, const void *));
}

struct MidRecTable {
    int field_00;
    int count;
    MidRec entries[1];

    static MidRec *find(MidRecTable *t, const void *key)
    {
        return (MidRec *)func_08934944(key, t->entries, t->count, sizeof(MidRec), func_08850870);
    }
};

struct MidRecView {
    MidRecTable *table;
};

extern "C" {

MidRec *func_0885089C(MidRecView *view, const void *key)
{
    return MidRecTable::find(view->table, key);
}

}
