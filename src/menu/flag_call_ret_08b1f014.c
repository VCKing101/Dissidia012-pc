/* Calls 0x088FDC98 on the object at 0x089D7F90 unless the byte at 0x089D5F20 is set; returns 0 then. */
extern unsigned char D_089D5F20;
extern void *D_089D7F90;
extern int func_088FDC98(void *p);
int func_08B1F014(void)
{
    if (D_089D5F20 != 0) { return 0; } else { return func_088FDC98(D_089D7F90); }
}
