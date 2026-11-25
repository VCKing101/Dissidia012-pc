extern float func_088A1C38(void *p);
extern int func_089918C0(float a, int b, int c, int d, int e);
int func_088A16B0(float pass, float x, char *base, int b, int row, int d, int e) {
    float y = func_088A1C38(base + row * 0x10 + 0x7C);
    int r = func_089918C0(pass, b, (int)(((x * y) / 100.0f) * 4096.0f), d, e);
    if (r == 0x100) { r = -1; }
    return r;
}
