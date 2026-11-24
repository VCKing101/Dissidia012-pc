/* Binary search over 4-byte records by their first field (short); returns the record or 0. */
struct Rec088B7204 { short id; char pad[2]; };
struct Ord088B7204 { signed char descending; };
struct Cmp088B7204 {
    static int compare(int a, int b)
    {
        if (a == b) return 0;
        if (a < b) return -1;
        return 1;
    }
};
extern "C" Rec088B7204 *func_088B7204(Rec088B7204 *begin, Rec088B7204 *end, const Rec088B7204 *key, Ord088B7204 order)
{
    int count = end - begin;
    int bottom = 0;
    int top = count - 1;
    while (!(top < bottom)) {
        int mid = (bottom + top) / 2;
        Rec088B7204 *r = &begin[mid];
        int c = Cmp088B7204::compare(r->id, key->id);
        if (c == 0) return r;
        if (c < 0) bottom = mid + 1;
        else top = mid - 1;
    }
    return 0;
}
