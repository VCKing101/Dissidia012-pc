/* Clears four floats at the start of the struct and returns it. */
typedef struct V4 { float x, y, z, w; } V4;
V4 *func_0895B49C(V4 *v)
{
    v->x = 0.0f;
    v->y = 0.0f;
    v->z = 0.0f;
    v->w = 0.0f;
    return v;
}
