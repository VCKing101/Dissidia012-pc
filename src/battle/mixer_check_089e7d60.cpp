struct Channel { char data[0xDC]; };
struct Stream { char data[0x28]; };
struct Mixer {
    unsigned char active;
    char pad_01[0x3];
    char device[0x64C];
    Channel channels[2];
    Stream streams[2];
    char queue[0x4];
};
extern "C" {
extern int func_089DE360(void *device);
extern void func_089E6FBC(void *queue);
extern int func_089E5598(Channel *channel);
extern int func_089E91D0(Stream *stream);

/* Checks the device, then each channel and stream: 0, or -1 to -4 for the first failure. */
int func_089E7D60(Mixer *self)
{
    if (self->active == 0) {
        return -1;
    }
    if (func_089DE360(self->device) != 0) {
        return -2;
    }
    func_089E6FBC(self->queue);
    for (int i = 0; i < 2; i++) {
        if (func_089E5598(&self->channels[i]) != 0) {
            return -3;
        }
        if (func_089E91D0(&self->streams[i]) != 0) {
            return -4;
        }
    }
    return 0;
}
}
