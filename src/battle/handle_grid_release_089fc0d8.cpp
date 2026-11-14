struct HandleGridC0D8 {
    char pad_00[0x4];
    void *field_04;
    void *field_08[4];
    void *field_18[4][13];
};

extern "C" {

extern void func_088DA060(void *handle);
extern void func_089FC068(HandleGridC0D8 *self);

void func_089FC0D8(HandleGridC0D8 *self)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 13; j++) {
            if (self->field_18[i][j] != 0) {
                func_088DA060(self->field_18[i][j]);
                self->field_18[i][j] = 0;
            }
        }
        if (self->field_08[i] != 0) {
            func_088DA060(self->field_08[i]);
            self->field_08[i] = 0;
        }
    }
    if (self->field_04 != 0) {
        func_088DA060(self->field_04);
        self->field_04 = 0;
    }
    func_089FC068(self);
}

}
