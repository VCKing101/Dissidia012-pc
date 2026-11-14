/* Frees the handle at 0 and resets the cached ids. */
typedef struct SoundHandle {
    void *handle;
    short count;
    short id;
    unsigned short key;
} SoundHandle;

extern void func_0899BB1C(void *handle);

void func_088DE050(SoundHandle *sound)
{
    if (sound->handle != 0) {
        func_0899BB1C(sound->handle);
        sound->id = -1;
        sound->key = 0xFFFF;
        sound->handle = 0;
    }
}
