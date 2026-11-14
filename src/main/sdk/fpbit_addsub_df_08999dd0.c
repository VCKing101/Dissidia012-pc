/* libgcc fp-bit.c (dp-bit): _fpadd_parts, __adddf3, __subdf3. */
#include "main/sdk_gcc1_types.h"

fp_number_type_d *func_08999DD0(fp_number_type_d *a, fp_number_type_d *b, fp_number_type_d *tmp)
{
    DItype tfraction;

    /* Put commonly used fields in local variables. */
    int a_normal_exp;
    int b_normal_exp;
    UDItype a_fraction;
    UDItype b_fraction;

    if (isnan_d(a)) {
        return a;
    }
    if (isnan_d(b)) {
        return b;
    }
    if (isinf_d(a)) {
        /* Adding infinities with opposite signs yields a NaN. */
        if (isinf_d(b) && a->sign != b->sign)
            return nan_d();
        return a;
    }
    if (isinf_d(b)) {
        return b;
    }
    if (iszero_d(b)) {
        if (iszero_d(a)) {
            *tmp = *a;
            tmp->sign = a->sign & b->sign;
            return tmp;
        }
        return a;
    }
    if (iszero_d(a)) {
        return b;
    }

    /* Got two numbers. shift the smaller and increment the exponent till
       they are the same */
    {
        int diff;

        a_normal_exp = a->normal_exp;
        b_normal_exp = b->normal_exp;
        a_fraction = a->fraction.ll;
        b_fraction = b->fraction.ll;

        diff = a_normal_exp - b_normal_exp;

        if (diff < 0)
            diff = -diff;
        if (diff < D_FRAC_NBITS) {
            while (a_normal_exp > b_normal_exp) {
                b_normal_exp++;
                LSHIFT(b_fraction);
            }
            while (b_normal_exp > a_normal_exp) {
                a_normal_exp++;
                LSHIFT(a_fraction);
            }
        } else {
            /* Something is up.. choose the biggest */
            if (a_normal_exp > b_normal_exp) {
                b_normal_exp = a_normal_exp;
                b_fraction = 0;
            } else {
                a_normal_exp = b_normal_exp;
                a_fraction = 0;
            }
        }
    }

    if (a->sign != b->sign) {
        if (a->sign) {
            tfraction = -a_fraction + b_fraction;
        } else {
            tfraction = a_fraction - b_fraction;
        }
        if (tfraction >= 0) {
            tmp->sign = 0;
            tmp->normal_exp = a_normal_exp;
            tmp->fraction.ll = tfraction;
        } else {
            tmp->sign = 1;
            tmp->normal_exp = a_normal_exp;
            tmp->fraction.ll = -tfraction;
        }
        /* and renormalize it */

        while (tmp->fraction.ll < D_IMPLICIT_1 && tmp->fraction.ll) {
            tmp->fraction.ll <<= 1;
            tmp->normal_exp--;
        }
    } else {
        tmp->sign = a->sign;
        tmp->normal_exp = a_normal_exp;
        tmp->fraction.ll = a_fraction + b_fraction;
    }
    tmp->class = CLASS_NUMBER;
    /* Now the fraction is added, we have to shift down to renormalize the
       number */

    if (tmp->fraction.ll >= D_IMPLICIT_2) {
        LSHIFT(tmp->fraction.ll);
        tmp->normal_exp++;
    }
    return tmp;
}

DFtype func_0899A118(DFtype arg_a, DFtype arg_b)
{
    fp_number_type_d a;
    fp_number_type_d b;
    fp_number_type_d tmp;
    fp_number_type_d *res;
    FLO_union_type_d au, bu;

    au.value = arg_a;
    bu.value = arg_b;

    func_0899AFC0(&au, &a);
    func_0899AFC0(&bu, &b);

    res = func_08999DD0(&a, &b, &tmp);

    return func_0899AD20(res);
}

DFtype func_0899A180(DFtype arg_a, DFtype arg_b)
{
    fp_number_type_d a;
    fp_number_type_d b;
    fp_number_type_d tmp;
    fp_number_type_d *res;
    FLO_union_type_d au, bu;

    au.value = arg_a;
    bu.value = arg_b;

    func_0899AFC0(&au, &a);
    func_0899AFC0(&bu, &b);

    b.sign ^= 1;

    res = func_08999DD0(&a, &b, &tmp);

    return func_0899AD20(res);
}
