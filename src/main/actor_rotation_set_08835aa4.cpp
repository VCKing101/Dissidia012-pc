/* Sets an actor's rotation from angles and marks it as turned. */
struct Vec3_08835AA4 {
    float x, y, z;
};

struct Turn08835AA4 {
    int order;
    Vec3_08835AA4 angles;
};

struct Rotation08835AA4 {
    char data[0x10];
};

struct Actor08835AA4 {
    char pad_000[0x2EC];
    Rotation08835AA4 rotation;
    char pad_2FC[0x388 - 0x2FC];
    unsigned long long state;
};

extern "C" {

extern unsigned long long D_089C7F90;

void func_089A34C0(Rotation08835AA4 *rotation, Vec3_08835AA4 *angles);

void func_08835AA4(Actor08835AA4 *self, const Vec3_08835AA4 *angles)
{
    Turn08835AA4 turn;
    Vec3_08835AA4 t = *angles;
    Vec3_08835AA4 unused;
    turn.angles = t;
    func_089A34C0(&self->rotation, &turn.angles);
    self->state |= D_089C7F90;
}

}
