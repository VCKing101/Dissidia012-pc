typedef struct Buffer0885AFD0 {
    void *data;
    unsigned char unknown_0004[0x4B0C];
    int field_4B10;
    int field_4B14;
} Buffer0885AFD0;

void func_0899BB1C(void *p);

void func_0885AFD0(Buffer0885AFD0 *self) {
    if (self->data != 0) {
        func_0899BB1C(self->data);
    }
    self->data = 0;
    self->field_4B14 = 0;
    self->field_4B10 = 0;
}
