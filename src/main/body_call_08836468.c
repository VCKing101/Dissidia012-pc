typedef struct Agent4Body36468 {
    unsigned char unknown_0000[0xF0];
    unsigned char field_F0[4];
} Agent4Body36468;

void func_08836120(Agent4Body36468 *self, void *part, unsigned char flag);

void func_08836468(Agent4Body36468 *self, unsigned char flag) {
    func_08836120(self, self->field_F0, flag);
}
