/* Maps kinds 2..8 to a rank, 4 by default. The switch's jump table is .rodata
   at 0x08ACA4E8, built from this unit and counted as data. Case order follows
   the code layout. */

int func_08AC76A4(void *self, int kind)
{
    int rank = 4;
    switch (kind) {
    case 4: rank = 0; break;
    case 5: rank = 1; break;
    case 2: rank = 2; break;
    case 3: rank = 3; break;
    case 7: rank = 4; break;
    case 8: rank = 5; break;
    }
    return rank;
}
