typedef struct Vec4 {
    float x;
    float y;
    float z;
    float w;
} Vec4;

extern void func_089A38FC(Vec4 *out, Vec4 *a, Vec4 *b);
extern Vec4 func_08A803AC(void *self, Vec4 *dir);

Vec4 func_08A80670(void *self, Vec4 *from, Vec4 *to)
{
    Vec4 dir;
    func_089A38FC(&dir, to, from);
    return func_08A803AC(self, &dir);
}
