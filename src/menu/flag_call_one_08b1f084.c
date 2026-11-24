/* Calls 0x088FCAE8 on the object at 0x089D7F90 unless the byte at 0x089D5F20 is set; returns 1 then. */
extern unsigned char D_089D5F20;
extern void *D_089D7F90;
extern int func_088FCAE8(void *p);
int func_08B1F084(void)
{
    if (D_089D5F20 != 0) { return 1; } else { return func_088FCAE8(D_089D7F90); }
}
