extern int func_08A22C78(unsigned int side);
extern int func_08842164(void);

int func_08A22CF0(unsigned int side)
{
    if (side >= 2) {
        return 0;
    }
    return func_08A22C78(side) == func_08842164();
}
