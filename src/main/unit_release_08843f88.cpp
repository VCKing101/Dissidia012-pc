struct CountedUnit {
    char pad_00[0xE0];
    short refs;
};

extern "C" {

void func_08843FF8(CountedUnit *self);

void func_08843F88(CountedUnit *self)
{
    self->refs--;
    if (self->refs == 0) {
        func_08843FF8(self);
    }
}

}
