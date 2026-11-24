// Runs func_0894978C, then scales three stored values into the first three floats of the block by 0.01.
extern void func_0894978C(int a, float *p, int b);
void func_08952D58(int unused0, int unused1, int a, float *p0, int b)
{
    float *p = p0;
    func_0894978C(a, p0, b);
    p[0] = p[4] * 0.01f;
    p[1] = p[5] * 0.01f;
    p[2] = p[6] * 0.01f;
}
