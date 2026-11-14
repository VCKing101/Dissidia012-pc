extern "C" int D_089C9680;

/* Loader with its vtable pointer at 0x74. */
struct Loader {
    int handle;
    int field_04;
    char pad_08[0x50];
    int field_58;
    char pad_5C[0x18];
    void *vtable;

    static Loader *construct(Loader *self, void *vt)
    {
        self->vtable = vt;
        self->handle = -1;
        self->field_04 = 0;
        self->field_58 = 0;
        return self;
    }
};

extern "C" {

Loader *func_08871318(Loader *self)
{
    return Loader::construct(self, &D_089C9680);
}

}
