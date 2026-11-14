typedef struct StateCtx {
    char pad_0000[0x540];
    int field_540;
} StateCtx;

typedef struct StateOwner {
    char pad_0000[0x584];
    StateCtx *field_584;
} StateOwner;

unsigned char func_08AB2F24(StateOwner *self)
{
    unsigned char result = 0;
    if (self->field_584->field_540 == 1) {
        result = 1;
    }
    return result;
}
