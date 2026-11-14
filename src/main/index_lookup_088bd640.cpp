struct Entry { short id; short pad; void *value; };
struct Order { signed char descending; Order() : descending(0) {} };
struct Index {
    Entry *entries;
    int count;
    unsigned int bits[1];
    static bool has(Index *x, unsigned int id) { return (*(unsigned int *)((char *)x->bits + (id >> 5) * 4) & (1 << (id & 0x1F))) != 0; }
};
extern "C" {
Entry *func_088BD2F0(Entry *begin, Entry *end, const Entry *key, Order order);

void *func_088BD640(Index *x, unsigned int id)
{
    if (Index::has(x, id) == true) {
        Entry key;
        key.id = id;
        Entry *found = func_088BD2F0(x->entries, x->entries + x->count, &key, Order());
        if (found != 0) {
            return found->value;
        }
    }
    return 0;
}
}
