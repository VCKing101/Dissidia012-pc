typedef struct Vec4 { float x, y, z, w; } Vec4;
typedef struct Camera088068A0 {
    char pad_000[0x90];
    Vec4 pos;                   /* 0x090 */
    char pad_0A0[0xA90 - 0xA0];
    char from[0x10];            /* 0xA90 */
    char to[0x10];              /* 0xAA0 */
    char pad_AB0[0x14];
    float a_time;               /* 0xAC4 */
    float a_value;
    float b_time;               /* 0xACC */
    float b_value;
    float c_time;               /* 0xAD4 */
    float c_value;
    float d_time;               /* 0xADC */
    float d_value;
    char pad_AE4[0xAF0 - 0xAE4];
    unsigned char started;      /* 0xAF0 */
} Camera088068A0;
extern void func_089A3868(void *dst, Vec4 *v);
void func_088068A0(Camera088068A0 *c, float a, float b)
{
    Vec4 v;
    Vec4 *pv = &v;
    c->c_time = a;
    c->c_value = 0.0f;
    c->d_time = b;
    c->d_value = 0.0f;
    v = c->pos;
    if (!c->started) {
        func_089A3868(c->from, pv);
        func_089A3868(c->to, pv);
        c->started = 1;
    }
}
