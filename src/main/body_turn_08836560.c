typedef struct Agent4Body36560 {
    unsigned char unknown_0000[0x9C];
    float field_9C;
} Agent4Body36560;

void func_08835C54(Agent4Body36560 *self, float angle);

void func_08836560(Agent4Body36560 *self, float delta) {
    func_08835C54(self, self->field_9C + delta);
}
