/* Moves an actor to a point dropped onto the ground below it and marks
   it as moved. */
struct Vec3_088358D0 {
    float x, y, z;
};

struct Vec4_088358D0 {
    float x, y, z, w;
};

struct Hit088358D0 {
    char data[0x30];
};

struct Actor088358D0 {
    char pad_000[0x80];
    Vec4_088358D0 pos;
    char pad_090[0x388 - 0x90];
    unsigned long long state;
    char pad_390[0x398 - 0x390];
    void *camera;
};

extern "C" {

extern unsigned long long D_089C7F88;

void func_0880CD90(void *camera);
void *func_08899300(void);
float func_088976E4(void *world, Hit088358D0 *hit, Vec3_088358D0 *from, int mask, int down,
                    int a, int b);

void func_088358D0(Actor088358D0 *self, unsigned char move_camera, float x, float y, float z)
{
    if (move_camera && self->camera != 0) {
        func_0880CD90(self->camera);
    }
    self->state |= D_089C7F88;
    float up = 3.0f;
    Hit088358D0 hit;
    Vec3_088358D0 from;
    from.x = x;
    from.y = up + y;
    from.z = z;
    Vec3_088358D0 *start = &from;
    float ground = func_088976E4(func_08899300(), &hit, start, 0x22, 1, 0, 0);
    self->pos.x = x;
    self->pos.y = ground;
    self->pos.z = z;
    self->pos.w = 1.0f;
}

}
