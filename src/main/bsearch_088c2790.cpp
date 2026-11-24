/* Binary search over 2-byte records by their first field (signed char); returns the record or 0. */
struct Rec088C2790 { signed char id; char pad[1]; };
struct Ord088C2790 { signed char descending; };
struct Cmp088C2790 {
    static int compare(int a, int b)
    {
        if (a == b) return 0;
        if (a < b) return -1;
        return 1;
    }
};
extern "C" Rec088C2790 *func_088C2790(Rec088C2790 *begin, Rec088C2790 *end, const Rec088C2790 *key, Ord088C2790 order)
{
    int count = end - begin;
    int bottom = 0;
    int top = count - 1;
    while (!(top < bottom)) {
        int mid = (bottom + top) / 2;
        Rec088C2790 *r = &begin[mid];
        int c = Cmp088C2790::compare(r->id, key->id);
        if (c == 0) return r;
        if (c < 0) bottom = mid + 1;
        else top = mid - 1;
    }
    return 0;
}
