extern "C" {
/* Turns the table's entry offsets into pointers once (the table is its own base). */
typedef struct Entry0885C93C {
    int field_00;
    int field_04;
    int offset;
    int size;
} Entry0885C93C;

typedef struct Table0885C93C {
    int field_00;
    unsigned int count;
    int base;
    int field_0C;
    Entry0885C93C entries[1];
} Table0885C93C;

void func_0885C93C(Table0885C93C *t)
{
    if (t->base != 0) {
        return;
    }
    t->base = (int)t;
    for (unsigned int i = 0; i < t->count; i++) {
        if (t->entries[i].size == 0) {
            t->entries[i].offset = 0;
        } else {
            int p = t->base + t->entries[i].offset;
            t->entries[i].offset = p;
        }
    }
}
}
