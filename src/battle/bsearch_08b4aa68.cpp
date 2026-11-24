/* Binary search over 4-byte records by their first field (short); returns the record or 0. */
struct Rec08B4AA68 { short id; char pad[2]; };
struct Ord08B4AA68 { signed char descending; };
struct Cmp08B4AA68 {
    static int compare(int a, int b)
    {
        if (a == b) return 0;
        if (a < b) return -1;
        return 1;
    }
};
extern "C" Rec08B4AA68 *func_08B4AA68(Rec08B4AA68 *begin, Rec08B4AA68 *end, const Rec08B4AA68 *key, Ord08B4AA68 order)
{
    int count = end - begin;
    int bottom = 0;
    int top = count - 1;
    while (!(top < bottom)) {
        int mid = (bottom + top) / 2;
        Rec08B4AA68 *r = &begin[mid];
        int c = Cmp08B4AA68::compare(r->id, key->id);
        if (c == 0) return r;
        if (c < 0) bottom = mid + 1;
        else top = mid - 1;
    }
    return 0;
}
