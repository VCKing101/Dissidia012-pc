struct ReadyResource {
    char pad_00[0x20];
    unsigned short flags;

    static bool isLoaded(ReadyResource *res) { return (res->flags & 0x20) != 0; }
};

struct ReadySlot {
    unsigned char active;
    char pad_01[0x3];
    int state;
    char pad_08[0x8];
    int frames;
    char pad_14[0x4];
    ReadyResource *res;
};

extern "C" {

unsigned char func_089EF80C(ReadySlot *self)
{
    if (!self->active) {
        return 0;
    }
    if (self->state != 1) {
        return 0;
    }
    int result = 0;
    if (self->frames >= 16 && ReadyResource::isLoaded(self->res) == true) {
        result = 1;
    }
    return result;
}

}
