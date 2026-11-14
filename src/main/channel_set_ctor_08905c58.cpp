/* Constructor: vtable, sixteen 0x2C-byte channels, no open handle. */
struct Channel {
    char data[0x2C];
};

struct ChannelSet {
    void *vtable;
    Channel channels[16];
    int count;
    int handle;

    static void reset(ChannelSet *set)
    {
        set->count = 0;
        set->handle = -1;
    }
};

extern "C" {
extern char D_089CDBE0[];
extern void func_08905884(void);
extern void func_08993560(void *array, int count, int size, void (*ctor)(void));

ChannelSet *func_08905C58(ChannelSet *set)
{
    set->vtable = D_089CDBE0;
    func_08993560(set->channels, 0x10, 0x2C, func_08905884);
    ChannelSet::reset(set);
    return set;
}
}
