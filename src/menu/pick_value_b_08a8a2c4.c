/* Returns one of four stored values by index 0..2 (anything else gives the fourth). */
extern struct Blk { int w[9]; } D_09B6CD60;
int func_08A8A2C4(int n) {
    if (n == 0) {
        return D_09B6CD60.w[1];
    } else if (n == 1) {
        return D_09B6CD60.w[2];
    } else if (n == 2) {
        return D_09B6CD60.w[3];
    } else {
        return D_09B6CD60.w[4];
    }
}
