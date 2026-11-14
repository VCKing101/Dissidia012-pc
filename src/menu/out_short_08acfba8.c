extern int func_08AD75F0(int self, int a, short b, short *out);

int func_08ACFBA8(int self, int a, short b)
{
    short out[2];
    out[0] = 0;
    func_08AD75F0(self, a, b, out);
    return out[0];
}
