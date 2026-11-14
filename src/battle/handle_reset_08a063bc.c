typedef struct HandleTimer {
    unsigned char active;
    char pad_01[0x3];
    int field_04;
    void *handle;
    float field_0C;
} HandleTimer;

extern void func_088DA060(void *handle);

int func_08A063BC(HandleTimer *self)
{
    if (!self->active) {
        return -1;
    }
    if (self->handle != 0) {
        func_088DA060(self->handle);
        self->handle = 0;
    }
    {
        float zero = 0.0f;
        self->field_04 = 0;
        self->field_0C = zero;
    }
    return 0;
}
