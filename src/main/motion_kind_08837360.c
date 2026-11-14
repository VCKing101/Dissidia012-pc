typedef struct Agent4MotionState {
    unsigned char unknown_0000[0x180];
    short motion;
    unsigned char unknown_0182[0x3C0];
    unsigned char stance;
} Agent4MotionState;

unsigned char func_08837360(Agent4MotionState *self) {
    int result = 0;
    if (self->motion == 0x1D6 || self->motion == 0x1D7) {
        result = 1;
    }
    return result;
}
