typedef struct Vec4 {
    float x;
    float y;
    float z;
    float w;
} Vec4;
typedef struct Vec3 {
    float x;
    float y;
    float z;
} Vec3;
typedef struct Mover {
    char pad0[0x80];
    Vec4 from;
    Vec4 to;
    Vec3 control;
    float time;
    float duration;
    float speed;
    int mode;
} Mover;
extern void func_089A3868(void *dst, void *src);
void func_0880B3FC(float duration, Mover *mover, Vec4 *to, Vec3 *control)
{
    func_089A3868(&mover->from, &mover->to);
    func_089A3868(&mover->to, to);
    mover->to.w = 1.0f;
    func_089A3868(&mover->control, control);
    mover->time = 0.0f;
    mover->duration = duration;
    mover->speed = duration;
    mover->mode = 3;
}
