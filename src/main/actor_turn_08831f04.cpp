/* Turn motions: switch an actor between the two turn motions of a pair,
   then place it. */
struct Vec4_08831F04 {
    float x, y, z, w;
};

struct Motion08831F04 {
    char data[0x50];
};

struct Target08831F04 {
    char pad_00[0x28];
    unsigned int flags;
};

struct Actor08831F04 {
    char pad_000[0x130];
    Motion08831F04 motion;
    char pad_180_[0x180 - 0x130 - sizeof(Motion08831F04)];
    short motion_id;
    char pad_182[0x388 - 0x182];
    unsigned long long state;

    static bool any(Actor08831F04 *a, unsigned long long mask) { return (a->state & mask) != 0; }
};

extern "C" {

extern unsigned long long D_089C7F28;
extern float D_089C7EBC;
extern char D_09AF84C0[];

int func_08838074(Actor08831F04 *self);
void func_08A58A5C(Actor08831F04 *self, void *pos);
void func_0882FA10(Motion08831F04 *motion, int id, int mode, float time, float start);
void func_0882FB90(Motion08831F04 *motion, int mode, float time, float start);
void func_08832150(Actor08831F04 *self, Target08831F04 *target, float time);
void func_089A3868(Vec4_08831F04 *dst, void *src);
void func_08834934(Actor08831F04 *self, int a, int b);

void func_08831F04(Actor08831F04 *self, Target08831F04 *target, float time)
{
    int next = -1;
    if (func_08838074(self)) {
        switch (self->motion_id) {
        case 500:
            next = 501;
            break;
        case 501:
            next = 500;
            break;
        }
    }
    if (next < 0) {
        next = Actor08831F04::any(self, D_089C7F28) ? 500 : 501;
    }
    func_08A58A5C(self, D_09AF84C0);
    Motion08831F04 *m = &self->motion;
    func_0882FA10(m, next, 0, 0.0f, 0.0f);
    func_0882FB90(m, 1, D_089C7EBC, 0.0f);
    func_08832150(self, target, time);
    target->flags |= 1;
}

void func_08832034(Actor08831F04 *self, Vec4_08831F04 *pos, float y, float w)
{
    int next = -1;
    if (func_08838074(self)) {
        switch (self->motion_id) {
        case 550:
            next = 551;
            break;
        case 551:
            next = 550;
            break;
        }
    }
    if (next < 0) {
        next = Actor08831F04::any(self, D_089C7F28) ? 550 : 551;
    }
    Vec4_08831F04 p;
    func_089A3868(&p, pos);
    p.y = y;
    p.w = w;
    func_08A58A5C(self, &p);
    ((Target08831F04 *)pos)->flags |= 1;
    func_0882FA10(&self->motion, next, 0, 0.0f, 0.0f);
    func_08834934(self, 0x65, 0x66);
}

}
