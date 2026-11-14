class TrackBase {
public:
    virtual void slot_008();
    virtual void slot_010();
    virtual void slot_018();
    virtual void slot_020();
    virtual void slot_028();
    virtual int isActive();
};

struct Vec4 {
    float x;
    float y;
    float z;
    float w;
};

struct AimBody {
    char pad_00[0x80];
    Vec4 pos;
};

struct AimOwner {
    char pad_00[0x4];
    AimBody *body;
};

struct AimLimits {
    char pad_00[0xB0];
    unsigned int count;
};

class AimTask : public TrackBase {
public:
    int field_04;
    AimOwner *owner;
    unsigned int slot;
    Vec4 pos;
    unsigned char started;
    char pad_21[0x3];
    int frames;
};

extern "C" {

extern int D_08AAAD80;
extern AimLimits *func_08AA79C4(int key);
extern int func_08A87AA4(AimOwner *owner, void *task);
extern void func_089A3868(Vec4 *to, Vec4 *from);

/* Registers the aim for slot at the owner's position; -1 inactive, -2 bad slot, -3 registration failed. */
int func_08A92A44(AimTask *self, unsigned int slot)
{
    if (!self->isActive()) {
        return -1;
    }
    if (slot >= func_08AA79C4(D_08AAAD80)->count) {
        return -2;
    }
    if (func_08A87AA4(self->owner, self) != 0) {
        return -3;
    }
    self->slot = slot;
    func_089A3868(&self->pos, &self->owner->body->pos);
    self->started = 1;
    self->frames = 30;
    return 0;
}

}
