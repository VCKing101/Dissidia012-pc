/* 1 when both flag bytes are set to 1. */
typedef struct Flags088CD9D4 {
    unsigned char a;
    char pad_01[0x27];
    unsigned char b;
} Flags088CD9D4;

unsigned char func_088CD9D4(Flags088CD9D4 *f)
{
    int result = 0;
    if (f->a == 1 && f->b == 1) {
        result = 1;
    }
    return result;
}
