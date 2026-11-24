/* Binary search over 52-byte records by their first field (short); returns the record or 0. */
struct Rec088C0CC0 { short id; char pad[50]; };
struct Ord088C0CC0 { signed char descending; };
struct Cmp088C0CC0 {
    static int compare(int a, int b)
    {
        if (a == b) return 0;
        if (a < b) return -1;
        return 1;
    }
};
extern "C" Rec088C0CC0 *func_088C0CC0(Rec088C0CC0 *begin, Rec088C0CC0 *end, const Rec088C0CC0 *key, Ord088C0CC0 order)
{
    int count = end - begin;
    int bottom = 0;
    int top = count - 1;
    while (!(top < bottom)) {
        int mid = (bottom + top) / 2;
        Rec088C0CC0 *r = &begin[mid];
        int c = Cmp088C0CC0::compare(r->id, key->id);
        if (c == 0) return r;
        if (c < 0) bottom = mid + 1;
        else top = mid - 1;
    }
    return 0;
}
