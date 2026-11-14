/* libgcc fp-bit.c (dp-bit): __ltdf2. */
#include "main/sdk_gcc1_types.h"

CMPtype func_0899B240(DFtype arg_a, DFtype arg_b)
{
    fp_number_type_d a;
    fp_number_type_d b;
    FLO_union_type_d au, bu;

    au.value = arg_a;
    bu.value = arg_b;

    func_0899AFC0(&au, &a);
    func_0899AFC0(&bu, &b);

    if (isnan_d(&a) || isnan_d(&b))
        return 1; /* false, truth < 0 */

    return func_0899B0DC(&a, &b);
}
