typedef struct SoundLink {
    unsigned char active;
    char pad_01[0x3];
    char link[0x20];
    void *buffer;
    int bank;
    int sound;
} SoundLink;

extern void func_08889F70(int bank, int sound);
extern void func_089F7E40(void *link);
extern void func_088DA060(void *buffer);

/* Releases the sound, unlinks it and frees its buffer, if it holds one. */
int func_08A203E4(SoundLink *self)
{
    if (self->active == 0) {
        return -1;
    }
    if (self->buffer != 0) {
        func_08889F70(self->bank, self->sound + 5);
        func_089F7E40(self->link);
        if (self->buffer != 0) {
            func_088DA060(self->buffer);
            self->buffer = 0;
        }
    }
    return 0;
}
