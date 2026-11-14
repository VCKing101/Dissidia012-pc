/* libgcc fp-bit.c (dp-bit): __truncdfsf2, double to float. */
#include "main/sdk_gcc1_types.h"

SFtype func_0899AA14(DFtype arg_a)
{
    fp_number_type_d in;
    USItype sffrac;
    FLO_union_type_d au;

    au.value = arg_a;
    func_0899AFC0(&au, &in);

    sffrac = in.fraction.ll >> F_D_BITOFF;

    /* We set the lowest guard bit in SFFRAC if we discarded any non-zero
       bits. */
    if ((in.fraction.ll & (((USItype)1 << F_D_BITOFF) - 1)) != 0)
        sffrac |= 1;

    return func_0899ACF0(in.class, in.sign, in.normal_exp, sffrac);
}
