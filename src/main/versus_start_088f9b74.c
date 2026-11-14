/* Stores the versus mode and starts the menu for `pad`. */
typedef struct VersusState {
    char pad_0000[0x2788];
    int mode;
} VersusState;

extern VersusState D_09AFB8A0;
extern void func_088FB5D0(unsigned char pad, int a, int b, int c, int d);

void func_088F9B74(int mode, unsigned char pad)
{
    VersusState *vs = &D_09AFB8A0;

    vs->mode = mode;
    func_088FB5D0(pad, 0, 1, 0, 0);
}
