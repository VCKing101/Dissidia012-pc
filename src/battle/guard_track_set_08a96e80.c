typedef struct GuardTrack {
    unsigned char active;
    char pad_01[0xB];
    char track[0x14];
} GuardTrack;

extern void func_08AB387C(void *track, int id);

int func_08A96E80(GuardTrack *self)
{
    if (self->active == 0) {
        return -1;
    }
    func_08AB387C(self->track, 17);
    return 0;
}
