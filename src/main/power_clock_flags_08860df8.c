extern int func_089C5818(void);            /* scePower import (a clock frequency in MHz) */
extern int D_089D5E80;
extern int D_089D5E84;

/* Set both flags when the clock the scePower import reports is above 222. */
void func_08860DF8(void)
{
    if (func_089C5818() < 0xDF) {
        D_089D5E80 = 0;
        D_089D5E84 = 0;
    } else {
        D_089D5E80 = 1;
        D_089D5E84 = 1;
    }
}
