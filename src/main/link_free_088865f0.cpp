struct Info088865F0 {
    unsigned char unknown_00[0xC];
    short kind;
    unsigned char unknown_0E[0x12];
    unsigned short flags;

    static bool locked(Info088865F0 *info) {
        return (info->flags & 0x20) != 0;
    }
};

struct Link088865F0 {
    int handle;
    Info088865F0 *info;
};

extern "C" {
int func_088865F0(Link088865F0 *self) {
    Info088865F0 *info = self->info;
    int result = 0;
    if (info != 0) {
        if (info->kind == 0) {
            result = 1;
        } else if (info->kind == 2 && !Info088865F0::locked(info)) {
            result = 1;
        }
    }
    return result;
}
}
