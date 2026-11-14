/* Initialises a voice: no handles, cleared state and full colour. */
typedef struct Vec4_088A4B9C {
    float x;
    float y;
    float z;
    float w;
} Vec4_088A4B9C;

typedef struct Voice088A4B9C {
    Vec4_088A4B9C color;
    int handle;
    int field_14;
    int flags;
    float remaining;
    int field_20;
    int field_24;
    float level;
    float step;
    int field_30;
    int field_34;
    int field_38;
    int field_3C;
} Voice088A4B9C;

void func_089A383C(Vec4_088A4B9C *v, float x, float y, float z, float w);

void func_088A4B9C(Voice088A4B9C *v)
{
    float zero = 0.0f;
    float one;
    v->handle = -1;
    v->field_14 = -1;
    v->flags = 0;
    v->remaining = zero;
    v->field_20 = 0;
    v->field_24 = 0;
    one = 1.0f;
    v->step = zero;
    v->level = one;
    func_089A383C(&v->color, one, one, one, one);
    v->field_30 = 0;
    v->field_34 = 0;
    v->field_38 = 0;
    v->field_3C = 0;
}
