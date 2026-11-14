/* World position of the bone an actor is attached by, or the actor's own
   position when it has no attachment bone. */
struct Vec4_08838D14 {
    float x, y, z, w;
};

struct Matrix08838D14 {
    float m[12];
    Vec4_08838D14 pos;
};

struct Attach08838D14 {
    char pad_00[0x4C];
    int bone;
};

struct Body08838D14 {
    char pad_00[0xA4];
    Attach08838D14 attach;
};

struct Actor08838D14 {
    char pad_000[0x80];
    Vec4_08838D14 pos;
    char pad_090[0x394 - 0x90];
    Body08838D14 *body;
};

extern "C" {

void func_088369A4(Actor08838D14 *self, Matrix08838D14 *out, int bone);

Vec4_08838D14 func_08838D14(Actor08838D14 *self)
{
    if (self->body != 0) {
        Attach08838D14 *a = &self->body->attach;
        int bone = a->bone;
        if (bone != -1) {
            Matrix08838D14 m;
            func_088369A4(self, &m, bone);
            return m.pos;
        }
    }
    return self->pos;
}

}
