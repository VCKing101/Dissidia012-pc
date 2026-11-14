struct Agent4FlagBytes {
    unsigned char unknown_0000[0x190];
    unsigned char flags[8];
};

extern "C" {
bool func_088378C4(Agent4FlagBytes *self) {
    unsigned char *flags = self->flags;
    return (flags[0] & 0x40) != 0;
}
}
