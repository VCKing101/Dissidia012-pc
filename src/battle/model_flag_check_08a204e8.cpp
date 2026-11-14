struct FlagModel204E8 {
    char pad_000[0x20];
    unsigned short flags;

    static bool has_flag_20(FlagModel204E8 *model)
    {
        return (model->flags & 0x20) != 0;
    }
};

struct ModelOwner204E8 {
    char pad_000[0x24];
    FlagModel204E8 *field_24;
};

extern "C" {

void func_08A203E4(ModelOwner204E8 *self);

int func_08A204E8(ModelOwner204E8 *self)
{
    if (self->field_24 && FlagModel204E8::has_flag_20(self->field_24)) {
        func_08A203E4(self);
    }
    return 0;
}

}
