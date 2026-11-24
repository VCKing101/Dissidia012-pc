/* Walks every entry, applies func_08B3FD7C to each that passes the test, returns whether any did. */
extern "C" int func_08B451E4(void);
extern "C" void *func_08B45200(int i);
extern "C" int func_08920800(void *e);
extern "C" void func_08B3FD7C(void *self, void *e, int z);

extern "C" int func_08B39F78(void *self)
{
    int hit = 0;
    for (int i = 0; i < func_08B451E4(); i++) {
        void *e = func_08B45200(i);
        if (func_08920800(e)) {
            func_08B3FD7C(self, e, 0);
            hit = 1;
        }
    }
    return hit;
}
