/* Starts a thrown effect arc toward a target: from the node's gravity
   works out the arc height for the flight time and the throw distance. */
struct Vec4_08823AA4 {
    float x, y, z, w;
};

struct Thrower08823AA4 {
    char pad_00[0x80];
    Vec4_08823AA4 pos;
};

struct Effect08823AA4 {
    char pad_00[0xC];
    int handle;
};

struct Owner08823AA4 {
    char pad_000[0x568];
    void *effects;
};

struct Node08823AA4 {
    char pad_00[0x24];
    Owner08823AA4 *owner;
};

extern "C" {

extern void *D_089D4910;

float func_08823A64(Node08823AA4 *self);
Effect08823AA4 *func_0881E67C(void *mgr, void *effects, int id);
Thrower08823AA4 *func_08842164(void);
void func_089A38FC(Vec4_08823AA4 *dst, const Vec4_08823AA4 *a, const Vec4_08823AA4 *b);
float func_089A3BF8(const Vec4_08823AA4 *v);
void *func_0881BAA8(void);
void func_0881C82C(void *mgr, int handle, float distance, float height);

int func_08823AA4(Node08823AA4 *self, int id, const Vec4_08823AA4 *target, float time)
{
    float gravity = -func_08823A64(self);
    if (gravity == 0.0f) {
        return -1;
    }
    Effect08823AA4 *e = func_0881E67C(D_089D4910, self->owner->effects, id);
    if (e == 0) {
        return -2;
    }
    float t = time / gravity;
    float height = gravity * t * t * -0.5f;
    Thrower08823AA4 *thrower = func_08842164();
    if (thrower == 0) {
        return -3;
    }
    Vec4_08823AA4 d;
    func_089A38FC(&d, &thrower->pos, target);
    float distance = func_089A3BF8(&d);
    func_0881C82C(func_0881BAA8(), e->handle, distance, height);
    return 0;
}

}
