/* Releases every handle in one of the part's handle arrays. */
typedef struct HandleArrays {
    void **field_00;
    void **field_04;
    void **field_08;
    int field_0C;
    int field_10;
    int field_14;
} HandleArrays;

extern void func_088DA060(void *handle);

void func_08A520CC(HandleArrays *self)
{
    int i;
    if (self->field_04 == 0) {
        return;
    }
    for (i = 0; i < self->field_10; i++) {
        if (self->field_04[i] != 0) {
            func_088DA060(self->field_04[i]);
            self->field_04[i] = 0;
        }
    }
}
