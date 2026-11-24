/* Binary search over 38-byte records by their first field (short); returns the record or 0. */
struct Rec08B4A9B4 { short id; char pad[36]; };
struct Ord08B4A9B4 { signed char descending; };
struct Cmp08B4A9B4 {
    static int compare(int a, int b)
    {
        if (a == b) return 0;
        if (a < b) return -1;
        return 1;
    }
};
extern "C" Rec08B4A9B4 *func_08B4A9B4(Rec08B4A9B4 *begin, Rec08B4A9B4 *end, const Rec08B4A9B4 *key, Ord08B4A9B4 order)
{
    int count = end - begin;
    int bottom = 0;
    int top = count - 1;
    while (!(top < bottom)) {
        int mid = (bottom + top) / 2;
        Rec08B4A9B4 *r = &begin[mid];
        int c = Cmp08B4A9B4::compare(r->id, key->id);
        if (c == 0) return r;
        if (c < 0) bottom = mid + 1;
        else top = mid - 1;
    }
    return 0;
}
