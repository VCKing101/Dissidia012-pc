/* Advances a spinner's time, wraps it to the animation length and sets the
   angle. Written on the field: SNC keeps it in a register and stores once. */
typedef struct AnimInfo { char pad_00[0x2C]; float length; } AnimInfo;
typedef struct Spinner { int field_00; int field_04; AnimInfo *anim; float time; char pad_10[0x20]; float angle; } Spinner;
extern float D_089C7660;
extern float func_089A2C70(float x);
void func_0881FE78(Spinner *s, float dt)
{
    float len = s->anim->length;
    if (len <= 0.0f) {
        return;
    }
    s->time += dt;
    while (s->time >= len) {
        s->time -= len;
    }
    s->angle = func_089A2C70(D_089C7660 * 2.0f * s->time / len);
}
