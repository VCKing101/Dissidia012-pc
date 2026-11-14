/* Deleting destructor body: frees the object when flag 1 is set. */
extern "C" void func_08992F04(void *block);

struct DeleteItem {
    static void destroy(DeleteItem *self, unsigned int flags)
    {
        if (self != 0) {
            if ((flags & 1) != 0) {
                func_08992F04(self);
            }
        }
    }
};

extern "C" {
void func_08AF2C84(DeleteItem *self, unsigned int flags)
{
    DeleteItem::destroy(self, flags);
}
}
