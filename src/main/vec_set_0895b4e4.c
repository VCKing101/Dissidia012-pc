/* Sets a vector from the first two floats of another, w cleared; returns 1. */
typedef struct V4 { float x, y, z, w; } V4;
int func_0895B4E4(V4 *d, float *s)
{
    float a = s[0];
    float z;
    d->x = a;
    d->y = a;
    z = s[1];
    d->w = 0.0f;
    d->z = z;
    return 1;
}
