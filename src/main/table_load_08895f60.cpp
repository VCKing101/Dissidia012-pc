/* Loads every table entry that has not been loaded yet. */
extern "C" {
typedef struct Entry08895F60 {
    char pad_00[0x14];
    void *data;
    int field_18;
} Entry08895F60;

typedef struct Table08895F60 {
    int field_00;
    int field_04;
    int count;
    int field_0C;
    Entry08895F60 entries[1];
} Table08895F60;

typedef struct Loader08895F60 {
    Table08895F60 *table;
} Loader08895F60;

void func_08896708(Loader08895F60 *l, Entry08895F60 *e);

void func_08895F60(Loader08895F60 *l)
{
    if (l->table != 0) {
        Entry08895F60 *e = l->table->entries;
        for (int i = 0; i < l->table->count; i++, e++) {
            if (e->data == 0) {
                func_08896708(l, e);
            }
        }
    }
}
}
