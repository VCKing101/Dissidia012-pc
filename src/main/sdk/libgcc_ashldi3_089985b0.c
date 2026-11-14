/* libgcc2: __ashldi3, 64-bit shift left. */

typedef int SItype;
typedef unsigned int USItype;
typedef long long DItype;

struct DWstruct {
    SItype low, high;
};

typedef union {
    struct DWstruct s;
    DItype ll;
} DWunion;

DItype func_089985B0(DItype u, SItype b)
{
    DWunion w;
    SItype bm;
    DWunion uu;

    if (b == 0)
        return u;

    uu.ll = u;

    bm = (sizeof(SItype) * 8) - b;
    if (bm <= 0) {
        w.s.low = 0;
        w.s.high = (USItype)uu.s.low << -bm;
    } else {
        USItype carries = (USItype)uu.s.low >> bm;
        w.s.low = (USItype)uu.s.low << b;
        w.s.high = ((USItype)uu.s.high << b) | carries;
    }

    return w.ll;
}
