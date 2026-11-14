typedef struct Unit0880612C {
    unsigned char unknown_0000[0x90];
    unsigned char field_0090[0x6A0];
    unsigned char field_0730[0x360];
    unsigned char field_0A90[0x10];
    unsigned char field_0AA0[0x50];
    unsigned char active;
} Unit0880612C;

void func_08806F5C(void *part, Unit0880612C *owner);
void func_08830B88(void *a, void *b, void *c);

void func_0880612C(Unit0880612C *self) {
    if (self->active == 0) {
        return;
    }
    func_08806F5C(self->field_0A90, self);
    func_08830B88(self->field_0730, self->field_0090, self->field_0AA0);
}
