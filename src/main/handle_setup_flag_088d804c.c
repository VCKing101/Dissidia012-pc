/* Resets the holder and stores a pointer and a flag byte, marking it active. */
typedef struct SetupFlag {
    unsigned char active;
    char pad_01[3];
    void *target;
    unsigned char flag;
} SetupFlag;

extern void func_088D8030(SetupFlag *holder);

int func_088D804C(SetupFlag *holder, void *target, unsigned char flag)
{
    func_088D8030(holder);
    if (target == 0) {
        return -1;
    }
    holder->target = target;
    holder->flag = flag;
    holder->active = 1;
    return 0;
}
