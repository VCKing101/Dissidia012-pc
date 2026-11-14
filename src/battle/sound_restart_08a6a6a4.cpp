extern "C" {
#include "battle/agent15_types.h"
}

template <class T> struct Kept {
    T value;
    Kept(T v) : value(v) {}
};

struct SoundTask {
    unsigned char active;
    char pad_01[0x63];
    int unit;
    char pad_68[0x8];
    int sound;
};

struct CommonPitch {
    char pad_00[0x28];
    float pitch;
};

extern "C" {

extern void func_08A65A84(int sound);
extern int func_08AB327C(int unit);
extern void func_08AB05EC(int id);
extern void func_08A65AAC(int sound);
extern void func_08A65B50(int sound, int volume);
extern void func_08A65C64(int sound, float pitch);
extern void func_08A65BAC(int sound, int mode);
extern void func_08A65AFC(int sound);

/* Restarts the unit's sound at the common pitch; -1 inactive, -2 no common parameters. */
int func_08A6A6A4(SoundTask *self)
{
    if (self->active == 0) {
        return -1;
    }
    CommonPitch *params = (CommonPitch *)func_08AACB10(func_08A80200(D_08A80218));
    if (params == 0) {
        return -2;
    }
    Kept<CommonPitch *> kept(params);
    func_08A65A84(self->sound);
    int id = func_08AB327C(self->unit);
    float pitch = kept.value->pitch;
    func_08AB05EC(id);
    func_08A65AAC(self->sound);
    func_08A65B50(self->sound, 0x3F);
    func_08A65C64(self->sound, pitch);
    func_08A65BAC(self->sound, 4);
    func_08A65AFC(self->sound);
    return 0;
}

}
