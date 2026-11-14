typedef struct Ref08851CEC {
    void *data;
    int refs;
} Ref08851CEC;

void func_08851C6C(Ref08851CEC *self);
void func_0899BB1C(void *p);

void func_08851CEC(Ref08851CEC *self) {
    int refs = self->refs - 1;
    self->refs = refs;
    if (refs <= 0) {
        func_08851C6C(self);
        func_0899BB1C(self->data);
        self->data = 0;
    }
}
