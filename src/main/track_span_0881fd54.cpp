/* The time span a track plays over: the full computed span for mode 1,
   otherwise the definition's own start and end. */
template <class T> struct Kept0881FD54 {
    T value;
    Kept0881FD54(T v) : value(v) {}
};

struct TrackDef0881FD54 {
    char pad_00[0x30];
    float start;
    float end;
    int mode;
};

struct Track0881FD54 {
    char pad_00[8];
    TrackDef0881FD54 *def;
};

extern "C" {

float func_0881FC58(Track0881FD54 *self);
float func_0881FCC4(Track0881FD54 *self);
float func_0881FCE8(Track0881FD54 *self);
float func_0881FDE4(Track0881FD54 *self, float from, float to);

float func_0881FD54(Track0881FD54 *self)
{
    if (self->def->mode == 1) {
        Kept0881FD54<Track0881FD54 *> track(self);
        float lead = func_0881FCE8(track.value);
        float body = func_0881FCC4(track.value);
        float tail = func_0881FC58(track.value);
        return func_0881FDE4(track.value, 0.0f, tail * 2.0f + body + lead);
    }
    return func_0881FDE4(self, self->def->start, self->def->end);
}

}
