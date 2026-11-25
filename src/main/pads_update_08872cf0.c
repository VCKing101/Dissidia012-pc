/* Updates the controller pads: single mode refreshes pad 5 (flag 8 picks the variant), otherwise pads 0 and 1. */
typedef struct Pad08872CF0 {
    unsigned char data[0x60];
} Pad08872CF0;

typedef struct Config08872CF0 {
    unsigned char unk_000[0x264];
    unsigned int flags;                /* +0x264 bit 0: single, bit 3: variant */
} Config08872CF0;

extern Config08872CF0 *D_089D6008;
extern void func_08871DAC(Pad08872CF0 *pad);
extern void func_08871E24(Pad08872CF0 *pad, int index);

void func_08872CF0(Pad08872CF0 *pads)
{
    unsigned int flags = D_089D6008->flags;

    while (flags & 1) {
        if ((unsigned char)((flags & 8) != 0)) {
            func_08871DAC(&pads[5]);
        } else {
            func_08871E24(&pads[5], 0);
        }
        return;
    }
    {
        int i = 0;
        Pad08872CF0 *pad;

        for (pad = &pads[i]; i < 2; i++, pad++) {
            func_08871E24(pad, i);
        }
    }
}
