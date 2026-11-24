// Advances slot 1 along its accelerate-then-cruise curve and finishes it (motion 3) when the time runs out.
typedef struct V3 { float x, y, z; } V3;
typedef struct MotionTiming { float a, b, c; void *source; } MotionTiming;
typedef struct MotionSlot {
    char pad_00[4];
    short motion;
    char pad_06[2];
    V3 pos;
    char pad_14[0x1C];
    V3 start;
    V3 dir;
    float s_end, accel_v, accel_a;
    float t, t_acc;
    char pad_5c[4];
    MotionTiming timing;
} MotionSlot;
extern void func_08ABD1BC(float dt, MotionSlot *self, MotionTiming *timing);
extern const float D_08ABD5C4, D_08ABD5C8;

void func_08ABD2E8(float dt, MotionSlot *self)
{
    float t = self->t + dt;
    float tacc = self->t_acc;
    float total = D_08ABD5C8 + tacc;
    float way;
    float px;
    float py;
    float pz;
    float sx = self->start.x;
    float dx = self->dir.x;
    float sy = self->start.y;
    float dy = self->dir.y;
    float sz = self->start.z;
    float dz = self->dir.z;
    if (t > total) {
        t = total;
    }
    self->t = t;
    if (t < tacc) {
        way = self->accel_v * t + self->accel_a * t * t * 0.5f;
        px = sx + dx * way;
        py = sy + dy * way;
        pz = sz + dz * way;
    } else {
        float c = D_08ABD5C4;
        way = self->s_end - c * (D_08ABD5C8 - (t - tacc));
        px = sx + dx * way;
        py = sy + dy * way;
        pz = sz + dz * way;
    }
    self->pos.x = px;
    self->pos.y = py;
    self->pos.z = pz;
    func_08ABD1BC(dt, self, &self->timing);
    if (t == total) {
        self->motion = 3;
    }
}
