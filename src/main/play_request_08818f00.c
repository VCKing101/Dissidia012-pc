typedef struct Agent4Player {
    unsigned char unknown_0000[0xC];
    unsigned char ready;
} Agent4Player;

int func_088192C8(Agent4Player *self, int id, int mode, unsigned char option);

int func_08818F00(Agent4Player *self, int id, unsigned char option) {
    if (!self->ready) {
        return -1;
    }
    return func_088192C8(self, id, 1, option);
}

int func_08818F4C(Agent4Player *self) {
    int result = -1;
    if (!self->ready) {
        return result;
    }
    return func_088192C8(self, -1, 0, 0);
}
