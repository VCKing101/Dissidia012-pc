/* Binary search over 40-byte records by their first field (short); returns the record or 0. */
struct Rec088B1C14 { short id; char pad[38]; };
struct Ord088B1C14 { signed char descending; };
struct Cmp088B1C14 {
    static int compare(int a, int b)
    {
        if (a == b) return 0;
        if (a < b) return -1;
        return 1;
    }
};
extern "C" Rec088B1C14 *func_088B1C14(Rec088B1C14 *begin, Rec088B1C14 *end, const Rec088B1C14 *key, Ord088B1C14 order)
{
    int count = end - begin;
    int bottom = 0;
    int top = count - 1;
    while (!(top < bottom)) {
        int mid = (bottom + top) / 2;
        Rec088B1C14 *r = &begin[mid];
        int c = Cmp088B1C14::compare(r->id, key->id);
        if (c == 0) return r;
        if (c < 0) bottom = mid + 1;
        else top = mid - 1;
    }
    return 0;
}
