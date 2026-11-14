class TrackBase {
public:
    virtual void slot_008();
    virtual void slot_010();
    virtual void slot_018();
    virtual void slot_020();
    virtual void slot_028();
    virtual int isActive();
};

struct TrackOwner {
    char pad_00[0x4];
    void *body;
    char pad_08[0x98];
    int limit;
};

class TrackTask : public TrackBase {
public:
    int field_04;
    TrackOwner *owner;
    int target;
    float time;
    float length;
    int count;
};

class AlertTask : public TrackBase {
public:
    int field_04;
    TrackOwner *owner;
    void *target;
    int limit;
    unsigned char started;
    char pad_15[0x3];
    int frames;
    unsigned char done;
};

extern "C" {

extern int func_08A87AA4(TrackOwner *owner, void *task);

/* Registers the alert for target with its owner and starts it; -1 inactive, -2 no target, -3 registration failed. */
int func_08A916C4(AlertTask *self, void *target)
{
    if (!self->isActive()) {
        return -1;
    }
    if (target == 0) {
        return -2;
    }
    if (func_08A87AA4(self->owner, self) != 0) {
        return -3;
    }
    self->target = target;
    self->limit = self->owner->limit;
    self->started = 1;
    self->frames = 30;
    self->done = 0;
    return 0;
}

}
