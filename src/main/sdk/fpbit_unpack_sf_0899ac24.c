/* libgcc fp-bit.c (sp-bit): __unpack_f. */
#include "main/sdk_gcc1_types.h"

void func_0899AC24(FLO_union_type_f *src, fp_number_type_f *dst)
{
    USItype fraction;
    int exp;
    int sign;

    fraction = src->bits.fraction;
    exp = src->bits.exp;
    sign = src->bits.sign;

    dst->sign = sign;
    if (exp == 0) {
        /* Hmm.  Looks like 0 */
        if (fraction == 0) {
            /* tastes like zero */
            dst->class = CLASS_ZERO;
        } else {
            /* Zero exponent with nonzero fraction - it is denormalized,
               so there is no leading implicit one - shift it so
               it gets one. */
            dst->normal_exp = exp - F_EXPBIAS + 1;
            fraction <<= F_NGARDS;

            dst->class = CLASS_NUMBER;
            while (fraction < F_IMPLICIT_1) {
                fraction <<= 1;
                dst->normal_exp--;
            }
            dst->fraction.ll = fraction;
        }
    } else if (exp == F_EXPMAX) {
        /* Huge exponent*/
        if (fraction == 0) {
            /* Attached to a zero fraction - means infinity */
            dst->class = CLASS_INFINITY;
        } else {
            /* Nonzero fraction, means nan */
            if (fraction & F_QUIET_NAN) {
                dst->class = CLASS_QNAN;
            } else {
                dst->class = CLASS_SNAN;
            }
            /* Keep the fraction part as the nan number */
            dst->fraction.ll = fraction;
        }
    } else {
        /* Nothing strange about this number */
        dst->normal_exp = exp - F_EXPBIAS;
        dst->class = CLASS_NUMBER;
        dst->fraction.ll = (fraction << F_NGARDS) | F_IMPLICIT_1;
    }
}
