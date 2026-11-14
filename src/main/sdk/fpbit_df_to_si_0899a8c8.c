/* libgcc fp-bit.c (dp-bit): __fixdfsi, double to int. */
#include "main/sdk_gcc1_types.h"

SItype func_0899A8C8(DFtype arg_a)
{
    fp_number_type_d a;
    SItype tmp;
    FLO_union_type_d au;

    au.value = arg_a;
    func_0899AFC0(&au, &a);

    if (iszero_d(&a))
        return 0;
    if (isnan_d(&a))
        return 0;
    /* get reasonable MAX_SI_INT... */
    if (isinf_d(&a))
        return a.sign ? (-MAX_SI_INT) - 1 : MAX_SI_INT;
    /* it is a number, but a small one */
    if (a.normal_exp < 0)
        return 0;
    if (a.normal_exp > BITS_PER_SI - 2)
        return a.sign ? (-MAX_SI_INT) - 1 : MAX_SI_INT;
    tmp = a.fraction.ll >> ((D_FRACBITS + D_NGARDS) - a.normal_exp);
    return a.sign ? (-tmp) : (tmp);
}
