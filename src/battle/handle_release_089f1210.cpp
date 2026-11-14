extern "C" void func_088DA060(void *handle);

struct HandleHolder {
    unsigned char active;
    char pad_01[0x7];
    void *handle;

    static void release(HandleHolder *self)
    {
        if (self->handle != 0) {
            func_088DA060(self->handle);
            self->handle = 0;
        }
    }
};

extern "C" {

int func_089F1210(HandleHolder *self)
{
    if (!self->active) {
        return -1;
    }
    if (self->handle == 0) {
        return -2;
    }
    HandleHolder::release(self);
    return 0;
}

}
