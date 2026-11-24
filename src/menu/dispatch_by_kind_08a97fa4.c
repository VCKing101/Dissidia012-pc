/* Runs the setup with arguments chosen by kind (5 also sets bit 0 of the flag word, 8 uses the passed object). */
extern int D_08AA3F44;
extern struct Blk { char g[0x1CC]; int second; char h[0x124]; unsigned w; } D_09B6EFB0;
extern void func_08A97E34(int obj, int a, int b, int c, int d);
void func_08A97FA4(int obj, int kind, int c, int d, unsigned char e) {
    if (kind == 5) {
        func_08A97E34(D_08AA3F44, 3, D_09B6EFB0.second, c, 0);
        D_09B6EFB0.w = D_09B6EFB0.w | 1;
    } else if (kind == 8) {
        func_08A97E34(obj, 2, D_09B6EFB0.second, 0, e);
    } else {
        func_08A97E34(obj, 0, D_09B6EFB0.second, 0, e);
    }
}
