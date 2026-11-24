/* Calls 0x088FCAB8 on the object at 0x089D7F90 unless the byte at 0x089D5F20 is set. */
extern unsigned char D_089D5F20;
extern void *D_089D7F90;
extern void func_088FCAB8(void *p);
void func_08B1F04C(void)
{
    if (D_089D5F20 != 0) { return; }
    func_088FCAB8(D_089D7F90);
}
