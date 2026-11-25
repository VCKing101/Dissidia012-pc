struct Ent;
struct Owner {
    int p0;
    unsigned flags;
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8(Ent *e);
};

struct Sub { int p0; unsigned flags; };
struct Fb { char pad[3]; unsigned char fb; };

struct Ent {
    char pad0[0x130];
    char motion[0x60];
    unsigned char fb;
    char pad1[0x3A4 - 0x191];
    Sub *sub;
};

extern "C" {
extern float D_089C7C2C;
int func_0883739C(Ent *e);
int func_08B33418(Ent *e);
int func_088374B0(Ent *e);
void func_08834934(Ent *e, int a, int b);
void func_08834948(Ent *e);
void func_0882FA10(void *motion, int id, float x, float y, int arg);
void func_08834C10(Ent *e, int a, int b);
}

struct B {
    static bool has(unsigned v, unsigned m) { return (v & m) != 0; }
};

extern "C" void func_08934114(Owner *self, Ent *e) {
    if (!B::has(self->flags, 0x100)) {
        Fb *fp = (Fb *)((char *)e + 0x190);
        unsigned char fb = fp->fb;
        if (B::has(fb, 1) && !B::has(fb, 2)) {
            if (func_0883739C(e) != 0 && func_08B33418(e) != 0) {
                if (func_088374B0(e) == 0) {
                    func_0882FA10(e->motion - 0x130 + 0x130, 1, D_089C7C2C, 0.0f, 0);
                    func_08834C10(e, 0, 0);
                    return;
                }
                if (!B::has(e->sub->flags, 1)) {
                    func_08834934(e, 0x65, 0x66);
                    func_08834948(e);
                    return;
                }
            }
        }
    }
    self->v8(e);
}
