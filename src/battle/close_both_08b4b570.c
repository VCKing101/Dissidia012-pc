// Clears the byte at +0x24C and calls func_088D9CFC on the pointers at +0 and +4.
extern void func_088D9CFC(void *);
typedef struct { void *p0; void *p4; char pad[0x24C - 8]; char flag; } Obj08B4B570;
void func_08B4B570(Obj08B4B570 *self)
{
    self->flag = 0;
    func_088D9CFC(self->p0);
    func_088D9CFC(self->p4);
}
