/* Resets the audio state block at D_089D7F00: byte 2 = 0, +0x140 = 1, +0x15C = -1; releases the handles at +0x164 and +0x160 (func_089C6028) and +0x14C (func_089C5E48) when set, resetting each to -1; then func_089C6000(1). */
typedef struct Aud088edb7c {
    unsigned char pad0[2]; unsigned char b2; unsigned char pad3[0x140 - 3];
    int f140; unsigned char pad144[0x14C - 0x144]; int h14c; unsigned char pad150[0x15C - 0x150];
    int f15c; int h160; int h164;
} Aud088edb7c;
extern Aud088edb7c *volatile D_089D7F00;
extern void func_089C6028(int h);
extern void func_089C5E48(int h);
extern void func_089C6000(int v);
void func_088EDB7C(void)
{
    D_089D7F00->b2 = 0;
    D_089D7F00->f140 = 1;
    D_089D7F00->f15c = -1;
    if (D_089D7F00->h164 != -1) {
        func_089C6028(D_089D7F00->h164);
        D_089D7F00->h164 = -1;
    }
    if (D_089D7F00->h160 != -1) {
        func_089C6028(D_089D7F00->h160);
        D_089D7F00->h160 = -1;
    }
    if (D_089D7F00->h14c != -1) {
        func_089C5E48(D_089D7F00->h14c);
        D_089D7F00->h14c = -1;
    }
    func_089C6000(1);
}
