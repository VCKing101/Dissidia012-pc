/* Whether neither global lock flag is set. */
extern unsigned char D_089D5F1F;
extern unsigned char D_089D5F20;

int func_08AF7338(void)
{
    if ((D_089D5F1F | D_089D5F20) != 0) {
        return 0;
    }
    return 1;
}
