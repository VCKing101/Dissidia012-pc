typedef struct Agent4Moded {
    unsigned char valid;
    unsigned char unknown_01[3];
    int *mode;
} Agent4Moded;

void func_08820AE0(Agent4Moded *self, int arg);

int func_08820A14(Agent4Moded *self, int arg) {
    if (!self->valid) {
        return -1;
    }
    if (*self->mode >= 0 && *self->mode < 3) {
        func_08820AE0(self, arg);
    }
    return 0;
}

int func_08820A78(Agent4Moded *self, int arg) {
    if (!self->valid) {
        return -1;
    }
    if (*self->mode >= 3 && *self->mode < 6) {
        func_08820AE0(self, arg);
    }
    return 0;
}
