/* Binary search over 4-byte records by their first field (signed char); returns the record or 0. */
struct Rec088B72AC { signed char id; char pad[3]; };
struct Ord088B72AC { signed char descending; };
struct Cmp088B72AC {
    static int compare(int a, int b)
    {
        if (a == b) return 0;
        if (a < b) return -1;
        return 1;
    }
};
extern "C" Rec088B72AC *func_088B72AC(Rec088B72AC *begin, Rec088B72AC *end, const Rec088B72AC *key, Ord088B72AC order)
{
    int count = end - begin;
    int bottom = 0;
    int top = count - 1;
    while (!(top < bottom)) {
        int mid = (bottom + top) / 2;
        Rec088B72AC *r = &begin[mid];
        int c = Cmp088B72AC::compare(r->id, key->id);
        if (c == 0) return r;
        if (c < 0) bottom = mid + 1;
        else top = mid - 1;
    }
    return 0;
}
