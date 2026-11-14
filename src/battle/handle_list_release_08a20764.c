typedef struct HandleList20764 {
    char pad_000[0x4];
    void *field_04;
    void *field_08[4];
} HandleList20764;

extern void func_088DA060(void *handle);
extern void func_08A20730(HandleList20764 *self);

void func_08A20764(HandleList20764 *self)
{
    {
        int i;
        for (i = 0; i < 4; i++) {
            if (self->field_08[i]) {
                func_088DA060(self->field_08[i]);
                self->field_08[i] = 0;
            }
        }
    }
    if (self->field_04) {
        func_088DA060(self->field_04);
        self->field_04 = 0;
    }
    func_08A20730(self);
}
