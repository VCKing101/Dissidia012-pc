/* Allocates size bytes (func_089110C4), copies src into them into field 0, creates an 8-byte object (func_089110C4(8) + func_088CAF64) into field 1, then func_088CAF74(obj, data, size) and func_088CB00C(obj). */
extern void *func_089110C4(unsigned n);
extern void func_08935524(void *dst, void *src, unsigned n);
extern void func_088CAF64(void *p);
extern void func_088CAF74(void *p, void *data, unsigned n);
extern void func_088CB00C(void *p);
void func_08910868(void **self, void *src, unsigned size)
{
    void *m;
    void *o;
    void *r;
    m = func_089110C4(size);
    func_08935524(m, src, size);
    self[0] = m;
    self[1] = func_089110C4(8);
    o = self[1];
    r = 0;
    if (o != 0) {
        func_088CAF64(o);
        r = o;
    }
    self[1] = r;
    {
        void *d = self[0];
        func_088CAF74(r, d, size);
    }
    func_088CB00C(self[1]);
}
