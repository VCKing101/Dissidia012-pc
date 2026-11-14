extern "C" void func_088DA060(void *handle);

struct LoadedResource {
    char pad_00[0x20];
    unsigned short flags;
};

struct ResourceOwner {
    unsigned char active;
    char pad_01[0x3];
    LoadedResource *res;

    static bool isLoaded(LoadedResource *res) { return (res->flags & 0x20) != 0; }
    static void release(ResourceOwner *self)
    {
        if (self->res != 0) {
            func_088DA060(self->res);
            self->res = 0;
        }
    }
};

extern "C" {

int func_08A0D814(ResourceOwner *self)
{
    if (!self->active) {
        return -1;
    }
    if (self->res != 0 && ResourceOwner::isLoaded(self->res) == true) {
        ResourceOwner::release(self);
    }
    return 0;
}

}
