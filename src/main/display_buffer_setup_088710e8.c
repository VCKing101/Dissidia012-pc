extern int D_089D5FD8;                 /* buffer size */
extern int D_089D5FE0;                 /* width */
extern int D_089D5FE4;                 /* height */
extern void func_089A0FC8(void);
extern void func_089A0DB8(int a, int b, int c, int d);
extern void func_089A0F04(int a, int width, int height, int stride, int size);

/* Display buffer setup: sizes rounded up (0x2000 / 64 bytes, powers of two for the texture). */
void func_088710E8(void)
{
    if (D_089D5FD8 != 0) {
        int size = (D_089D5FD8 + 0x1FFF) & ~0x1FFF;
        int width = D_089D5FE0;
        int stride = (width + 0x3F) & ~0x3F;
        int height = D_089D5FE4;
        int pw;
        int ph;

        for (pw = 1; pw <= width; pw <<= 1) {
        }
        for (ph = 1; ph <= height; ph <<= 1) {
        }
        func_089A0FC8();
        func_089A0DB8(1, 0, 0, 0);
        func_089A0F04(0, pw, ph, stride, size);
    }
}
