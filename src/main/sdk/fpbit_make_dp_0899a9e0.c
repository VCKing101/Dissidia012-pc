/* libgcc fp-bit.c (dp-bit): __make_dp. */
#include "main/sdk_gcc1_types.h"

DFtype func_0899A9E0(fp_class_type class, unsigned int sign, int exp, UDItype frac)
{
    fp_number_type_d in;

    in.class = class;
    in.sign = sign;
    in.normal_exp = exp;
    in.fraction.ll = frac;
    return func_0899AD20(&in);
}
