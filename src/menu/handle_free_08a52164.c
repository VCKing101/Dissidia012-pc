/* Releases every handle in one of the part's handle arrays. */
typedef struct HandleArrays {
    void **field_00;
    void **field_04;
    void **field_08;
    int field_0C;
    int field_10;
    int field_14;
} HandleArrays;

extern void func_0887BA5C(void *handle);

void func_08A52164(HandleArrays *self)
{
    int i;
    if (self->field_08 == 0) {
        return;
    }
    for (i = 0; i < self->field_14; i++) {
        if (self->field_08[i] != 0) {
            func_0887BA5C(self->field_08[i]);
            self->field_08[i] = 0;
        }
    }
}
