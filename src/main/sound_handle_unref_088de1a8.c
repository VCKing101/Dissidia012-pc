/* Drops one reference; frees the handle when it reaches zero and returns 1. */
typedef struct SoundHandle {
    void *handle;
    short count;
    short id;
    unsigned short key;
} SoundHandle;

extern void func_088DE050(SoundHandle *sound);

int func_088DE1A8(SoundHandle *sound)
{
    sound->count = sound->count - 1;
    if (sound->count == 0) {
        func_088DE050(sound);
        return 1;
    }
    return 0;
}
