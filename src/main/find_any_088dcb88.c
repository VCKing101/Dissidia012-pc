/* Forwards to func_088DCBA8 with no limit (-1) and no flags. */
extern int func_088DCBA8(void *a, void *b, void *c, int limit, int flags);

int func_088DCB88(void *a, void *b, void *c)
{
    return func_088DCBA8(a, b, c, -1, 0);
}
