typedef struct Owner08896AB0 {
    int field_00;
    int field_04;
    int field_08;
    int field_0C;
    int ids[9];
} Owner08896AB0;

extern "C" int D_089D6C80;
extern "C" void func_088A0DFC(int system, int id);

extern "C" void func_08896AB0(Owner08896AB0 *self) {
    int system = D_089D6C80;
    for (int i = 0; i < 9; i++) {
        if (self->ids[i] != -1) {
            func_088A0DFC(system, self->ids[i]);
            self->ids[i] = -1;
        }
    }
    self->field_00 = 0;
    self->field_04 = 0;
    self->field_08 = 0;
    self->field_0C = -1;
}
