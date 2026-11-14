struct Agent2Vector {
    float x;
    float y;
    float z;
    float w;
};
extern "C" float D_089C9468;
extern "C" float func_089941C0(float, float);
extern "C" float func_089A47A4(float);
extern "C" float func_089A46DC(float);
/* Wraps an angle into (-pi, pi). */
struct Agent2Angle {
    static float wrap(float angle) {
        if (-D_089C9468 < angle && angle < D_089C9468) return angle;
        if (angle > 0.0f) {
            return func_089941C0(D_089C9468 + angle, D_089C9468 * 2.0f) -
                   D_089C9468;
        }
        return D_089C9468 -
               func_089941C0(D_089C9468 - angle, D_089C9468 * 2.0f);
    }
};
extern "C" void func_08867F08(Agent2Vector *out, float angle) {
    Agent2Vector result;
    float wrapped = Agent2Angle::wrap(angle);
    result.x = func_089A47A4(wrapped);
    result.y = 0.0f;
    result.z = func_089A46DC(wrapped);
    result.w = wrapped;
    *out = result;
}
