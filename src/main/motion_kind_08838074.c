typedef struct Agent4MotionState {
    unsigned char unknown_0000[0x180];
    short motion;
} Agent4MotionState;

unsigned char func_08838074(Agent4MotionState *self) {
    int result = 0;
    if (self->motion >= 500 && self->motion < 0x31F) {
        result = 1;
    }
    return result;
}
