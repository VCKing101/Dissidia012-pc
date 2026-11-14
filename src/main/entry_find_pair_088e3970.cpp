struct Entry { short a; short b; void *data;
    static bool empty(Entry *e) { return e->data == 0; } };
struct Table { char data_000[0x160]; Entry entries[64]; };
extern "C" {
Entry *func_088E3970(Table *t, int a, int b)
{
    for (int i = 0; i < 64; i++) {
        if (!Entry::empty(&t->entries[i]) && t->entries[i].a == a && t->entries[i].b == b) {
            return &t->entries[i];
        }
    }
    return 0;
}
}
