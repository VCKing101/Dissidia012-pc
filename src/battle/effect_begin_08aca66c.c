// Starts the effect on the owner actor: creates the 0xD marker, plays the 3000 and 0x65 motions and sets the actor's state flags.
struct Flags { char d0[5]; unsigned char b5; };
struct Link { char d0[0x84]; int f84; };
struct Blk { char d0[0x18]; float f18; };
struct Actor {
    char d0[0x100];
    float x, y, z;
    char d10C[0x24];
    char motion[0x254];
    unsigned int f384;
    char d388[0x1A4];
    struct Blk *blk;
    char d530[0x74];
    struct Link *link;
    char d5A8[8];
    struct Flags flags;
};
struct Pool { int f0; };
struct Effect { struct Actor *actor; char d4[0x14]; float f18; char d1C[0xC]; void *h28; };
extern void *func_088A9DE8(int a, int id, int b, int c, int d);
extern void func_088A7284(float x, float y, float z, void *h);
extern void func_0882FA10(void *motion, int id, float x, float y, int arg);
extern void func_0882FB90(void *motion, int id, float x, float y);
extern void func_08834934(struct Actor *a, int x, int y);
extern void func_08B33F8C(struct Actor *a, int v, int b, int c);
extern struct Pool *D_089D6DD0;
extern float D_089C7C2C;
extern float D_08ACBB8C;

void func_08ACA66C(struct Effect *e)
{
    float zero;
    void *h = e->h28 = func_088A9DE8(D_089D6DD0->f0, 0xD, 0, 0, 0);
    if (h != 0) {
        func_088A7284(e->actor->x, e->actor->y, e->actor->z, h);
    }
    zero = 0.0f;
    {
        void *mo = e->actor->motion;
        int id = 3000;
        func_0882FA10(mo, id, zero, zero, 0);
    }
    func_0882FB90(e->actor->motion, 0x65, D_089C7C2C, zero);
    func_08834934(e->actor, 0x65, 0x66);
    e->actor->f384 = e->actor->f384 & ~0x800;
    func_08B33F8C(e->actor, e->actor->link->f84, 0, 0);
    {
        struct Flags *fl = &e->actor->flags;
        fl->b5 = fl->b5 | 0x80;
    }
    e->actor->blk->f18 = zero;
    e->f18 = D_08ACBB8C;
}
