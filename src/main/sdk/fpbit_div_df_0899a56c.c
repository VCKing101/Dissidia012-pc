/* libgcc fp-bit.c (dp-bit): __divdf3 with _fpdiv_parts inlined. */
#include "main/sdk_gcc1_types.h"

static __inline__ __attribute__((__always_inline__)) fp_number_type_d *
_fpdiv_parts(fp_number_type_d *a, fp_number_type_d *b)
{
    UDItype bit;
    UDItype numerator;
    UDItype denominator;
    UDItype quotient;

    if (isnan_d(a)) {
        return a;
    }
    if (isnan_d(b)) {
        return b;
    }

    a->sign = a->sign ^ b->sign;

    if (isinf_d(a) || iszero_d(a)) {
        if (a->class == b->class)
            return nan_d();
        return a;
    }

    if (isinf_d(b)) {
        a->fraction.ll = 0;
        a->normal_exp = 0;
        return a;
    }
    if (iszero_d(b)) {
        a->class = CLASS_INFINITY;
        return a;
    }

    /* Calculate the mantissa by multiplying both 64bit numbers to get a
       128 bit number */
    {
        /* quotient =
           ( numerator / denominator) * 2^(numerator exponent -  denominator exponent)
         */

        a->normal_exp = a->normal_exp - b->normal_exp;
        numerator = a->fraction.ll;
        denominator = b->fraction.ll;

        if (numerator < denominator) {
            /* Fraction will be less than 1.0 */
            numerator *= 2;
            a->normal_exp--;
        }
        bit = D_IMPLICIT_1;
        quotient = 0;
        /* ??? Does divide one bit at a time.  Optimize. */
        while (bit) {
            if (numerator >= denominator) {
                quotient |= bit;
                numerator -= denominator;
            }
            bit >>= 1;
            numerator *= 2;
        }

        if ((quotient & D_GARDMASK) == D_GARDMSB) {
            if (quotient & (1 << D_NGARDS)) {
                /* half way, so round to even */
                quotient += D_GARDROUND + 1;
            } else if (numerator) {
                /* but we really were not half way, more bits exist */
                quotient += D_GARDROUND + 1;
            }
        }

        a->fraction.ll = quotient;
        return (a);
    }
}

DFtype func_0899A56C(DFtype arg_a, DFtype arg_b)
{
    fp_number_type_d a;
    fp_number_type_d b;
    fp_number_type_d *res;
    FLO_union_type_d au, bu;

    au.value = arg_a;
    bu.value = arg_b;

    func_0899AFC0(&au, &a);
    func_0899AFC0(&bu, &b);

    res = _fpdiv_parts(&a, &b);

    return func_0899AD20(res);
}
