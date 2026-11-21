/* Evaluates the curve at step (n - 1); 0 when inactive or no curve. */
typedef struct Cv38 { char pad[4]; float a; float b; float step; } Cv38;
extern float func_08AC3DA4(float a, float b);
float func_08ABC364(unsigned char *self, Cv38 *c, int n)
{
    if (*self == 0) {
        return 0;
    } else if (c == 0) {
        return 0;
    } else {
        return func_08AC3DA4(c->a, c->b + c->step * (float)(n - 1));
    }
}
