typedef struct FlagBlock {
    char pad_00[0x1C];
    char pad_1C_unused[0];
    char *field_1C;
    char *field_20;
} FlagBlock;

typedef struct FlagOwner {
    char pad_00[0x128];
    FlagBlock *field_128;
} FlagOwner;

extern void func_08A6165C(FlagOwner *self);

unsigned char func_08A618F4(FlagOwner *self)
{
    unsigned char *flag = (unsigned char *)self->field_128->field_1C;
    if (*flag == 0) {
        return 0;
    }
    *flag = 0;
    func_08A6165C(self);
    return 1;
}
