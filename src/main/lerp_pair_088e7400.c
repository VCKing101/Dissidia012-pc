/* Linear blend between the floats at +0xC and +0x10; the factor at +0x18 is shaped when mode byte is 1. */
typedef struct S { char p[0xC]; float *a; float *b; float t; unsigned char mode; } S;
typedef struct S2 { char p[0x18]; float t; char q[1]; unsigned char mode; } S2;
extern float D_089CC748;
extern float func_08867EEC(float f);
void func_088E7400(S *self, float *out) {
    float t = ((S2 *)self)->t;
    if (((S2 *)self)->mode == 1) {
        { float tscale = D_089CC748; float h = 0.5f; t = func_08867EEC(t * tscale * h); }
    }
    {
        float a = *self->a;
        float b = *self->b;
        *out = a + (b - a) * t;
    }
}
