/* Stores two floats and sets flag 0x80 unless the state at +4 is negative. */
typedef struct S { int pad0; int state; char pad8[0x24]; unsigned flags; char pad30[0x18]; float x; float y; } S;
int func_089928AC(S *s, float x, float y)
{
    if (s->state < 0) return -1;
    s->x = x;
    s->flags |= 0x80;
    s->y = y;
    return 0;
}
