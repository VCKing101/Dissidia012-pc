/* Zeroes a record, with the byte at 0x34 set to 1; returns it. */
typedef struct {
    int w0, w4, w8, wC, w10, w14, w18, w1C, w20, w24, w28, w2C, w30;
    unsigned char b34, b35, b36, b37, b38, b39;
    char pad[2];
    int w3C, w40;
    unsigned char b44;
} Rec089405B0;
Rec089405B0 *func_089405B0(Rec089405B0 *p) {
    p->w0 = 0; p->w4 = 0; p->w8 = 0; p->wC = 0; p->w14 = 0; p->w10 = 0;
    p->w18 = 0; p->w1C = 0; p->w20 = 0; p->w28 = 0; p->w30 = 0; p->w3C = 0;
    int one = 1;
    p->b34 = one;
    p->b35 = 0; p->b36 = 0; p->b37 = 0; p->b38 = 0; p->b39 = 0;
    p->w40 = 0; p->w2C = 0;
    p->b44 = 0;
    return p;
}
