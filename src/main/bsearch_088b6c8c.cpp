/* Binary search over 16-byte records by their first field (int); returns the record or 0. */
struct Rec088B6C8C { int id; char pad[12]; };
struct Ord088B6C8C { signed char descending; };
struct Cmp088B6C8C {
    static int compare(int a, int b)
    {
        if (a == b) return 0;
        if (a < b) return -1;
        return 1;
    }
};
extern "C" Rec088B6C8C *func_088B6C8C(Rec088B6C8C *begin, Rec088B6C8C *end, const Rec088B6C8C *key, Ord088B6C8C order)
{
    int count = end - begin;
    int bottom = 0;
    int top = count - 1;
    while (!(top < bottom)) {
        int mid = (bottom + top) / 2;
        Rec088B6C8C *r = &begin[mid];
        int c = Cmp088B6C8C::compare(r->id, key->id);
        if (c == 0) return r;
        if (c < 0) bottom = mid + 1;
        else top = mid - 1;
    }
    return 0;
}
