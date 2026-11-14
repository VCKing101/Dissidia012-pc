typedef struct HandleSet3 {
    void *field_00;
    void *field_04;
    void *field_08;
} HandleSet3;

extern void func_088DA060(void *handle);
extern void func_088DAB44(void *handle);
extern void func_0887BA5C(void *handle);

void func_08AF3C94(HandleSet3 *self)
{
    if (self->field_04 != 0) {
        func_088DA060(self->field_04);
        self->field_04 = 0;
    }
    if (self->field_00 != 0) {
        func_088DAB44(self->field_00);
        self->field_00 = 0;
    }
    if (self->field_08 != 0) {
        func_0887BA5C(self->field_08);
        self->field_08 = 0;
    }
}
