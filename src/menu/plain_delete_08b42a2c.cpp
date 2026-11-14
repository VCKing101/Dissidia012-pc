/* Deleting destructor of a class with a trivial destructor. */
extern "C" void func_08992F04(void *block);

struct Plain08B42A2C {
    static void destroy(Plain08B42A2C *self, int flags)
    {
        if (self != 0) {
            if ((flags & 1) != 0) {
                func_08992F04(self);
            }
        }
    }
};

extern "C" {
void func_08B42A2C(Plain08B42A2C *self, int flags)
{
    Plain08B42A2C::destroy(self, flags);
}
}
