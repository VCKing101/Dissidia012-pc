typedef struct Camera {
    char pad0[0x380];
    unsigned int flags;
    char pad384[0x10C];
    float current;
    float minimum;
    float maximum;
    char pad49C[0x1E8];
    float start;
    float target;
    float speed;
} Camera;

void func_088063AC(float target, float speed, float minimum, float maximum, Camera *camera)
{
    camera->minimum = minimum <= 0.0f ? 0.1f : minimum;
    camera->maximum = maximum <= 0.0f ? 10.0f : maximum;
    camera->target = target;
    camera->speed = speed;
    if (speed == 0.0f) {
        camera->start = camera->target;
        camera->current = camera->target;
        camera->flags |= 0x200000;
    }
}
