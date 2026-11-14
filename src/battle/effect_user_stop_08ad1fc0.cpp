struct EffectMode {
    char pad_00[0x14];
    int mode;

    static int modeOf(EffectMode *mode) { return mode->mode; }
};

struct EffectInfo {
    char pad_00[0x38];
    EffectMode mode;
};

struct EffectHandle {
    char pad_00[0xC0];
    EffectInfo *info;
};

class EffectUserBase {
public:
    virtual void slot_008();
    virtual void slot_010();
    virtual void slot_018();
    virtual int slot_020();
};

class EffectUser : public EffectUserBase {
public:
    EffectHandle *handle;
};

extern "C" {

extern void func_088A6AA4(EffectHandle *handle);
extern void func_088A6A78(EffectHandle *handle);

/* Stop or fade out the effect; modes 1 and 2 force the choice. */
void func_08AD1FC0(EffectUser *self, unsigned char fade)
{
    if (self->slot_020() == 1) {
        EffectHandle *handle = self->handle;
        switch (EffectMode::modeOf(&handle->info->mode)) {
        case 1:
            fade = 0;
            break;
        case 2:
            fade = 1;
            break;
        }
        if (fade == 1) {
            func_088A6AA4(handle);
        } else {
            func_088A6A78(handle);
        }
        self->handle = 0;
    }
}

}
