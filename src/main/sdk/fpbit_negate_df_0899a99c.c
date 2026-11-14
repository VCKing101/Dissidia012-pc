/* libgcc fp-bit.c (dp-bit): __negdf2. */
#include "main/sdk_gcc1_types.h"

DFtype func_0899A99C(DFtype arg_a)
{
    fp_number_type_d a;
    FLO_union_type_d au;

    au.value = arg_a;
    func_0899AFC0(&au, &a);

    flip_sign_d(&a);
    return func_0899AD20(&a);
}
