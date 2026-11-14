/* Places an effect at an object's position with the given height and
   angle, and marks the object as having one. */
struct Vec4_088325D0 {
    float x, y, z, w;
};

struct Obj088325D0 {
    Vec4_088325D0 pos;
    char pad_10[0x28 - 0x10];
    unsigned int flags;
};

extern "C" {

void func_089A3868(Vec4_088325D0 *dst, Obj088325D0 *src);
void func_08A56F98(void *effect, Vec4_088325D0 *pos, int mode, unsigned char flag);

void func_088325D0(void *effect, Obj088325D0 *obj, unsigned char flag, float height, float angle)
{
    int one = 1;
    Vec4_088325D0 pos;
    func_089A3868(&pos, obj);
    pos.y = height;
    pos.w = angle;
    func_08A56F98(effect, &pos, one, flag);
    obj->flags |= 1;
}

}
