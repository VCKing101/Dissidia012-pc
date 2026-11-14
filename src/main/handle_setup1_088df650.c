typedef struct Setup1 {
    unsigned char active;
    char pad_01[3];
    void *target;
} Setup1;

extern void func_088DF54C(Setup1 *holder);

int func_088DF650(Setup1 *holder, void *target)
{
    func_088DF54C(holder);
    if (target == 0) {
        return -1;
    }
    holder->target = target;
    holder->active = 1;
    return 0;
}
