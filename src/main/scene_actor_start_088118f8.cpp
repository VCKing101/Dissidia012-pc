/* Starts the scene actor's opening motion: the special pair 0x65/0x66 when
   the actor supports it, otherwise motion 1; then finishes the setup. */
struct Motion088118F8 {
    char data[1];
};

extern "C" {
extern float D_089C7C2C;
void func_0882FA10(Motion088118F8 *motion, int id, int mode, float time, float start);
}

struct Actor088118F8 {
    char pad_000[0x130];
    Motion088118F8 motion;

    static void play(Actor088118F8 *a, int id) { func_0882FA10(&a->motion, id, 0, D_089C7C2C, 0.0f); }
};

struct SceneCtl088118F8 {
    Actor088118F8 *actor;
    void *info;

    static bool active(SceneCtl088118F8 *c) { return c->info != 0; }
};

extern "C" {

int func_088374B0(Actor088118F8 *actor);
void func_08834934(Actor088118F8 *actor, int a, int b);
void func_08834948(Actor088118F8 *actor);
void func_088118B8(SceneCtl088118F8 *self);

void func_088118F8(SceneCtl088118F8 *self)
{
    if (!SceneCtl088118F8::active(self)) {
        return;
    }
    if (func_088374B0(self->actor)) {
        Actor088118F8::play(self->actor, 0x65);
        func_08834934(self->actor, 0x65, 0x66);
        func_08834948(self->actor);
    } else {
        Actor088118F8::play(self->actor, 1);
    }
    func_088118B8(self);
}

}
