/* Adds an entry of kind 2 to the list; its extra byte is the record's field
   0x12 unless flag 1 is set. */
struct Record08B0EADC {
    char pad_00[0x12];
    signed char field_12;
};

extern struct Record08B0EADC *func_088B29D8(int id);
extern void func_08B0E920(void *self, int kind, short id, unsigned short value, unsigned char flags,
                          unsigned char extra, int last);

void func_08B0EADC(void *self, short id, unsigned short value, unsigned char flags)
{
    unsigned char extra = 0;
    if ((flags & 1) == 0) {
        extra = func_088B29D8(id)->field_12;
    }
    func_08B0E920(self, 2, id, value, flags, extra, 0);
}
