/* Table of 0x74-byte entries keyed by a 16-bit id. */
struct Ent {
    unsigned short id;
    char data[0x72];
};

struct EntTable {
    int field_00;
    int count;
    Ent entries[1];
};

struct EntView {
    EntTable *table;
};

extern "C" {

Ent *func_088480D0(EntView *view, int idx)
{
    if (idx >= view->table->count) {
        return 0;
    }
    return &view->table->entries[idx];
}

int func_08848110(EntView *view, unsigned int id)
{
    for (int i = 0; i < view->table->count; i++) {
        if (view->table->entries[i].id == id) {
            return i;
        }
    }
    return 0;
}

}
