typedef struct ReleaseHolder {
    int field_00;
    int field_04;
} ReleaseHolder;

extern void func_088DA060(int handle);

void func_089EAD78(ReleaseHolder *self)
{
    if (self->field_04 != 0) {
        func_088DA060(self->field_04);
        self->field_04 = 0;
    }
}
