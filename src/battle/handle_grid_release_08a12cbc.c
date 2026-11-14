typedef struct HandleGrid12CBC {
    char pad_000[0x4];
    void *field_04;
    char field_08[0x20];
    void *field_28[2][2];
} HandleGrid12CBC;

extern void func_088DA060(void *handle);
extern void func_089F7D0C(void *sub);
extern void func_08A12C5C(HandleGrid12CBC *self);

void func_08A12CBC(HandleGrid12CBC *self)
{
    {
        int i;
        for (i = 0; i < 2; i++) {
            int j;
            for (j = 0; j < 2; j++) {
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
    func_08A12C5C(self);
}
