/* Puts the item's id (word 0) into the first free (-1) of six shorts at +0x1E of D_09B6DAC0 when its flags (word 1) have bit 0, and likewise at +0x38 for bit 1; then calls func_08B3C488(self, item word 2) when asked. */
typedef struct Item08B38F40 { int id; int flags; int extra; } Item08B38F40;
extern short D_09B6DAC0[];
extern void func_08B3C488(void *self, int extra);
void func_08B38F40(void *self, Item08B38F40 *item, unsigned char run)
{
    if (item->flags & 1) {
        int id = item->id;
        int i = 0;
        short *p = D_09B6DAC0;
        do {
            i++;
            if (p[0xF] == -1) {
                p[0xF] = id;
                break;
            }
            p++;
        } while (i < 6);
    }
    if (item->flags & 2) {
        int id = item->id;
        int i = 0;
        short *p = D_09B6DAC0;
        do {
            i++;
            if (p[0x1C] == -1) {
                p[0x1C] = id;
                break;
            }
            p++;
        } while (i < 6);
    }
    if (run) {
        func_08B3C488(self, item->extra);
    }
}
