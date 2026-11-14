struct EffectHandle;

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

void func_08AD206C(EffectUser *self)
{
    if (self->slot_020() == 1) {
        func_088A6AA4(self->handle);
        self->handle = 0;
    }
}

}
