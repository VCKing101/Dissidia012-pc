/* Plays the sounds of the menu's sixteen sound slots that the mask selects:
   bit 0 for slots with a delay or loop, bit 1 for plain slots. */
extern "C" {
struct SoundSlot {
    short id;
    char pad_02[2];
    signed char delay;
    char pad_05;
    unsigned char volume;
    char pad_07;
    signed char loops;
    char pad_09;
};

struct Menu {
    char pad_000[0x74];
    int voice;
    char pad_078[0x612 - 0x78];
    SoundSlot slots[16];
};

struct SoundBank {
    int loaded;
};

SoundBank *func_08899300(void);
int func_088985F0(SoundBank *bank, int id);
int func_08893A18(Menu *self);
void func_0886C38C(int voice, int sound, int flag, int volume, int pan, int arg);

void func_0889245C(Menu *self, unsigned int mask)
{
    if (!(unsigned char)(func_08899300()->loaded != 0)) {
        return;
    }
    for (int i = 0; i < 16; i++) {
        if (self->slots[i].id >= 0) {
            if (self->slots[i].delay != 0 || self->slots[i].loops > 0) {
                if (!(mask & 1)) {
                    continue;
                }
            } else if (!(mask & 2)) {
                continue;
            }
            int sound = func_088985F0(func_08899300(), self->slots[i].id);
            int volume = self->slots[i].volume;
            if (volume > 0) {
                func_0886C38C(self->voice, sound, 1, volume, func_08893A18(self), 0);
            }
        }
    }
}

}
