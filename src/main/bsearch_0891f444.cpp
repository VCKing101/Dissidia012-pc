/* Binary search over 140-byte records by their first field (short); returns the record or 0. */
struct Rec0891F444 { short id; char pad[140 - 2]; };
struct Cmp0891F444 {
    static int compare(int a, int b)
    {
        if (a == b) return 0;
        if (a < b) return -1;
        return 1;
    }
};
extern "C" Rec0891F444 *func_0891F444(Rec0891F444 *begin, Rec0891F444 *end, const Rec0891F444 *key)
{
    int count = end - begin;
    int bottom = 0;
    int top = count - 1;
    while (!(top < bottom)) {
        int mid = (bottom + top) / 2;
        Rec0891F444 *r = &begin[mid];
        int c = Cmp0891F444::compare(r->id, key->id);
        if (c == 0) return r;
        if (c < 0) bottom = mid + 1;
        else top = mid - 1;
    }
    return 0;
}
