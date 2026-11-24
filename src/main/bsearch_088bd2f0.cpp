/* Binary search over 8-byte records by their first field (short); returns the record or 0. */
struct Rec088BD2F0 { short id; char pad[6]; };
struct Ord088BD2F0 { signed char descending; };
struct Cmp088BD2F0 {
    static int compare(int a, int b)
    {
        if (a == b) return 0;
        if (a < b) return -1;
        return 1;
    }
};
extern "C" Rec088BD2F0 *func_088BD2F0(Rec088BD2F0 *begin, Rec088BD2F0 *end, const Rec088BD2F0 *key, Ord088BD2F0 order)
{
    int count = end - begin;
    int bottom = 0;
    int top = count - 1;
    while (!(top < bottom)) {
        int mid = (bottom + top) / 2;
        Rec088BD2F0 *r = &begin[mid];
        int c = Cmp088BD2F0::compare(r->id, key->id);
        if (c == 0) return r;
        if (c < 0) bottom = mid + 1;
        else top = mid - 1;
    }
    return 0;
}
