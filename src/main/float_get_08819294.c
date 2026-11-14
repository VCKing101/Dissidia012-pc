typedef struct Agent4FloatSource {
    unsigned char unknown_0000[0x14];
    void *source;
} Agent4FloatSource;

extern float D_089D5EAC;
float func_08838300(void *source);

float func_08819294(Agent4FloatSource *self) {
    return self->source != 0 ? func_08838300(self->source) : D_089D5EAC;
}
