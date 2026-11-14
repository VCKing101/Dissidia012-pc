struct Agent4ByteOwner {
    unsigned char unknown_0000[0x6BC];
    unsigned char field_06BC[4];
};

extern "C" {
void func_08806C8C(void *part, unsigned char value);

/* Applies each non-zero byte of a packed word, lowest byte first. */
void func_088065EC(Agent4ByteOwner *self, unsigned int packed) {
    for (int i = 0; i < 4; i++) {
        int value = packed & 0xFF;
        bool present = value > 0;
        if (present) {
            func_08806C8C(self->field_06BC, value);
        }
        packed >>= 8;
    }
}
}
