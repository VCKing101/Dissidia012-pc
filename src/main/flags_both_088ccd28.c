/* 1 when both flag bytes are set to 1. */
typedef struct Flags088CCD28 {
    unsigned char a;
    char pad_01[0xF];
    unsigned char b;
} Flags088CCD28;

unsigned char func_088CCD28(Flags088CCD28 *f)
{
    int result = 0;
    if (f->a == 1 && f->b == 1) {
        result = 1;
    }
    return result;
}
