struct SoundBank {
    unsigned char active;
    char pad_01[0x3];
    void *main;
    void *buffers[4];
    int bank;
    int sound;
};

extern "C" {

extern void func_088DA060(void *buffer);
extern void func_08889F70(int bank, int sound);

/* Frees the four buffers and the main one, then releases the bank's sound. */
int func_08A00BCC(SoundBank *self)
{
    if (self->active == 0) {
        return -1;
    }
    for (int i = 0; i < 4; i++) {
        if (self->buffers[i] != 0) {
            func_088DA060(self->buffers[i]);
            self->buffers[i] = 0;
        }
    }
    if (self->main != 0) {
        func_088DA060(self->main);
        self->main = 0;
    }
    func_08889F70(self->bank, self->sound + 5);
    return 0;
}

}
