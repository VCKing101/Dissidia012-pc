/* Sorted lookup in a table of 0x17C-byte records. */
struct BigRec {
    char data[0x17C];
};

extern "C" {
int func_0884E3A0(const void *a, const void *b);
void *func_08934944(const void *key, const void *base, unsigned int count, unsigned int size,
                    int (*cmp)(const void *, const void *));
}

struct BigRecTable {
    int field_00;
    int count;
    BigRec entries[1];

    static BigRec *find(BigRecTable *t, const void *key)
    {
        return (BigRec *)func_08934944(key, t->entries, t->count, sizeof(BigRec), func_0884E3A0);
    }
};

struct BigRecView {
    BigRecTable *table;
};

extern "C" {

BigRec *func_0884E58C(BigRecView *view, const void *key)
{
    return BigRecTable::find(view->table, key);
}

}
