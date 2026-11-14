typedef struct FlagGlobal {
    char pad_00[0x11];
    unsigned char field_11;
} FlagGlobal;

extern void func_08A50134(int flag);
extern FlagGlobal D_09B6DCE8;

void func_08A4D41C(void)
{
    if (D_09B6DCE8.field_11 == 0) {
        func_08A50134(0);
    } else {
        func_08A50134(1);
    }
}
