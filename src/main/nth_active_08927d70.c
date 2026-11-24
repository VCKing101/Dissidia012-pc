/* Returns the index of the n-th (1-based) active entry of the five 0x154-byte records, or -1. */
typedef struct Rec { char d[0x154]; } Rec;
extern char *D_089D712C;
extern int func_0892A1E0(Rec *p);
int func_08927D70(int a, int n)
{
    Rec *arr = (Rec *)(D_089D712C + 0x3E0B0);
    int r;
    int k;
    int i;
    if (n == 0) {
        return -1;
    }
    r = -1;
    k = 1;
    for (i = 0; i < 5; i++) {
        if (func_0892A1E0(&arr[i]) != 0) {
            if (k == n) {
                r = i;
                break;
            }
            k++;
        }
    }
    return r;
}
