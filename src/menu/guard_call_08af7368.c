/* Forwards to func_08AF73D4 unless func_08AF96B0 reports set. */
extern int func_08AF96B0(void);
extern void func_08AF73D4(void *self, void *item, unsigned char a, unsigned char b);

void func_08AF7368(void *self, void *item, unsigned char a, unsigned char b)
{
    if (func_08AF96B0() != 0) {
        return;
    }
    func_08AF73D4(self, item, a, b);
}
