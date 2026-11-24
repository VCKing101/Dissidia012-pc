/* Binary search over 8-byte records by their first field (short); returns the record or 0. */
struct Rec08B4A860 { short id; char pad[6]; };
struct Ord08B4A860 { signed char descending; };
struct Cmp08B4A860 {
    static int compare(int a, int b)
    {
        if (a == b) return 0;
        if (a < b) return -1;
        return 1;
    }
};
extern "C" Rec08B4A860 *func_08B4A860(Rec08B4A860 *begin, Rec08B4A860 *end, const Rec08B4A860 *key, Ord08B4A860 order)
{
    int count = end - begin;
    int bottom = 0;
    int top = count - 1;
    while (!(top < bottom)) {
        int mid = (bottom + top) / 2;
        Rec08B4A860 *r = &begin[mid];
        int c = Cmp08B4A860::compare(r->id, key->id);
        if (c == 0) return r;
        if (c < 0) bottom = mid + 1;
        else top = mid - 1;
    }
    return 0;
}
