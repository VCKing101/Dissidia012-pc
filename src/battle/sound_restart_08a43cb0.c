typedef struct SoundData43CB0 {
    char pad_000[0x10];
    int field_10;
    float field_14;
    char pad_018[0x34];
    void *voice;
} SoundData43CB0;

typedef struct SoundBank43CB0 {
    char pad_000[0xCC];
    int field_CC;
} SoundBank43CB0;

typedef struct SoundFighter43CB0 {
    char pad_000[0x390];
    SoundBank43CB0 *bank;
} SoundFighter43CB0;

typedef struct SoundOwner43CB0 {
    SoundFighter43CB0 *fighter;
    char pad_004[0x10];
    SoundData43CB0 *data;
} SoundOwner43CB0;

extern void func_0887BA5C(void *voice);
extern void *func_0887AD78(int bank, int a, int id, int b, int c, int d);
extern void func_0887B9C4(void *voice);

void func_08A43CB0(SoundOwner43CB0 *self, int id)
{
    if (self->data->voice) {
        func_0887BA5C(self->data->voice);
        self->data->voice = 0;
    }
    self->data->voice = func_0887AD78(self->fighter->bank->field_CC, 0, id, 0, 0, 0);
    if (self->data->voice) {
        func_0887B9C4(self->data->voice);
    }
    self->data->field_10 = 0;
    self->data->field_14 = 0.0f;
}
