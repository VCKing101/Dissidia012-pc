/* Sets the vtable and copies three single floats from src (+0x1C twice, +0x2C, +0x30) into the object. */
extern int D_089DC020[];
int *func_0895F578(int *self, float *src) {
    unsigned int i;
    float *d;
    float *s;
    float v;
    *self = (int)D_089DC020;
    for (s = src + 7, i = 0, d = (float *)self; i < 1; i++, s++, d++) {
        v = *s;
        d[2] = v;
        d[1] = v;
    }
    for (s = src + 11, i = 0, d = (float *)self; i < 1; i++, s++, d++) {
        d[3] = *s;
    }
    {
        float *s3 = src + 12;
        unsigned int i3;
        float *d3;
        for (i3 = 0, d3 = (float *)self; i3 < 1; i3++, s3++, d3++) {
            d3[4] = *s3;
        }
    }
    return self;
}
