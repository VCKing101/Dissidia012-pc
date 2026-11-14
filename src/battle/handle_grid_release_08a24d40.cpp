struct HandleGrid {
    char pad_00[0x4];
    void *field_04;
    char field_08[0x20];
    void *field_28[4];
    void *field_38[4][4];
};

extern "C" {

extern void func_088DA060(void *handle);
extern void func_089F7D0C(void *sub);
extern void func_08A24CE4(HandleGrid *self);

void func_08A24D40(HandleGrid *self)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (self->field_38[i][j] != 0) {
                func_088DA060(self->field_38[i][j]);
                self->field_38[i][j] = 0;
            }
        }
        if (self->field_28[i] != 0) {
            func_088DA060(self->field_28[i]);
            self->field_28[i] = 0;
        }
    }
    func_089F7D0C(self->field_08);
    if (self->field_04 != 0) {
        func_088DA060(self->field_04);
        self->field_04 = 0;
    }
    func_08A24CE4(self);
}

}
