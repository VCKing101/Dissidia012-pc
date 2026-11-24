/* Binary search over 200-byte records by their first field (short); returns the record or 0. */
struct Rec088C561C { short id; char pad[198]; };
struct Ord088C561C { signed char descending; };
struct Cmp088C561C {
    static int compare(int a, int b)
    {
        if (a == b) return 0;
        if (a < b) return -1;
        return 1;
    }
};
extern "C" Rec088C561C *func_088C561C(Rec088C561C *begin, Rec088C561C *end, const Rec088C561C *key, Ord088C561C order)
{
    int count = end - begin;
    int bottom = 0;
    int top = count - 1;
    while (!(top < bottom)) {
        int mid = (bottom + top) / 2;
        Rec088C561C *r = &begin[mid];
        int c = Cmp088C561C::compare(r->id, key->id);
        if (c == 0) return r;
        if (c < 0) bottom = mid + 1;
        else top = mid - 1;
    }
    return 0;
}
