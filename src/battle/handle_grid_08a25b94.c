typedef struct HandleGrid25B94 {
    unsigned char field_00;
    char pad_001[0x3];
    void *field_04;
    char field_08[0x20];
    void *field_28[4][4];
    int field_68;
    int field_6C;
} HandleGrid25B94;

extern void func_088DA060(void *handle);
extern void func_089F7D0C(void *sub);

void func_08A25B94(HandleGrid25B94 *self)
{
    self->field_00 = 0;
    self->field_04 = 0;
    {
        int i;
        for (i = 0; i < 4; i++) {
            int j;
            for (j = 0; j < 4; j++) {
                self->field_28[i][j] = 0;
            }
        }
    }
    self->field_68 = 0;
    self->field_6C = 0;
}

void func_08A25BDC(HandleGrid25B94 *self)
{
    {
        int i;
        for (i = 0; i < 4; i++) {
            int j;
            for (j = 0; j < 4; j++) {
                if (self->field_28[i][j]) {
                    func_088DA060(self->field_28[i][j]);
                    self->field_28[i][j] = 0;
                }
            }
        }
    }
    func_089F7D0C(self->field_08);
    if (self->field_04) {
        func_088DA060(self->field_04);
        self->field_04 = 0;
    }
    func_08A25B94(self);
}
