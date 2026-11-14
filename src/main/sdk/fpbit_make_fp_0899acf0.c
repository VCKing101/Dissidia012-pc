/* libgcc fp-bit.c (sp-bit): __make_fp. */
#include "main/sdk_gcc1_types.h"

SFtype func_0899ACF0(fp_class_type class, unsigned int sign, int exp, USItype frac)
{
    fp_number_type_f in;

    in.class = class;
    in.sign = sign;
    in.normal_exp = exp;
    in.fraction.ll = frac;
    return func_0899B45C(&in);
}
