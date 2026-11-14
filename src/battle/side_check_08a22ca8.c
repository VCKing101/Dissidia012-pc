extern int func_08A22CF0(unsigned int side);

int func_08A22CA8(unsigned int side)
{
    if (side >= 2) {
        return -1;
    }
    if (func_08A22CF0(side) == 1) {
        return 0;
    }
    return 1;
}
