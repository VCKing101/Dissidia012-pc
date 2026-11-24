/* Binary search over 18-byte records by their first field (short); returns the record or 0. */
struct Rec088C5CE8 { short id; char pad[16]; };
struct Ord088C5CE8 { signed char descending; };
struct Cmp088C5CE8 {
    static int compare(int a, int b)
    {
        if (a == b) return 0;
        if (a < b) return -1;
        return 1;
    }
};
extern "C" Rec088C5CE8 *func_088C5CE8(Rec088C5CE8 *begin, Rec088C5CE8 *end, const Rec088C5CE8 *key, Ord088C5CE8 order)
{
    int count = end - begin;
    int bottom = 0;
    int top = count - 1;
    while (!(top < bottom)) {
        int mid = (bottom + top) / 2;
        Rec088C5CE8 *r = &begin[mid];
        int c = Cmp088C5CE8::compare(r->id, key->id);
        if (c == 0) return r;
        if (c < 0) bottom = mid + 1;
        else top = mid - 1;
    }
    return 0;
}
