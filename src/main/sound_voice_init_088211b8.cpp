/* Sets up a voice for a sound definition: allocates its channel buffer
   and starts it. Returns 0, or -1 (bad arguments), -2 (no memory) or -3
   (start failed, voice cleared again). */
struct SoundDef088211B8 {
    char pad_00[0x19];
    signed char channels;
};

struct Voice088211B8 {
    unsigned char active;
    signed char channels;
    char pad_02[2];
    void *owner;
    SoundDef088211B8 *def;
    char *buffer;
};

extern "C" {

void func_0882117C(Voice088211B8 *self);
char *func_08842100(int size);
int func_0882130C(Voice088211B8 *self);

int func_088211B8(Voice088211B8 *self, void *owner, SoundDef088211B8 *def)
{
    func_0882117C(self);
    if (owner == 0 || def == 0) {
        return -1;
    }
    self->channels = def->channels;
    if (self->channels != 0) {
        self->buffer = func_08842100(self->channels * 3);
        if (self->buffer == 0) {
            return -2;
        }
    }
    self->owner = owner;
    self->def = def;
    self->active = 1;
    if (func_0882130C(self)) {
        func_0882117C(self);
        return -3;
    }
    return 0;
}

}
