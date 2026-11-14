/* Effect manager: fades a looping sound entry by how far a value has gone
   past the entry's threshold. */
struct SoundEntry0881C82C {
    int sound;
    float left;
    float right;
    float range;
    float threshold;
};

struct SoundTable0881C82C {
    char pad_00[0xC];
    unsigned int count;
    SoundEntry0881C82C *entries;
};

struct EffectMgr0881C82C {
    unsigned char ready;
    char pad_01[7];
    unsigned char locked;
};

extern "C" {

extern SoundTable0881C82C *D_089D4910;

void func_088749A0(int sound, int mode, float left, float right);

int func_0881C82C(EffectMgr0881C82C *self, unsigned int index, float value, float limit)
{
    if (!self->ready) {
        return -1;
    }
    SoundTable0881C82C *t = D_089D4910;
    if (index >= t->count) {
        return 0;
    }
    if (self->locked == 1) {
        return 0;
    }
    SoundEntry0881C82C *e = &t->entries[index];
    if (!(e->threshold <= limit)) {
        return 0;
    }
    if (e->range == 0.0f) {
        return 0;
    }
    if (e->range < value) {
        return 0;
    }
    float f = 1.0f - value / e->range;
    func_088749A0(e->sound, 0, e->left * f * 0.01f, e->right * f * 0.01f);
    return 0;
}

}
