typedef struct ApplyActor {
    char pad_00[0xD64];
    char field_D64[4];
} ApplyActor;

typedef struct Fighter {
    char pad_00[0x5D4];
    ApplyActor *actor;
    char pad_5D8[0x34];
    short field_60C;
    char pad_60E[5];
    unsigned char field_613;
} Fighter;

extern void *func_0884F6F4(void *dst, int value);

void *func_0885142C(Fighter *self)
{
    ApplyActor *actor = self->actor;
    if (self->field_613 != 0) {
        return func_0884F6F4(actor->field_D64, self->field_60C);
    }
    return actor;
}
