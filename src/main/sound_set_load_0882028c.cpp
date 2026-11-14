/* Builds a sound set for an actor's voice group from the sound table. */
struct SoundGroup0882028C {
    int count;
    int first;
};

struct SoundDef0882028C {
    char data[0x48];
};

struct SoundTable0882028C {
    char pad_00[0x2C];
    unsigned int group_count;
    SoundGroup0882028C *groups;
    char pad_34[4];
    SoundDef0882028C *defs;
};

struct Voice0882028C {
    char pad_0[9];
    signed char group;
};

struct Actor0882028C {
    char pad_000[0x564];
    Voice0882028C *voice;
};

struct Sound0882028C {
    char data[0x3C];
};

struct SoundSet0882028C {
    short state;
    short count;
    Sound0882028C *sounds;
};

extern "C" {

extern SoundTable0882028C *D_089D4910;

void func_08820250(SoundSet0882028C *self);
Sound0882028C *func_08842100(int size);
void func_088206E8(Sound0882028C *sound, SoundDef0882028C *def);

int func_0882028C(SoundSet0882028C *self, Actor0882028C *actor)
{
    func_08820250(self);
    if (actor == 0) {
        return -1;
    }
    SoundTable0882028C *t = D_089D4910;
    int group = actor->voice->group;
    if (group < t->group_count) {
        SoundGroup0882028C *g = &t->groups[group];
        self->count = g->count;
        self->sounds = func_08842100(self->count * sizeof(Sound0882028C));
        if (self->sounds == 0) {
            return -2;
        }
        for (int i = 0; i < self->count; i++) {
            func_088206E8(&self->sounds[i], &t->defs[i + g->first]);
        }
    }
    self->state = 1;
    return 0;
}

}
