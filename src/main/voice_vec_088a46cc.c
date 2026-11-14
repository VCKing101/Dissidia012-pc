/* Sets the voice direction. */
typedef struct Vec4_088A46CC {
    float x;
    float y;
    float z;
    float w;
} Vec4_088A46CC;

void func_088A46A0(void *voice, Vec4_088A46CC *param);

void func_088A46CC(void *voice, float x, float y, float z)
{
    Vec4_088A46CC pos;
    pos.x = x;
    pos.y = y;
    pos.z = z;
    pos.w = 0.0f;
    func_088A46A0(voice, &pos);
}
