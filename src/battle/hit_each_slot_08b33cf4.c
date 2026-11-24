// Calls func_08B2E1A8 for each of the 16 entries in the list at +0xD64 whose id equals want (or any when want < 0).
extern int func_0884F630(void *);
extern int func_0884F67C(void *, int);
extern int func_0884F6B8(void *, int);
extern void func_08B2E1A8(int self, int a, int b, int c, int d, float f);
void func_08B33CF4(float f, int self, int a, int b, int c, int want)
{
    int list = self + 0xD64;
    int n = func_0884F630((void *)list);
    unsigned char ok = n > 0;
    if (ok) {
        int i = 0;
        do {
            if (func_0884F67C((void *)list, i) == want || want < 0) {
                int e = func_0884F6B8((void *)list, i);
                if (e != 0) {
                    func_08B2E1A8(self, a, b, c, e, f);
                }
            }
            i = i + 1;
        } while (i < 0x10);
    }
}
