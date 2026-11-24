// Finds the smallest byte at offset 1 (capped at 100) of func_088CB164(a, D_089CBB7A[i]) over 31 entries; out[0] = min, out[1] = *limit; returns limit <= min.
struct Mx44 { unsigned min; unsigned limit; };
extern "C" signed char D_089CBB7A[];
extern "C" unsigned char *func_088CB164(int, int);
extern "C" int func_088C329C(Mx44 *out, int a, unsigned *limit)
{
    int best = 100;
    for (int i = 0; i < 0x1F; i++) {
        unsigned char *e = func_088CB164(a, D_089CBB7A[i]);
        if (e[1] < best) best = e[1];
    }
    out->min = best;
    unsigned lim = *limit;
    out->limit = lim;
    return (int)lim <= best;
}
