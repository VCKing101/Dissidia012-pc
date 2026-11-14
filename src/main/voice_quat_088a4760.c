/* Sets the voice orientation from four components. */
typedef struct Vec4_088A4760 {
    float x;
    float y;
    float z;
    float w;
} Vec4_088A4760;

void func_088A46FC(void *voice, Vec4_088A4760 *param);

void func_088A4760(void *voice, float x, float y, float z, float w)
{
    Vec4_088A4760 v;
    v.x = x;
    v.y = y;
    v.z = z;
    v.w = w;
    func_088A46FC(voice, &v);
}
