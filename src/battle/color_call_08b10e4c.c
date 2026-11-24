/* Calls func_08893B4C with grey (value * 128) and alpha (value * 255) bytes taken from the float at +4 of the message; returns 0. */
extern void func_08893B4C(void *obj, int r, int g, int b, int a, int extra);
extern void *D_089D6AC8;
int func_08B10E4C(int a0, float *msg) {
    float *p = msg + 1;
    float v = *p;
    int grey = (int)(v * 128.0f) & 0xFF;
    int alpha = (int)(*p * 255.0f) & 0xFF;
    func_08893B4C(D_089D6AC8, grey, grey, grey, alpha, *(int *)(p + 1));
    return 0;
}
