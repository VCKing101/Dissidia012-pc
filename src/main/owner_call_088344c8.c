typedef struct Agent4Owner344C8 {
    unsigned char unknown_0000[0x330];
    unsigned char field_0330[4];
} Agent4Owner344C8;

void func_089A3EFC(void *part, Agent4Owner344C8 *owner);

void func_088344C8(Agent4Owner344C8 *self) {
    func_089A3EFC(self->field_0330, self);
}
