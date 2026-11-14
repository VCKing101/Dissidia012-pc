typedef struct Sound { char pad_00[0xC4]; int muted; } Sound;
typedef struct SoundRef { int field_00; int field_04; int handle; } SoundRef;
typedef struct Pad { char pad_00[0x84]; SoundRef *sound; char pad_88[0x1C]; int field_A4[25]; } Pad;
extern Sound *func_089B0E78(int handle);
void func_0886CB0C(Pad *self, int idx, unsigned char on)
{
    if (idx < 25) {
        if (self->field_A4[idx] != -1) {
            Sound *s = func_089B0E78(self->sound->handle);
            s->muted = on == 0;
        }
    }
}
