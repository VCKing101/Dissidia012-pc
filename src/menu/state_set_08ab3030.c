typedef struct SetCtx {
    char pad_0000[0x490];
    int field_490;
    char pad_494[0x49C - 0x494];
    int field_49C;
} SetCtx;

int func_08AB3030(SetCtx **self)
{
    int result = 0;
    (*self)->field_490 = 1;
    (*self)->field_49C = 0;
    return result;
}
