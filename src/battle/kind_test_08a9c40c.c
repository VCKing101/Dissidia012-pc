/* Membership test over kinds 0..0x2D; the switch's jump table is .rodata at
   0x08AA1D60, built from this unit and counted as data. */

int func_08A9C40C(void *self, int kind)
{
    switch (kind) {
    case 0x00: case 0x01: case 0x02: case 0x11: case 0x14: case 0x15: case 0x16:
    case 0x17: case 0x18: case 0x19: case 0x1A: case 0x1B: case 0x1C: case 0x1D:
    case 0x1E: case 0x1F: case 0x20: case 0x21: case 0x23: case 0x24: case 0x27:
    case 0x28: case 0x29: case 0x2B: case 0x2D:
        return 1;
    default:
        return 0;
    }
}
