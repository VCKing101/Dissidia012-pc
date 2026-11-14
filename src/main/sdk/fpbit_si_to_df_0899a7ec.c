/* libgcc fp-bit.c (dp-bit): __floatsidf, int to double. */
#include "main/sdk_gcc1_types.h"

/* (DFtype)(-MAX_SI_INT - 1), from the constant pool */
extern const DFtype D_089D27D0;

DFtype func_0899A7EC(SItype arg_a)
{
    fp_number_type_d in;

    in.class = CLASS_NUMBER;
    in.sign = arg_a < 0;
    if (!arg_a) {
        in.class = CLASS_ZERO;
    } else {
        in.normal_exp = D_FRACBITS + D_NGARDS;
        if (in.sign) {
            /* Special case for minint, since there is no +ve integer
               representation for it */
            if (arg_a == (-MAX_SI_INT - 1)) {
                return D_089D27D0;
            }
            in.fraction.ll = (-arg_a);
        } else
            in.fraction.ll = arg_a;

        while (in.fraction.ll < ((UDItype)1 << (D_FRACBITS + D_NGARDS))) {
            in.fraction.ll <<= 1;
            in.normal_exp -= 1;
        }
    }
    return func_0899AD20(&in);
}
