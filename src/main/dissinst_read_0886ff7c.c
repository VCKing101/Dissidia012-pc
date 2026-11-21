extern void *D_089D5FA4;               /* "DISSINST.DAT" */
extern int func_089C5D48(void *file, int size, unsigned long long offset, int size2, int arg);

/* A read from DISSINST.DAT at a 64-bit offset; 0 without a target. */
int func_0886FF7C(unsigned long long offset, int size, int arg)
{
    if (arg == 0) {
        return 0;
    }
    return func_089C5D48(D_089D5FA4, size, offset, size, arg);
}
