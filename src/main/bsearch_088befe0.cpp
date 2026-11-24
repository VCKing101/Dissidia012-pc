/* Binary search over 8-byte records by their first field (int); returns the record or 0. */
struct Rec088BEFE0 { int id; char pad[4]; };
struct Ord088BEFE0 { signed char descending; };
struct Cmp088BEFE0 {
    static int compare(int a, int b)
    {
        if (a < b) return -1;
        if (b < a) return 1;
        return 0;
    }
};
extern "C" Rec088BEFE0 *func_088BEFE0(Rec088BEFE0 *begin, Rec088BEFE0 *end, const Rec088BEFE0 *key, Ord088BEFE0 order)
{
    int count = end - begin;
    int bottom = 0;
    int top = count - 1;
    while (!(top < bottom)) {
        int mid = (bottom + top) / 2;
        Rec088BEFE0 *r = &begin[mid];
        int c = Cmp088BEFE0::compare(r->id, key->id);
        if (c == 0) return r;
        if (c < 0) bottom = mid + 1;
        else top = mid - 1;
    }
    return 0;
}
