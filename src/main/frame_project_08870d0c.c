typedef struct Vec3 { float x; float y; float z; } Vec3;
typedef struct Frame { int field_00; int field_04; int field_08; Vec3 axes[3]; Vec3 origin; } Frame;

extern float func_089A3718(Vec3 *a, Vec3 *b);
extern void func_089A35CC(Vec3 *out, Vec3 *in);

void func_08870D0C(Frame *frame, Vec3 *v)
{
    Vec3 local;
    Vec3 *axes = frame->axes;
    local.x = func_089A3718(v, &axes[0]);
    local.y = func_089A3718(v, &axes[1]);
    local.z = func_089A3718(v, &axes[2]);
    func_089A35CC(&local, &local);
    func_089A3718(&frame->origin, &local);
}
