extern unsigned int func_089C6078(void);   /* UtilsForUser import */

/* Bit 0 of the word the UtilsForUser import returns, as 0/1. */
int func_08860D88(void)
{
    int r = 0;

    if (func_089C6078() & 1) {
        r = 1;
    }
    return r;
}
