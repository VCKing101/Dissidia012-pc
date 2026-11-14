/* Sets the byte at 0x1850 of the global menu. */
typedef struct MenuFlags {
    char pad_0000[0x1850];
    unsigned char flag;
} MenuFlags;

extern MenuFlags *D_089D7E88;

void func_088E9958(int flag)
{
    D_089D7E88->flag = flag;
}
