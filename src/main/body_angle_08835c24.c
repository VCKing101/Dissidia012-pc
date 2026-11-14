typedef struct Agent4Vec3 {
    float x;
    float y;
    float z;
} Agent4Vec3;

typedef struct Agent4Body {
    unsigned char unknown_0000[0x9C];
    float field_9C;
    unsigned char unknown_00A0[0x20];
    unsigned char field_C0[0xC];
    float field_CC;
    unsigned char unknown_00D0[0x250];
    float angle;
} Agent4Body;

void func_089A3868(void *slot, int value);
float func_08835B3C(Agent4Body *self, float angle);
float func_089A457C(float x, float z);

void func_08835C24(Agent4Body *self, int value) {
    func_089A3868(self->field_C0, value);
    self->field_CC = 0.0f;
}

void func_08835C54(Agent4Body *self, float angle) {
    self->angle = func_08835B3C(self, angle);
}

void func_08835C7C(Agent4Body *self, Agent4Vec3 *dir) {
    self->angle = func_089A457C(dir->x, dir->z);
}

void func_08835CAC(Agent4Body *self) {
    self->angle = self->field_9C;
}
