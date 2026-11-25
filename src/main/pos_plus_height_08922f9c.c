/* Writes the position vector of func_089230F4(o) to out, with y raised by func_08922F88(o). */
typedef struct V3 { float x, y, z; } V3;
extern V3 *func_089230F4(void *o);
extern float func_08922F88(void *o);
void func_08922F9C(V3 *out, void *o) {
    V3 *p = func_089230F4(o);
    V3 v = *p;
    v.y = v.y + func_08922F88(o);
    *out = v;
}
