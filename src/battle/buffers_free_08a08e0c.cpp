struct BufferBank {
    unsigned char active;
    char pad_01[0x3];
    void *main;
    void *buffers[2];
};
extern "C" {
extern void func_088DA060(void *buffer);

/* Frees the two buffers and the main one. */
int func_08A08E0C(BufferBank *self)
{
    if (self->active == 0) {
        return -1;
    }
    for (int i = 0; i < 2; i++) {
        if (self->buffers[i] != 0) {
            func_088DA060(self->buffers[i]);
            self->buffers[i] = 0;
        }
    }
    if (self->main != 0) {
        func_088DA060(self->main);
        self->main = 0;
    }
    return 0;
}
}
