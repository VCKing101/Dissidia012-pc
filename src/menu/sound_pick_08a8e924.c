typedef struct SoundState {
    char pad_000[0x21C];
    int field_21C;
    int field_220;
} SoundState;

typedef struct SoundOwner {
    char pad_00[0x8];
    SoundState *field_08;
} SoundOwner;

typedef struct SoundIds {
    unsigned short field_00;
    unsigned short field_02;
} SoundIds;

extern SoundIds D_08A93B42;
extern void func_088D9F5C(int target, int kind, int value);

void func_08A8E924(SoundOwner *self, int second)
{
    SoundState *state = self->field_08;
    if (second == 0) {
        func_088D9F5C(state->field_21C, D_08A93B42.field_00, 1);
    } else {
        func_088D9F5C(state->field_220, D_08A93B42.field_02, 1);
    }
}
