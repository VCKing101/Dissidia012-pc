/* {address, size} filled by the sceSuspendForUser import (sceKernelVolatileMemLock-style). */
extern int *D_089D69B8;
extern int func_089C6098(int unk, int *address, int *size);

/* Lock the volatile memory block; returns its address, or 0 on failure. */
int func_08882E80(void)
{
    int *block = D_089D69B8;

    if (func_089C6098(0, block, block + 1) < 0) {
        return 0;
    } else {
        return *block;
    }
}
