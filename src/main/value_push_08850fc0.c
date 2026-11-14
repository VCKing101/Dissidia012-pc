typedef struct Queue08850FC0 {
    unsigned char unknown_000[0x5F8];
    short values[8];
    short count;
} Queue08850FC0;

void func_08850FC0(Queue08850FC0 *self, int value) {
    if (self->count < 8) {
        short *count = &self->count;
        self->values[(*count)++] = value;
    }
}
