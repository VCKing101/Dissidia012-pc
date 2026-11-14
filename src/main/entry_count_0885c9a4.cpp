extern "C" {
/* Counts the entries with the id. */
typedef struct Entry0885C9A4 {
    unsigned short id;
    char pad_02[0xE];
} Entry0885C9A4;

typedef struct Table0885C9A4 {
    int field_00;
    int count;
    int field_08;
    int field_0C;
    Entry0885C9A4 entries[1];
} Table0885C9A4;

int func_0885C9A4(Table0885C9A4 *t, unsigned int id)
{
    int n = 0;
    for (int i = 0; i < t->count; i++) {
        Entry0885C9A4 *e = &t->entries[i];
        if (e->id == id) {
            n++;
        }
    }
    return n;
}
}
