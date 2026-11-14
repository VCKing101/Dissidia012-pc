typedef struct Agent4Mover {
    unsigned char unknown_0000[0xB4];
    float field_00B4;
    int field_00B8;
} Agent4Mover;

void func_0880B3FC(Agent4Mover *self, float a);

void func_0880B494(Agent4Mover *self, float a, float b) {
    func_0880B3FC(self, a);
    self->field_00B4 = b;
    self->field_00B8 = 1;
}
