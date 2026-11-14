/* Deleting destructor of an object with nothing to clean up. */
extern "C" void func_08992F04(void *p);

struct Object088B15D8 {
    int field_00;

    static void destroy(Object088B15D8 *self, int flags)
    {
        if (self != 0) {
            if (flags & 1) {
                func_08992F04(self);
            }
        }
    }
};

extern "C" {

void func_088B15D8(Object088B15D8 *self, int flags)
{
    Object088B15D8::destroy(self, flags);
}

}
