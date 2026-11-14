typedef struct FlagGlobal {
    char pad_000[0x304];
    char field_304;
} FlagGlobal;

extern FlagGlobal D_09B6EFB0;

char *func_08AA58C4(void)
{
    return &D_09B6EFB0.field_304;
}
