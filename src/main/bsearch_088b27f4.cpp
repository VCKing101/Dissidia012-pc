/* Binary search over 8-byte records by their first field (short); returns the record or 0. */
struct Rec088B27F4 { short id; char pad[6]; };
struct Ord088B27F4 { signed char descending; };
struct Cmp088B27F4 {
    static int compare(int a, int b)
    {
        if (a == b) return 0;
        if (a < b) return -1;
        return 1;
    }
};
extern "C" Rec088B27F4 *func_088B27F4(Rec088B27F4 *begin, Rec088B27F4 *end, const Rec088B27F4 *key, Ord088B27F4 order)
{
    int count = end - begin;
    int bottom = 0;
    int top = count - 1;
    while (!(top < bottom)) {
        int mid = (bottom + top) / 2;
        Rec088B27F4 *r = &begin[mid];
        int c = Cmp088B27F4::compare(r->id, key->id);
        if (c == 0) return r;
        if (c < 0) bottom = mid + 1;
        else top = mid - 1;
    }
    return 0;
}
