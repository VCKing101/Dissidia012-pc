/* Index of the entry with the id, or -1. */
struct Entry0889F5E8 {
    short id;
    char pad_02[0x2A];
};

struct Info0889F5E8 {
    int field_00;
    int field_04;
    int count;
};

struct Obj0889F5E8 {
    int field_00;
    Info0889F5E8 *info;
    Entry0889F5E8 *entries;
};

extern "C" {

int func_0889F5E8(Obj0889F5E8 *o, int id)
{
    for (int i = 0; i < o->info->count; i++) {
        Entry0889F5E8 *e = &o->entries[i];
        if (e->id == id) {
            return i;
        }
    }
    return -1;
}

}
