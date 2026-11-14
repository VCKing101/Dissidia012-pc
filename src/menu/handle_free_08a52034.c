/* Releases every handle in the array. */
typedef struct HandleArray {
    void **field_00;
    int field_04;
    int field_08;
    int field_0C;
} HandleArray;

extern void func_088DAB44(void *handle);

void func_08A52034(HandleArray *self)
{
    int i;
    if (self->field_00 == 0) {
        return;
    }
    for (i = 0; i < self->field_0C; i++) {
        if (self->field_00[i] != 0) {
            func_088DAB44(self->field_00[i]);
            self->field_00[i] = 0;
        }
    }
}
