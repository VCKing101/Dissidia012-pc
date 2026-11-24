// Searches [begin, end) with func_08B4A908 for the 16-bit id unless it is -1; returns the record or 0.
struct Key08B4AD7C { short id; char pad[8]; };
struct Ord08B4AD7C { signed char descending; Ord08B4AD7C() { descending = 0; } };
extern "C" {
Key08B4AD7C *func_08B4A908(Key08B4AD7C *begin, Key08B4AD7C *end, const Key08B4AD7C *key, Ord08B4AD7C order);
Key08B4AD7C *func_08B4AD7C(Key08B4AD7C *begin, Key08B4AD7C *end, int id)
{
    Key08B4AD7C *r = 0;
    int none = -1;
    if (id != none) {
        Key08B4AD7C key;
        key.id = id;
        r = func_08B4A908(begin, end, &key, Ord08B4AD7C());
    }
    return r;
}
}
