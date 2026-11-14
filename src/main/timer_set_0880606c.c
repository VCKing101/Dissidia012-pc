typedef struct Agent4Timer {
    unsigned char unknown_0000[0x560];
    unsigned char field_0560[0x10];
    float field_0570;
    float field_0574;
    unsigned char unknown_0578[8];
    unsigned int flags;
} Agent4Timer;

extern int D_09AF84C0;
void func_089A3868(void *slot, void *value);

void func_0880606C(Agent4Timer *self, void *value, float time) {
    func_089A3868(self->field_0560, value);
    self->field_0570 = time;
    self->flags |= 1;
}

void func_088060B0(Agent4Timer *self) {
    self->flags &= ~1;
    func_089A3868(self->field_0560, &D_09AF84C0);
    self->field_0570 = 0.0f;
}
