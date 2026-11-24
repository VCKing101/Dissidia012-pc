// If the byte at +0 is 1, stores the flag byte at +1 and calls func_08B41830 on the record at +8.
extern void func_08B41830(void *);
typedef struct { unsigned char kind; unsigned char flag; char pad[6]; char rec[1]; } Obj08B41B40;
void func_08B41B40(Obj08B41B40 *self, unsigned char flag)
{
    unsigned char k = self->kind;
    int one = 1;
    if (k == one) {
        self->flag = flag;
        func_08B41830(self->rec);
    }
}
