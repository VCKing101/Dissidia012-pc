/* Binary search over 12-byte records by their first field (short); returns the record or 0. */
struct Rec088C5780 { short id; char pad[10]; };
struct Ord088C5780 { signed char descending; };
struct Cmp088C5780 {
    static int compare(int a, int b)
    {
        if (a == b) return 0;
        if (a < b) return -1;
        return 1;
    }
};
extern "C" Rec088C5780 *func_088C5780(Rec088C5780 *begin, Rec088C5780 *end, const Rec088C5780 *key, Ord088C5780 order)
{
    int count = end - begin;
    int bottom = 0;
    int top = count - 1;
    while (!(top < bottom)) {
        int mid = (bottom + top) / 2;
        Rec088C5780 *r = &begin[mid];
        int c = Cmp088C5780::compare(r->id, key->id);
        if (c == 0) return r;
        if (c < 0) bottom = mid + 1;
        else top = mid - 1;
    }
    return 0;
}
