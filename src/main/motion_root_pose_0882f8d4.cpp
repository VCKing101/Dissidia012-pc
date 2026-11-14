/* Gets a motion's root pose: the root position with its facing angle in w,
   or the zero pose when the motion has no root. */
struct Vec4_0882F8D4 {
    float x, y, z, w;
};

struct Root0882F8D4 {
    char pad_00[0x20];
    float dir_x;
    float dir_y;
    float dir_z;
    char pad_2C[4];
    Vec4_0882F8D4 pos;
};

struct Actor0882F8D4 {
    char pad_000[0x394];
    void *body;
};

struct Motion0882F8D4 {
    char pad_00[0x30];
    Actor0882F8D4 *actor;
    char pad_34[0x50 - 0x34];
    short root;

    static bool rootless(Motion0882F8D4 *m) { return m->root < 0; }
};

extern "C" {

extern Vec4_0882F8D4 D_09AF84C0;

Root0882F8D4 *func_0886B5B8(void *body);
void func_089A3868(Vec4_0882F8D4 *dst, const Vec4_0882F8D4 *src);
float func_089A457C(float x, float z);

Vec4_0882F8D4 func_0882F8D4(Motion0882F8D4 *self)
{
    if (Motion0882F8D4::rootless(self)) {
        return D_09AF84C0;
    }
    Root0882F8D4 *root = func_0886B5B8(self->actor->body);
    Vec4_0882F8D4 v;
    func_089A3868(&v, &root->pos);
    v.w = func_089A457C(root->dir_x, root->dir_z);
    return v;
}

}
