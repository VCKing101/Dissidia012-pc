typedef struct Agent4MotionState {
    unsigned char unknown_0000[0x542];
    unsigned char stance;
} Agent4MotionState;

unsigned char func_0883739C(Agent4MotionState *self) {
    int result = 0;
    if (self->stance == 0 || self->stance == 1) {
        result = 1;
    }
    return result;
}
