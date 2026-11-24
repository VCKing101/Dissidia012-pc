/* Copies a 6-word block into an active record when it has a valid kind; -1 inactive, -2 bad source. */
typedef struct Rec6 { unsigned a; unsigned kind; unsigned c; unsigned d; unsigned e; unsigned f; } Rec6;
typedef struct Dst { unsigned char active; char pad[3]; Rec6 rec; } Dst;
int func_08B19CB8(Dst *dst, Rec6 *src)
{
    if (dst->active == 0) {
        return -1;
    }
    if (src == 0 || src->kind >= 5) {
        return -2;
    }
    dst->rec = *src;
    return 0;
}
