/* Updates the camera's yaw and pitch from the stick, clamped to the
   camera's limits. */
struct Limits08830B88 {
    char pad_00[4];
    float yaw_max;
    float yaw_min;
    float pitch_min;
    float pitch_max;
    float yaw_max_extra;
    float yaw_min_extra;
};

struct Camera08830B88 {
    char pad_00[4];
    Limits08830B88 *limits;
    char pad_08[8];
    float pitch;
    float yaw;
    float roll;

    static float clamp_a(float &v, float lo, float hi)
    {
        if (v < lo) {
            return lo;
        }
        if (v <= hi) {
            return v;
        }
        return hi;
    }

    static float clamp_b(float &v, float lo, float hi)
    {
        if (v < lo) {
            return lo;
        }
        if (v > hi) {
            return hi;
        }
        return v;
    }
};

extern "C" {

float func_08868128(int pad, int stick, int mode);
float func_088686B0(int stick);

void func_08830B88(Camera08830B88 *self, int pad, int stick)
{
    self->yaw = func_08868128(pad, stick, 0);
    Limits08830B88 *l = self->limits;
    self->yaw = Camera08830B88::clamp_a(self->yaw, -(l->yaw_min + l->yaw_min_extra), l->yaw_max + l->yaw_max_extra);
    self->pitch = func_088686B0(stick);
    l = self->limits;
    self->pitch = Camera08830B88::clamp_b(self->pitch, -l->pitch_min, l->pitch_max);
    self->roll = 0.0f;
}

}
