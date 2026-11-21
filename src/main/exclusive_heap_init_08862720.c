extern char D_09B18E10[];              /* the exclusive memory block */
extern char D_089C8FC8[];              /* "EXCLUSIVE MEMORY" */
extern void *D_089D5E98;
extern void *func_0885C40C(void *base, int size, const char *name, int unk);

void func_08862720(void)
{
    D_089D5E98 = func_0885C40C(D_09B18E10, 0xDC5C, D_089C8FC8, 4);
}
