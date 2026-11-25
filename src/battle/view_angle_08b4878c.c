// Returns the angle in degrees (0..360) from straight down to the point's xz direction.
typedef struct V4 { float x, y, z, w; } V4;
extern float func_0886808C(V4 *a, V4 *b, V4 *c);
extern const float D_08B491B0;

float func_08B4878C(void *unused, float *p)
{
    V4 down;
    V4 pt;
    V4 fwd;
    float zero = 0.0f;
    float r;
    down.x = zero;
    down.y = -1.0f;
    down.z = zero;
    down.w = 1.0f;
    pt.x = p[0];
    pt.y = p[2];
    pt.z = zero;
    pt.w = 1.0f;
    V4 *pb = &pt;
    fwd.x = zero;
    fwd.y = zero;
    fwd.z = 1.0f;
    fwd.w = 1.0f;
    r = func_0886808C(&down, pb, &fwd);
    r = (180.0f / D_08B491B0) * r;
    if (r < zero) {
        r = r + 360.0f;
    }
    if (r < zero) {
        r = zero;
    }
    if (360.0f < r) {
        r = 360.0f;
    }
    return r;
}
