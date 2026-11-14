typedef struct ModelLink {
    char pad_00[0x3C];
    int field_3C;
} ModelLink;

typedef struct ModelOwner {
    char pad_00[0x48];
    ModelLink *field_48;
} ModelOwner;

extern void func_08834DF4(int model, float value);
extern void func_08834D60(int model, float value);

int func_08A31DD0(ModelOwner *self)
{
    if (self->field_48->field_3C != 0) {
        func_08834DF4(self->field_48->field_3C, 0.0f);
        func_08834D60(self->field_48->field_3C, 0.0f);
        return 1;
    }
    return 0;
}
