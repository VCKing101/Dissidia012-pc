/* Adds an entry of kind 0 to the list. */
extern void func_08B0E920(void *self, int kind, short id, unsigned short value, unsigned char flags,
                          unsigned char extra, int last);

void func_08B0EA1C(void *self, short id, unsigned short value, unsigned char flags, int last)
{
    func_08B0E920(self, 0, id, value, flags, 0, last);
}
