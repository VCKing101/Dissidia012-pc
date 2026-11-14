/* Adds an entry of kind 1 to the list; its extra byte comes from the item's
   class unless flag 1 is set. */
struct Item08B0EA50 {
    char pad_0[0xC];
    signed char field_C;
};

extern struct Item08B0EA50 *func_088C0F8C(int id);
extern int func_08B09D2C(void *self, short index);
extern void func_08B0E920(void *self, int kind, short id, unsigned short value, unsigned char flags,
                          unsigned char extra, int last);

void func_08B0EA50(void *self, short id, unsigned short value, unsigned char flags)
{
    unsigned char extra = 0;
    if ((flags & 1) == 0) {
        extra = func_08B09D2C(self, func_088C0F8C(id)->field_C);
    }
    func_08B0E920(self, 1, id, value, flags, extra, 0);
}
