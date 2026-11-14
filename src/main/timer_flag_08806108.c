typedef struct Agent4Timer {
    unsigned char unknown_0000[0x570];
    float field_0570;
    float field_0574;
    unsigned char unknown_0578[8];
    unsigned int flags;
} Agent4Timer;

void func_08806108(Agent4Timer *self, float value) {
    self->field_0574 = value;
    self->flags |= 2;
}
