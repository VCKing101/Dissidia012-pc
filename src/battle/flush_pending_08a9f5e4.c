/* Runs the pending-update check for the object; returns 0, -1 (no flag) or -2 (check failed). */
typedef struct Pend38 { char pad[0x158]; void *a; void *b; } Pend38;
extern int func_08A694E8(void *b, unsigned char *flag);
extern int func_08A65904(void *a);
extern void func_08A9B734(Pend38 *self);
extern void func_08A9B8A8(Pend38 *self);
int func_08A9F5E4(Pend38 *self, unsigned char *flag)
{
    if (flag == 0) {
        return -1;
    }
    *flag = 0;
    if (func_08A694E8(self->b, flag) != 0) {
        return -2;
    }
    if (*flag == 0) {
        if (func_08A65904(self->a) == 0) {
            func_08A9B734(self);
            func_08A9B8A8(self);
        }
    }
    return 0;
}
