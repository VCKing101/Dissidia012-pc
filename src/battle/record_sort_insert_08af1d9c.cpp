/* std::__unguarded_insertion_sort over 12-byte records. */
struct SortRecord {
    char data[0xC];
};

struct SortLess {
    signed char unused;
};

extern "C" {

extern void func_08AF1CB8(SortRecord *position, SortRecord *value, SortLess less);

void func_08AF1D9C(SortRecord *first, SortRecord *last, SortRecord *type, SortLess less)
{
    for (SortRecord *i = first; i != last; ++i) {
        func_08AF1CB8(i, i, less);
    }
}

}
