/* Adds an entry of kind 3 to the list. */
extern void func_08B0E920(void *self, int kind, short id, unsigned short value, unsigned char flags,
                          unsigned char extra, int last);

void func_08B0EB60(void *self, short id, unsigned short value, unsigned char flags)
{
    func_08B0E920(self, 3, id, value, flags, 0, 0);
}
