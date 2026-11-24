/* Copies one word inside a global block to another global. */
extern char D_09AFB8A0[];
extern int D_09B6C2A8;

void func_08AFE2E4(void)
{
    D_09B6C2A8 = *(int *)(D_09AFB8A0 + 0x25A8);
}
