typedef struct HandleGrid {
    char pad_000[0x4];
    void *field_04;
    void *field_08[4][3];
} HandleGrid;

extern void func_088DA060(void *handle);
extern void func_089F5048(HandleGrid *self);

void func_089F5098(HandleGrid *self)
{
    {
        int i;
        for (i = 0; i < 4; i++) {
            int j;
            for (j = 0; j < 3; j++) {
                if (self->field_08[i][j]) {
                    func_088DA060(self->field_08[i][j]);
                    self->field_08[i][j] = 0;
                }
            }
        }
    }
    if (self->field_04) {
        func_088DA060(self->field_04);
        self->field_04 = 0;
    }
    func_089F5048(self);
}
