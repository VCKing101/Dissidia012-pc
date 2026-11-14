/* Ends an actor's held stance states once their motion has finished,
   then clears the per-frame stance bits. */
struct Shadow08833228 {
    char pad_000[0x302];
    unsigned char show;
    unsigned char cast;
};

struct Actor08833228 {
    char pad_000[0x180];
    short motion_id;
    char pad_182[0x380 - 0x182];
    unsigned int flags;
    unsigned int stance;
    char pad_388[0x534 - 0x388];
    Shadow08833228 *shadow;
    char pad_538[0x540 - 0x538];
    short held_motion;
    char pad_542[0x544 - 0x542];
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual int motion_playing();

    static bool stance_set(Actor08833228 *a, unsigned int bit) { return (a->stance & bit) != 0; }
    static bool flag_set(Actor08833228 *a, unsigned int bit) { return (a->flags & bit) != 0; }
};

extern "C" {

void func_088341A4(Actor08833228 *self);
int func_08837310(Actor08833228 *self);
int func_088377C0(Actor08833228 *self);

void func_08833228(Actor08833228 *self)
{
    int clear = 0;
    if (!Actor08833228::stance_set(self, 0x200000) && !Actor08833228::flag_set(self, 0x20)) {
        if (self->held_motion >= 0) {
            if (self->motion_id != self->held_motion) {
                clear = 1;
                self->held_motion = -1;
            }
        } else if (!self->motion_playing() || func_08837310(self)) {
            clear = 1;
        }
    }
    if (clear) {
        func_088341A4(self);
    }
    if (!func_08837310(self) && !func_088377C0(self)) {
        self->stance &= ~0x40000;
        self->stance &= ~0x8000000;
    }
    self->stance &= ~0x10000000;
    Shadow08833228 *shadow = self->shadow;
    if (shadow != 0) {
        shadow->show = 1;
        shadow->cast = 1;
    }
}

}
