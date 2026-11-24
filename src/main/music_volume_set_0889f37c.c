/* Sets the music player's volume to the current track's volume times
   `scale`. */
typedef struct TrackInfo {
    char pad_00[0xD];
    signed char volume;
} TrackInfo;

typedef struct Music {
    char pad_00[0x1C];
    char player[4];
} Music;

extern TrackInfo *func_0889F3E4(Music *self);
extern void func_0889F8D4(float volume, void *player, int flag);

void func_0889F37C(float scale, Music *self)
{
    TrackInfo *info = func_0889F3E4(self);

    if (info != 0) {
        func_0889F8D4(info->volume * 0.01f * scale, self->player, 1);
    }
}
