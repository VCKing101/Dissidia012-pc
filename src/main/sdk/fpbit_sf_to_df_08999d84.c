/* libgcc fp-bit.c (sp-bit): __extendsfdf2, float to double. */
#include "main/sdk_gcc1_types.h"

DFtype func_08999D84(SFtype arg_a)
{
    fp_number_type_f in;
    FLO_union_type_f au;

    au.value = arg_a;
    func_0899AC24(&au, &in);

    return func_0899A9E0(in.class, in.sign, in.normal_exp,
                         ((UDItype)in.fraction.ll) << F_D_BITOFF);
}
