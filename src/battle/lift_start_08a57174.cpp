struct Vec4 {
    float x;
    float y;
    float z;
    float w;
    static void set(Vec4 *v, float x, float y, float z, float w)
    {
        v->x = x;
        v->y = y;
        v->z = z;
        v->w = w;
    }
};

struct LiftFighter {
    char pad_000[0x130];
    char effect[0x254];
    unsigned int flags;
};

extern "C" {

extern void func_08834934(LiftFighter *fighter, int first, int second);
extern void func_0882FA10(void *effect, int id, float x, float y, int flags);
extern void func_08A5BF84(LiftFighter *fighter, Vec4 *velocity);
extern void func_08A5B40C(void *self, LiftFighter *fighter);

/* Starts the lift: motions 0x65/0x66, effect 300, clears flag 0x800 and pushes the fighter up. */
void func_08A57174(void *self, LiftFighter *fighter)
{
    Vec4 velocity;
    func_08834934(fighter, 0x65, 0x66);
    float zero = 0.0f;
    func_0882FA10(fighter->effect, 300, zero, zero, 0);
    fighter->flags &= ~0x800;
    Vec4::set(&velocity, zero, 3.0f, zero, zero);
    func_08A5BF84(fighter, &velocity);
    func_08A5B40C(self, fighter);
}

}
