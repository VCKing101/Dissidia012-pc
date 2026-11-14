typedef struct SoundRow {
    char pad_00[0x6];
    unsigned short field_06;
    char pad_08[0x30];
} SoundRow;

typedef struct SoundLink {
    int field_00;
    int field_04;
} SoundLink;

typedef struct SoundState {
    char pad_0000[0x668];
    int field_668;
    char pad_066C[0xF18 - 0x66C];
    unsigned int field_F18;
    char pad_F1C[0x1224 - 0xF1C];
    int field_1224;
} SoundState;

typedef struct SoundOwner {
    char pad_0000[0xDB30];
    SoundState *field_DB30;
} SoundOwner;

extern SoundLink D_08ADA3E0[];
extern SoundRow D_08ADA428[];
extern void func_088D9F5C(int target, int kind, int value);
extern void func_08ACC948(SoundOwner *self);

void func_08ACC8B0(SoundOwner *self)
{
    SoundState *state = self->field_DB30;
    unsigned short sound;
    if (state->field_1224 != -1) {
        sound = D_08ADA428[D_08ADA3E0[state->field_1224].field_00].field_06;
        if (sound != 0xFFFF) {
            func_088D9F5C(state->field_668, sound, 1);
            self->field_DB30->field_F18 = self->field_DB30->field_F18 | 0x10;
        } else {
            func_08ACC948(self);
        }
    }
}
