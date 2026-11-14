typedef struct Owner08893D04 {
    unsigned char unknown_000[0x718];
    int start;
    int end;
    int field_720;
    int field_724;
    int field_728;
} Owner08893D04;

void func_08893CF8(Owner08893D04 *self);

void func_08893D04(Owner08893D04 *self, int start, int end, int value) {
    func_08893CF8(self);
    self->start = start;
    self->end = end;
    self->field_720 = 0;
    self->field_724 = 1;
    self->field_728 = value;
}
