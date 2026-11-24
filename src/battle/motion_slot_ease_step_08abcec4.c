// Advances slot 5's timer by dt (clamped), moves it toward the target along an eased curve, and closes the slot at the end.
typedef struct V3 { float x, y, z; } V3;
typedef struct MotionSlot {
    char pad_00[8];
    V3 pos;
    float f14;
    char pad_18[0x18];
    V3 start;
    float t;
    float dur;
    char pad_44[0xC];
    float *range;
} MotionSlot;
extern void func_089A3528(V3 *dst, void *a, V3 *b);
extern float func_089A37DC(V3 *v);
extern void func_089A379C(V3 *dst, V3 *src);
extern void func_08ABC88C(MotionSlot *self);

void func_08ABCEC4(float dt, MotionSlot *self, void *target)
{
    V3 from;
    V3 d;
    float t = self->t + dt;
    float dur = self->dur;
    float dist;
    float k;
    float ease;
    if (dur < t) {
        t = dur;
    }
    self->t = t;
    from = self->start;
    func_089A3528(&d, target, &from);
    dist = func_089A37DC(&d);
    func_089A379C(&d, &d);
    k = t / dur;
    ease = k * k;
    ease = ease * ease;
    dist = dist * ease;
    self->pos.x = from.x + d.x * dist;
    self->pos.y = from.y + d.y * dist;
    self->pos.z = from.z + d.z * dist;
    {
        float hi = self->range[0];
        float lo = self->range[2];
        self->f14 = (hi - lo) * (1.0f - ease) + lo;
    }
    if (dur == t) {
        func_08ABC88C(self);
    }
}
