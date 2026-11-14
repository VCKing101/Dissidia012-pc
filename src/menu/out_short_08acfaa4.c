extern int func_08ACF81C(int self, short a, short *out);

int func_08ACFAA4(int self, short a)
{
    short out[2];
    out[0] = 0;
    func_08ACF81C(self, a, out);
    return out[0];
}
