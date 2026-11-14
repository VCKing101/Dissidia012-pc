/* Moves an actor toward a point, running when it has speed or may run,
   and marks the target as handled. */
struct Target08832C20 {
    char pad_00[0x28];
    unsigned int flags;
};

struct Actor08832C20 {
    char pad_000[0x388];
    unsigned long long state;

    static bool any(Actor08832C20 *a, unsigned long long mask) { return (a->state & mask) != 0; }
};

extern "C" {

extern unsigned long long D_089C7F28;

int func_088374B0(Actor08832C20 *self);
void func_08A527CC(Actor08832C20 *self, float x, float y, void *path, float speed,
                   int flip, float a, int turn, float b, int run);

void func_08832C20(Actor08832C20 *self, Target08832C20 *target, void *path,
                   float x, float y, float speed, float a, float b)
{
    int flip = 0;
    if (Actor08832C20::any(self, D_089C7F28)) {
        flip = 1;
    }
    if (speed > 0.0f || func_088374B0(self)) {
        func_08A527CC(self, x, y, path, speed, flip, a, 1, b, 1);
    } else {
        func_08A527CC(self, x, y, path, speed, flip, a, 1, b, 0);
    }
    target->flags |= 1;
}

}
