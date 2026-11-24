/* In state 3, finds the first of 0x80 sub-slots passing two tests and runs func_08B1B230 on it; -1 inactive. */
extern "C" {
struct SubSlot { char pad[0x40]; };
struct ScanSelf { unsigned char active; char pad[7]; int state; char pad_0c[0x18]; SubSlot slots[0x80]; };
extern int func_08B1AE60(SubSlot *s);
extern int func_08B1AF00(SubSlot *s);
extern int func_08B1B230(SubSlot *s);
int func_08B1C6EC(ScanSelf *self)
{
    if (self->active == 0) {
        return -1;
    } else if (self->state == 3) {
        for (int i = 0; i < 0x80; i++) {
            SubSlot *s = (SubSlot *)((char *)self + i * 0x40 + 0x24);
            if (func_08B1AE60(s) == 0) {
                continue;
            }
            if (func_08B1AF00(s) == 0) {
                continue;
            }
            func_08B1B230(s);
            break;
        }
        return 0;
    } else {
        return 0;
    }
}
}
