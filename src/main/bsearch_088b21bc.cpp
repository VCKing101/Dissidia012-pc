/* Binary search over 8-byte records by their first field (short); returns the record or 0. */
struct Rec088B21BC { short id; char pad[6]; };
struct Ord088B21BC { signed char descending; };
struct Cmp088B21BC {
    static int compare(int a, int b)
    {
        if (a == b) return 0;
        if (a < b) return -1;
        return 1;
    }
};
extern "C" Rec088B21BC *func_088B21BC(Rec088B21BC *begin, Rec088B21BC *end, const Rec088B21BC *key, Ord088B21BC order)
{
    int count = end - begin;
    int bottom = 0;
    int top = count - 1;
    while (!(top < bottom)) {
        int mid = (bottom + top) / 2;
        Rec088B21BC *r = &begin[mid];
        int c = Cmp088B21BC::compare(r->id, key->id);
        if (c == 0) return r;
        if (c < 0) bottom = mid + 1;
        else top = mid - 1;
    }
    return 0;
}
