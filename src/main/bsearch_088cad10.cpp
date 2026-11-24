/* Binary search over 8-byte records by their first field (short); returns the record or 0. */
struct Rec088CAD10 { short id; char pad[6]; };
struct Ord088CAD10 { signed char descending; };
struct Cmp088CAD10 {
    static int compare(int a, int b)
    {
        if (a == b) return 0;
        if (a < b) return -1;
        return 1;
    }
};
extern "C" Rec088CAD10 *func_088CAD10(Rec088CAD10 *begin, Rec088CAD10 *end, const Rec088CAD10 *key, Ord088CAD10 order)
{
    int count = end - begin;
    int bottom = 0;
    int top = count - 1;
    while (!(top < bottom)) {
        int mid = (bottom + top) / 2;
        Rec088CAD10 *r = &begin[mid];
        int c = Cmp088CAD10::compare(r->id, key->id);
        if (c == 0) return r;
        if (c < 0) bottom = mid + 1;
        else top = mid - 1;
    }
    return 0;
}
