/* Binary-searches a sorted array of 16-byte ranges for the one that
   overlaps [key->lo, key->hi]; returns it or 0. */
struct Range16 {
    int lo;
    int hi;
    int a;
    int b;

    static int Compare(Range16 *r, Range16 *key)
    {
        if (r->hi < key->lo) {
            return -1;
        }
        if (key->hi < r->lo) {
            return 1;
        }
        return 0;
    }
};

extern "C" {

Range16 *func_088A41A4(Range16 *begin, Range16 *end, Range16 *key)
{
    int count = end - begin;
    int bottom = 0;
    int top = count - 1;

    while (bottom <= top) {
        int mid = (bottom + top) / 2;
        Range16 *r = &begin[mid];
        int c = Range16::Compare(r, key);
        if (c == 0) {
            return r;
        }
        if (c < 0) {
            bottom = mid + 1;
        } else {
            top = mid - 1;
        }
    }
    return 0;
}

}
