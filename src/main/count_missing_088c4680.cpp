// Counts over 31 entries how many have func_088CBB60(func_088CB164(a, D_089CBB7A[i])) == 0 into out[0]; out[1] = *limit; returns count >= limit.
struct Cnt44 { int count; int limit; };
extern "C" signed char D_089CBB7A[];
extern "C" char *func_088CB164(int, int);
extern "C" int func_088CBB60(char *);
extern "C" int func_088C4680(Cnt44 *out, int a, int *limit)
{
    out->count = 0;
    out->limit = *limit;
    for (int i = 0; i < 0x1F; i++) {
        if (func_088CBB60(func_088CB164(a, D_089CBB7A[i])) == 0)
            out->count = out->count + 1;
    }
    return !(out->count < out->limit);
}
