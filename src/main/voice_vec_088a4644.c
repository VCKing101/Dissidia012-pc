/* Sets the voice velocity. */
typedef struct Vec4_088A4644 {
    float x;
    float y;
    float z;
    float w;
} Vec4_088A4644;

void func_088A45D8(void *voice, Vec4_088A4644 *param);

void func_088A4644(void *voice, float x, float y, float z)
{
    Vec4_088A4644 pos;
    pos.x = x;
    pos.y = y;
    pos.z = z;
    pos.w = 0.0f;
    func_088A45D8(voice, &pos);
}
