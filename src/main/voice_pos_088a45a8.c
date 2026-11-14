/* Sets the voice position. */
typedef struct Vec4_088A45A8 {
    float x;
    float y;
    float z;
    float w;
} Vec4_088A45A8;

void func_088A457C(void *voice, Vec4_088A45A8 *param);

void func_088A45A8(void *voice, float x, float y, float z)
{
    Vec4_088A45A8 pos;
    pos.x = x;
    pos.y = y;
    pos.z = z;
    pos.w = 0.0f;
    func_088A457C(voice, &pos);
}
