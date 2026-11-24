// Finds the largest byte at offset 1 of func_088CB164(a, D_089CBB7A[i]) over 31 entries; out[0] = max, out[1] = *limit; returns limit <= max.
struct Mx44 { unsigned max; unsigned limit; };
extern "C" signed char D_089CBB7A[];
extern "C" unsigned char *func_088CB164(int, int);
extern "C" int func_088C31F4(Mx44 *out, int a, unsigned *limit)
{
    int best = 0;
    for (int i = 0; i < 0x1F; i++) {
        unsigned char *e = func_088CB164(a, D_089CBB7A[i]);
        if (best < e[1]) best = e[1];
    }
    out->max = best;
    unsigned lim = *limit;
    out->limit = lim;
    return (int)lim <= best;
}
