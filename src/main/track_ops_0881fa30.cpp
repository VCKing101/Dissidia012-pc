/* Animation track: restarts the track from its beginning. */
template <class T> struct Kept0881FA30 {
    T value;
    Kept0881FA30(T v) : value(v) {}
};

struct TrackDef0881FA30 {
    char pad_00[0x30];
    float start;
    float end;
    int mode;
};

struct Track0881FA30 {
    char pad_00[4];
    unsigned char active;
    char pad_05[3];
    TrackDef0881FA30 *def;
    float time;
    int loops;
    float elapsed;
    float base;
    float range;
    float scale;
    unsigned char direction;
    char pad_25[3];
    float position;
    float length;
    float blend;
    float offset_x;
    float offset_y;
    float offset_z;
};

extern "C" {

float func_0881FC58(Track0881FA30 *self);
float func_0881FC7C(Track0881FA30 *self);
float func_0881FCA0(Track0881FA30 *self);
float func_0881FCC4(Track0881FA30 *self);
float func_0881FCE8(Track0881FA30 *self);
float func_0881FD0C(Track0881FA30 *self);
float func_0881FD30(Track0881FA30 *self);
float func_0881FD54(Track0881FA30 *self);
int func_0881FAEC(Track0881FA30 *self, float time);
float func_0881FDE4(Track0881FA30 *self, float from, float to);

/* Restarts the track from its beginning. */
int func_0881FA30(Track0881FA30 *self)
{
    if (!self->active) {
        return -1;
    }
    self->time = 0.0f;
    self->loops = 1;
    self->elapsed = 0.0f;
    self->base = func_0881FC58(self);
    self->range = func_0881FC7C(self);
    self->scale = func_0881FCA0(self);
    self->direction = 0;
    self->position = 1.0f;
    self->length = 1.0f;
    self->blend = 0.0f;
    self->offset_x = 0.0f;
    self->offset_y = 0.0f;
    self->offset_z = 0.0f;
    if (func_0881FAEC(self, func_0881FD54(self))) {
        return -2;
    }
    return 0;
}

}
