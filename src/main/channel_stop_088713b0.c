typedef struct Channel088713B0 {
    int id;
    int field_04;
} Channel088713B0;

void func_088718F0(Channel088713B0 *self);

void func_088713B0(Channel088713B0 *self) {
    if (self->id >= 0) {
        func_088718F0(self);
    }
    self->field_04 = 0;
}
