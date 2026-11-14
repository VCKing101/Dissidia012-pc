/* std::sort over 12-byte records: introsort to depth 2*lg(n), then insertion sort. */
struct SortRecord {
    char data[0xC];
};

struct SortLess {
    signed char unused;
};

struct SortMath {
    static int lg(int n)
    {
        int k;
        for (k = 0; n != 1; n >>= 1) {
            ++k;
        }
        return k;
    }
};

extern "C" {

extern void func_08AF1184(SortRecord *first, SortRecord *last, SortRecord *type, int depth, SortLess less);
extern void func_08AF1A3C(SortRecord *first, SortRecord *last, SortLess less);

void func_08AF0E84(SortRecord *first, SortRecord *last, SortLess less)
{
    if (first != last) {
        func_08AF1184(first, last, 0, SortMath::lg(last - first) * 2, less);
        func_08AF1A3C(first, last, less);
    }
}

}
