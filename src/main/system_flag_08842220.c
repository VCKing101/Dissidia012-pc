typedef struct SystemFlags {
    char pad_00[0x4C];
    unsigned int field_4C;
} SystemFlags;

extern SystemFlags *D_089D4958;

void func_08842220(int bit)
{
    D_089D4958->field_4C |= 1 << bit;
}
