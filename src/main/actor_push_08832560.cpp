/* Pushes an actor unless its weight exceeds the push limit. */
struct Actor08832560 {
    char pad_000[0x3C0];
    float weight;
};

extern "C" {

extern float D_089C7EC8;

void func_0883241C(Actor08832560 *self, int dir, int mode, float x, float y, float z);

bool func_08832560(Actor08832560 *self, int dir, float x, float y, float z)
{
    if (D_089C7EC8 < self->weight) {
        return false;
    }
    func_0883241C(self, dir, 0, x, y, z);
    return true;
}

}
