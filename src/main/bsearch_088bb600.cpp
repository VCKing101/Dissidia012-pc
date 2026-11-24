/* Binary search over 20-byte records by their first field (short); returns the record or 0. */
struct Rec088BB600 { short id; char pad[18]; };
struct Ord088BB600 { signed char descending; };
struct Cmp088BB600 {
    static int compare(int a, int b)
    {
        if (a == b) return 0;
        if (a < b) return -1;
        return 1;
    }
};
extern "C" Rec088BB600 *func_088BB600(Rec088BB600 *begin, Rec088BB600 *end, const Rec088BB600 *key, Ord088BB600 order)
{
    int count = end - begin;
    int bottom = 0;
    int top = count - 1;
    while (!(top < bottom)) {
        int mid = (bottom + top) / 2;
        Rec088BB600 *r = &begin[mid];
        int c = Cmp088BB600::compare(r->id, key->id);
        if (c == 0) return r;
        if (c < 0) bottom = mid + 1;
        else top = mid - 1;
    }
    return 0;
}
