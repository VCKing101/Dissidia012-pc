/* Stores four values into the global block at 0x09B6CD74. */
extern struct Blk { int w[9]; } D_09B6CD60;
void func_08A8A2A8(int a, int b, int c, int d) {
    D_09B6CD60.w[5] = a;
    D_09B6CD60.w[6] = b;
    D_09B6CD60.w[7] = c;
    D_09B6CD60.w[8] = d;
}
