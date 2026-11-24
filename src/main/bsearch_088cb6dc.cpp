/* Binary search over 48-byte records by their first field (short); returns the record or 0. */
struct Rec088CB6DC { short id; char pad[46]; };
struct Ord088CB6DC { signed char descending; };
struct Cmp088CB6DC {
    static int compare(int a, int b)
    {
        if (a < b) return -1;
        if (b < a) return 1;
        return 0;
    }
};
extern "C" Rec088CB6DC *func_088CB6DC(Rec088CB6DC *begin, Rec088CB6DC *end, const Rec088CB6DC *key, Ord088CB6DC order)
{
    int count = end - begin;
    int bottom = 0;
    int top = count - 1;
    while (!(top < bottom)) {
        int mid = (bottom + top) / 2;
        Rec088CB6DC *r = &begin[mid];
        int c = Cmp088CB6DC::compare(r->id, key->id);
        if (c == 0) return r;
        if (c < 0) bottom = mid + 1;
        else top = mid - 1;
    }
    return 0;
}
