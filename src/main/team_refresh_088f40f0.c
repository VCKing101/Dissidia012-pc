/* Refreshes the team when its first entry is used. */
extern int func_088F2D8C(void *team, int index);
extern void func_088F2FE0(void *team, unsigned char flag);
extern void func_088F3A7C(void *team, unsigned char flag);
extern void func_088F3B18(void *team);

void func_088F40F0(void *team, unsigned char flag)
{
    if (func_088F2D8C(team, 0) != 0) {
        func_088F2FE0(team, flag);
        func_088F3A7C(team, flag);
        func_088F3B18(team);
    }
}
