/* Controller checks. */
struct Motion0881F128 {
    char pad_0[0xA];
    short flags;
};

struct Actor0881F128 {
    char pad_000[0x380];
    unsigned int flags;
    char pad_384[4];
    unsigned long long state;
    char pad_390[0x564 - 0x390];
    Motion0881F128 *motion;

    static bool any(Actor0881F128 *a, unsigned long long mask) { return (a->state & mask) != 0; }
};

struct Ctl0881F128 {
    unsigned char ready;
    char pad_01[3];
    Actor0881F128 *actor;
    char a[0x1C];
    char b[1];
};

extern "C" {

extern unsigned long long D_089C7648;

int func_0881F688(char *a);
int func_0882039C(char *b);
int func_0881F288(Ctl0881F128 *self, int mode);
int func_0881F70C(char *a, float time);
int func_08820420(char *b, float time);

int func_0881F128(Ctl0881F128 *self)
{
    if (!self->ready) {
        return -1;
    }
    if (func_0881F688(self->a)) {
        return -2;
    }
    if (func_0882039C(self->b)) {
        return -3;
    }
    if (self->actor->motion->flags & 4) {
        if (func_0881F288(self, 1)) {
            return -5;
        }
    }
    return 0;
}

int func_0881F1C0(Ctl0881F128 *self, float time)
{
    if (!self->ready) {
        return -1;
    }
    Actor0881F128 *a = self->actor;
    unsigned int active = a->flags & 1;
    if (active != 1 || Actor0881F128::any(a, D_089C7648)) {
        return 0;
    }
    if (func_0881F70C(self->a, time)) {
        return -2;
    }
    if (func_08820420(self->b, time)) {
        return -3;
    }
    return 0;
}

}
