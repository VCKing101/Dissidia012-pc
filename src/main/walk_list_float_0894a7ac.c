// Stores a block pointer, then calls func_0894BA60 for each node of one of its lists.
struct N { char d[0x14]; struct N *next; char e[0x18]; float w; };
struct B { char d[0x1B8]; struct N *head[4]; };
struct T { char d[0x88]; struct B *blk; };
extern void func_0894BA60(float w, struct T *t, struct N *n, int a, int b);
void func_0894A7AC(struct T *t, struct B *b, int a, int idx)
{
    struct N *n;
    t->blk = b;
    for (n = b->head[idx]; n != 0; n = n->next)
        func_0894BA60(n->w, t, n, a, idx);
}
