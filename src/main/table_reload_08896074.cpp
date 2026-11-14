/* Reloads every table entry with the id. */
extern "C" {

typedef struct Entry08896074 {
    int id;
    char pad_04[0x18];
} Entry08896074;

typedef struct Table08896074 {
    int field_00;
    int field_04;
    int count;
    int field_0C;
    Entry08896074 entries[1];
} Table08896074;

typedef struct Loader08896074 {
    Table08896074 *table;
} Loader08896074;

void func_08896708(Loader08896074 *l, Entry08896074 *e);

void func_08896074(Loader08896074 *l, int id)
{
    if (l->table != 0) {
        Entry08896074 *e = l->table->entries;
        for (int i = 0; i < l->table->count; i++, e++) {
            if (e->id == id) {
                func_08896708(l, e);
            }
        }
    }
}

}
