/* Clears fields, sets a vtable-like pointer at +0x20 and returns the struct. */
typedef struct S { int a, b, c, d, e, f; char g; char pad[3]; float h; int p; } S;
extern char D_089DC508[];
S *func_08969F74(S *s)
{
    *(char **)((char *)s + 0x20) = D_089DC508;
    s->a = 0;
    s->b = 0;
    s->c = 0;
    s->d = 0;
    s->e = 0;
    s->f = 0;
    s->g = 0;
    s->h = 0.0f;
    return s;
}
