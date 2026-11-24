/* Binary search over 16-byte records by their first word; returns the record or 0. */
struct R16 { int id, a, b, c; };
struct Ord16 { signed char descending; };
struct S {
    static int compare(int a, int b)
    {
        if (a == b) return 0;
        if (a < b) return -1;
        return 1;
    }
};
extern "C" R16 *func_088B1A78(R16 *begin, R16 *end, const R16 *key, Ord16 order)
{
    int count = end - begin;
    int bottom = 0;
    int top = count - 1;
    while (!(top < bottom)) {
        int mid = (bottom + top) / 2;
        R16 *r = &begin[mid];
        int c = S::compare(r->id, key->id);
        if (c == 0) return r;
        if (c < 0) bottom = mid + 1;
        else top = mid - 1;
    }
    return 0;
}
