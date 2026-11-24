/* Starts the music player at the current track's volume; returns whether a
   track was available. */
typedef struct TrackInfo {
    char pad_00[0xD];
    signed char volume;
} TrackInfo;

typedef struct Music {
    char pad_00[0x1C];
    char player[4];
} Music;

extern TrackInfo *func_0889F3E4(Music *self);
extern void func_0889F750(float volume, void *player, int flag, void (*fn)(void), void *arg);

int func_0889F2D4(Music *self)
{
    TrackInfo *info = func_0889F3E4(self);

    if (info != 0) {
        func_0889F750(info->volume * 0.01f, self->player, 0, 0, 0);
        return 1;
    } else {
        return 0;
    }
}
