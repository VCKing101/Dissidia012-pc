/* Stop the link's effect. Retail keeps self in the frame across the call. */
template <class T> struct Kept {
    T value;
    Kept(T v) : value(v) {}
};

typedef struct LinkState {
    unsigned char active;
    char pad_01[0x2F];
    void *effect;
} LinkState;

extern "C" {

extern void func_088A6A78(void *effect);

int func_08AC0840(LinkState *self)
{
    void *effect;
    if (self->active == 0) {
        return -1;
    }
    effect = self->effect;
    if (effect != 0) {
        Kept<LinkState *> saved(self);
        func_088A6A78(effect);
        self = saved.value;
        self->effect = 0;
    }
    return 0;
}

}
