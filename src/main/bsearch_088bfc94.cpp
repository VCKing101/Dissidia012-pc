/* Binary search over 8-byte records by their first field (short); returns the record or 0. */
struct Rec088BFC94 { short id; char pad[6]; };
struct Ord088BFC94 { signed char descending; };
struct Cmp088BFC94 {
    static int compare(int a, int b)
    {
        if (a == b) return 0;
        if (a < b) return -1;
        return 1;
    }
};
extern "C" Rec088BFC94 *func_088BFC94(Rec088BFC94 *begin, Rec088BFC94 *end, const Rec088BFC94 *key, Ord088BFC94 order)
{
    int count = end - begin;
    int bottom = 0;
    int top = count - 1;
    while (!(top < bottom)) {
        int mid = (bottom + top) / 2;
        Rec088BFC94 *r = &begin[mid];
        int c = Cmp088BFC94::compare(r->id, key->id);
        if (c == 0) return r;
        if (c < 0) bottom = mid + 1;
        else top = mid - 1;
    }
    return 0;
}
