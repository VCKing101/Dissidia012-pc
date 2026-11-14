/* std::__final_insertion_sort over 12-byte records. */
struct SortRecord {
    char data[0xC];
};

struct SortLess {
    signed char unused;
};

extern "C" {
extern void func_08AF1ACC(SortRecord *first, SortRecord *last, SortLess less);
extern void func_08AF1D9C(SortRecord *first, SortRecord *last, SortRecord *type, SortLess less);
}

struct SortOps {
    static void unguarded(SortRecord *first, SortRecord *last, SortLess less)
    {
        func_08AF1D9C(first, last, 0, less);
    }
};

extern "C" {

void func_08AF1A3C(SortRecord *first, SortRecord *last, SortLess less)
{
    if (last - first > 16) {
        func_08AF1ACC(first, first + 16, less);
        SortOps::unguarded(first + 16, last, less);
    } else {
        func_08AF1ACC(first, last, less);
    }
}

}
