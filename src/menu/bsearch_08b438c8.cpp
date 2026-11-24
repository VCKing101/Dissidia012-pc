/* Binary search over 48-byte records by their first field (unsigned short); returns the record or 0. */
struct Rec08b438c8 { unsigned short id; char pad[48 - 2]; };
struct Cmp08b438c8 {
    static int compare(int a, int b)
    {
        if (a == b) return 0;
        if (a < b) return -1;
        return 1;
    }
};
extern "C" Rec08b438c8 *func_08B438C8(Rec08b438c8 *begin, Rec08b438c8 *end, const Rec08b438c8 *key)
{
    int count = end - begin;
    int bottom = 0;
    int top = count - 1;
    while (!(top < bottom)) {
        int mid = (bottom + top) / 2;
        Rec08b438c8 *r = &begin[mid];
        int c = Cmp08b438c8::compare(r->id, key->id);
        if (c == 0) return r;
        if (c < 0) bottom = mid + 1;
        else top = mid - 1;
    }
    return 0;
}
