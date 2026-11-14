/* Moves an actor onto its own ground point, keeps a fall below the ground
   in its body and sets the body's landing time. */
struct Vec4_0883497C {
    float x, y, z, w;
};

struct Body0883497C {
    char pad_00[0x20];
    float fall;
    char pad_24[0x38 - 0x24];
    float land_time;
};

struct Actor0883497C {
    char pad_000[0xF0];
    Vec4_0883497C pos;
    char pad_100[0x314 - 0x100];
    Body0883497C *body;
};

extern "C" {

void func_08835C24(Actor0883497C *self, Vec4_0883497C *pos);
void func_08834948(Actor0883497C *self);

void func_0883497C(Actor0883497C *self, float time)
{
    float zero = 0.0f;
    Vec4_0883497C p;
    Vec4_0883497C t = self->pos;
    t.w = zero;
    p = t;
    func_08835C24(self, &p);
    func_08834948(self);
    if (self->pos.y < zero) {
        self->body->fall = self->pos.y;
    }
    self->body->land_time = time;
}

}
