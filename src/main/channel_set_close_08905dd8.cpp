/* Closes every open channel and the set handle. */
struct Channel {
    char pad_00[8];
    int handle;
    char pad_0C[0x2C - 0xC];
};

struct ChannelSet {
    void *vtable;
    Channel channels[16];
    int count;
    int handle;
};

extern "C" {
extern void func_089C59A8(int handle, int arg);

int func_08905DD8(ChannelSet *set)
{
    for (int i = 0; i < set->count; i++) {
        Channel *ch = &set->channels[i];

        func_089C59A8(ch->handle, 0);
    }
    if (set->handle != -1) {
        func_089C59A8(set->handle, 0);
        set->handle = -1;
    }
    set->count = 0;
    return 1;
}
}
