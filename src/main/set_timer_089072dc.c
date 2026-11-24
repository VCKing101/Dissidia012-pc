/* Sets state 8 and a scaled float from an int. */
typedef struct S { char pad[0x1658]; int state; char pad2[0x2C]; float t; } S;
extern float D_089C9F54;
void func_089072DC(S *s, int n)
{
    float f = (float)n * D_089C9F54;
    s->state = 8;
    s->t = f;
}
