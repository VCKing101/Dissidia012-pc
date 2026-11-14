typedef struct Delay0887A544 {
    int field_00;
    void *data;
    unsigned char unknown_08[0x20];
    volatile int delay;
} Delay0887A544;

void *func_0887A544(Delay0887A544 *self) {
    if (self->delay <= 0) {
        if (self->delay < 0) {
            void *data = self->data;
            self->data = 0;
            return data;
        }
        self->delay = self->delay - 1;
    }
    return 0;
}
