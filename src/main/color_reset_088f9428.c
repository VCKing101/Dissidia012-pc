/* Resets a colour to opaque white. */
typedef struct ColorState {
    float r;
    float g;
    float b;
    float a;
    short time;
    short mode;
} ColorState;

void func_088F9428(ColorState *c)
{
    float one = 1.0f;

    c->time = 0;
    c->r = one;
    c->g = one;
    c->b = one;
    c->a = one;
    c->mode = 0;
}
