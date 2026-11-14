struct SortRecord {
    char data[0xC];
};

struct SortLess {
    signed char unused;
};

extern "C" {

extern void func_08AF12C0(SortRecord *first, SortRecord *last, SortRecord *type, int depth, SortLess less);

void func_08AF129C(SortRecord *first, SortRecord *last, SortRecord *type, SortLess less)
{
    func_08AF12C0(first, last, type, 0, less);
}

}
