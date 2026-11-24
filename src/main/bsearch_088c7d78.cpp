/* Binary search over 8-byte records by their first field (short); returns the record or 0. */
struct Rec088C7D78 { short id; char pad[6]; };
struct Ord088C7D78 { signed char descending; };
struct Cmp088C7D78 {
    static int compare(int a, int b)
    {
        if (a == b) return 0;
        if (a < b) return -1;
        return 1;
    }
};
extern "C" Rec088C7D78 *func_088C7D78(Rec088C7D78 *begin, Rec088C7D78 *end, const Rec088C7D78 *key, Ord088C7D78 order)
{
    int count = end - begin;
    int bottom = 0;
    int top = count - 1;
    while (!(top < bottom)) {
        int mid = (bottom + top) / 2;
        Rec088C7D78 *r = &begin[mid];
        int c = Cmp088C7D78::compare(r->id, key->id);
        if (c == 0) return r;
        if (c < 0) bottom = mid + 1;
        else top = mid - 1;
    }
    return 0;
}
