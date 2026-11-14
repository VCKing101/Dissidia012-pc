/* Clears the four float pairs, the two trailing floats and the flag. */
typedef struct MotionPairs {
    float a[2];
    float b[2];
    float c[2];
    float d[2];
    float e;
    float f;
    unsigned char active;
} MotionPairs;

void func_088E0970(MotionPairs *m)
{
    float zero = 0.0f;
    int i;

    for (i = 0; i < 2; i++) {
        m->a[i] = m->b[i] = m->c[i] = m->d[i] = zero;
    }
    m->e = zero;
    m->f = zero;
    m->active = 0;
}
