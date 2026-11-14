extern "C" void func_088DA060(void *handle);

struct LoadedResource110DC {
    char pad_00[0x20];
    unsigned short flags;
};

struct PendingRelease {
    unsigned char active;
    char pad_01[0x7];
    LoadedResource110DC *res;
    unsigned char pending;

    static bool isLoaded(LoadedResource110DC *res) { return (res->flags & 0x20) != 0; }
    static void release(PendingRelease *self)
    {
        if (self->res != 0) {
            func_088DA060(self->res);
            self->res = 0;
        }
    }
};

extern "C" {

int func_08A110DC(PendingRelease *self)
{
    if (!self->active) {
        return -1;
    }
    if (self->res != 0 && PendingRelease::isLoaded(self->res) == true && self->pending == 1) {
        PendingRelease::release(self);
        self->pending = 0;
    }
    return 0;
}

}
