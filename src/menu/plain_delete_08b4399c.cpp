/* Deleting destructor of a class with a trivial destructor. */
extern "C" void func_08992F04(void *block);

struct Plain08B4399C {
    static void destroy(Plain08B4399C *self, int flags)
    {
        if (self != 0) {
            if ((flags & 1) != 0) {
                func_08992F04(self);
            }
        }
    }
};

extern "C" {
void func_08B4399C(Plain08B4399C *self, int flags)
{
    Plain08B4399C::destroy(self, flags);
}
}
