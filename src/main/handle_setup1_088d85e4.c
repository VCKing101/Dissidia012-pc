typedef struct Setup1 {
    unsigned char active;
    char pad_01[3];
    void *target;
} Setup1;

extern void func_088D85A4(Setup1 *holder);

int func_088D85E4(Setup1 *holder, void *target)
{
    func_088D85A4(holder);
    if (target == 0) {
        return -1;
    }
    holder->target = target;
    holder->active = 1;
    return 0;
}
