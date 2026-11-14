/* Picks a random index below 0x22 with the menu's linear congruential
   generator (the MSVC rand() constants). */
extern int D_09B6DA54;

short func_089EF3D8(void)
{
    int value;

    D_09B6DA54 = D_09B6DA54 * 0x343FD + 0x269EC3;
    value = (D_09B6DA54 >> 16) & 0x7FFF;
    return value % 0x22;
}
