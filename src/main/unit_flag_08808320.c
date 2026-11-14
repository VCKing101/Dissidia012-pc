typedef struct Agent4Unit08808320 {
    unsigned char unknown_0000[0x380];
    unsigned int flags;
} Agent4Unit08808320;

typedef struct Agent4Holder08808320 {
    unsigned char unknown_0000[0x18];
    Agent4Unit08808320 *unit;
    unsigned char unknown_001C[0x10];
    short kind;
} Agent4Holder08808320;

unsigned char func_08808320(Agent4Holder08808320 *self) {
    int result = 0;
    if (self->unit != 0 && self->kind == 1 && (unsigned char)((self->unit->flags & 0x80000) != 0)) {
        result = 1;
    }
    return result;
}
