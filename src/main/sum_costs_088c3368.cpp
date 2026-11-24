// Sums func_088B959C(func_088CB164(a, D_089CBB7A[i]) + 0x10) over 31 entries into out[0], copies limit[0] to out[1]; returns total <= limit.
struct Sum44 { int total; int limit; };
extern "C" signed char D_089CBB7A[];
extern "C" char *func_088CB164(int, int);
extern "C" int func_088B959C(char *);
extern "C" int func_088C3368(Sum44 *out, int a, int *limit)
{
    int total = 0;
    for (int i = 0; i < 0x1F; i++) {
        total += func_088B959C(func_088CB164(a, D_089CBB7A[i]) + 0x10);
    }
    out->total = total;
    out->limit = *limit;
    return !(total < *limit);
}
