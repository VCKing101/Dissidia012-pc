typedef struct Agent4Target {
    unsigned char unknown_0000[0x48];
    int target;
    int field_4C;
} Agent4Target;

void func_0882F708(Agent4Target *self, int target) {
    self->target = target != 0 ? target : 0;
    self->field_4C = 0;
}
