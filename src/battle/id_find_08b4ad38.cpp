// Searches [begin, end) with func_08B4A860 for the 16-bit id unless it is -1; returns the record or 0.
struct Key08B4AD38 { short id; short pad; int pad2; };
struct Ord08B4AD38 { signed char descending; Ord08B4AD38() { descending = 0; } };
extern "C" {
Key08B4AD38 *func_08B4A860(Key08B4AD38 *begin, Key08B4AD38 *end, const Key08B4AD38 *key, Ord08B4AD38 order);
Key08B4AD38 *func_08B4AD38(Key08B4AD38 *begin, Key08B4AD38 *end, int id)
{
    Key08B4AD38 *r = 0;
    int none = -1;
    if (id != none) {
        Key08B4AD38 key;
        key.id = id;
        r = func_08B4A860(begin, end, &key, Ord08B4AD38());
    }
    return r;
}
}
