/* Binary search over 388-byte records by their first field (short); returns the record or 0. */
struct Rec088C582C { short id; char pad[386]; };
struct Ord088C582C { signed char descending; };
struct Cmp088C582C {
    static int compare(int a, int b)
    {
        if (a == b) return 0;
        if (a < b) return -1;
        return 1;
    }
};
extern "C" Rec088C582C *func_088C582C(Rec088C582C *begin, Rec088C582C *end, const Rec088C582C *key, Ord088C582C order)
{
    int count = end - begin;
    int bottom = 0;
    int top = count - 1;
    while (!(top < bottom)) {
        int mid = (bottom + top) / 2;
        Rec088C582C *r = &begin[mid];
        int c = Cmp088C582C::compare(r->id, key->id);
        if (c == 0) return r;
        if (c < 0) bottom = mid + 1;
        else top = mid - 1;
    }
    return 0;
}
