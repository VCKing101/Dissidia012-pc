/* Sets the vtable and copies three runs of two floats from src into the object (the first run twice). */
extern int D_089DBFD8[];
int *func_0895ED40(int *self, float *src) {
    unsigned int i;
    float *d;
    float *s;
    float v;
    *self = (int)D_089DBFD8;
    for (s = src, i = 0, d = (float *)self; i < 2; i++, s++, d++) {
        v = *s;
        d[3] = v;
        d[1] = v;
    }
    for (s = src + 4, i = 0, d = (float *)self; i < 2; i++, s++, d++) {
        d[5] = *s;
    }
    {
        float *s3 = src + 8;
        unsigned int i3;
        float *d3;
        for (i3 = 0, d3 = (float *)self; i3 < 2; i3++, s3++, d3++) {
            d3[7] = *s3;
        }
    }
    return self;
}
