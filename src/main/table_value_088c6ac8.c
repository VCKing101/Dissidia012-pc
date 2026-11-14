/* Value i of the loaded table at 0x089D7730; 100 when none is loaded. */
extern unsigned short *D_089D7730;
unsigned short func_088C6AC8(int i)
{
    if (D_089D7730 != 0) {
        return D_089D7730[i];
    }
    return 100;
}
