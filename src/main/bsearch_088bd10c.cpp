/* Binary search over 28-byte records by their first field (short); returns the record or 0. */
struct Rec088BD10C { short id; char pad[26]; };
struct Ord088BD10C { signed char descending; };
struct Cmp088BD10C {
    static int compare(int a, int b)
    {
        if (a == b) return 0;
        if (a < b) return -1;
        return 1;
    }
};
extern "C" Rec088BD10C *func_088BD10C(Rec088BD10C *begin, Rec088BD10C *end, const Rec088BD10C *key, Ord088BD10C order)
{
    int count = end - begin;
    int bottom = 0;
    int top = count - 1;
    while (!(top < bottom)) {
        int mid = (bottom + top) / 2;
        Rec088BD10C *r = &begin[mid];
        int c = Cmp088BD10C::compare(r->id, key->id);
        if (c == 0) return r;
        if (c < 0) bottom = mid + 1;
        else top = mid - 1;
    }
    return 0;
}
