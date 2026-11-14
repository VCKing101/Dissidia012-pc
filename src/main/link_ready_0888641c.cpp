struct Info0888641C {
    unsigned char unknown_00[0xC];
    short kind;
    unsigned char unknown_0E[0x12];
    unsigned short flags;

    static bool locked(Info0888641C *info) {
        return (info->flags & 0x20) != 0;
    }
};

struct Link0888641C {
    int handle;
    Info0888641C *info;
};

extern "C" {
int func_0888641C(Link0888641C *self) {
    if (self->handle != 0 && self->info != 0) {
        if (self->info->kind == 2 && Info0888641C::locked(self->info)) {
            return 0;
        }
        return 1;
    }
    return 0;
}
}
