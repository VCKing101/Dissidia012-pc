/* Binary search over 2-byte records by their first field (signed char); returns the record or 0. */
struct Rec088B7354 { signed char id; char pad[1]; };
struct Ord088B7354 { signed char descending; };
struct Cmp088B7354 {
    static int compare(int a, int b)
    {
        if (a == b) return 0;
        if (a < b) return -1;
        return 1;
    }
};
extern "C" Rec088B7354 *func_088B7354(Rec088B7354 *begin, Rec088B7354 *end, const Rec088B7354 *key, Ord088B7354 order)
{
    int count = end - begin;
    int bottom = 0;
    int top = count - 1;
    while (!(top < bottom)) {
        int mid = (bottom + top) / 2;
        Rec088B7354 *r = &begin[mid];
        int c = Cmp088B7354::compare(r->id, key->id);
        if (c == 0) return r;
        if (c < 0) bottom = mid + 1;
        else top = mid - 1;
    }
    return 0;
}
