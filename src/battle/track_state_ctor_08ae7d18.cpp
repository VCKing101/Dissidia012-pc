struct TrackState {
    int field_00;
    int index;
    float time;
    float speed;
    char pad_10[0x18];
    int field_28;
    char pad_2C[0x8];
    const void *vtable;

    static TrackState *construct(TrackState *self, const void *vtable)
    {
        self->vtable = vtable;
        self->field_00 = 0;
        self->index = -1;
        self->time = 0.0f;
        self->speed = 0.0f;
        self->field_28 = 0;
        return self;
    }
};

extern "C" {

extern const unsigned char D_08AEA828[];

TrackState *func_08AE7D18(TrackState *self)
{
    return TrackState::construct(self, D_08AEA828);
}

}
