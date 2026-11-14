extern "C" {
extern void func_08992F04(void *p);
}

struct PlainObject {
    int field_00;

    static void destroy(PlainObject *self, unsigned int flags)
    {
        if (self != 0) {
            if (flags & 1) {
                func_08992F04(self);
            }
        }
    }
};

extern "C" {
void func_088DE0BC(PlainObject *self, unsigned int flags)
{
    PlainObject::destroy(self, flags);
}
}
