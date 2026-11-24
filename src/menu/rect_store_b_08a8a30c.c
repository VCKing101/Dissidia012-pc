/* Stores four values into the global block at 0x09B6CD64. */
extern struct Blk { int w[9]; } D_09B6CD60;
void func_08A8A30C(int a, int b, int c, int d) {
    D_09B6CD60.w[1] = a;
    D_09B6CD60.w[2] = b;
    D_09B6CD60.w[3] = c;
    D_09B6CD60.w[4] = d;
}
