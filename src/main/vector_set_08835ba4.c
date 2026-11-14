typedef struct Agent4Vec3 {
    float x;
    float y;
    float z;
} Agent4Vec3;

typedef struct Agent4Body {
    unsigned char unknown_0000[0xA0];
    float field_A0;
    float field_A4;
    float field_A8;
    unsigned char unknown_00AC[0x21C];
    float field_2C8;
    float field_2CC;
    float field_2D0;
} Agent4Body;

float func_089A457C(float x, float z);
void func_08835C54(Agent4Body *self, float angle);

void func_08835BA4(Agent4Body *self, Agent4Vec3 *dir) {
    func_08835C54(self, func_089A457C(dir->x, dir->z));
}

void func_08835BDC(Agent4Body *self, Agent4Vec3 *pos) {
    self->field_A0 = pos->x;
    self->field_A4 = pos->y;
    self->field_A8 = pos->z;
}

void func_08835BF8(Agent4Body *self, float x, float y, float z) {
    self->field_2C8 = x;
    self->field_2CC = y;
    self->field_2D0 = z;
}
