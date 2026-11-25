/* Creates an actor's shadow: a scaled disc under the actor. */
struct Vec4_0883919C {
    float x, y, z, w;
};

struct Shadow0883919C {
    float scale_x;
    float scale_y;
    float scale_z;
    float radius;
    Vec4_0883919C offset;
    int flags;
    char pad_24[0xC];
};

struct Actor0883919C {
    char pad_000[0x4A8];
    Shadow0883919C *shadow;
};

extern "C" {

extern float D_089C7EA8;
extern float D_089C7EAC;

void *func_088312D0(Actor0883919C *self, int size);
void func_089A3868(Vec4_0883919C *dst, Vec4_0883919C *src);

struct H {
    static void run(Actor0883919C *self)
    {
    Vec4_0883919C offset;
    Shadow0883919C *result[1];
    Actor0883919C *owner[1];
    owner[0] = self;
    Shadow0883919C *none = 0;
    Shadow0883919C *made = none;
    result[0] = none;
    Shadow0883919C *shadow = (Shadow0883919C *)func_088312D0(self, sizeof(Shadow0883919C));
    made = result[0];
    Actor0883919C *me = owner[0];
    if (shadow != 0) {
        made = shadow;
    }
    me->shadow = made;
    Shadow0883919C *s = made;
    s->scale_x = D_089C7EA8;
    s->scale_y = 1.0f;
    s->scale_z = 1.0f;
    s->radius = D_089C7EAC;
    offset.x = 0.0f;
    offset.y = 0.5f;
    offset.z = 0.0f;
    offset.w = 1.0f;
    func_089A3868(&s->offset, &offset);
    s->flags = 0;
    }
};
void func_0883919C(Actor0883919C *self) { H::run(self); }
}
