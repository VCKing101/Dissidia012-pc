/* Binary search over 68-byte records by their first field (short); returns the record or 0. */
struct Rec088B2748 { short id; char pad[66]; };
struct Ord088B2748 { signed char descending; };
struct Cmp088B2748 {
    static int compare(int a, int b)
    {
        if (a == b) return 0;
        if (a < b) return -1;
        return 1;
    }
};
extern "C" Rec088B2748 *func_088B2748(Rec088B2748 *begin, Rec088B2748 *end, const Rec088B2748 *key, Ord088B2748 order)
{
    int count = end - begin;
    int bottom = 0;
    int top = count - 1;
    while (!(top < bottom)) {
        int mid = (bottom + top) / 2;
        Rec088B2748 *r = &begin[mid];
        int c = Cmp088B2748::compare(r->id, key->id);
        if (c == 0) return r;
        if (c < 0) bottom = mid + 1;
        else top = mid - 1;
    }
    return 0;
}
