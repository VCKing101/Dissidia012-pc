/* Binary search over 16-byte records by their first field (short); returns the record or 0. */
struct Rec088CC034 { short id; char pad[14]; };
struct Ord088CC034 { signed char descending; };
struct Cmp088CC034 {
    static int compare(int a, int b)
    {
        if (a == b) return 0;
        if (a < b) return -1;
        return 1;
    }
};
extern "C" Rec088CC034 *func_088CC034(Rec088CC034 *begin, Rec088CC034 *end, const Rec088CC034 *key, Ord088CC034 order)
{
    int count = end - begin;
    int bottom = 0;
    int top = count - 1;
    while (!(top < bottom)) {
        int mid = (bottom + top) / 2;
        Rec088CC034 *r = &begin[mid];
        int c = Cmp088CC034::compare(r->id, key->id);
        if (c == 0) return r;
        if (c < 0) bottom = mid + 1;
        else top = mid - 1;
    }
    return 0;
}
