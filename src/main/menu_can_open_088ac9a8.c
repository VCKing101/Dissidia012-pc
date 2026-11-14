/* 1 when the menu may open: forced by the global switch, or idle with no blocking flag. */
typedef struct Menu088AC9A8 {
    char pad_00[0xEF8];
    unsigned int flags;
    char pad_EFC[0xF00 - 0xEFC];
    int current;
} Menu088AC9A8;

extern unsigned char D_089D5F1F;

int func_088AC9A8(Menu088AC9A8 *menu)
{
    unsigned int flags;
    if (D_089D5F1F != 0) {
        return 1;
    }
    if (menu->current != 0) {
        return 0;
    }
    flags = menu->flags;
    if ((flags & 1) || (flags & 0x100) || (flags & 0x1000)) {
        return 0;
    }
    return 1;
}
