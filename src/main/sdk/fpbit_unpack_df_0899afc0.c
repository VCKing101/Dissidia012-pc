/* libgcc fp-bit.c (dp-bit): __unpack_d. */
#include "main/sdk_gcc1_types.h"

void func_0899AFC0(FLO_union_type_d *src, fp_number_type_d *dst)
{
    UDItype fraction;
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
            dst->normal_exp = exp - D_EXPBIAS + 1;
            fraction <<= D_NGARDS;

            dst->class = CLASS_NUMBER;
            while (fraction < D_IMPLICIT_1) {
                fraction <<= 1;
                dst->normal_exp--;
            }
            dst->fraction.ll = fraction;
        }
    } else if (exp == D_EXPMAX) {
        /* Huge exponent*/
        if (fraction == 0) {
            /* Attached to a zero fraction - means infinity */
            dst->class = CLASS_INFINITY;
        } else {
            /* Nonzero fraction, means nan */
            if (fraction & D_QUIET_NAN) {
                dst->class = CLASS_QNAN;
            } else {
                dst->class = CLASS_SNAN;
            }
            /* Keep the fraction part as the nan number */
            dst->fraction.ll = fraction;
        }
    } else {
        /* Nothing strange about this number */
        dst->normal_exp = exp - D_EXPBIAS;
        dst->class = CLASS_NUMBER;
        dst->fraction.ll = (fraction << D_NGARDS) | D_IMPLICIT_1;
    }
}
