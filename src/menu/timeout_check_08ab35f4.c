/* Returns 1 when the flag bit 0x20 is set and the stored time plus 5,000,000 is below the current time. */
extern unsigned int func_089C5FB8(void);
int func_08AB35F4(int *self) {
    unsigned int now;
    if ((*(unsigned *)(*self + 0x498) & 0x20) == 0) {
        return 0;
    } else {
        now = func_089C5FB8();
        if (*(int *)(*self + 0x4A8) + 5000000U < now) {
            return 1;
        } else {
            return 0;
        }
    }
}
