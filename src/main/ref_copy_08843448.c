typedef struct Ref {
    int field_00;
    int field_04;
    int field_08;
    int field_0C;
} Ref;

void func_08843448(Ref *self)
{
    int zero = 0;

    self->field_0C = self->field_08 != 0 ? self->field_08 : zero;
}
