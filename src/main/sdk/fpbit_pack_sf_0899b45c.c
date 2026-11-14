/* libgcc fp-bit.c (sp-bit): __pack_f. */
#include "main/sdk_gcc1_types.h"

SFtype func_0899B45C(fp_number_type_f *src)
{
    FLO_union_type_f dst;
    USItype fraction = src->fraction.ll;
    int sign = src->sign;
    int exp = 0;

    if (isnan_f(src)) {
        exp = F_EXPMAX;
        if (src->class == CLASS_QNAN || 1) {
            fraction |= F_QUIET_NAN;
        }
    } else if (isinf_f(src)) {
        exp = F_EXPMAX;
        fraction = 0;
    } else if (iszero_f(src)) {
        exp = 0;
        fraction = 0;
    } else if (fraction == 0) {
        exp = 0;
    } else {
        if (src->normal_exp < (-(F_EXPBIAS) + 1)) {
            /* This number's exponent is too low to fit into the bits
               available in the number, so store 0 in the exponent and
               shift the fraction to the right to make up for it. */

            int shift = (-(F_EXPBIAS) + 1) - src->normal_exp;

            exp = 0;

            if (shift > F_FRAC_NBITS - F_NGARDS) {
                /* No point shifting, since it is more that 64 out. */
                fraction = 0;
            } else {
                int lowbit = (fraction & (((USItype)1 << shift) - 1)) ? 1 : 0;
                fraction = (fraction >> shift) | lowbit;
            }
            if ((fraction & F_GARDMASK) == F_GARDMSB) {
                if ((fraction & (1 << F_NGARDS)))
                    fraction += F_GARDROUND + 1;
            } else {
                /* Add to the guards to round up. */
                fraction += F_GARDROUND;
            }
            /* Perhaps the rounding means we now need to change the
               exponent, because the fraction is no longer denormal. */
            if (fraction >= F_IMPLICIT_1) {
                exp += 1;
            }
            fraction >>= F_NGARDS;
        } else if (src->normal_exp > F_EXPBIAS) {
            exp = F_EXPMAX;
            fraction = 0;
        } else {
            exp = src->normal_exp + F_EXPBIAS;
            /* IF the gard bits are the all zero, but the first, then we are
               half way between two numbers, choose the one which makes the
               lsb of the answer 0. */
            if ((fraction & F_GARDMASK) == F_GARDMSB) {
                if (fraction & (1 << F_NGARDS))
                    fraction += F_GARDROUND + 1;
            } else {
                /* Add a one to the guards to round up */
                fraction += F_GARDROUND;
            }
            if (fraction >= F_IMPLICIT_2) {
                fraction >>= 1;
                exp += 1;
            }
            fraction >>= F_NGARDS;
        }
    }

    dst.bits.fraction = fraction;
    dst.bits.exp = exp;
    dst.bits.sign = sign;

    return dst.value;
}
