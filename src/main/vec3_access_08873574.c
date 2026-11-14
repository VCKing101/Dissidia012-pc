typedef struct Vec3 { float x; float y; float z; } Vec3;
extern float D_089C9B40;
extern float D_089C9B44;
float func_08873574(Vec3 *v) { return v->x; }
float func_0887357C(Vec3 *v) { return v->y; }
float func_08873584(Vec3 *v) { return v->z; }
void func_0887358C(Vec3 *v, float x) { v->x = x; }
void func_08873594(Vec3 *v, float y, float z)
{
    v->y = y;
    v->z = z;
}
void func_088735A0(Vec3 *v)
{
    v->y = D_089C9B40;
    v->z = D_089C9B44;
}
