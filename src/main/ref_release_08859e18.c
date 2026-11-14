typedef struct Ref08859E18 {
    void *data;
    int refs;
} Ref08859E18;

void func_0899BB1C(void *p);

void func_08859E18(Ref08859E18 *self) {
    int refs = self->refs - 1;
    self->refs = refs;
    if (refs <= 0) {
        func_0899BB1C(self->data);
        self->data = 0;
    }
}
