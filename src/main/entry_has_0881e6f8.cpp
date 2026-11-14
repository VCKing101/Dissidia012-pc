extern "C" {
/* 1 when an entry with the id exists. */
typedef struct Entry0881E6F8 {
    short id;
    char pad_02[0x22];
} Entry0881E6F8;

typedef struct Table0881E6F8 {
    unsigned char valid;
    char pad_01[0x87];
    int count;
    Entry0881E6F8 *entries;
} Table0881E6F8;

int func_0881E6F8(Table0881E6F8 *t, int id)
{
    if (!t->valid) {
        return 0;
    }
    for (int i = 0; i < t->count; i++) {
        if (t->entries[i].id == id) {
            return 1;
        }
    }
    return 0;
}
}
