extern unsigned int func_089C6078(void);   /* UtilsForUser import */

/* Bit 1 of the word the UtilsForUser import returns, as 0/1. */
int func_08860DC0(void)
{
    int r = 0;

    if (func_089C6078() & 2) {
        r = 1;
    }
    return r;
}
