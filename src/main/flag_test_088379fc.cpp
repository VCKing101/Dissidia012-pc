struct Agent4FlagBytes {
    unsigned char unknown_0000[0x190];
    unsigned char flags[8];
};

extern "C" {
bool func_088379FC(Agent4FlagBytes *self) {
    unsigned char *flags = self->flags;
    return (flags[4] & 0x40) != 0;
}

bool func_08837A10(Agent4FlagBytes *self) {
    unsigned char *flags = self->flags;
    return (flags[2] & 0x40) != 0;
}
}
