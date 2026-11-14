typedef struct Agent4MotionState {
    unsigned char unknown_0000[0x180];
    short motion;
} Agent4MotionState;

unsigned char func_08837430(Agent4MotionState *self) {
    int result = 0;
    if (self->motion >= 400 && self->motion < 0x1D6) {
        result = 1;
    }
    return result;
}
