struct HandleGrid16DE0 {
    char pad_00[0x4];
    void *field_04;
    void *field_08[4];
    void *field_18[4];
    void *field_28[4][4];
};

extern "C" {

extern void func_088DA060(void *handle);
extern void func_08A16DE0(HandleGrid16DE0 *self);

void func_08A16E50(HandleGrid16DE0 *self)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (self->field_28[i][j] != 0) {
                func_088DA060(self->field_28[i][j]);
                self->field_28[i][j] = 0;
            }
        }
        if (self->field_18[i] != 0) {
            func_088DA060(self->field_18[i]);
            self->field_18[i] = 0;
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
    func_08A16DE0(self);
}

}
