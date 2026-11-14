/* Moves an actor straight to a point: shifts its saved positions by the
   same step, raises a grounded body by the climb and marks it as moved. */
struct Vec4_088357B8 {
    float x, y, z, w;
};

struct Ground088357B8 {
    char pad_0[4];
    unsigned int flags;

    static bool grounded(Ground088357B8 *g) { return g->flags & 1; }
};

struct Body088357B8 {
    char pad_00[0x18];
    float height;

    static void add(float &v, float d) { v += d; }
};

struct Actor088357B8 {
    char pad_000[0x80];
    Vec4_088357B8 pos;
    char pad_090[0x100 - 0x90];
    Vec4_088357B8 last_pos;
    char pad_110[0x314 - 0x110];
    Body088357B8 *body;
    char pad_318[0x388 - 0x318];
    unsigned long long state;
    char pad_390[0x398 - 0x390];
    void *camera;
    char pad_39C[0x3A4 - 0x39C];
    Ground088357B8 *ground;
};

extern "C" {

extern unsigned long long D_089C7F88;

void func_089A38FC(Vec4_088357B8 *dst, const Vec4_088357B8 *a, const Vec4_088357B8 *b);
void func_089A38C4(Vec4_088357B8 *dst, const Vec4_088357B8 *a, const Vec4_088357B8 *b);
void func_0880CD90(void *camera);

void func_088357B8(Actor088357B8 *self, unsigned char move_camera, float x, float y, float z)
{
    Vec4_088357B8 step;
    Vec4_088357B8 to;
    to.x = x;
    to.y = y;
    to.z = z;
    float one = 1.0f;
    to.w = one;
    func_089A38FC(&step, &to, &self->pos);
    func_089A38C4(&self->last_pos, &self->last_pos, &step);
    if (self->ground != 0 && Ground088357B8::grounded(self->ground)) {
        Body088357B8::add(self->body->height, step.y);
    }
    if (move_camera && self->camera != 0) {
        func_0880CD90(self->camera);
    }
    self->state |= D_089C7F88;
    self->pos.x = x;
    self->pos.y = y;
    self->pos.z = z;
    self->pos.w = one;
}

}
