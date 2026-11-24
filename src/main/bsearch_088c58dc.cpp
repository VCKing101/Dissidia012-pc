/* Binary search over 20-byte records by their first field (short); returns the record or 0. */
struct Rec088C58DC { short id; char pad[18]; };
struct Ord088C58DC { signed char descending; };
struct Cmp088C58DC {
    static int compare(int a, int b)
    {
        if (a == b) return 0;
        if (a < b) return -1;
        return 1;
    }
};
extern "C" Rec088C58DC *func_088C58DC(Rec088C58DC *begin, Rec088C58DC *end, const Rec088C58DC *key, Ord088C58DC order)
{
    int count = end - begin;
    int bottom = 0;
    int top = count - 1;
    while (!(top < bottom)) {
        int mid = (bottom + top) / 2;
        Rec088C58DC *r = &begin[mid];
        int c = Cmp088C58DC::compare(r->id, key->id);
        if (c == 0) return r;
        if (c < 0) bottom = mid + 1;
        else top = mid - 1;
    }
    return 0;
}
