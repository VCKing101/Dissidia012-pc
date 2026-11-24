/* Binary search over 8-byte records by their first field (short); returns the record or 0. */
struct Rec088B6598 { short id; char pad[6]; };
struct Ord088B6598 { signed char descending; };
struct Cmp088B6598 {
    static int compare(int a, int b)
    {
        if (a == b) return 0;
        if (a < b) return -1;
        return 1;
    }
};
extern "C" Rec088B6598 *func_088B6598(Rec088B6598 *begin, Rec088B6598 *end, const Rec088B6598 *key, Ord088B6598 order)
{
    int count = end - begin;
    int bottom = 0;
    int top = count - 1;
    while (!(top < bottom)) {
        int mid = (bottom + top) / 2;
        Rec088B6598 *r = &begin[mid];
        int c = Cmp088B6598::compare(r->id, key->id);
        if (c == 0) return r;
        if (c < 0) bottom = mid + 1;
        else top = mid - 1;
    }
    return 0;
}
