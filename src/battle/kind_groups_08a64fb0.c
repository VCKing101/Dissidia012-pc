/* Three membership tests on a kind value, each gated by a nonzero flag byte.
   Every switch compiles to a jump table; the three tables follow the code in
   .rodata at 0x08A65660 (built from this unit, counted as data). */

int func_08A64FB0(unsigned char *enabled, unsigned int kind)
{
    if (*enabled == 0) {
        return 0;
    }
    switch (kind) {
    case 3: case 4: case 5: case 9: case 10: case 11:
        return 1;
    default:
        return 0;
    }
}

int func_08A65000(unsigned char *enabled, int kind)
{
    if (*enabled == 0) {
        return 0;
    }
    switch (kind) {
    case 1: case 4: case 7: case 10:
        return 1;
    default:
        return 0;
    }
}

int func_08A65054(unsigned char *enabled, int kind)
{
    if (*enabled == 0) {
        return 0;
    }
    switch (kind) {
    case 2: case 5: case 8: case 11:
        return 1;
    default:
        return 0;
    }
}
