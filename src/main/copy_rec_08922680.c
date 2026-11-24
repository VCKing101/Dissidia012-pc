/* Copy a 13-word record (first word, then the 12-word rest) and return the destination. */
typedef struct Rest { int v[12]; } Rest;
typedef struct Rec13 { int first; Rest rest; } Rec13;
Rec13 *func_08922680(Rec13 *dst, Rec13 *src)
{
    dst->first = src->first;
    dst->rest = src->rest;
    return dst;
}
