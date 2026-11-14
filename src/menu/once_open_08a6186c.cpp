struct OnceFlag {
    bool field_00;
};

struct OnceState {
    char pad_00[0x20];
    OnceFlag *field_20;
};

struct OnceOwner {
    char pad_000[0x128];
    OnceState *field_128;
};

extern "C" void func_08A6165C(OnceOwner *self);

extern "C" {
bool func_08A6186C(OnceOwner *self)
{
    OnceFlag *flag = self->field_128->field_20;
    if (flag->field_00 == 1) {
        return false;
    }
    flag->field_00 = true;
    func_08A6165C(self);
    return true;
}
}
