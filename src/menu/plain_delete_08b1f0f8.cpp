/* Deleting destructor of a class with a trivial destructor. */
extern "C" void func_08992F04(void *block);

struct Plain08B1F0F8 {
    static void destroy(Plain08B1F0F8 *self, int flags)
    {
        if (self != 0) {
            if ((flags & 1) != 0) {
                func_08992F04(self);
            }
        }
    }
};

extern "C" {
void func_08B1F0F8(Plain08B1F0F8 *self, int flags)
{
    Plain08B1F0F8::destroy(self, flags);
}
}
