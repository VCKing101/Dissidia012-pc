/* libgcc fp-bit.c (dp-bit): __muldf3 with _fpmul_parts inlined. */
#include "main/sdk_gcc1_types.h"

static __inline__ __attribute__((__always_inline__)) fp_number_type_d *
_fpmul_parts(fp_number_type_d *a, fp_number_type_d *b, fp_number_type_d *tmp)
{
    UDItype low = 0;
    UDItype high = 0;

    if (isnan_d(a)) {
        a->sign = a->sign != b->sign;
        return a;
    }
    if (isnan_d(b)) {
        b->sign = a->sign != b->sign;
        return b;
    }
    if (isinf_d(a)) {
        if (iszero_d(b))
            return nan_d();
        a->sign = a->sign != b->sign;
        return a;
    }
    if (isinf_d(b)) {
        if (iszero_d(a)) {
            return nan_d();
        }
        b->sign = a->sign != b->sign;
        return b;
    }
    if (iszero_d(a)) {
        a->sign = a->sign != b->sign;
        return a;
    }
    if (iszero_d(b)) {
        b->sign = a->sign != b->sign;
        return b;
    }

    /* fractype is DImode, but we need the result to be twice as wide.
       Assuming a widening multiply from DImode to TImode is not
       available, build one by hand. */
    {
        USItype nl = a->fraction.ll;
        USItype nh = a->fraction.ll >> BITS_PER_SI;
        USItype ml = b->fraction.ll;
        USItype mh = b->fraction.ll >> BITS_PER_SI;
        UDItype pp_ll = (UDItype)ml * nl;
        UDItype pp_hl = (UDItype)mh * nl;
        UDItype pp_lh = (UDItype)ml * nh;
        UDItype pp_hh = (UDItype)mh * nh;
        UDItype res2 = 0;
        UDItype res0 = 0;
        UDItype ps_hh__ = pp_hl + pp_lh;
        if (ps_hh__ < pp_hl)
            res2 += (UDItype)1 << BITS_PER_SI;
        pp_hl = (UDItype)(USItype)ps_hh__ << BITS_PER_SI;
        res0 = pp_ll + pp_hl;
        if (res0 < pp_ll)
            res2++;
        res2 += (ps_hh__ >> BITS_PER_SI) + pp_hh;
        high = res2;
        low = res0;
    }

    tmp->normal_exp = a->normal_exp + b->normal_exp + D_FRAC_NBITS - (D_FRACBITS + D_NGARDS);
    tmp->sign = a->sign != b->sign;
    while (high >= D_IMPLICIT_2) {
        tmp->normal_exp++;
        if (high & 1) {
            low >>= 1;
            low |= 0x8000000000000000LL;
        }
        high >>= 1;
    }
    while (high < D_IMPLICIT_1) {
        tmp->normal_exp--;

        high <<= 1;
        if (low & 0x8000000000000000LL)
            high |= 1;
        low <<= 1;
    }
    /* rounding is tricky. if we only round if it will not make us round later. */
    if ((high & D_GARDMASK) == D_GARDMSB) {
        if (high & (1 << D_NGARDS)) {
            /* half way, so round to even */
            high += D_GARDROUND + 1;
        } else if (low) {
            /* but we really were not half way */
            high += D_GARDROUND + 1;
        }
    }
    tmp->fraction.ll = high;
    tmp->class = CLASS_NUMBER;
    return tmp;
}

DFtype func_0899A1F4(DFtype arg_a, DFtype arg_b)
{
    fp_number_type_d a;
    fp_number_type_d b;
    fp_number_type_d tmp;
    fp_number_type_d *res;
    FLO_union_type_d au, bu;

    au.value = arg_a;
    bu.value = arg_b;

    func_0899AFC0(&au, &a);
    func_0899AFC0(&bu, &b);

    res = _fpmul_parts(&a, &b, &tmp);

    return func_0899AD20(res);
}
