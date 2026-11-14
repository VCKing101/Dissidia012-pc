struct FlagModel {
    char pad_000[0x20];
    unsigned short flags;

    static void mark(FlagModel *model)
    {
        if (model) {
            model->flags |= 0x1000;
        }
    }
};

struct ModelPair {
    unsigned char active;
    char pad_001[0x3];
    FlagModel *field_04;
    FlagModel *field_08;
    char pad_00C[0x18];
    unsigned char field_24;
};

extern "C" {

int func_08A10EA0(ModelPair *self)
{
    if (!self->active) {
        return -1;
    }
    if (!self->field_24) {
        FlagModel::mark(self->field_04);
    } else {
        FlagModel::mark(self->field_08);
    }
    return 0;
}

}
