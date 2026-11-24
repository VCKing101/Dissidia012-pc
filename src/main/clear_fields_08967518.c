/* Clears a few fields and returns the struct. */
typedef struct S { int a, b; char pad[0x28]; int c, d; float e, f; } S;
S *func_08967518(S *s)
{
    s->b = 0;
    s->a = 0;
    s->c = 0;
    s->d = 0;
    s->e = 0.0f;
    s->f = 0.0f;
    return s;
}
