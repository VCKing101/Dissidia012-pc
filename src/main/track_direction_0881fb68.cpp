/* Animation track: switches direction keeping the relative position. */
template <class T> struct Kept0881FB68 {
    T value;
    Kept0881FB68(T v) : value(v) {}
};

struct TrackDef0881FB68 {
    char pad_00[0x30];
    float start;
    float end;
    int mode;
};

struct Track0881FB68 {
    char pad_00[4];
    unsigned char active;
    char pad_05[3];
    TrackDef0881FB68 *def;
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

float func_0881FD0C(Track0881FB68 *self);
float func_0881FD30(Track0881FB68 *self);

}

extern "C" {

/* Switches the track's direction, keeping its relative position. */
int func_0881FB68(Track0881FB68 *self, unsigned char direction)
{
    if (!self->active) {
        return -1;
    }
    if (self->direction != direction) {
        float rest = 1.0f - self->position / self->length;
        float length;
        if (direction == 1) {
            length = func_0881FD0C(self);
        } else {
            length = func_0881FD30(self);
        }
        self->direction = direction;
        self->length = length;
        self->position = length * rest;
    }
    return 0;
}

}
