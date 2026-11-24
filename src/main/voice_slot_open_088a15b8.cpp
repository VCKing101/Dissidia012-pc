/* Claims the first free of twenty voice slots and opens a voice on it;
   returns the slot, or -1 when none is free or the open fails. */
extern "C" {
struct VoicePool {
    char pad_00[0x68];
    unsigned char used[20];
};

int func_0899075C(int sound, int channel, int slot);

int func_088A15B8(VoicePool *self, int sound, int channel)
{
    for (int i = 0; i < 20; i++) {
        if (*((unsigned char *)self + 0x68 + i) == 0) {
            *((unsigned char *)self + 0x68 + i) = 1;
            if (func_0899075C(sound, channel, i) == 1) {
                return i;
            }
            return -1;
        }
    }
    return -1;
}

}
