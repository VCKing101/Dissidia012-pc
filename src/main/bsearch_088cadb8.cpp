/* Binary search over 36-byte records by their first field (int); returns the record or 0. */
struct Rec088CADB8 { int id; char pad[32]; };
struct Ord088CADB8 { signed char descending; };
struct Cmp088CADB8 {
    static int compare(int a, int b)
    {
        if (a == b) return 0;
        if (a < b) return -1;
        return 1;
    }
};
extern "C" Rec088CADB8 *func_088CADB8(Rec088CADB8 *begin, Rec088CADB8 *end, const Rec088CADB8 *key, Ord088CADB8 order)
{
    int count = end - begin;
    int bottom = 0;
    int top = count - 1;
    while (!(top < bottom)) {
        int mid = (bottom + top) / 2;
        Rec088CADB8 *r = &begin[mid];
        int c = Cmp088CADB8::compare(r->id, key->id);
        if (c == 0) return r;
        if (c < 0) bottom = mid + 1;
        else top = mid - 1;
    }
    return 0;
}
