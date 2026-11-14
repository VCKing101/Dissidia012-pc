/* Deleting destructor of the singleton at D_089D8148. */
extern "C" {
extern void *D_089D8148;
extern void func_08992F04(void *p);

void func_08903650(void *self, unsigned int flags)
{
    if (self != 0) {
        D_089D8148 = 0;
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
}
