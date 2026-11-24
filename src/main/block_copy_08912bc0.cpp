/* Allocates n bytes with func_089110C4, copies src into them with func_08935524, then stores end pointer at +4 and (non-null) begin pointer at +0 of self. */
extern "C" void *func_089110C4(unsigned n);
extern "C" void func_08935524(void *dst, void *src, unsigned n);
extern "C" void func_08912BC0(void **self, void *src, unsigned n)
{
    void *slot[2];
    slot[1] = self;
    void *m = func_089110C4(n);
    slot[0] = m;
    func_08935524(m, src, n);
    self = (void **)slot[1];
    void *b = slot[0];
    self[1] = m;
    self[0] = b ? b : 0;
}
