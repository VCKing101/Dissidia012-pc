// Searches [begin, end) with func_08B4AA68 for the 16-bit id unless it is -1; returns the record or 0.
struct Key08B4AE04 { short id; char pad[2]; };
struct Ord08B4AE04 { signed char descending; Ord08B4AE04() { descending = 0; } };
extern "C" {
Key08B4AE04 *func_08B4AA68(Key08B4AE04 *begin, Key08B4AE04 *end, const Key08B4AE04 *key, Ord08B4AE04 order);
Key08B4AE04 *func_08B4AE04(Key08B4AE04 *begin, Key08B4AE04 *end, int id)
{
    Key08B4AE04 *r = 0;
    int none = -1;
    if (id != none) {
        Key08B4AE04 key;
        key.id = id;
        r = func_08B4AA68(begin, end, &key, Ord08B4AE04());
    }
    return r;
}
}
