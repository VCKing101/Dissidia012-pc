/* Binary search over 4-byte records by their first field (short); returns the record or 0. */
struct Rec088B715C { short id; char pad[2]; };
struct Ord088B715C { signed char descending; };
struct Cmp088B715C {
    static int compare(int a, int b)
    {
        if (a == b) return 0;
        if (a < b) return -1;
        return 1;
    }
};
extern "C" Rec088B715C *func_088B715C(Rec088B715C *begin, Rec088B715C *end, const Rec088B715C *key, Ord088B715C order)
{
    int count = end - begin;
    int bottom = 0;
    int top = count - 1;
    while (!(top < bottom)) {
        int mid = (bottom + top) / 2;
        Rec088B715C *r = &begin[mid];
        int c = Cmp088B715C::compare(r->id, key->id);
        if (c == 0) return r;
        if (c < 0) bottom = mid + 1;
        else top = mid - 1;
    }
    return 0;
}
