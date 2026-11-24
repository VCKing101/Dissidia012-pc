/* Binary search over 8-byte records by their first field (signed char); returns the record or 0. */
struct Rec088B6D34 { signed char id; char pad[7]; };
struct Ord088B6D34 { signed char descending; };
struct Cmp088B6D34 {
    static int compare(int a, int b)
    {
        if (a == b) return 0;
        if (a < b) return -1;
        return 1;
    }
};
extern "C" Rec088B6D34 *func_088B6D34(Rec088B6D34 *begin, Rec088B6D34 *end, const Rec088B6D34 *key, Ord088B6D34 order)
{
    int count = end - begin;
    int bottom = 0;
    int top = count - 1;
    while (!(top < bottom)) {
        int mid = (bottom + top) / 2;
        Rec088B6D34 *r = &begin[mid];
        int c = Cmp088B6D34::compare(r->id, key->id);
        if (c == 0) return r;
        if (c < 0) bottom = mid + 1;
        else top = mid - 1;
    }
    return 0;
}
