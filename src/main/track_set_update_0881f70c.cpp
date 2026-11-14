/* Advances every track of a track set. */
struct Track0881F70C {
    char data[0x44];
};

struct TrackSet0881F70C {
    short active;
    short count;
    Track0881F70C *tracks;

    static bool valid(TrackSet0881F70C *s) { return s->active != 0; }
};

extern "C" {

void func_0881FAEC(Track0881F70C *track, float time);

int func_0881F70C(TrackSet0881F70C *self, float time)
{
    if (!TrackSet0881F70C::valid(self)) {
        return -1;
    }
    for (int i = 0; i < self->count; i++) {
        func_0881FAEC(&self->tracks[i], time);
    }
    return 0;
}

}
