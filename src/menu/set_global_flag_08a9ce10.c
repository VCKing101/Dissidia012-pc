/* Runs the setup with fixed arguments, stores two arguments in the global object, clears bit 0 of a flag word and sets it again when the last argument is 1. */
extern int D_08AA3F44;
extern struct Blk { char g[0x2F4]; unsigned w; } D_09B6EFB0;
extern void func_08A97E34(int obj, int a, int b, int c, int d);
void func_08A9CE10(int a, int b, int c, int d, int e, int f, int g) {
    func_08A97E34(D_08AA3F44, 3, 2, 0, 0);
    *(int *)(D_08AA3F44 + 0x138) = a;
    *(int *)(D_08AA3F44 + 0x134) = b;
    D_09B6EFB0.w = D_09B6EFB0.w & 0xFFFFFFFE;
    if (f == 1) {
        D_09B6EFB0.w = D_09B6EFB0.w | 1;
    }
}
