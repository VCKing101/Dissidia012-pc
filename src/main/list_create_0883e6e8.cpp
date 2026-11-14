/* Allocates an empty entry list sized for the entries of a group. */
struct EntryList0883E6E8 {
    int owner;
    short count;
    unsigned char group;
    unsigned char flags;
    void *first;
    char entries[1];
};

extern "C" {

int func_08812014(int table, int group);
EntryList0883E6E8 *func_08842100(int size);

EntryList0883E6E8 *func_0883E6E8(int owner, int table, int group)
{
    int n = func_08812014(table, group);
    if (n == 0) {
        return 0;
    }
    EntryList0883E6E8 *l = func_08842100(n * 28 + 12);
    l->count = 0;
    l->owner = owner;
    l->group = group;
    l->first = 0;
    l->flags = 0;
    return l;
}

}
