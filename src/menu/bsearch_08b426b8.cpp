/* Binary search over 80-byte records by their first field (short); returns the record or 0. */
struct Rec08b426b8 { short id; char pad[80 - 2]; };
struct Cmp08b426b8 {
    static int compare(int a, int b)
    {
        if (a == b) return 0;
        if (a < b) return -1;
        return 1;
    }
};
extern "C" Rec08b426b8 *func_08B426B8(Rec08b426b8 *begin, Rec08b426b8 *end, const Rec08b426b8 *key)
{
    int count = end - begin;
    int bottom = 0;
    int top = count - 1;
    while (!(top < bottom)) {
        int mid = (bottom + top) / 2;
        Rec08b426b8 *r = &begin[mid];
        int c = Cmp08b426b8::compare(r->id, key->id);
        if (c == 0) return r;
        if (c < 0) bottom = mid + 1;
        else top = mid - 1;
    }
    return 0;
}
