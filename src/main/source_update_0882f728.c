typedef struct Agent4SourceUser {
    unsigned char unknown_0000[0x90];
    void *source;
    int value;
} Agent4SourceUser;

int func_088298E4(void *source, int arg);

void func_0882F728(Agent4SourceUser *self, int arg) {
    if (self->source == 0) {
        return;
    }
    self->value = func_088298E4(self->source, arg);
}
