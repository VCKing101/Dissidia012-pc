/* Returns the next used entry after `prev` (from the start when prev is
   0), or 0 when there is none. */
struct Entry0881E950 {
    int id;
    char pad_04[0x64];
};

struct Table0881E950 {
    unsigned char ready;
    char pad_01[0x4C - 1];
    int count;
    Entry0881E950 *entries;
};

extern "C" {

Entry0881E950 *func_0881E950(Table0881E950 *self, Entry0881E950 *prev)
{
    if (!self->ready) {
        return 0;
    }
    unsigned int i = 0;
    if (prev != 0) {
        for (int k = 0; k < self->count; k++) {
            Entry0881E950 *e = &self->entries[k];
            i++;
            if (e == prev) {
                break;
            }
        }
    }
    for (; i < self->count; i++) {
        Entry0881E950 *e = &self->entries[i];
        if (e->id != -1) {
            return e;
        }
    }
    return 0;
}

}
