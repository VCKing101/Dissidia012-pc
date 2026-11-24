/* Binary search over 24-byte records by their first field (short); returns the record or 0. */
struct Rec08b43270 { short id; char pad[24 - 2]; };
struct Cmp08b43270 {
    static int compare(int a, int b)
    {
        if (a == b) return 0;
        if (a < b) return -1;
        return 1;
    }
};
extern "C" Rec08b43270 *func_08B43270(Rec08b43270 *begin, Rec08b43270 *end, const Rec08b43270 *key)
{
    int count = end - begin;
    int bottom = 0;
    int top = count - 1;
    while (!(top < bottom)) {
        int mid = (bottom + top) / 2;
        Rec08b43270 *r = &begin[mid];
        int c = Cmp08b43270::compare(r->id, key->id);
        if (c == 0) return r;
        if (c < 0) bottom = mid + 1;
        else top = mid - 1;
    }
    return 0;
}
