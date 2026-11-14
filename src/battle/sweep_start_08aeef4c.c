typedef struct Vec4 {
    float x;
    float y;
    float z;
    float w;
} Vec4;

typedef struct Sweep {
    Vec4 origin;
    Vec4 from;
    Vec4 to;
    float t;
    float step;
} Sweep;

extern void func_089A3868(Vec4 *dst, Vec4 *src);

/* Start a sweep from the origin to target over frames steps. */
void func_08AEEF4C(Sweep *self, Vec4 *target, int frames)
{
    func_089A3868(&self->from, &self->origin);
    func_089A3868(&self->to, target);
    self->t = 0.0f;
    if (frames != 0) {
        self->step = 1.0f / (float)frames;
    } else {
        self->step = 1.0f;
    }
}
