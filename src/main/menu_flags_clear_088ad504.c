/* Clears flag bits 8 and 16. */
typedef struct Menu088AD504 {
    char pad_00[0xEF8];
    union {
        unsigned int word;
        struct {
            unsigned int low : 8;
            unsigned int b8 : 1;
            unsigned int mid : 7;
            unsigned int b16 : 1;
        } bits;
    } flags;
} Menu088AD504;

void func_088AD504(Menu088AD504 *menu)
{
    menu->flags.bits.b8 = 0;
    menu->flags.bits.b16 = 0;
}
