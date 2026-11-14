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
extern float func_08A89A84(TrackTask *self);

/* Registers the task with its owner and restarts it; -1 inactive, -2 no body, -3 registration failed. */
int func_08A89934(TrackTask *self)
{
    if (!self->isActive()) {
        return -1;
    }
    if (self->owner->body == 0) {
        return -2;
    }
    if (func_08A87AA4(self->owner, self) != 0) {
        return -3;
    }
    self->target = 0;
    self->time = 0.0f;
    self->length = func_08A89A84(self);
    self->count = 0;
    return 0;
}

}
