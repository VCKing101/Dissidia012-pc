/* Reads the capped total of the nested pair and reports whether it reaches the limit. */
typedef struct Pair088C29F8 { int a; int b; } Pair088C29F8;
typedef struct Sub088C29F8 { char pad[0x1BBC]; Pair088C29F8 pair; } Sub088C29F8;
typedef struct Big088C29F8 { char pad[0xFD94]; Sub088C29F8 sub; } Big088C29F8;
typedef struct Result088C29F8 { int value; int limit; } Result088C29F8;
int func_088B2B8C(Pair088C29F8 *pair);
int func_088C29F8(Result088C29F8 *r, Big088C29F8 *big, int *limit)
{
    Sub088C29F8 *sub = &big->sub;
    int v = func_088B2B8C(&sub->pair);
    r->value = v;
    r->limit = *limit;
    return v >= r->limit;
}
