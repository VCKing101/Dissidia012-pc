struct RangeObject {
    char pad_00[0x4];
    int refs;
    const void *vtable;

    static void construct(RangeObject *self, const void *vtable)
    {
        self->vtable = vtable;
        self->refs = 1;
    }
};

extern "C" {

extern const unsigned char D_08AA5818[];
extern void func_08AA56C0(RangeObject *self);

RangeObject *func_08AA5628(RangeObject *self)
{
    RangeObject::construct(self, D_08AA5818);
    func_08AA56C0(self);
    return self;
}

}
